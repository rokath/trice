# Repository Agent Rules

## Source Code Changes

* All repository source files, including files inside `./src`, are protected by Git and may be modified without separate confirmation when the change is directly required by the requested task.
* Keep changes to user-owned target source code focused and reviewable, but do not stop merely to request additional permission for editing it.

---

## Scope Control

* Only modify files that are directly relevant to the requested task.
* Do not refactor, rename, or reorganize unrelated code.
* Avoid "drive-by" improvements outside the requested scope.
* If additional improvements seem beneficial, propose them separately instead of applying them automatically.
* Treat directories named `obsolete` and other explicitly archived material as read-only history, even when the user allows edits to their parent directory. Do not edit, move, delete, reindex, or repair links in archived files unless the user explicitly names the archived material as a target.
* When an active-document change leaves a stale reference inside an archive, leave the archive unchanged. Update active references within scope and report any remaining link limitation when relevant.
* Changes to `demo*.json` at the repository root are usually generated or local-test artifacts. Unless the user explicitly asks for them, ignore such worktree changes silently and do not treat them as part of the task.

## Change scope and confirmation

- Make only the changes needed for the task the user explicitly requested.
- Treat explicit user decisions, exclusions, and clarifications from earlier conversation turns as binding until the user changes them. They take precedence over repository drafts, issues, plans, and inferred implementation requirements.
- Use documentation to interpret the requested work, but never use it to revive rejected or deferred work or to expand the agreed scope.
- Do not add migration, compatibility, data-conversion, or cleanup work as an implementation detail unless the user explicitly included or approved that effort.
- Before implementation, establish the intended observable result and its scope. If any material part is not completely clear, or multiple plausible results remain, ask the user before starting the dependent implementation; do not choose an interpretation and proceed silently.
- When the user refers to a numbered item in a plan or issue list, distinguish editing that item's specification from implementing it. Requests such as "adjust A4" or "update A4" mean revise the plan text for review; change code, tests, CLI help, or user documentation only when the user explicitly asks to implement the item or names those changes.
- A decision made while discussing a planned item authorizes recording that decision in the plan when requested; it is not a start command for implementation. If the wording could reasonably mean either plan revision or implementation, ask which one the user intends before touching implementation files.
- Do not perform opportunistic refactors, broad cleanups, renames, rewrites, or diagnostic-output removals unless the user explicitly asks for them.
- Preserve existing comments, usage/help text, workflow steps, log output, and diagnostics unless changing them is required for the requested task.
- If you see a useful improvement outside the requested scope, describe it and ask before implementing it.
- Prefer focused patches over wholesale rewrites. Keep unrelated formatting churn out of the change.
- In CI, release, and packaging scripts, prefer actionable failure messages over terse `test` failures. Missing files, empty files, and unmatched globs should report the expected path or pattern and print nearby context such as generated artifact lists and relevant tool versions.

---

## Autonomy and Approvals

* Work continuously through the requested task and do not pause for routine, piecemeal permission requests.
* All read-only operations and all task-relevant builds, formatting commands, diagnostics, and tests are pre-approved and must not require separate user confirmation.
* Never ask the user to approve or reconfirm task-relevant tests, reruns, coverage, test builds, or the existing pinned dependencies and caches needed for them. This standing authorization includes local TCP ports and network access required by those checks. Reuse existing approved execution paths and command prefixes; do not introduce another approval question merely by changing cache paths or command wrappers. If the platform unavoidably enforces an approval, use only its technical mechanism, without a separate conversational question, and never claim that AGENTS.md can disable that enforcement.
* Task-relevant edits to version-controlled files are pre-approved, including source files in `./src`; do not request a special file or directory permission.
* Pause only for a genuinely important ambiguous decision, an unrecoverable or destructive action outside the clearly requested scope, or a technically enforced platform approval that cannot be avoided.
* Only `git commit` and `git push` require an explicit user instruction. Never infer either operation from a request to implement, fix, test, or format changes. A plain "commit" request means copyable commit commands only, as defined below.
* Once the user has authorized a task or confirmed an action, continue through its routine implementation steps without asking for the same permission again. Technical details such as staging related files, splitting requested commits, choosing temporary paths, or running focused checks do not require another conversational approval.
* Use existing platform approvals and approved command prefixes directly. Do not turn optional tooling choices into additional approval gates; prefer an already-authorized equivalent. If a platform prompt is unavoidable, keep independent work moving and do not request the same pending approval repeatedly.
* Task-relevant diagnostics, validation, and access to caches for existing project dependencies are covered by the task authorization. Do not request separate conversational permission to activate an already-installed tool or the project's existing pinned build environment.
* Before escalating a diagnostic, check whether the installed dependency can be read or loaded directly without its package launcher. Prefer direct read-only access or the existing authorized build environment over launcher-induced cache, lock, or log writes. An optional diagnostic must not introduce a new approval gate when an equivalent authorized check or a qualified result is available. Choosing a restricted tool does not make its optional diagnostic required. Technically enforced platform restrictions still apply; request a platform prompt only for a required action that has no equally effective authorized alternative.

