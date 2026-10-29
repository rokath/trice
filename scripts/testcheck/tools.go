// SPDX-License-Identifier: MIT

package main

import (
	"archive/tar"
	"archive/zip"
	"bytes"
	"compress/gzip"
	"crypto/sha256"
	"debug/buildinfo"
	"encoding/hex"
	"errors"
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

// errInstallRequired distinguishes a missing installation from setup errors.
// The shell can request consent for this case without prompting on cache hits.
var errInstallRequired = errors.New("pinned installation required")

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
	return checkToolIdentity(binary, version, module) == nil
}

// checkToolIdentity retains startup errors and version output so a failed
// staged executable can be diagnosed before it becomes a selectable cache.
func checkToolIdentity(binary, version, module string) error {
	output, err := exec.Command(binary, "--version").CombinedOutput()
	pattern := regexp.MustCompile(`(^|[\s-])v?` + regexp.QuoteMeta(version) + `(\s|$)`)
	if err == nil && pattern.Match(output) {
		return nil
	}
	if module != "" {
		command := exec.Command("go", "version", "-m", binary)
		command.Env = append(os.Environ(), "GOTOOLCHAIN=local", "GOPROXY=off")
		buildOutput, buildErr := command.CombinedOutput()
		if buildErr == nil && !strings.Contains(string(buildOutput), "\t=>\t") {
			for _, line := range strings.Split(string(buildOutput), "\n") {
				fields := strings.Fields(line)
				if len(fields) >= 3 && fields[0] == "mod" && fields[2] == "v"+version &&
					(module == fields[1] || strings.HasPrefix(module, fields[1]+"/")) {
					return nil
				}
			}
		}
	}
	if err != nil {
		return fmt.Errorf("%s --version failed: %w; output: %q", binary, err, strings.TrimSpace(string(output)))
	}
	return fmt.Errorf("%s must report version %s; --version output: %q", binary, version, strings.TrimSpace(string(output)))
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

// inspectTool reports a read-only selection using the same version, resource
// and cache checks as installation. Tab-separated fields have nonempty values
// so Bash 3.2 can consume the plan without losing empty columns.
func inspectTool(config, cache, kind, name string, output io.Writer) error {
	values, data, err := readToolConfig(config)
	if err != nil {
		return err
	}
	prefix := "TRICE_TOOL_" + strings.ToUpper(strings.ReplaceAll(name, "-", "_"))
	version, executable := values[prefix+"_VERSION"], name
	if kind == "bundle" {
		prefix = "TRICE_BUNDLE_" + strings.ToUpper(name)
		version, executable = values[values[prefix+"_VERSION_KEY"]], values[prefix+"_EXECUTABLE"]
	}
	if version == "" || executable == "" || (kind != "tools" && kind != "bundle") {
		return fmt.Errorf("no pinned %s declaration for %s", kind, name)
	}
	// Display the first host version even when selection chooses a later PATH
	// entry or a valid checkout cache. Flatten diagnostics for the TSV protocol.
	found, selected, action, method := "missing", "-", "install", "release"
	global, lookupErr := exec.LookPath(executable)
	if lookupErr == nil {
		result, runErr := exec.Command(global, "--version").CombinedOutput()
		if runErr != nil {
			found = "cannot execute"
		} else if lines := strings.Split(strings.TrimSpace(string(result)), "\n"); len(lines) > 0 {
			found = strings.TrimSpace(lines[0])
		}
	}
	if kind == "bundle" {
		selected, err = installBundle(config, cache, name, false)
		if err != nil && !errors.Is(err, errInstallRequired) {
			return err
		}
		if err == nil {
			action, method = "reuse", "complete package"
		} else {
			selected = "-"
		}
	} else {
		if lookupErr == nil && toolIdentity(global, version, values[prefix+"_MODULE"]) {
			selected, action, method = global, "reuse", "installed"
		} else {
			digest := sha256.Sum256(data)
			filename := name
			if runtime.GOOS == "windows" {
				filename += ".exe"
			}
			current, absErr := filepath.Abs(filepath.Join(cache, runtime.GOOS+"-"+runtime.GOARCH, fmt.Sprintf("%x", digest[:8]), "bin"))
			if absErr != nil {
				return absErr
			}
			candidates, _ := filepath.Glob(filepath.Join(cache, runtime.GOOS+"-"+runtime.GOARCH, "*", "bin", filename))
			for _, candidate := range candidates {
				candidate, absErr = filepath.Abs(candidate)
				if absErr == nil && cachedToolMatches(candidate, current, name, values) {
					selected, action, method = candidate, "reuse", "verified cache"
					break
				}
			}
		}
	}
	platform := strings.ToUpper(runtime.GOOS + "_" + runtime.GOARCH)
	if action == "install" && (values[prefix+"_BUILD_"+platform] != "" || values[prefix+"_URL_"+platform] == "") {
		method = "source build"
	}
	for _, field := range []string{found, selected, cache} {
		if strings.ContainsAny(field, "\t\r\n") {
			return fmt.Errorf("tool paths and version lines cannot contain tabs or newlines: %q", field)
		}
	}
	_, err = fmt.Fprintf(output, "%s\t%s\t%s\t%s\t%s\t%s\n", name, version, found, action, method, selected)
	return err
}

// goToolModuleMatches verifies the declared main package and source tag without
// invoking Go's launcher or downloading a toolchain merely to read metadata.
func goToolModuleMatches(binary, module, version string) bool {
	info, err := buildinfo.ReadFile(binary)
	if err != nil || info.Path != module || info.Main.Replace != nil || info.Main.Version != "v"+version ||
		(info.Main.Path != module && !strings.HasPrefix(module, info.Main.Path+"/")) {
		return false
	}
	for _, dependency := range info.Deps {
		if dependency.Replace != nil {
			return false
		}
	}
	return true
}

// cachedToolMatches allows read-only reuse across unrelated manifest changes.
// Current caches keep their digest contract. Older caches additionally need
// either the exact Go source identity or the still-matching publisher archive.
func cachedToolMatches(binary, currentBin, name string, values map[string]string) bool {
	prefix := "TRICE_TOOL_" + strings.ToUpper(strings.ReplaceAll(name, "-", "_"))
	version, module := values[prefix+"_VERSION"], values[prefix+"_MODULE"]
	marker, err := os.ReadFile(binary + ".sha256")
	digest, hashErr := binaryDigest(binary)
	if err != nil || hashErr != nil || string(marker) != digest || !toolIdentity(binary, version, module) {
		return false
	}
	if filepath.Dir(binary) == currentBin || (module != "" && goToolModuleMatches(binary, module, version)) {
		return true
	}
	platform := strings.ToUpper(runtime.GOOS + "_" + runtime.GOARCH)
	url, expected := values[prefix+"_URL_"+platform], values[prefix+"_SHA_"+platform]
	if url == "" || len(expected) != 64 {
		return false
	}
	archive, err := binaryDigest(filepath.Join(filepath.Dir(filepath.Dir(binary)), filepath.Base(url)))
	return err == nil && archive == expected
}

// compatibleToolCaches returns only directories that cannot shadow another
// requested, already available pin. Missing versions will be installed into a
// higher-priority new bin directory, so obsolete copies of those are harmless.
func compatibleToolCaches(root, currentBin string, names []string, values map[string]string) ([]string, map[string]bool) {
	directories, _ := filepath.Glob(filepath.Join(root, runtime.GOOS+"-"+runtime.GOARCH, "*", "bin"))
	missing := make(map[string]bool)
	for _, name := range names {
		prefix := "TRICE_TOOL_" + strings.ToUpper(strings.ReplaceAll(name, "-", "_"))
		global, err := exec.LookPath(name)
		if err == nil && toolIdentity(global, values[prefix+"_VERSION"], values[prefix+"_MODULE"]) {
			continue
		}
		missing[name] = true
		for _, directory := range directories {
			filename := name
			if runtime.GOOS == "windows" {
				filename += ".exe"
			}
			if cachedToolMatches(filepath.Join(directory, filename), currentBin, name, values) {
				missing[name] = false
				break
			}
		}
	}
	var compatible []string
	for _, directory := range directories {
		valid := true
		useful := false
		for _, name := range names {
			filename := name
			if runtime.GOOS == "windows" {
				filename += ".exe"
			}
			binary := filepath.Join(directory, filename)
			matches := cachedToolMatches(binary, currentBin, name, values)
			useful = useful || matches
			if _, err := os.Stat(binary); err == nil && !missing[name] && !matches {
				valid = false
				break
			}
		}
		if valid && useful {
			compatible = append(compatible, directory)
		}
	}
	return compatible, missing
}

// installTools serializes publication for one manifest/host. The digest in the
// cache path keeps version changes independent and includes all pinned hashes.
func installTools(config, root string, names []string, allowInstall bool) (string, error) {
	values, data, err := readToolConfig(config)
	if err != nil {
		return "", err
	}
	// Preserve the original PATH and relative launcher resources when the host
	// already supplies every requested version. This fast path writes nothing.
	installed := true
	for _, name := range names {
		prefix := "TRICE_TOOL_" + strings.ToUpper(strings.ReplaceAll(name, "-", "_"))
		global, lookupErr := exec.LookPath(name)
		if lookupErr != nil || values[prefix+"_VERSION"] == "" || !toolIdentity(global, values[prefix+"_VERSION"], values[prefix+"_MODULE"]) {
			installed = false
			break
		}
	}
	if installed && len(names) > 0 {
		return os.Getenv("PATH"), nil
	}
	cacheRoot, err := filepath.Abs(root)
	if err != nil {
		return "", err
	}
	digest := sha256.Sum256(data)
	root, err = filepath.Abs(filepath.Join(root, runtime.GOOS+"-"+runtime.GOARCH, fmt.Sprintf("%x", digest[:8])))
	if err != nil {
		return "", err
	}
	bin := filepath.Join(root, "bin")
	compatible, missing := compatibleToolCaches(cacheRoot, bin, names, values)
	// Reuse complete prior selections in place; neither cache migration nor
	// copying large standalone binaries is necessary for an unrelated pin change.
	allReady := len(names) > 0
	for _, name := range names {
		prefix := "TRICE_TOOL_" + strings.ToUpper(strings.ReplaceAll(name, "-", "_"))
		global, lookupErr := exec.LookPath(name)
		covered := lookupErr == nil && toolIdentity(global, values[prefix+"_VERSION"], values[prefix+"_MODULE"])
		filename := name
		if runtime.GOOS == "windows" {
			filename += ".exe"
		}
		for _, directory := range compatible {
			if cachedToolMatches(filepath.Join(directory, filename), bin, name, values) {
				covered = true
				break
			}
		}
		if !covered && !missing[name] {
			return "", fmt.Errorf("cached %s conflicts with another selected pin in PATH; adjust the local tool selection and rerun", name)
		}
		if !covered {
			allReady = false
		}
	}
	if allReady && len(compatible) > 0 {
		return strings.Join(compatible, string(os.PathListSeparator)), nil
	}
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
		cached := false
		for _, directory := range compatible {
			if directory != bin && cachedToolMatches(filepath.Join(directory, filename), bin, name, values) {
				cached = true
				break
			}
		}
		if cached {
			// Remove only an invalid shadow in this manifest's own publication.
			if err := os.Remove(binary); err != nil && !os.IsNotExist(err) {
				return "", err
			}
			if err := os.Remove(binary + ".sha256"); err != nil && !os.IsNotExist(err) {
				return "", err
			}
			continue
		}
		// Existing matching tools can be frozen locally without installation.
		// Reject downloads/builds here so a changed cache cannot bypass consent.
		global, globalErr := exec.LookPath(name)
		matchingGlobal := globalErr == nil && toolIdentity(global, version, module)
		platform := strings.ToUpper(runtime.GOOS + "_" + runtime.GOARCH)
		url, expected := values[prefix+"_URL_"+platform], values[prefix+"_SHA_"+platform]
		if !matchingGlobal && module == "" && (url == "" || len(expected) != 64) {
			return "", fmt.Errorf("no verified %s package for %s", name, platform)
		}
		if !matchingGlobal && !allowInstall {
			return "", fmt.Errorf("%w: %s %s", errInstallRequired, name, version)
		}
		if matchingGlobal {
			// An invalid cached executable must not shadow a working host tool
			// after the cache's bin directory is prepended for another tool.
			if err := os.Remove(binary); err != nil && !os.IsNotExist(err) {
				return "", err
			}
			if err := os.Remove(binary + ".sha256"); err != nil && !os.IsNotExist(err) {
				return "", err
			}
			fmt.Fprintf(os.Stderr, "Reusing installed %s %s: %s\n", name, version, global)
			continue
		}
		staging, err := os.MkdirTemp(root, ".stage-")
		if err != nil {
			return "", err
		}
		staged := filepath.Join(staging, filename)
		if url != "" && len(expected) == 64 {
			archive := filepath.Join(root, filepath.Base(url))
			err = downloadTool(url, archive, expected)
			if err == nil {
				err = extractTool(archive, filename, staged)
			}
		} else if module != "" {
			cmd := exec.Command("go", "install", module+"@v"+version)
			cmd.Env = append(os.Environ(), "GOBIN="+filepath.Dir(staged))
			cmd.Stdout, cmd.Stderr = os.Stderr, os.Stderr
			err = cmd.Run()
		}
		if err == nil {
			err = checkToolIdentity(staged, version, module)
		}
		if err != nil {
			return "", fmt.Errorf("prepare %s %s: %w; incomplete files retained in %s", name, version, err, staging)
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
		os.RemoveAll(staging)
		fmt.Fprintf(os.Stderr, "Selected %s %s: %s\n", name, version, binary)
	}
	selected := []string{bin}
	for _, directory := range compatible {
		if directory != bin {
			selected = append(selected, directory)
		}
	}
	return strings.Join(selected, string(os.PathListSeparator)), nil
}

// findInstalledBundle selects a complete, matching toolchain in place, resolving
// symlinks before finding its companions. It never copies or installs resources.
func findInstalledBundle(name, executable, version string) (string, bool) {
	if executable == "" || version == "" {
		return "", false
	}
	switch name {
	case "cmake", "clang", "gcc", "arm":
	default:
		return "", false
	}
	if runtime.GOOS == "windows" {
		executable += ".exe"
	}
	// Distribution Clang packages and keg-only Homebrew LLVM installations need
	// not replace the host's default command. Their own executable/resource tree
	// can still satisfy this exact pin without a checkout-local download.
	directories := filepath.SplitList(os.Getenv("PATH"))
	if runtime.GOOS == "linux" && name == "clang" {
		if candidate, err := exec.LookPath("clang-" + strings.Split(version, ".")[0]); err == nil {
			if resolved, err := filepath.EvalSymlinks(candidate); err == nil {
				directories = append(directories, filepath.Dir(resolved))
			}
		}
	}
	if runtime.GOOS == "darwin" {
		formula := name
		switch name {
		case "clang":
			formula = "llvm"
		case "arm":
			formula = "arm-none-eabi-gcc"
		}
		command := exec.Command("brew", "--prefix", formula)
		command.Env = append(os.Environ(), "HOMEBREW_NO_AUTO_UPDATE=1")
		if output, err := command.Output(); err == nil {
			directories = append(directories, filepath.Join(strings.TrimSpace(string(output)), "bin"))
		}
	}
	for _, directory := range directories {
		if directory == "" {
			continue
		}
		original := filepath.Join(directory, executable)
		candidate, err := filepath.EvalSymlinks(original)
		if err != nil {
			continue
		}
		candidate, err = filepath.Abs(candidate)
		if err == nil && toolIdentity(candidate, version, "") {
			// Run-local aliases deliberately pair versioned GCC/g++ commands.
			// Prefer that complete pair before trying the resolved package root.
			for _, bin := range []string{filepath.Dir(original), filepath.Dir(candidate)} {
				if installedBundleIsComplete(name, bin, candidate, version) {
					bin, err = filepath.Abs(bin)
					return bin, err == nil
				}
			}
		}
	}
	return "", false
}

// installedBundleIsComplete checks resources as well as frontend identities.
// These are read-only setup checks; actual compilation remains test work.
func installedBundleIsComplete(name, bin, executable, version string) bool {
	// Companions must live beside the resolved executable, not elsewhere in PATH.
	companion := func(name string) string {
		if runtime.GOOS == "windows" {
			name += ".exe"
		}
		return filepath.Join(bin, name)
	}
	matching := func(name string) bool { return toolIdentity(companion(name), version, "") }
	present := func(name string) bool {
		info, err := os.Stat(companion(name))
		return err == nil && !info.IsDir()
	}
	// Compiler-reported paths must point at real resources. A bare filename
	// means GCC did not find its runtime and cannot represent a complete install.
	resource := func(arguments ...string) bool {
		output, err := exec.Command(executable, arguments...).Output()
		file := strings.TrimSpace(string(output))
		if err != nil || !filepath.IsAbs(file) {
			return false
		}
		info, err := os.Stat(file)
		return err == nil && !info.IsDir()
	}
	switch name {
	case "cmake":
		if !matching("ctest") || !matching("cpack") {
			return false
		}
		output, err := exec.Command(executable, "--help-module-list").Output()
		return err == nil && len(bytes.TrimSpace(output)) > 0
	case "clang":
		if !matching("clang++") || !matching("clangd") {
			return false
		}
		if runtime.GOOS == "windows" {
			target, err := exec.Command(executable, "-dumpmachine").Output()
			if err != nil || (!strings.Contains(string(target), "windows-gnu") && !strings.Contains(string(target), "mingw")) {
				return false
			}
		}
		output, err := exec.Command(executable, "-print-resource-dir").Output()
		directory := strings.TrimSpace(string(output))
		if err != nil || !filepath.IsAbs(directory) {
			return false
		}
		info, err := os.Stat(filepath.Join(directory, "include", "stddef.h"))
		return err == nil && !info.IsDir()
	case "gcc":
		return matching("g++") && resource("-print-libgcc-file-name") && resource("-print-file-name=libstdc++.a")
	case "arm":
		return matching("arm-none-eabi-g++") && present("arm-none-eabi-objcopy") && present("arm-none-eabi-size") &&
			resource("-print-libgcc-file-name") && resource("-print-file-name=libc.a")
	}
	// Private fixture/custom bundles have no additional declared resource query.
	return true
}

// installBundle preserves complete compiler runtimes and tool resources such
// as CMake's modules. Copying an executable alone can break these relative
// dependencies or mix host installations. Only verified publisher archives
// are handed to the native archive tool, inside a private staging directory.
func installBundle(config, root, name string, allowInstall bool) (string, error) {
	values, _, err := readToolConfig(config)
	if err != nil {
		return "", err
	}
	platform := strings.ToUpper(runtime.GOOS + "_" + runtime.GOARCH)
	prefix := "TRICE_BUNDLE_" + strings.ToUpper(name)
	version := values[values[prefix+"_VERSION_KEY"]]
	if installed, ok := findInstalledBundle(name, values[prefix+"_EXECUTABLE"], version); ok {
		fmt.Fprintf(os.Stderr, "Reusing installed complete %s %s: %s\n", name, version, installed)
		return installed, nil
	}
	url, expected := values[prefix+"_URL_"+platform], values[prefix+"_SHA_"+platform]
	if url == "" || len(expected) != 64 {
		return "", fmt.Errorf("no pinned %s package for %s", name, platform)
	}
	root, err = filepath.Abs(filepath.Join(root, name, runtime.GOOS+"-"+runtime.GOARCH, expected[:16]))
	if err != nil {
		return "", err
	}
	// Publisher layouts can differ by platform, including macOS app bundles.
	// Existing compiler bundles retain their common directory and bin defaults.
	directory := values[prefix+"_DIRECTORY_"+platform]
	if directory == "" {
		directory = values[prefix+"_DIRECTORY"]
	}
	binDirectory := values[prefix+"_BIN_DIRECTORY_"+platform]
	if binDirectory == "" {
		binDirectory = "bin"
	}
	bin := filepath.Join(root, "package", binDirectory)
	compiler := filepath.Join(bin, values[prefix+"_EXECUTABLE"])
	if runtime.GOOS == "windows" {
		compiler += ".exe"
	}
	if marker, err := os.ReadFile(filepath.Join(root, ".ready")); err == nil && string(marker) == expected &&
		toolIdentity(compiler, version, "") && installedBundleIsComplete(name, bin, compiler, version) {
		return bin, nil
	}
	if !allowInstall {
		return "", fmt.Errorf("%w: complete %s compiler package %s", errInstallRequired, name, version)
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
	// Native tar detects gzip and xz; LLVM publishes xz archives on Unix.
	command := exec.Command("tar", "-xf", archive, "-C", stage)
	if strings.HasSuffix(archive, ".zip") {
		command = exec.Command("unzip", "-q", archive, "-d", stage)
	}
	command.Stdout, command.Stderr = os.Stderr, os.Stderr
	if err := command.Run(); err != nil {
		return "", err
	}
	packageDir := filepath.Join(root, "package")
	if build := values[prefix+"_BUILD_"+platform]; build != "" {
		if build != "llvm" {
			return "", fmt.Errorf("unsupported %s bundle build: %s", name, build)
		}
		// Publish a source build through the same version check and ready marker
		// as a binary package. Failed builds never become selectable caches.
		if err := buildClangBundle(filepath.Join(stage, directory), filepath.Join(stage, "build"), filepath.Join(stage, "installed"), nil); err != nil {
			return "", err
		}
		directory = "installed"
	}
	if err := os.RemoveAll(packageDir); err != nil {
		return "", err
	}
	if err := os.Rename(filepath.Join(stage, directory), packageDir); err != nil {
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

// buildClangBundle supplies native Intel macOS where upstream has no binary.
// Apple's SDK compilers bootstrap LLVM; no package manager or system install
// is changed. Limit parallelism to keep the one-time build's memory use bounded.
// A nil progress writer selects the preparation log and any available terminal;
// an explicit writer lets tests capture status without writing to the user's TTY.
func buildClangBundle(source, build, destination string, progress io.Writer) error {
	// Preparation redirects stderr to tools.log. Mirror only short status lines
	// to the controlling terminal, leaving the full compiler output in the log.
	// Direct terminal invocations already display stderr; CI may have no terminal.
	if progress == nil {
		progress = os.Stderr
		if info, err := os.Stderr.Stat(); err == nil && info.Mode()&os.ModeCharDevice == 0 && runtime.GOOS != "windows" {
			if terminal, err := os.OpenFile("/dev/tty", os.O_WRONLY, 0); err == nil {
				defer terminal.Close()
				progress = io.MultiWriter(os.Stderr, terminal)
			}
		}
	}
	fmt.Fprintln(progress, "Preparing LLVM for Intel macOS from source. This can take many minutes or longer; detailed compiler output is in the preparation log.")
	fmt.Fprintln(progress, "The completed compiler is cached under this worktree's temp/tools and reused on later runs. A new version or a missing cache requires another build.")
	var sdkTools []string
	for _, tool := range []string{"clang", "clang++"} {
		output, err := exec.Command("xcrun", "--find", tool).CombinedOutput()
		if err != nil {
			fmt.Fprintf(progress, "LLVM source build: locating SDK %s failed.\n", tool)
			return fmt.Errorf("locate Apple SDK %s: %w: %s", tool, err, output)
		}
		sdkTools = append(sdkTools, strings.TrimSpace(string(output)))
	}
	commands := [][]string{
		{"-S", filepath.Join(source, "llvm"), "-B", build, "-G", "Unix Makefiles",
			"-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_INSTALL_PREFIX=" + destination,
			"-DCMAKE_C_COMPILER=" + sdkTools[0], "-DCMAKE_CXX_COMPILER=" + sdkTools[1],
			"-DLLVM_ENABLE_PROJECTS=clang;clang-tools-extra;lld", "-DLLVM_TARGETS_TO_BUILD=X86;ARM;AArch64",
			"-DLLVM_DISTRIBUTION_COMPONENTS=clang;clang-resource-headers;clangd;lld;llvm-ar;llvm-ranlib;llvm-objcopy;llvm-size",
			"-DLLVM_INCLUDE_TESTS=OFF", "-DLLVM_INCLUDE_BENCHMARKS=OFF",
			"-DLLVM_ENABLE_ZLIB=OFF", "-DLLVM_ENABLE_ZSTD=OFF", "-DLLVM_ENABLE_LIBXML2=OFF"},
		{"--build", build, "--target", "distribution", "--parallel", "2"},
		{"--build", build, "--target", "install-distribution"},
	}
	// Phases describe actual work, without estimating a completion percentage.
	phases := []string{"configuration", "compilation", "installation"}
	for index, args := range commands {
		fmt.Fprintln(os.Stderr, "Building pinned Intel macOS LLVM: cmake", strings.Join(args, " "))
		command := exec.Command("cmake", args...)
		command.Stdout, command.Stderr = os.Stderr, os.Stderr
		if err := runLLVMBuildStep(command, phases[index], progress, 30*time.Second); err != nil {
			return fmt.Errorf("local LLVM build failed: %w", err)
		}
	}
	return nil
}

// runLLVMBuildStep keeps long native builds visibly alive even while a linker
// is quiet. It reaps the child and stops its timer on success and failure;
// compiler diagnostics retain the command's separate output destinations.
func runLLVMBuildStep(command *exec.Cmd, phase string, progress io.Writer, interval time.Duration) error {
	started := time.Now()
	fmt.Fprintf(progress, "LLVM source build: %s started.\n", phase)
	if err := command.Start(); err != nil {
		fmt.Fprintf(progress, "LLVM source build: %s failed to start: %v\n", phase, err)
		return err
	}
	// Waiting in one goroutine lets the caller emit periodic status without
	// buffering the compiler output or leaving a process running after failure.
	completed := make(chan error, 1)
	go func() { completed <- command.Wait() }()
	ticker := time.NewTicker(interval)
	defer ticker.Stop()
	for {
		select {
		case err := <-completed:
			if err != nil {
				fmt.Fprintf(progress, "LLVM source build: %s failed after %s: %v\n", phase, time.Since(started).Round(time.Second), err)
			} else {
				fmt.Fprintf(progress, "LLVM source build: %s finished after %s.\n", phase, time.Since(started).Round(time.Second))
			}
			return err
		case <-ticker.C:
			fmt.Fprintf(progress, "LLVM source build: %s still running (%s elapsed).\n", phase, time.Since(started).Round(time.Second))
		}
	}
}
