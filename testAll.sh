#!/bin/sh
date

function f(){
    go clean -cache
    go test ./...
    cd _test
    go test ./...
    cd -
}

time f