---

## Switching Computers with Codex

### Tagged handovers

* Treat an exact user prompt `handsoff <tag>` as a request to create or fully refresh `docs/_scratchPad/handsoff_<tag>.md`. Replace existing contents instead of appending.
* Treat an exact user prompt `handson <tag>` as a request to load `docs/_scratchPad/handsoff_<tag>.md`, verify it against the current checkout, restore that handover as task context, and then wait for the user's next instruction. Do not resume implementation automatically.
* `<tag>` is required. Accept only a short lowercase ASCII tag matching `[a-z0-9][a-z0-9_-]*`. If the tag is missing or invalid, report the expected syntax and do not create, read, or guess a file.
* `handsoff_<tag>.md` files in `docs/_scratchPad` are intentionally version-controlled handover files. Do not add them to `.gitignore`.
* A `handsoff` file must be concise and contain only information useful for continuing the work on another computer:
  - current goal and task
  - current Git branch and HEAD commit
  - relevant working-tree state
  - work already completed
  - binding decisions, rationale, exclusions, and important constraints
  - relevant files
  - tests/checks already performed and their exact outcomes
  - unresolved issues
  - concrete next steps
* Clearly distinguish repository state already contained in Git from uncommitted local changes, assumptions, skipped work, interrupted work, and unfinished work.
* Do not include conversational history, lengthy logs, credentials, dead-end discussions, or information that can be recovered trivially from Git.
* `handsoff` must not run `git add`, `git commit`, or `git push`. After writing the file, report its path and remind the user to add and commit the handover manually. If the handover is intended for another computer, also remind the user that the commit must be transferred, normally by pushing it. Mention any other uncommitted changes that would also need to be committed or otherwise transferred.
* On `handson <tag>`, if the requested handover file does not exist, report that briefly and stop; do not infer its contents from another handover file.
* On `handson <tag>`, compare at least the recorded branch, HEAD commit, working tree, and relevant files with the actual checkout. Briefly report material discrepancies before waiting for further instructions.

### Full computer-switch workflow

* Treat "Rechnerwechsel vorbereiten" ("prepare a computer switch") as a request to reach a recoverable stopping point. Start no new implementation or long test; let necessary cleanup and test-state restoration finish. Do not kill unrelated sessions or leave temporarily rewritten sources behind.
* For unfinished work, update one concise handover at `docs/_scratchPad/Codex_Handover.md`: goal, binding decisions and exclusions, current results, unfinished work, exact test commands and outcomes, required local-only files, and the next concrete step. Clearly distinguish passed, failed, skipped and interrupted tests. Do not put credentials into the handover.
* Inspect tracked and untracked changes and existing local commits. Commit and push only when the user explicitly requests them, for example "Rechnerwechsel vorbereiten, committen und pushen". After a requested push, verify the actual remote branch commit; local tracking refs alone do not prove that the transfer succeeded. Otherwise report the remaining Git step concisely.
* Finish with branch, commit, any remaining local-only material, and the next concrete transfer step. Use the tagged handover workflow above to transfer task context through Git; do not require users to choose a Python invocation or repeat a long manual checklist.
* The former automated profile-transfer scripts are archived and are not an active computer-switch entry point. Never export/import a personal profile while this session is still active, silently stop other Codex processes, or copy credentials/configuration to make a transfer work.
* On "Rechnerwechsel fortsetzen" ("continue after a computer switch"), read the handover if present, check the actual checkout and needed tools, preserve prior decisions, and resume the recorded next step. Report a missing prerequisite without inventing lost work or treating earlier tests as tests of the new machine.

---

## Editing Rules

