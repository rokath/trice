# Trice Maintenance

<details markdown="1">
<summary>Contents</summary>

---

<!-- mdtoc -->

* [1. Trice Project structure (Files and Folders)](#trice-project-structure-files-and-folders)
* [2. 📁 The .github Folder — Purpose and Contents](#the-github-folder--purpose-and-contents)
  * [2.1. 📁 .github Root](#github-root)
  * [2.2. 📂 .github/workflows — GitHub Actions Workflows](#githubworkflows--github-actions-workflows)
  * [2.3. GitHub Action clang-format.yml - Check C Code Formatting](#github-action-clang-formatyml---check-c-code-formatting)
  * [2.4. GitHub Action codeql.yml - Static Code Analysis](#github-action-codeqlyml---static-code-analysis)
  * [2.5. GitHub Action coverage.yml - Test Coverage and Coveralls Integration](#github-action-coverageyml---test-coverage-and-coveralls-integration)
  * [2.6. GitHub Action go.yml - Building and Testing Go Code](#github-action-goyml---building-and-testing-go-code)
  * [2.7. GitHub Action goreleaser.yml - Build & Pack Trice Distribution](#github-action-goreleaseryml---build--pack-trice-distribution)
  * [2.8. GitHub Action label.yml - Automatic Labeling Rules](#github-action-labelyml---automatic-labeling-rules)
  * [2.9. GitHub Action link-check.yml - Broken Links Check](#github-action-link-checkyml---broken-links-check)
  * [2.10. GitHub Action manual.ym - To Be Triggered Manually](#github-action-manualym---to-be-triggered-manually)
  * [2.11. GitHub Action shellcheck.yml - Catching Common Bash Scripts Bugs](#github-action-shellcheckyml---catching-common-bash-scripts-bugs)
  * [2.12. GitHub Action shfmt.yml - Ensure Consistent Shell Scripts Formatting](#github-action-shfmtyml---ensure-consistent-shell-scripts-formatting)
  * [2.13. GitHub Action stale.yml - Automatic Stale Issue Handling](#github-action-staleyml---automatic-stale-issue-handling)
  * [2.14. GitHub Action superlinter.yml - Ensure Consistent YAML and Markdown Formatting](#github-action-superlinteryml---ensure-consistent-yaml-and-markdown-formatting)
  * [2.15. GitHub Action pages.yml - Creates The Trice GitHub Pages](#github-action-pagesyml---creates-the-trice-github-pages)
* [3. Trice Reference Manual Maintenance (or any *.md file)](#trice-reference-manual-maintenance-or-any-md-file)
* [4. Cleaning the Sources](#cleaning-the-sources)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

---

</details>

## 1. <a id="trice-project-structure-files-and-folders"></a>Trice Project structure (Files and Folders)

| Trice Root Folder File                                                                                                  | Details                                                                                                                           |
|-------------------------------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------|
| [.clang-format](../../.clang-format)                                                                                       | See [GitHub Action clang-format.yml - Check C Code Formatting](#github-action-clang-formatyml---check-c-code-formatting)          |
| [.clang-format-ignore](../../.clang-format-ignore)                                                                         | See [GitHub Action clang-format.yml - Check C Code Formatting](#github-action-clang-formatyml---check-c-code-formatting)          |
| [.code_snippets](../../.code_snippets)                                                                                     | Some legacy helper code for copying where to use                                                                                  |
| [.editorconfig](../../.editorconfig)                                                                                       | See [GitHub Action clang-format.yml - Check C Code Formatting](#github-action-clang-formatyml---check-c-code-formatting)          |
| `.git/`                                                                                                                 | Git repository metadata (exists locally after cloning; not part of the repository content)                                        |
| [.gitattributes](../../.gitattributes)                                                                                     | See [GitHub Action clang-format.yml - Check C Code Formatting](#github-action-clang-formatyml---check-c-code-formatting)          |
| [.github/](../../.github/)                                                                                                 | [📁 The .github Folder — Purpose and Contents](#the-github-folder--purpose-and-contents)                                          |
| [.gitignore](../../.gitignore)                                                                                             | git ignores these files                                                                                                           |
| [.goreleaser.yaml](../../.goreleaser.yaml)                                                                                 | goreleaser configuration                                                                                                          |
| [.idea/](../../.idea/)                                                                                                     | GoLand settings                                                                                                                   |
| [lychee.toml](../../lychee.toml)                                                                                           | [GitHub Action link-check.yml - Broken Links Check](#github-action-link-checkyml---broken-links-check)                            |
| [.markdownlint.yaml](../../.markdownlint.yaml)                                                                             | [Cleaning the Sources](#cleaning-the-sources)                                                                                     |
| [.markdownlintignore](../../.markdownlintignore)                                                                           | [Cleaning the Sources](#cleaning-the-sources)                                                                                     |
| [.vscode/](../../.vscode/)                                                                                                 | VS Code settings                                                                                                                  |
| [AUTHORS.md](../../AUTHORS.md)                                                                                             | contributors                                                                                                                      |
| [CHANGELOG.md](../../CHANGELOG.md)                                                                                         | History                                                                                                                           |
| [CODE_OF_CONDUCT.md](../../CODE_OF_CONDUCT.md)                                                                             | How to communicate                                                                                                                |
| [CONTRIBUTING.md](../../CONTRIBUTING.md)                                                                                   | Helper                                                                                                                            |
| [LICENSE.md](../../LICENSE.md)                                                                                             | [MIT](https://opensource.org/license/mit)                                                                                         |
| [README.md](../../README.md)                                                                                               | GitHub first page                                                                                                                 |
| [_config.yml](../../_config.yml)                                                                                           | [jekyll configuration](https://jekyllrb.com/docs/configuration/)                                                                  |
| [_test](../../_test)                                                                                                       | automatic target code tests                                                                                                       |
| [scripts/buildTriceTool.sh](../../scripts/buildTriceTool.sh)                                                               | [Build Trice tool from Go sources](tool_build_release.md#build-trice-tool-from-go-sources)                                                             |
| [scripts/_150_setup_build_environment.sh](../../scripts/_150_setup_build_environment.sh)                                   | see inside                                                                                                                        |
| [scripts/_280_format_c_code.sh](../../scripts/_280_format_c_code.sh)                                                       | See [GitHub Action clang-format.yml - Check C Code Formatting](#github-action-clang-formatyml---check-c-code-formatting)          |
| [scripts/_300_clean_dsstore.sh](../../scripts/_300_clean_dsstore.sh)                                                       | Run to remove macOS artifacts                                                                                                     |
| `temp/log/coverage.out`                                                                                                 | Go test coverage output                                                                                                           |


| [cmd/clang-filter](../../cmd/clang-filter)                                                                                 | [ReadMe](../../cmd/clang-filter/ReadMe.md)                                                                                           |
| [cmd/trice](../../cmd/trice)                                                                                               | Trice tool command Go sources                                                                                                     |
| `demoLI.json`                                                                                                            | locally generated location information; see [LI initialization](decoding.md#trice-location-information-file-lijson)                   |
| [demoTIL.json](../../demoTIL.json)                                                                                         | Trice ID list example                                                                                                             |
| `dist/`                                                                                                                 | local distribution files folder created by GoReleaser                                                                             |
| [docs](..)                                                                                                         | Guide, User Manual, Reference Manual, and supporting material; start with the [Trice Guide](../TriceGuide.md)                          |
| [examples/](../../examples)                                                                                                | example target projects                                                                                                           |
| [scripts/_310_refresh_trice_user_manual.sh](../../scripts/_310_refresh_trice_user_manual.sh)                               | [Trice Reference Manual Maintenance (or any `*.md` file)](#trice-reference-manual-maintenance-or-any-md-file)                               |
| [scripts/gitAddWorktreeFromGitLogLineData.sh](../../scripts/gitAddWorktreeFromGitLogLineData.sh)                           | helper to get easy a git worktree folder from any git hash for easy folder compare, see inside                                    |
| [scripts/gitAddWorktreesBetween.sh](../../scripts/gitAddWorktreesBetween.sh)                                               | helper to get easy git worktree folders from any time range                                                                       |
| [scripts/gitLogWithBranches.sh](../../scripts/gitLogWithBranches.sh)                                                       | helper to get easy a history view                                                                                                 |
| [go.mod](../../go.mod)                                                                                                     | Go modules file                                                                                                                   |
| [go.sum](../../go.sum)                                                                                                     | Go modules sums                                                                                                                   |
| [index.md](../../index.md)                                                                                                 | Jekyll index site for README.md                                                                                                   |
| [internal/](../../internal)                                                                                                | Trice tool internal Go packages                                                                                                   |
| [pkg/](../../pkg)                                                                                                          | Trice tool common Go packages                                                                                                     |
| [scripts/_330_renew_ids_and_refresh_tests.sh](../../scripts/_330_renew_ids_and_refresh_tests.sh)                           | renew all ID data                                                                                                                 |
| [src/](../../src)                                                                                                          | C sources for trice instrumentation -> Add to target project                                                                      |
| `temp/`                                                                                                                 | ignored local workspace for binary logfiles and other runtime artifacts like `scripts/testAll.sh` helper files under `./temp/log` |
| `testAll.log`                                                                                                           | ignored local output of the last `./scripts/testAll.sh` run                                                                       |
| [scripts/testAll.sh](../../scripts/testAll.sh)                                                                             | run all tests                                                                                                                     |
| [third_party/](../../third_party)                                                                                          | external components                                                                                                               |
| [trice_bindIDs_in_examples_and_test_folder.sh](../../trice_bindIDs_in_examples_and_test_folder.sh)                         | run the canonical Trice Bind workflow                                                                                              |
| [scripts/_240_legacy_clean_ids.sh](../../scripts/_240_legacy_clean_ids.sh)                                                 | [Cleaning the Sources](#cleaning-the-sources)  [Activating the Trice Cache](insert_clean_cache.md#activating-the-trice-cache)                          |
| [scripts/_120_setup_trice_environment.sh](../../scripts/_120_setup_trice_environment.sh)                                   | [Cleaning the Sources](#cleaning-the-sources)  [Activating the Trice Cache](insert_clean_cache.md#activating-the-trice-cache)                          |
| [scripts/_230_legacy_insert_ids.sh](../../scripts/_230_legacy_insert_ids.sh)                                               | [Cleaning the Sources](#cleaning-the-sources)  [Activating the Trice Cache](insert_clean_cache.md#activating-the-trice-cache)                          |

<p align="right">(<a href="#trice-maintenance">back to top</a>)</p>

## 2. <a id="the-github-folder--purpose-and-contents"></a>📁 The .github Folder — Purpose and Contents

GitHub automatically recognizes and uses everything contained inside the [.github/](../../.github/) directory.
This folder defines how the project behaves on GitHub: issue templates, automated workflows, labels, code scanning, greetings, and release automation. Details:

### 2.1. <a id="github-root"></a>📁 `.github` Root

It contains issue templates, labels, workflow automation, code scanning, linting, and the CI/CD release pipeline.

* [ISSUE_TEMPLATE/](../../.github/ISSUE_TEMPLATE/) Used to create structured bug reports and feature requests.
* [FUNDING.yml](../../.github/FUNDING.yml) Enables the GitHub “Sponsor” button.
* [labeler.yml](../../.github/labeler.yml) configuration file consumed by actions/labeler. It defines the labeling rules.
  * Because labeler.yml is configuration and not an executable workflow, it is placed directly under .github/, not under .github/workflows/.
  * The actions/labeler action expects .github/labeler.yml as the default configuration path, which is why this layout is standard and correct.
  * This is intentional and follows GitHub Actions conventions.
    * .github/workflows/label.yml is a workflow. It defines when and how the GitHub Action runs (triggers, permissions, runner, action reference).
    * .github/labeler.yml is not a workflow. It is a configuration file consumed by actions/labeler. It defines the labeling rules.
* [.github/workflows/](../../.github/workflows) Contains GitHub Actions automation



### 2.2. <a id="githubworkflows--github-actions-workflows"></a>📂 `.github/workflows` — GitHub Actions Workflows

The [.github/workflows/](../../.github/workflows/) folder contains YAML descriptions for various actions, which will be triggered automatically on certain events or are started manually.
Every *yml* file in this directory defines an automated process. These processes run on GitHub’s servers (CI/CD).

* [README.md](../../.github/workflows/README.md): Documentation specifically for the /workflows folder.
  * Can be useful for contributors who want to understand or modify CI behaviors.
Each workflow defines its own triggers and permissions in its YAML file. Depending on the workflow, it runs on pushes, pull requests, a schedule, a manual request, or a call from another workflow. The directory contains no separate icon or properties metadata for configuring these workflows.
 
| GitHub Action                                             | About                                                                                                                                                            |
|-----------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| [clang-format.yml](../../.github/workflows/clang-format.yml) | [GitHub Action clang-format.yml - Check C Code Formatting](#github-action-clang-formatyml---check-c-code-formatting)                                             |
| [codeql.yml](../../.github/workflows/codeql.yml)             | [GitHub Action codeql.yml - Static Code Analysis](#github-action-codeqlyml---static-code-analysis)                                                               |
| [coverage.yml](../../.github/workflows/coverage.yml)         | [GitHub Action coverage.yml - Test Coverage and Coveralls Integration](#github-action-coverageyml---test-coverage-and-coveralls-integration)                     |
| [go.yml](../../.github/workflows/go.yml)                     | [GitHub Action go.yml - Building and Testing Go Code](#github-action-goyml---building-and-testing-go-code)                                                       |
| [goreleaser.yml](../../.github/workflows/goreleaser.yml)     | [GitHub Action goreleaser.yml - Build & Pack Trice Distribution](#github-action-goreleaseryml---build--pack-trice-distribution)                                  |
| [label.yml](../../.github/workflows/label.yml)               | [GitHub Action label.yml - Automatic Labeling Rules](#github-action-labelyml---automatic-labeling-rules)                                                         |
| [link-check.yml](../../.github/workflows/link-check.yml)     | [GitHub Action link-check.yml - Broken Links Check](#github-action-link-checkyml---broken-links-check)                                                           |
| [shellcheck.yml](../../.github/workflows/shellcheck.yml)     | [GitHub Action shellcheck.yml - Catching Common Bash Scripts Bugs](#github-action-shellcheckyml---catching-common-bash-scripts-bugs)                             |
| [shfmt.yml](../../.github/workflows/shfmt.yml)               | [GitHub Action shfmt.yml - Ensure Consistent Shell Scripts Formatting](#github-action-shfmtyml---ensure-consistent-shell-scripts-formatting)                     |
| [stale.yml](../../.github/workflows/stale.yml)               | [GitHub Action stale.yml - Automatic Stale Issue Handling](#github-action-staleyml---automatic-stale-issue-handling)                                             |
| [superlinter.ym](../../.github/workflows/superlinter.yml)    | [GitHub Action superlinter.yml - Ensure Consistent YAML and Markdown Formatting](#github-action-superlinteryml---ensure-consistent-yaml-and-markdown-formatting) |
| [pages.yml](../../.github/workflows/pages.yml)               | [GitHub Action pages.yml - Creates The Trice GitHub Pages](#github-action-pagesyml---creates-the-trice-github-pages)                                             |

### 2.3. <a id="github-action-clang-formatyml---check-c-code-formatting"></a>GitHub Action clang-format.yml - Check C Code Formatting

* **Local Action (developer machine):** [./scripts/_280_format_c_code.sh](../../scripts/_280_format_c_code.sh) - adjust all C files excluding [.clang-format-ignore](../../.clang-format-ignore) according rule set in [.clang-format](../../.clang-format).
  > The file [./scripts/_280_format_c_code.sh](../../scripts/_280_format_c_code.sh) is used to auto-format the Trice code.
  > 
  > File [.clang-format](../../.clang-format)
  > 
  > *Contributor: @Sazerac4*
  > 
  > Sazerac4 commented Aug 29, 2024:
  > I have a code formatter when I make changes to my application but I would like to keep the style of the library when modifying.
  > I couldn't find a code formatter, is there a tool used? If not, I propose this to provide one as an example by using clang-format.
  > 
  > ```bash
  > ## I have created a default style :
  > clang-format -style=llvm -dump-config > .clang-format
  > ## Then format the code:
  > find ./src  -name '*.c' -o  -name '*.h'| xargs clang-format-18 -style=file -i
  > ```
  > 
  > The style of the example does not correspond to the original one. Configurations are necessary for this to be the case. Tags can be placed to prevent certain macros from being formatted
  > 
  > ```C
  > int formatted_code;
  > // clang-format off
  >     void    unformatted_code  ;
  > // clang-format on
  > void formatted_code_again;
  > ```
  > 
  > I have tuned some settings for clang-format :
  > 
  > ```bash
  > * IndentWidth: 4  // original code size indentation
  > * ColumnLimit: 0  // avoid breaking long line (like macros)
  > * PointerAlignment: Left  // like original files (mostly)
  > ```
  > 
  > With preprocessor indentation, the result is a bit strange in some cases. It's possible with the option IndentPPDirectives ([doc](https://releases.llvm.org/18.1.6/tools/clang/docs/ClangFormatStyleOptions.html)).
  > 
  > Staying as close as possible to a default version (LLVM in this case) makes it easier to regenerate the style if necessary.
  > 
  > See also: https://github.com/rokath/trice/pull/487#issuecomment-2318003072
  > 
  > File [.clang-format-ignore](../../.clang-format-ignore):
  > 
  > *Contributor: @Sazerac4*
  > 
  > Sazerac4 commented Aug 30, 2024:
  > I have added .clang-format-ignore to ignore formatting for specific files
  > 
  > File [.editorconfig](../../.editorconfig):
  > 
  > *Contributor: @Sazerac4*
  > 
  > The`.editorconfig` file allows to better identify the basic style for every files. (endline, charset, ...). It is a file accepted by a wide list of IDEs and editors : [link](https://editorconfig.org/#file-format-details)
  > This addition is motivated by forgetting the end of line in the .gitattributes file.
  > 
  > File [.gitattributes](../../.gitattributes)
  > 
  > *Contributor: @Sazerac4*
  > 
  > With the`.gitattributes` file avoid problems with "diff" and end of lines. [Here](https://www.aleksandrhovhannisyan.com/blog/crlf-vs-lf-normalizing-line-endings-in-git/) is an article that presents the problem.
  > 
  > To fill the`.gitattributes`, I used the command below to view all the extensions currently used.
  > 
  > ```bash
  > git ls-tree -r HEAD --name-only | perl -ne 'print $1 if m/\.([^.\/]+)$/' | sort -u
  > ```
* **GitHub Action (Continuous Integration):** [.github/workflows/clang-format.yml](../../.github/workflows/clang-format.yml) does not format, it only checks.

### 2.4. <a id="github-action-codeqlyml---static-code-analysis"></a>GitHub Action codeql.yml - Static Code Analysis

* This workflow runs CodeQL, GitHub’s static code analysis tool. Purpose:
  * scan the codebase for potential security vulnerabilities
  * detect unsafe code patterns
  * provide security alerts in the “Security” tab
  * Runs automatically on pushes and pull requests.
* **GitHub Action (Continuous Integration):** [.github/workflows/codeql.yml](../../.github/workflows/codeql.yml)
  * This workflow configures **GitHub CodeQL code scanning** for the repository. 
  * This workflow continuously scans the codebase for security and quality problems using CodeQL.
  * It performs **static code analysis** on source code.
  * It looks for:
    * Security vulnerabilities (e.g. injection flaws, unsafe API usage)
    * Common programming errors
    * Code quality issues     
  *  It runs automatically:
    * On pushes to `main`
    * On pull requests targeting `main`
    * Manually via the GitHub Actions UI 
  * The results are uploaded to GitHub and appear under **Security → Code scanning alerts**.
  * The “QL” is the same concept as SQL, but specialized for code analysis.

### 2.5. <a id="github-action-coverageyml---test-coverage-and-coveralls-integration"></a>GitHub Action coverage.yml - Test Coverage and Coveralls Integration

Trice uses Go’s built-in coverage tooling to measure how much of the Go codebase is exercised by automated tests.

* **Local Action (developer machine):** 

| Action                                        | Command                                                                 |
|-----------------------------------------------|-------------------------------------------------------------------------|
| Generate a coverage profile locally           | `go test ./... -covermode=atomic -coverprofile=./temp/log/coverage.out` |
| Show results as list in terminal              | `go tool cover -func=./temp/log/coverage.out`                           |
| Show results colored file specific in browser | `go tool cover -html=./temp/log/coverage.out`                           |

* **GitHub Action (Continuous Integration):**  
On GitHub, the workflow `.github/workflows/coverage.yml` runs automatically for every pull request and also on a monthly schedule. The workflow:
  1.  executes `go test` with coverage enabled,
  2.  prints a coverage summary in the CI logs,
  3.  uploads the raw `coverage.out` file as a workflow artifact, and
  4.  publishes the coverage results to Coveralls (if configured).
    
**Coverage badge:**  
The README displays the current coverage status for the default branch using the Coveralls badge:

`[![Coverage Status](https://coveralls.io/repos/github/rokath/trice/badge.svg?branch=master)](https://coveralls.io/github/rokath/trice?branch=master)`

This badge is updated whenever the CI workflow successfully uploads a new coverage report for the `master` branch.

### 2.6. <a id="github-action-goyml---building-and-testing-go-code"></a>GitHub Action go.yml - Building and Testing Go Code

* A workflow for building and testing Go code. It runs GitHub CodeQL for Go and C (cpp language pack), detecting security vulnerabilities and code issues.
  * Typically includes steps such as:
  * setting up the Go toolchain
  * checking out the repository
  * compiling the project
  * running unit tests
* **Local Action (developer machine):** `go test ./...` or better `./scripts/testAll.sh full` (takes long)
* **GitHub Action (Continuous Integration):** [.github/workflows/go.yml](../../.github/workflows/go.yml)

### 2.7. <a id="github-action-goreleaseryml---build--pack-trice-distribution"></a>GitHub Action goreleaser.yml - Build & Pack Trice Distribution

This workflow runs GoReleaser, the tool that builds and packages Trice for distribution.
  * Purpose:
    * create release binaries for all supported platforms
    * generate archives (ZIP, tar.gz, etc.)
    * compute checksums
    * create a GitHub Release with all artifacts
  * Triggers:
    * manually from the GitHub UI (“Run workflow”)
    * automatically when pushing a tag matching v* (e.g., v0.44.0)
    * This is the workflow responsible for generating official Trice releases.
* **Local Action (developer machine):** `goreleaser`
* **GitHub Action (Continuous Integration):** [.github/workflows/goreleaser.yml](../../.github/workflows/goreleaser.yml)

See also [Publish a release manually from a selected tag](tool_build_release.md#publish-a-release-manually-from-a-selected-tag)

### 2.8. <a id="github-action-labelyml---automatic-labeling-rules"></a>GitHub Action label.yml - Automatic Labeling Rules

* Defines automatic labeling rules for issues and PRs.
  * For example, files in certain directories may automatically get category labels.
  * This helps maintainers classify submissions more easily.
* **GitHub Action (Continuous Integration):** [.github/workflows/label.yml](../../.github/workflows/label.yml)

### 2.9. <a id="github-action-link-checkyml---broken-links-check"></a>GitHub Action link-check.yml - Broken Links Check

* **Local Action (developer machine):** `./scripts/_530_test_links.sh` from the repository root, also included in `./scripts/testAll.sh`.
  * Uses [lychee.toml](../../lychee.toml) as configuration; the direct checker command is `lychee --config lychee.toml .`.
  * The script additionally maps the CLI-help source URL to the local checkout and records results in `temp/log/_530_test_links.log`. If Lychee is missing, it reports a skip.
  * For GitHub URLs, set `GITHUB_TOKEN` or `GH_TOKEN` locally as well to reduce API throttling and timeouts during checks
* **GitHub Action (Continuous Integration):** [.github/workflows/link-check.yml](../../.github/workflows/link-check.yml)
  * The workflow already provides `GITHUB_TOKEN` to the Lychee action for GitHub-hosted links

### 2.10. <a id="github-action-manualym---to-be-triggered-manually"></a>GitHub Action manual.ym - To Be Triggered Manually

A workflow that is designed to be triggered manually (similar to _workflow_dispatch_ workflows). Common use cases:
  * executing maintenance tasks
  * running scripts on demand
  * testing workflow behavior without making a commit
  * This workflow does not run automatically.

### 2.11. <a id="github-action-shellcheckyml---catching-common-bash-scripts-bugs"></a>GitHub Action shellcheck.yml - Catching Common Bash Scripts Bugs

Runs ShellCheck on all *.sh files, catching common bugs in Bash scripts.

* **GitHub Action (Continuous Integration):** [.github/workflows/shellcheck.yml](../../.github/workflows/shellcheck.yml)

### 2.12. <a id="github-action-shfmtyml---ensure-consistent-shell-scripts-formatting"></a>GitHub Action shfmt.yml - Ensure Consistent Shell Scripts Formatting

Runs shfmt in diff mode on pull requests to ensure consistent formatting of shell scripts.

* **Local Action (developer machine):** `go test ./...` or better `./scripts/testAll.sh full` (takes long)
* **GitHub Action (Continuous Integration):** [.github/workflows/shfmt.yml](../../.github/workflows/shfmt.yml)

### 2.13. <a id="github-action-staleyml---automatic-stale-issue-handling"></a>GitHub Action stale.yml - Automatic Stale Issue Handling

Automates stale issue handling. Function:
  * marks inactive issues or PRs as “stale”
  * closes them after a configurable time period if there is no further activity
  * eps the issue tracker manageable.

Mark stale issues and pull requests

* **GitHub Action (Continuous Integration):** [.github/workflows/stale.yml](../../.github/workflows/stale.yml)

### 2.14. <a id="github-action-superlinteryml---ensure-consistent-yaml-and-markdown-formatting"></a>GitHub Action superlinter.yml - Ensure Consistent YAML and Markdown Formatting

* **Local Action (developer machine):** `markdownlint .`
* Runs GitHub Super Linter, a powerful linting suite. Purpose:
  * ensure consistent code formatting
  * detect stylistic issues
  * catch potential errors in supported languages
  * Helps maintain code quality across the entire repository.
* **GitHub Action (Continuous Integration):** [.github/workflows/superlinter.yml](../../.github/workflows/superlinter.yml)
  * Checks YAML and Markdown files

### 2.15. <a id="github-action-pagesyml---creates-the-trice-github-pages"></a>GitHub Action pages.yml - Creates The Trice GitHub Pages

This workflow creates the Trice github pages avaliable under [rokath.github.io/trice/](https://rokath.github.io/trice/).

* **GitHub Action (Continuous Integration):** [.github/workflows/pages.yml](../../.github/workflows/pages.yml)

## 3. <a id="trice-reference-manual-maintenance-or-any-md-file"></a>Trice Reference Manual Maintenance (or any `*.md` file)

* Recommended Tool: VS Code with some extensions:
  * Markdown All in One (Yu Zhang)
  * Markdown Table Prettifier (Kristin Daroczi)
    * Click in mouse context menu the entry "Format Document" to adjust Markdown tables.
  * Markdown Preview Enhanced
    * Click the preview button in the top right.
  * `mdtoc`
    * The checked-in manual already contains the managed TOC container and persisted config. Keep these markers in place where the table of contents should be generated.
    ```md
    <!-- mdtoc -->
    <!-- mdtoc-config
    container-version=v2
    numbering=true
    min-level=2
    max-level=4
    anchor=github
    toc=true
    bullets=auto
    state=stripped
    -->
    <!-- /mdtoc -->
    ```
    * Run `./scripts/_310_refresh_trice_user_manual.sh format` to regenerate the reference manual's TOC, numbering, and anchors with `mdtoc`.
    * Run `./scripts/_310_refresh_trice_user_manual.sh check` to verify that the checked-in reference manual matches the persisted `mdtoc` state.
    * The repository no longer uses a VS Code TOC extension for manual maintenance.
  * Markdown Paste (telesoho)
    * Helpful to get web site content preformatted as Markdown. Use mouse context menu.
  * markdownlint (David Anson)
    * Uses [.markdownlint.yaml](../../.markdownlint.yaml) as rule set and [.markdownlintignore](../../.markdownlintignore) to ignore files.
    * You can also run `markdownlint .` in a terminal after installing the CLI tool. It uses the same control files.
    * The GitHub action [superlinter.yml](../../.github/workflows/superlinter.yml) uses [.markdownlint.yaml](../../.markdownlint.yaml) and the same config files.
  * Markdown PDF (yzane)
    * Use Shift-Command-P "markdown PDF:export" to generate a PDF
    * page break for PDF generation: `<div style="page-break-before: always;"></div>`

## 4. <a id="cleaning-the-sources"></a>Cleaning the Sources

In GitHub are some Actions defined. Some of them get triggered on a `git push` and perform some checks. To get no fail, some scripts should run before committing:

* `npx markdownlint *.md` (or just `markdownlint`) - uses [.markdownlint.yaml](../../.markdownlint.yaml) to allow exceptions.
  * `npx markdownlint ./docs/rm/*.md 2>&1 | awk '!seen[$2]++'` for example to reduce message count in case of errors.
* [./scripts/_300_clean_dsstore.sh](../../scripts/_300_clean_dsstore.sh) - remove macOS maintenance data.
* [./scripts/_240_legacy_clean_ids.sh](../../scripts/_240_legacy_clean_ids.sh) removes all IDs in the legacy workflow.

<p align="right">(<a href="#trice-maintenance">back to top</a>)</p>

