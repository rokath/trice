#!/usr/bin/env bash

# Clear all IDs inside ./example date
touch til.json li.json # make sure we have json files for "trice clean"
trice clean -src ./exampleData

# Insert fresh IDs
rm til.json li.json # remove old stuff
touch til.json li.json
trice insert -src ./exampleData -IDMin 16300 -IDMax 16379 -IDMethod upward
rm til.json li.json # remove JSON files, because the IDs are inside the ./example files now and we do not use these JSON files at all
