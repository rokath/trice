// SPDX-License-Identifier: MIT

package main

import (
	"archive/tar"
	"archive/zip"
	"compress/gzip"
	"crypto/sha256"
	"encoding/hex"
	"fmt"
	"io"
	"net/http"
	"os"
	"os/exec"
	"path/filepath"
	"regexp"
	"runtime"
	"strconv"
	"strings"
	"time"
)

// readToolConfig reads data-only KEY=value assignments shared with Bash. It
// deliberately does not evaluate substitutions, shell commands or environment.
func readToolConfig(path string) (map[string]string, []byte, error) {
	data, err := os.ReadFile(path)
	if err != nil {
		return nil, nil, err
	}
	values := make(map[string]string)
	for _, line := range strings.Split(string(data), "\n") {
		line = strings.TrimSpace(line)
		if line == "" || strings.HasPrefix(line, "#") {
			continue
		}
		key, value, ok := strings.Cut(line, "=")
		if !ok || !regexp.MustCompile(`^[A-Z][A-Z0-9_]*$`).MatchString(key) {
			return nil, nil, fmt.Errorf("invalid tool configuration line: %q", line)
		}
		values[key] = strings.Trim(value, `"`)
	}
	return values, data, nil
}

// toolIdentity validates the executable, including Go module build identities
// for tools whose official go install form reports a development display name.
func toolIdentity(binary, version, module string) bool {
	output, err := exec.Command(binary, "--version").CombinedOutput()
	pattern := regexp.MustCompile(`(^|[\s-])v?` + regexp.QuoteMeta(version) + `(\s|$)`)
	if err == nil && pattern.Match(output) {
		return true
	}
	if module != "" {
		output, err = exec.Command("go", "version", "-m", binary).CombinedOutput()
		if err == nil && !strings.Contains(string(output), "\t=>\t") {
			for _, line := range strings.Split(string(output), "\n") {
				fields := strings.Fields(line)
				if len(fields) >= 3 && fields[0] == "mod" && fields[2] == "v"+version &&
					(module == fields[1] || strings.HasPrefix(module, fields[1]+"/")) {
					return true
				}
			}
		}
	}
	return false
}

// binaryDigest makes interrupted or externally modified caches ineligible.
func binaryDigest(path string) (string, error) {
	file, err := os.Open(path)
	if err != nil {
		return "", err
	}
	defer file.Close()
	hash := sha256.New()
	if _, err := io.Copy(hash, file); err != nil {
		return "", err
	}
	return hex.EncodeToString(hash.Sum(nil)), nil
}

// downloadTool verifies the pinned archive bytes before any extraction. A
// bounded HTTP client prevents an unavailable release server hanging setup.
func downloadTool(url, destination, expected string) error {
	if len(expected) != 64 {
		return fmt.Errorf("missing pinned SHA-256 for %s", url)
	}
	if digest, err := binaryDigest(destination); err == nil && digest == expected {
		return nil
	}
	client := &http.Client{Timeout: 10 * time.Minute}
	response, err := client.Get(url)
	if err != nil {
		return err
	}
	defer response.Body.Close()
	if response.StatusCode != http.StatusOK {
		return fmt.Errorf("download %s: HTTP %d", url, response.StatusCode)
	}
	file, err := os.Create(destination + ".partial")
	if err != nil {
		return err
	}
	_, copyErr := io.Copy(file, response.Body)
	closeErr := file.Close()
	if copyErr != nil {
		return copyErr
	}
	if closeErr != nil {
		return closeErr
	}
	digest, err := binaryDigest(destination + ".partial")
	if err != nil || digest != expected {
		return fmt.Errorf("SHA-256 mismatch for %s: got %s, want %s", url, digest, expected)
	}
	if err := os.Remove(destination); err != nil && !os.IsNotExist(err) {
		return err
	}
	return os.Rename(destination+".partial", destination)
}

