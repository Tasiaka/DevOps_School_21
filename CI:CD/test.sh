#!/bin/bash

if ! grep -q "FAILED: 0" src/cat/test.txt || ! grep -q "FAILED: 0" src/grep/test.txt; then
    exit 1
fi