# Working with the Repository

<details markdown="1">
<summary>Contents</summary>

<!-- mdtoc -->

* [1. Install `opencommit` on macOS](#install-opencommit-on-macos)
  * [1.1. Prerequisites](#prerequisites)
  * [1.2. Step 1 — Get Your OpenAI API Key](#step-1--get-your-openai-api-key)
  * [1.3. Step 2 — Install OpenCommit](#step-2--install-opencommit)
  * [1.4. Step 3 — Set Up Your API Key on macOS](#step-3--set-up-your-api-key-on-macos)
  * [1.5. Step 4 — Optional Configuration](#step-4--optional-configuration)
  * [1.6. Step 5 — Use OpenCommit](#step-5--use-opencommit)
  * [1.7. Step 6 — (Optional) Install Git Hook](#step-6--optional-install-git-hook)
  * [1.8. Step 7 — Troubleshooting](#step-7--troubleshooting)
* [2. Install `opencommit` on Windows](#install-opencommit-on-windows)
  * [2.1. Overview](#overview)
  * [2.2. Prerequisites](#prerequisites-1)
  * [2.3. Installation Steps](#installation-steps)
  * [2.4. Usage](#usage)
  * [2.5. Troubleshooting](#troubleshooting)
  * [2.6. Example](#example)
* [3. Fork It (get a contributor)](#fork-it-get-a-contributor)
  * [3.1. What “forking” means](#what-forking-means)
  * [3.2. How to Fork (GitHub)](#how-to-fork-github)
* [4. Clone It](#clone-it)

<!-- numbering=true min=2 max=4 slug=github anchor=true link=true toc=true bullets=auto -->
<!-- /mdtoc -->

</details>

Repository work is optional for firmware users. For installation and first use follow the [UM](../TriceUserManual.md). OpenCommit below is an optional contributor tool, not a Trice prerequisite.

Action                                    | Command
-------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Get a local repository copy.              | `git clone github.com/rokath/trice.git trice`
Show current folder                       | `pwd`
Show repository status.                   | `git status`
Clean the repo, if needed.                | `git stash push`
Show all branches.                        | `git branch -a`
Switch to main.                           | `git switch main`
Fetch a pull request as new branch PRIDa. | `git fetch origin pull/ID/head:PRIDa`
List worktree.                            | `git worktree list`
Add to worktree.                          | `git worktree add ../trice_wt_PRIDa PRIDa`
Add branch dev to worktree                | `git worktree add ../trice-dev dev`
Rstore the repo if needed.                | `git stash pop`
Change to new folder.                     | `cd ../trice_wt_PRIDa`
Show repository status.                   | `git status`
Test pull request.                        | `./scripts/testAll.sh full`
Show repository status.                   | `git status`
Clean pull request.                       | `git restore .`
Change to previous folder.                | `cd -`
Delete worktree branch.                   | `git worktree remove ../trice_wt_PRIDa`
Delete git branch.                        | `git branch -d PRIDa`
Log last 3 commits in branch maste        | `git log -3 main`
Checkout by hash                          | `git checkout <hash>`
One Liner Log until shortly before v1.0.0 | `git log --graph --decorate --all --pretty=format:'%C(bold yellow)%h%Creset %C(bold green)%ad%Creset %C(bold cyan)%d%Creset %C(white)%s%Creset' --date=format:'%Y-%m-%d %H:%M' --since 2025-04-01`
One Liner Log for branch `devel`          | `git log --graph --decorate devel --pretty=format:'%C(bold yellow)%h%Creset %C(bold green)%ad%Creset %C(bold cyan)%d%Creset %C(white)%s%Creset' --date=format:'%Y-%m-%d %H:%M'`
One Liner Log with author                 | `git log --graph --decorate --all --pretty=format:'%C(bold yellow)%h%Creset %C(bold green)%ad%Creset %C(bold blue)%an%Creset %C(bold cyan)%d%Creset %C(white)%s%Creset' --date=format:'%Y-%m-%d %H:%M'`
New worktree detached branch for compare  | `git worktree add --detach ../trice_9995fdc4b 9995fdc4b`
Add a special commit worktree             | `./AddWorktreeFromGitLogLineData.sh <commit-hash> <YYYY-MM-DD> <HH:MM>`
Create a bunch of worktrees               | `./AddWorktreesBetween.sh "<since-date>" "<until-date>"` or `./AddWorktreesBetween.sh <older-hash> <newer-hash>`
Delete all `trice_*` worktrees            | `cd ~/repos && rm trice_* && cd trice && git worktree prune && git worktree list`
Delete all `trice_*` branches             | ```git branch -D `git branch \| grep -E 'trice_'` ```
Show all opencommit parameter             | `oco config describe`
Show some config settings                 | `oco config get OCO_MODEL && oco config get OCO_PROMPT_MODULE && oco config get OCO_EMOJI`

## 1. <a id="install-opencommit-on-macos"></a>Install `opencommit` on macOS

* * *

### 1.1. <a id="prerequisites"></a>Prerequisites

Before you begin, make sure you have:

* Install [Homebrew](https://brew.sh) first if you don’t have it.
* **Git**:  Check if Git is installed: `git --version`
If not, install the Xcode Command Line Tools:
  *  `xcode-select --install`
*  **Node.js and npm**
  OpenCommit runs on Node.js. Check with: `node -v` and `npm -v`
  If not installed: `brew install node`
* **An OpenAI API key** (or compatible provider like OpenRouter, see below).

* * *

### 1.2. <a id="step-1--get-your-openai-api-key"></a>Step 1 — Get Your OpenAI API Key

* Go to **[https://platform.openai.com](https://platform.openai.com)**
* Log in (or sign up) using your email, Google, Microsoft, or Apple account.
* Navigate to your API Keys page: [https://platform.openai.com/account/api-keys](https://platform.openai.com/account/api-keys)
  * Click **“Create new secret key”**.
  * Give it a name (e.g., `opencommit-mac`) and copy it immediately.
    It will look like this: `sk-proj-1a2b3c4d5e6f...`
    > ⚠️ **Warning:** You’ll only see it once — store it securely (e.g., 1Password, Bitwarden).

* * *

### 1.3. <a id="step-2--install-opencommit"></a>Step 2 — Install OpenCommit

* Run the following in your Terminal: `npm install -g opencommit`
  You may see warnings about deprecated dependencies — those are safe to ignore.
* Once installed, check: `opencommit --version`

* * *

### 1.4. <a id="step-3--set-up-your-api-key-on-macos"></a>Step 3 — Set Up Your API Key on macOS

* Add your key as an environment variable: `export OPENAI_API_KEY="sk-proj-your-key-here"`
* To make it **permanent**, add it to your shell configuration file (`~/.zshrc`):
  `echo 'export OPENAI_API_KEY="sk-proj-your-key-here"' >> ~/.zshrc`
  Then reload the configuration with `source ~/.zshrc`.
* Check presence without displaying the key: `test -n "$OPENAI_API_KEY" && echo "API key is set"`
  This checks that the variable is nonempty, not that the service accepts it.

* * *

### 1.5. <a id="step-4--optional-configuration"></a>Step 4 — Optional Configuration

* You can customize OpenCommit’s behavior by setting additional environment variables, for example:

```bash
export OCO_LANG="en"           # or "de", "fr", etc.
export OCO_MODEL="gpt-5"       # or another model like "gpt-4-turbo"
export OCO_PROMPT_MODULE="conventional"
export OCO_EMOJI=true
 ```

Add these to your `~/.zshrc` for persistence.

* * *

### 1.6. <a id="step-5--use-opencommit"></a>Step 5 — Use OpenCommit

* Go to a Git repository: `cd /path/to/your/repo`
* Stage your changes: `git add .`
* Run OpenCommit: `opencommit`
* It will analyze your staged changes and automatically generate a commit message.
    Example:
    `🔍 Analyzing changes... ✅ Commit message generated: feat(api): add endpoint for user authentication`

* * *

### 1.7. <a id="step-6--optional-install-git-hook"></a>Step 6 — (Optional) Install Git Hook

* To have OpenCommit run automatically every time you commit: `npx opencommit install-hook`
* Now every time you run `git commit`, OpenCommit will propose a commit message for you.

* * *

### 1.8. <a id="step-7--troubleshooting"></a>Step 7 — Troubleshooting

If OpenCommit says:

* **“Missing API key”** → Repeat the [presence check](#step-3--set-up-your-api-key-on-macos).
* **“Unauthorized”** → Verify your key is valid or not expired.
* **“Cannot find opencommit command”** → Reinstall globally with `npm install -g opencommit`.

* * *

## 2. <a id="install-opencommit-on-windows"></a>Install `opencommit` on Windows

### 2.1. <a id="overview"></a>Overview

OpenCommit is a tool that uses AI (like GPT models) to automatically generate meaningful Git commit messages based on your code changes.

This guide explains how to install and configure OpenCommit on Windows step by step.

* * *

### 2.2. <a id="prerequisites-1"></a>Prerequisites

* Before installing OpenCommit, make sure you have:
  * Git installed: Download and install from https://git-scm.com/download/win
* Verify installation by running in PowerShell or CMD: `git --version`
* Node.js and npm (Node Package Manager) installed: Download from https://nodejs.org/en (LTS version recommended).
  * Verify:

  ```bash
  node -v
  npm -v
  ```

* An OpenAI API key (for GPT access): Get it from https://platform.openai.com/account/api-keys

* * *

### 2.3. <a id="installation-steps"></a>Installation Steps

**1. Install OpenCommit Globally**

* Open a terminal (PowerShell or CMD) and run: `npm install -g opencommit`
* Verify installation: `opencommit --version`
  If you get an error like “opencommit not recognized”, restart your terminal or ensure your npm global path is in your system PATH environment variable.

**2. Configure the API Key**

* Set your OpenAI API key as an environment variable permanently:
  * Press Win + R, type SystemPropertiesAdvanced, and press Enter.
  * Click Environment Variables...
  * Under User variables, click New, and add:
    * Variable name: OPENAI_API_KEY
    * Variable value: your_api_key_here
    * You can also customize OpenCommit’s behavior by setting additional environment variables, for example:

    ```bash
    OCO_LANG="en"            # or "de", "fr", etc.
    OCO_MODEL="gpt-4o"       # or another model like "gpt-4-turbo"
    OCO_PROMPT_MODULE="conventional"
    OCO_EMOJI=true
    ```

    * Click OK on all windows.
* Then restart PowerShell.

**3. (Optional) Configure Defaults**

* You can set up default parameters in your Git repository or globally: `opencommit config`
  * Follow the interactive prompts to define:
    * Default model (e.g., gpt-3.5-turbo)
    * Commit style
    * Language

* * *

### 2.4. <a id="usage"></a>Usage

* Once installed, simply run: `opencommit`. This will:
  * Analyze your staged changes (git diff --cached)
  * Generate an AI-powered commit message
  * Ask for confirmation before committing
* You can also use: `opencommit --no-verify` to skip confirmation and commit directly.

### 2.5. <a id="troubleshooting"></a>Troubleshooting

* Issue	Solution
  * opencommit: command not found	Ensure npm global bin path is added to your Windows PATH (e.g., C:\Users\<YourUser>\AppData\Roaming\npm).
  * Error: Missing OPENAI_API_KEY	Set your OpenAI API key as shown above.
  * Model too slow / API errors	Try setting a smaller model: opencommit config --model gpt-3.5-turbo.

* * *

### 2.6. <a id="example"></a>Example

```bash
git add .
oco
```

Output:

```txt
Generated commit message:
...
```

## 3. <a id="fork-it-get-a-contributor"></a>Fork It (get a contributor)

If you wish to get a contributor please fork the Trice repository.

### 3.1. <a id="what-forking-means"></a>What “forking” means

Forking creates **your own copy** of someone else’s repository under your account.
You can then:

* freely make changes,
* push commits to your fork,
* and later submit a **pull request** to propose changes back to the original repo.

### 3.2. <a id="how-to-fork-github"></a>How to Fork (GitHub)

**1\. Go to the repository you want to fork**

Example: `https://github.com/rokath/trice`

**2\. Click the **“Fork”** button (top-right)**

You’ll be taken to a _Create Fork_ page.

**3\. Choose options (usually leave defaults)**

* **Owner** → your GitHub account
* **Repository name** → auto-filled
* Optional: copy only the default branch

Click **Create Fork**.

**4\. Clone your fork locally**

`git clone https://github.com/YOUR_USERNAME/trice.git && cd trice`

**5\. (Optional but recommended) Add the original repo as `upstream`**

This lets you pull updates later.

`git remote add upstream https://github.com/rokath/trice.git`

Check remotes:

`git remote -v`

**6\. Keep your fork updated**

Run `git fetch upstream`, then `git merge upstream/main` on the branch you want to update.

Or:

`git pull upstream main`

## 4. <a id="clone-it"></a>Clone It

**1\. Make sure Git is installed**

Check with:

`git --version`

If not installed, download from [https://git-scm.com](https://git-scm.com/)

**2\. Clone the repository**

Run this command in your terminal or command prompt:

`git clone https://github.com/rokath/trice.git`

This creates a local folder named **trice** with the full project history.

**3\. (Optional) Enter the project folder**

`cd trice`

