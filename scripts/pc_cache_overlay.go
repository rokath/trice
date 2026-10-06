// SPDX-License-Identifier: MIT

//go:build ignore

// This internal tool runs from _test and prepares content-stamped CGO overlays.
// Go does not otherwise notice changes to C files included outside a package.
package main

import (
	"crypto/sha256"
	"encoding/json"
	"flag"
	"fmt"
	"go/parser"
	"go/token"
	"io/fs"
	"os"
	"path/filepath"
	"strconv"
	"strings"
)

// treeDigest hashes names and bytes, never timestamps. WalkDir sorts paths, so
// additions, removals and renames invalidate the digest on every supported OS.
// Symlinks are rejected rather than silently omitting dependencies they hide.
func treeDigest(dir string) (string, error) {
	h := sha256.New()
	err := filepath.WalkDir(dir, func(path string, entry fs.DirEntry, err error) error {
		if err != nil {
			return err
		}
		if entry.Type()&os.ModeSymlink != 0 {
			return fmt.Errorf("unsupported symbolic link in PC cache inputs: %s", path)
		}
		if entry.IsDir() {
			return nil
		}
		if !entry.Type().IsRegular() {
			return fmt.Errorf("unsupported non-regular PC cache input: %s", path)
		}
		data, err := os.ReadFile(path)
		if err != nil {
			return err
		}
		rel, err := filepath.Rel(dir, path)
		if err != nil {
			return err
		}
		name := filepath.ToSlash(rel)
		fmt.Fprintf(h, "%d:%s%d:", len(name), name, len(data))
		_, err = h.Write(data)
		return err
	})
	return fmt.Sprintf("%x", h.Sum(nil)), err
}

// prepare overlays every CGO importer, including the ABC bridges that do not
// use generated harness copies. Shared tree digests are calculated once; a
// package-local change invalidates that package rather than all configurations.
func prepare(out, inputs, workflow string) error {
	cwd, err := os.Getwd()
	if err != nil {
		return err
	}
	shared := "pc-cache-v1\nworkflow=" + workflow + "\n"
	for _, dir := range []string{filepath.Join(cwd, "..", "src"), filepath.Join(cwd, "testdata")} {
		digest, err := treeDigest(dir)
		if err != nil {
			return fmt.Errorf("read shared PC cache inputs %s: %w", dir, err)
		}
		shared += digest + "\n"
	}
	// The managed Bind workflow exports the actual sidecar directory, including
	// custom directories. Insert has no sidecar dependency. Direct worker runs
	// conservatively include the default generated tree when it exists.
	genDir := os.Getenv("TRICE_BIND_INCLUDE_DIR")
	if genDir == "" && workflow != "insert" {
		genDir = filepath.Join(cwd, "..", "generated")
		if _, err := os.Stat(genDir); os.IsNotExist(err) {
			genDir = ""
		}
	}
	if genDir != "" {
		digest, err := treeDigest(genDir)
		if err != nil {
			return fmt.Errorf("read Bind PC cache inputs %s: %w", genDir, err)
		}
		shared += "sidecars=" + digest + "\n"
	}
	// Go already keys its cache on compiler identity and CGO flags. Include the
	// surrounding header-search environment too, which Go does not fully track.
	for _, name := range []string{"CC", "CXX", "CGO_CFLAGS", "CGO_CPPFLAGS", "CGO_CXXFLAGS", "CGO_LDFLAGS", "CPATH", "C_INCLUDE_PATH", "CPLUS_INCLUDE_PATH", "SDKROOT"} {
		shared += name + "=" + strconv.Quote(os.Getenv(name)) + "\n"
	}
	inputs, err = filepath.Abs(inputs)
	if err != nil {
		return err
	}
	if err := os.MkdirAll(inputs, 0755); err != nil {
		return err
	}
	replace := map[string]string{}
	packages := 0
	err = filepath.WalkDir(cwd, func(dir string, entry fs.DirEntry, err error) error {
		if err != nil || !entry.IsDir() {
			return err
		}
		if dir != cwd && (entry.Name() == "testdata" || entry.Name() == "vendor" || strings.HasPrefix(entry.Name(), ".") || strings.HasPrefix(entry.Name(), "_")) {
			return filepath.SkipDir
		}
		files, err := filepath.Glob(filepath.Join(dir, "*.go"))
		if err != nil {
			return err
		}
		var cgoFiles []string
		for _, file := range files {
			if strings.HasSuffix(file, "_test.go") {
				continue
			}
			source := file
			if filepath.Base(file) == "generated_cgoPackage.go" {
				source = filepath.Join(cwd, "testdata", "cgoPackage.go")
			}
			parsed, err := parser.ParseFile(token.NewFileSet(), source, nil, parser.ImportsOnly)
			if err != nil {
				return fmt.Errorf("read CGO importer %s: %w", source, err)
			}
			for _, spec := range parsed.Imports {
				if spec.Path.Value == `"C"` {
					cgoFiles = append(cgoFiles, file)
				}
			}
		}
		if len(cgoFiles) == 0 {
			return nil
		}
		local, err := treeDigest(dir)
		if err != nil {
			return fmt.Errorf("read configuration PC cache inputs %s: %w", dir, err)
		}
		digest := fmt.Sprintf("%x", sha256.Sum256([]byte(shared+local)))
		packages++
		for index, file := range cgoFiles {
			source := file
			if filepath.Base(file) == "generated_cgoPackage.go" {
				source = filepath.Join(cwd, "testdata", "cgoPackage.go")
				testSource := filepath.Join(cwd, "testdata", "cgoPackage_test.go")
				if _, err := os.Stat(testSource); err != nil {
					return fmt.Errorf("read canonical harness tests: %w", err)
				}
				replace[filepath.Join(dir, "generated_cgoPackage_test.go")] = testSource
			}
			data, err := os.ReadFile(source)
			if err != nil {
				return err
			}
			// Go's ./... discovery ignores underscore-prefixed files. Retained
			// artifacts must not become extra packages during later Go tests.
			copyPath := filepath.Join(inputs, fmt.Sprintf("_package-%d-cgo-%d.go", packages, index))
			// Append after the source so line numbers and the preamble remain intact.
			data = append(data, []byte("\n// PC external input SHA256: "+digest+"\n")...)
			if err := os.WriteFile(copyPath, data, 0644); err != nil {
				return err
			}
			replace[file] = copyPath
		}
		return nil
	})
	if err != nil {
		return err
	}
	if packages == 0 {
		return fmt.Errorf("no PC-test CGO importers found under _test")
	}
	data, err := json.MarshalIndent(struct{ Replace map[string]string }{replace}, "", "  ")
	if err != nil {
		return err
	}
	if err := os.WriteFile(out, append(data, '\n'), 0644); err != nil {
		return err
	}
	fmt.Printf("PC cache overlay: %d configurations; content-stamped inputs: %s\n", packages, inputs)
	return nil
}

// main publishes the overlay only after every input has been read successfully.
// A preparation error stops the worker before any potentially stale test runs.
func main() {
	out := flag.String("out", "", "output overlay JSON")
	inputs := flag.String("inputs", "", "retained directory for stamped Go sources")
	workflow := flag.String("workflow", "current", "prepared ID workflow")
	flag.Parse()
	if *out == "" || *inputs == "" {
		fmt.Fprintln(os.Stderr, "FAIL: PC cache overlay requires -out and -inputs")
		os.Exit(1)
	}
	if err := prepare(*out, *inputs, *workflow); err != nil {
		fmt.Fprintln(os.Stderr, "FAIL: PC cache overlay:", err)
		os.Exit(1)
	}
}
