# Commit Message Guidelines

Purpose: Ensure that all commits follow a simple convention to facilitate readability and automation.

Rule:
- The commit message must start with one of the following tags: `[DEV]`, `[DOC]`, `[MINOR]`.

Regular expression (match at the beginning of the message):

Valid examples:
- `[DEV] Fix temperature sensor`
- `[DOC] Update README`
- `[MINOR] Fix typo`

Invalid examples:
- `Fix bug in ADC reading`
- `Updated README`

Automatic application:
- A GitHub Actions workflow is provided: `.github/workflows/commit_rules.yml`.  
  - Enable branch protection (Settings → Branches → Branch protection rules) for the target branch (e.g., `main`) and require the workflow check to pass before merging.
- Optional (local control): add a versioned `commit-msg` hook (e.g., `.githooks/commit-msg`) and configure `git config core.hooksPath .githooks`.

Notes:
- Adjust the regex if you want to allow the tag elsewhere than at the beginning of the message.
- For PRs from public forks, the workflow can still check messages; some checks using secrets will not run.