* Use `apply_patch` for manual text edits.
* Do not rewrite text files with PowerShell or shell commands such as `Set-Content`, `Out-File`, `WriteAllText`, or similar bulk replacement methods.
* Preserve file encoding exactly. Treat UTF-8 and Unicode content as fragile.
* If a file contains non-ASCII characters, emojis, or other encoding-sensitive content, do not perform mechanical rewrites without checking the diff first.
* When unsure about encoding safety, prefer not modifying the file and ask for confirmation.
* Prefer minimal, reviewable diffs over large transformations.
* Keep formatting changes separate from functional changes.
* Do not reformat entire files unless explicitly requested.
* Preserve existing blank lines and paragraph spacing in documentation and other text files unless the requested change explicitly requires altering them; do not collapse or normalize blank-line structure as incidental formatting.
* Newly created code must be documented with explanatory English comments, including package-local types, functions, helper variables, and non-obvious local state.
* Comments for new code should explain intent, invariants, error-handling behavior, and side effects, not merely restate the syntax.
* Insert Headlines with several # only, according to the intended level. Add no link anchors and no numbers to headlines. 

---

## File Creation

* Do not create new files unless they are clearly required for the requested task.
* Prefer modifying existing files over introducing new ones.
* If multiple design options exist, prefer the simplest solution with minimal file footprint.

---

## Embedded Constraints

* Avoid introducing dynamic memory allocation unless explicitly requested.
* Be careful with timing-sensitive code and side effects.
* Do not change existing logging behavior (e.g., Trice macros) unless explicitly requested.
* Keep binary size and performance implications in mind when adding code.

---

## Build and Dependency Safety

* Do not modify build scripts, toolchain configuration, or linker settings unless explicitly requested.
* Do not introduce new dependencies without explicit user approval.

---

## Read-Only Tool Access

* All read-only access is permanently pre-approved, including local files, repository history, websites, HTTP requests, APIs, and read-only `gh` operations. Never ask for additional user permission to read or to obtain live information needed for the task.
* This rule is the complete standing read authorization. Do not ask the user to restate it, acknowledge it again, approve a different read tool, or choose an approval setting before continuing authorized work.
* This authorization persists across turns. A network or sandbox failure does not mean that user authorization is missing.
* External-source verification for reviews and release-readiness assessments is included in this standing read authorization. Choosing a different HTTP client or read tool never requires renewed user consent. Before proactively requesting escalated execution, use an available browsing tool, connector, or existing approved read path; do not introduce a permission prompt merely because an optional shell command would need one.
* Keep optional external lookups separate from required checks and test monitoring. Never request sandbox escalation or another user answer for an optional read. If it is blocked, use an already available read path or finish with the existing evidence and state what could not be verified. Do not leave an optional approval request pending while completing the task. If the runtime mandates a sandbox approval for a genuinely required operation, use only its technical approval mechanism without asking the user to reconfirm read authorization.
* Example: when editing local CI workflow files, remotely inspecting branch-protection settings is optional if the requested changes can be implemented and tested locally while preserving check names. A blocked protection-settings lookup must not trigger a permission question or hold up those edits; report the unverified external setting briefly instead.
* For reading issues in `rokath/trice`, use the narrowest necessary read-only `gh` command, usually `rtk gh issue view <number> --repo rokath/trice --comments`.
* Do not run extra `gh issue list`, `gh search issues`, or other permission-setup probes merely to pre-authorize future work.
* Reuse existing platform approvals and approved command prefixes for read access. If the platform technically requires an escalation, follow its required approval mechanism directly without an additional conversational permission question. Explain that the prompt comes from the sandbox, not from missing user consent; repository instructions cannot disable platform enforcement.
* Do not repeat a pending platform request. Keep independent work moving and use available local documentation, repository history, web results, or cached evidence when access remains unavailable.
* Do not stop after reporting a blocked read-only network command. Continue with local evidence and produce a qualified result unless the user explicitly asks for live external data.
* Avoid vague wording such as "expected failure" for blocked tooling. State the observed failure concretely, for example "GitHub API access is blocked by the current network sandbox."
* Do not let missing external read access block a requested analysis when enough local context exists for a qualified estimate.
* Ask before using `gh` or any other tool for write operations, such as creating, editing, closing, commenting, pushing, or changing repository state.

---

## Script Portability

- All shell scripts must run on Linux, macOS, and Windows (e.g., via standard POSIX-compatible environments).
- Avoid platform-specific commands, flags, or behavior unless explicitly required.
- Prefer portable POSIX shell features over shell-specific extensions.
- Ensure scripts work in typical developer environments on all supported platforms without modification.