// extractTool copies only the named executable. Archive paths, symlinks and
// unrelated files can never escape the staging directory or alter the checkout.
func extractTool(archive, binaryName, destination string) error {
	write := func(input io.Reader) error {
		file, err := os.OpenFile(destination, os.O_CREATE|os.O_TRUNC|os.O_WRONLY, 0o755)
		if err != nil {
			return err
		}
		_, copyErr := io.Copy(file, input)
		closeErr := file.Close()
		if copyErr != nil {
			return copyErr
		}
		return closeErr
	}
	if strings.HasSuffix(archive, ".zip") {
		reader, err := zip.OpenReader(archive)
		if err != nil {
			return err
		}
		defer reader.Close()
		for _, entry := range reader.File {
			if filepath.Base(entry.Name) == binaryName && entry.Mode().IsRegular() {
				file, err := entry.Open()
				if err != nil {
					return err
				}
				defer file.Close()
				return write(file)
			}
		}
	} else if strings.HasSuffix(archive, ".tar.gz") {
		file, err := os.Open(archive)
		if err != nil {
			return err
		}
		defer file.Close()
		compressed, err := gzip.NewReader(file)
		if err != nil {
			return err
		}
		defer compressed.Close()
		reader := tar.NewReader(compressed)
		for {
			entry, err := reader.Next()
			if err == io.EOF {
				break
			}
			if err != nil {
				return err
			}
			if filepath.Base(entry.Name) == binaryName && entry.Typeflag == tar.TypeReg {
				return write(reader)
			}
		}
	} else {
		file, err := os.Open(archive)
		if err != nil {
			return err
		}
		defer file.Close()
		return write(file)
	}
	return fmt.Errorf("archive %s lacks executable %s", archive, binaryName)
}

// installTools serializes publication for one manifest/host. The digest in the
// cache path keeps version changes independent and includes all pinned hashes.
func installTools(config, root string, names []string) (string, error) {
	values, data, err := readToolConfig(config)
	if err != nil {
		return "", err
	}
	digest := sha256.Sum256(data)
	root, err = filepath.Abs(filepath.Join(root, runtime.GOOS+"-"+runtime.GOARCH, fmt.Sprintf("%x", digest[:8])))
	if err != nil {
		return "", err
	}
	bin := filepath.Join(root, "bin")
	if err := os.MkdirAll(bin, 0o755); err != nil {
		return "", err
	}
	lock := filepath.Join(root, ".install-lock")
	deadline := time.Now().Add(2 * time.Minute)
	for {
		if err := os.Mkdir(lock, 0o755); err == nil {
			break
		} else if !os.IsExist(err) || time.Now().After(deadline) {
			return "", fmt.Errorf("tool cache is locked at %s; inspect the owning setup process before removing a stale lock: %w", lock, err)
		}
		time.Sleep(200 * time.Millisecond)
	}
	if err := os.WriteFile(filepath.Join(lock, "owner"), []byte(strconv.Itoa(os.Getpid())+"\n"), 0o644); err != nil {
		_ = os.Remove(lock)
		return "", err
	}
	defer func() { _ = os.Remove(filepath.Join(lock, "owner")); _ = os.Remove(lock) }()
	for _, name := range names {
		prefix := "TRICE_TOOL_" + strings.ToUpper(strings.ReplaceAll(name, "-", "_"))
		version, module := values[prefix+"_VERSION"], values[prefix+"_MODULE"]
		if version == "" {
			return "", fmt.Errorf("no pinned version for %s", name)
		}
		filename := name
		if runtime.GOOS == "windows" {
			filename += ".exe"
		}
		binary := filepath.Join(bin, filename)
		ready, readErr := os.ReadFile(binary + ".sha256")
		actual, hashErr := binaryDigest(binary)
		if readErr == nil && hashErr == nil && string(ready) == actual && toolIdentity(binary, version, module) {
			fmt.Fprintf(os.Stderr, "Selected %s %s: %s\n", name, version, binary)
			continue
		}
		staging, err := os.MkdirTemp(root, ".stage-")
		if err != nil {
			return "", err
		}
		staged := filepath.Join(staging, filename)
		// Reuse only a matching global executable; copy it to freeze this run's
		// selection even if the caller subsequently changes PATH.
		global, globalErr := exec.LookPath(name)
		if globalErr == nil && toolIdentity(global, version, module) {
			err = extractTool(global, filename, staged)
		} else if module != "" {
			cmd := exec.Command("go", "install", module+"@v"+version)
			cmd.Env = append(os.Environ(), "GOBIN="+staging)
			cmd.Stdout, cmd.Stderr = os.Stderr, os.Stderr
			err = cmd.Run()
		} else {
			platform := strings.ToUpper(runtime.GOOS + "_" + runtime.GOARCH)
			url, expected := values[prefix+"_URL_"+platform], values[prefix+"_SHA_"+platform]
			if url == "" {
				err = fmt.Errorf("no verified %s package for %s", name, platform)
			} else {
				archive := filepath.Join(root, filepath.Base(url))
				err = downloadTool(url, archive, expected)
				if err == nil {
					err = extractTool(archive, filename, staged)
				}
			}
		}
		if err != nil || !toolIdentity(staged, version, module) {
			return "", fmt.Errorf("prepare %s %s: %v; incomplete files retained in %s", name, version, err, staging)
		}
		actual, err = binaryDigest(staged)
		if err != nil {
			return "", err
		}
		// Remove only this installer's previous publication; Windows cannot
		// rename over an existing executable. The lock prevents competing writers.
		if err := os.Remove(binary); err != nil && !os.IsNotExist(err) {
			return "", err
		}
		if err := os.Rename(staged, binary); err != nil {
			return "", err
		}
		if err := os.WriteFile(binary+".sha256", []byte(actual), 0o644); err != nil {
			return "", err
		}
		os.Remove(staging)
		fmt.Fprintf(os.Stderr, "Selected %s %s: %s\n", name, version, binary)
	}
	return bin, nil
}

