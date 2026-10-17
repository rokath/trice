// SPDX-License-Identifier: MIT

package scripts_test

import (
	"os"
	"testing"

	"github.com/stretchr/testify/assert"
	"github.com/stretchr/testify/require"
	"gopkg.in/yaml.v3"
)

// TestStatisticsBranchGuard executes the production check without API calls or
// Git writes, including retargeted PRs, missing metadata and untrusted ref text.
func TestStatisticsBranchGuard(t *testing.T) {
	script := workflowShellStep(t, "statistics-branch-guard.yml", "Reject statistics branch merges")
	for _, tc := range []struct {
		name, event, head, base string
		wantError               string
	}{
		{name: "development_to_main", event: "pull_request_target", head: "wip", base: "main"},
		{name: "feature_to_development", event: "pull_request_target", head: "feature/fix", base: "wip"},
		{name: "statistics_to_main", event: "pull_request_target", head: "github-repo-stats", base: "main", wantError: "Do not merge this branch."},
		{name: "statistics_to_development", event: "pull_request_target", head: "github-repo-stats", base: "wip", wantError: "Do not merge this branch."},
		{name: "retargeted_to_statistics", event: "pull_request_target", head: "main", base: "github-repo-stats", wantError: "Do not merge this branch."},
		{name: "missing_source", event: "pull_request_target", base: "main", wantError: "source or target branch is missing"},
		{name: "missing_target", event: "pull_request_target", head: "wip", wantError: "source or target branch is missing"},
		{name: "manual_registration", event: "workflow_dispatch", head: "main"},
		{name: "manual_statistics_branch", event: "workflow_dispatch", head: "github-repo-stats", wantError: "Do not merge this branch."},
		{name: "merge_queue", event: "merge_group", head: "gh-readonly-queue/main/pr-1"},
		{name: "branch_text_is_not_executed", event: "pull_request_target", head: "feature/$(touch${IFS}injected)", base: "main"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := t.TempDir()
			out, err := runFixture(t, root, script, map[string]string{
				"EVENT_NAME": tc.event, "HEAD_BRANCH": tc.head, "BASE_BRANCH": tc.base,
			})
			if tc.wantError != "" {
				assert.Error(t, err, out)
				assert.Contains(t, out, "::error::")
				assert.Contains(t, out, tc.wantError)
				assert.NotContains(t, out, "PASS:")
			} else {
				assert.NoError(t, err, out)
				assert.Contains(t, out, "PASS:")
			}
			assert.NoFileExists(t, root+"/injected", "branch names must remain data")
		})
	}
}

// TestStatisticsBranchGuardBindings preserves the required-check identity and
// trusted event context even when the statistics branch drops project sources.
func TestStatisticsBranchGuardBindings(t *testing.T) {
	data, err := os.ReadFile("../.github/workflows/statistics-branch-guard.yml")
	require.NoError(t, err)
	var workflow struct {
		On map[string]struct {
			Types []string
		}
		Permissions map[string]string
		Jobs        map[string]struct {
			Name  string
			If    string
			Steps []struct {
				Uses string
				Env  map[string]string
			}
		}
	}
	require.NoError(t, yaml.Unmarshal(data, &workflow))
	assert.ElementsMatch(t, []string{"opened", "synchronize", "reopened", "edited"}, workflow.On["pull_request_target"].Types)
	assert.Contains(t, workflow.On, "merge_group")
	assert.Contains(t, workflow.On, "workflow_dispatch")
	assert.NotContains(t, workflow.On, "pull_request", "the source branch must not supply the guard")
	assert.Empty(t, workflow.Permissions, "the check needs neither repository access nor secrets")
	guard := workflow.Jobs["guard"]
	assert.Equal(t, "Statistics branch isolation", guard.Name)
	assert.Empty(t, guard.If, "a skipped required check would not block a forbidden PR")
	require.Len(t, guard.Steps, 1)
	assert.Empty(t, guard.Steps[0].Uses, "never check out pull-request content")
	assert.Equal(t, "${{ github.event.pull_request.head.ref || github.ref_name }}", guard.Steps[0].Env["HEAD_BRANCH"])
	assert.Equal(t, "${{ github.event.pull_request.base.ref }}", guard.Steps[0].Env["BASE_BRANCH"])
}