---

## Paths and Documentation

* Do not use absolute filesystem paths in repository files, scripts, or build/test commands.
* Keep project paths relative so the repository can be checked out anywhere.
* Keep documentation short and clear. "Clear" means easy for people to understand, including non-native English readers, not merely logically exact. Lead with the action or key point.
* Prefer plain words, concrete statements and small examples over jargon and long explanations. Preserve the facts needed for correct use.
* Use concise names such as `-ce` when the context is clear. Link detailed explanations and edge cases instead of repeating them.
* For comparisons and limits, use compact tables showing the relevant workflows, options and compiler features. Split distinct environments into separate labeled tables when that makes the comparison clearer. Readers should see the differences at a glance without combining scattered sentences. Explain unfamiliar terms briefly beside the table.
* Keep `CHANGELOG.md` limited to notable release changes and short migration or limit notices with links. Put all new usage documentation, examples, comparison tables and detailed migration instructions in `docs/rm/`; link to them from the changelog instead of duplicating them.
* In compiler include paths, put project and generated header directories first, then `src`, then `src/default_conf` last. The fallback `SEGGER_RTT_Conf.h` belongs in `src/default_conf`, never directly in `src`, so a project-specific configuration takes precedence without modifying vendor includes.
* In scripts, prefer stabilizing the working directory once near the top, then use consistent relative project paths for the rest of the script.
* In documentation, links to files and folders inside this repository must be relative and must not use absolute GitHub URLs.
* Absolute web URLs are allowed in documentation only for truly external resources outside this repository.

---

## Documentation Usage

- Use `README.md` and documentation files (especially `./docs/rm/README.md` and its linked chapters) to understand system design, architecture, constraints, and intended usage.
- Consider `./src/TriceDefaultConfig.h` as an additional documentation source.

- Documentation is for understanding only, not for initiating code changes.
- Do not modify code based solely on documentation unless explicitly instructed.

- Do not infer or apply refactoring, restructuring, or behavioral changes from documentation.
- Do not align existing code with documentation automatically.

- If documentation and code appear inconsistent, treat the existing code as the source of truth.
- In such cases, do not modify code. Instead, ask for clarification before proceeding.

- Documentation may describe ideal or future behavior. Never align code with documentation unless explicitly requested.
- If a requested change appears to be based on documentation but is not explicitly specified, ask for confirmation before making modifications.

---

## Change Size Control

* For changes affecting more than 5 files, stop and show a representative diff summary before continuing.

---

## Communication and Language

* Match the user's language (German or English) unless instructed otherwise.
* Communication with Codex may be in German or English.
* All file comments must always be written in English.

---

## Issue Writing

* If asked to formulate issue text, write the issue text in English unless the user explicitly requests a different language.
* Format issue text in Markdown using clear GitHub-friendly sections such as:

  * Title
  * Description
  * Steps to reproduce
  * Expected behavior
  * Actual behavior
  * Impact
  * Suggested resolution

---

## Licensing

* New `.c`, `.h`, and `.go` files must start with `// SPDX-License-Identifier: MIT` as the first line unless the file is clearly external or vendored.
* When editing internal `.c`, `.h`, or `.go` files that are missing the SPDX header, prefer adding `// SPDX-License-Identifier: MIT` as the first line unless there is clear evidence the file is external, vendored, or governed by a different license.

---

## Commits