// installBundle preserves a compiler's complete headers, libraries and
// support programs. Copying its gcc executable alone would mix runtimes and
// produce host-dependent firmware builds. Only verified publisher archives
// are handed to the native archive tool, inside a private staging directory.
func installBundle(config, root, name string) (string, error) {
	values, _, err := readToolConfig(config)
	if err != nil {
		return "", err
	}
	platform := strings.ToUpper(runtime.GOOS + "_" + runtime.GOARCH)
	prefix := "TRICE_BUNDLE_" + strings.ToUpper(name)
	url, expected := values[prefix+"_URL_"+platform], values[prefix+"_SHA_"+platform]
	if url == "" || len(expected) != 64 {
		return "", fmt.Errorf("no pinned %s package for %s", name, platform)
	}
	root, err = filepath.Abs(filepath.Join(root, name, runtime.GOOS+"-"+runtime.GOARCH, expected[:16]))
	if err != nil {
		return "", err
	}
	bin := filepath.Join(root, "package", "bin")
	compiler := filepath.Join(bin, values[prefix+"_EXECUTABLE"])
	version := values[values[prefix+"_VERSION_KEY"]]
	if runtime.GOOS == "windows" {
		compiler += ".exe"
	}
	if marker, err := os.ReadFile(filepath.Join(root, ".ready")); err == nil && string(marker) == expected &&
		toolIdentity(compiler, version, "") {
		return bin, nil
	}
	if err := os.MkdirAll(root, 0o755); err != nil {
		return "", err
	}
	lock := filepath.Join(root, ".install-lock")
	if err := os.Mkdir(lock, 0o755); err != nil {
		return "", fmt.Errorf("%s setup is locked at %s: %w", name, lock, err)
	}
	if err := os.WriteFile(filepath.Join(lock, "owner"), []byte(strconv.Itoa(os.Getpid())+"\n"), 0o644); err != nil {
		_ = os.Remove(lock)
		return "", err
	}
	defer func() { _ = os.Remove(filepath.Join(lock, "owner")); _ = os.Remove(lock) }()
	archive := filepath.Join(root, filepath.Base(url))
	fmt.Fprintln(os.Stderr, "Preparing complete compiler package:", url)
	if err := downloadTool(url, archive, expected); err != nil {
		return "", err
	}
	stage, err := os.MkdirTemp(root, ".stage-")
	if err != nil {
		return "", err
	}
	defer os.RemoveAll(stage)
	command := exec.Command("tar", "-xzf", archive, "-C", stage)
	if runtime.GOOS == "windows" {
		command = exec.Command("unzip", "-q", archive, "-d", stage)
	}
	command.Stdout, command.Stderr = os.Stderr, os.Stderr
	if err := command.Run(); err != nil {
		return "", err
	}
	packageDir := filepath.Join(root, "package")
	if err := os.RemoveAll(packageDir); err != nil {
		return "", err
	}
	if err := os.Rename(filepath.Join(stage, values[prefix+"_DIRECTORY"]), packageDir); err != nil {
		return "", err
	}
	if !toolIdentity(compiler, version, "") {
		return "", fmt.Errorf("compiler cannot execute or has the wrong version: %s (expected %s)", compiler, version)
	}
	if err := os.WriteFile(filepath.Join(root, ".ready"), []byte(expected), 0o644); err != nil {
		return "", err
	}
	return bin, nil
}
