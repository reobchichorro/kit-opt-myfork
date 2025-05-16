#!/bin/bash

# Create output directory if it doesn't exist
mkdir -p results

# Loop through all files in the 'files' directory
for file in ../instances-rel/*; do
    filename=$(basename "$file")           # Extract filename without path
    ./solve.out "$file" > "results/${filename}.txt" # Run command and redirect output
done