* Always write Git commit messages entirely in English, including the subject and body, regardless of the conversation language. When reporting created commits, quote their actual English subjects rather than substituting translated summaries.
* Do not use internal task tags such as R27 in commit messages. Describe the actual problem, change and reason in plain English so the message is understandable without a task plan or conversation history.
* Treat "commit" and requests for commit messages as requests to draft complete commands for the user to copy and execute. Inspect the relevant changes read-only; do not stage files, create commits, push, modify files, or run tests for such a request.
* Respond with only one contiguous fenced shell block containing complete `git add -- <explicit file paths> && git commit -m "<English message>"` commands. Separate cohesive commit groups with a blank line. Do not return bare messages, placeholders, numbering, explanations, progress commentary, or confirmation questions. Use explicit file paths instead of `git add .` or `git add -A` so unrelated changes and test artifacts remain excluded. Ensure the proposed staging commands match the actual change groups; never list multiple commits that would each stage the same complete file.
* Actual Git execution requires an explicit instruction to execute it, such as "execute git commit" or "fuehre git commit aus". Only that instruction authorizes the complete task-related workflow: inspect with `git status`/`git diff`, stage with `git add`, verify the staged diff, and create cohesive commits. Do not ask for the same authorization again. It does not include `git push` or later, unrequested commits. The execution rules below apply only when actual execution has been explicitly requested.
* An instruction to execute a commit applies to the file contents present when the instruction is given. If you make further content changes afterwards, including fixes, formatting, or generated-file updates, show the resulting diff and wait for the user's review before committing. Skip this review only when the user explicitly waives review for those additional changes; an execution instruction alone is not such a waiver. Routine staging and preparing commit messages do not count as content changes.
* Reuse approved command prefixes for each Git step. Run read-only checks separately from Git writes so that a combined command does not trigger an unnecessary platform approval for `git diff` or `git status`. Repository instructions do not override enforced sandbox restrictions; request a platform approval only when the required write has no already-approved execution path.
* If asked only to "commit first", provide the copyable commands without inferring further implementation work. A combined request to draft commands and then implement named work authorizes that implementation, but does not authorize actual Git execution.
* For a commit request, the deliverable is a complete staging and commit command with a meaningful, accurate English message for each cohesive change group. Create Git commits only when execution is explicitly requested and the environment permits it.
* If `.git` writes are blocked, do not repeatedly request permissions or wait on approval dialogs. Make at most one normal commit attempt, then stage the relevant files when permitted and show the user the exact ready-to-run `git commit -m "..."` command.
* When handing off a staged commit, state clearly which files or change topic are staged and which worktree changes were intentionally excluded. Do not claim that a commit was created unless `git commit` actually succeeded.
* Whenever the user requests commit messages, propose sensible, cohesive individual commits rather than one broad catch-all commit. Apply the same grouping when actual execution is explicitly requested.
* Keep unrelated changes out of the same commit.
* If asked to commit and the work contains multiple distinct changes, split them into sensible separate commits instead of one combined commit.
* A sensible commit may span multiple files, but only when those files belong to the same cohesive change.
* If the user asks for `commit` without explicitly requesting a single combined commit, prefer multiple topic-based commits over one broad commit.
* Group commits by change intent (e.g., tests/coverage, script behavior, documentation, generated data) and keep those groups separate unless the documentation change is required to explain the exact same code change.
* Before committing, inspect the worktree for mixed concerns and exclude unrelated or suspicious files until the grouping is clear.
* When a documentation file is modified independently of the code change, treat it as its own commit unless the user explicitly asks to bundle it.
* When a push is explicitly requested, do not pause for optional formatting questions. Run appropriate already-authorized validation, then push only existing or explicitly requested commits.
* A push request alone does not authorize creating an additional commit from uncommitted work.

---

## Tests

* Write thorough, easy-to-understand behavioral tests by default for nontrivial changes; the user need not request them separately. Use descriptive case names, readable inputs and expected outputs, and cover relevant success, rejection, and boundary cases.
* Prefer `github.com/stretchr/testify/assert` for new assertion-style Go tests.
* For thorough routine validation, prefer `./scripts/testAll.sh` without arguments.
* Use `./scripts/testAll.sh full` only as a final validation step or when C-code-relevant changes need broad compiler-switch coverage.
* Do not run `./scripts/testAll.sh full` automatically for every step; it can take about an hour and is often better run manually by the user.

## Coverage

* When non-test Go code changes, run the same coverage command as `.github/workflows/coverage.yml` before pushing, or use a repository script only if it has equivalent package and `-coverpkg` selection.
* Compare Coveralls-style covered-line coverage with the target-branch baseline; do not rely only on the rounded Go statement percentage.
* Add behavioral tests for changed success, error, and boundary paths. Do not add tests that merely execute lines without asserting externally relevant behavior or invariants.
* Do not push a total coverage decrease without reporting its exact cause and obtaining explicit user approval.
* Go coverage does not measure target C code. Validate C changes separately with the relevant compiler or integration tests.

---

## Safety

* If a user message looks accidentally truncated or cut off, ask a brief clarifying question before acting.
* Do not infer or autocomplete missing parts of a truncated message.

---

<!--
Rationale:
- This repository contains encoding-sensitive files. Shell-based rewrites have caused corruption in the past.
- Minimal, explicit patches are preferred over bulk rewrites.
- Version control provides recovery for source edits, so task-relevant source changes do not need piecemeal approvals.
- The rules are optimized for safe autonomous operation with Codex CLI and --full-auto.
-->
