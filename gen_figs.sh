#!/bin/bash

# echo "removing previous data and plots..."

# find "results" -mindepth 2 -maxdepth 2 -type f -exec rm -f {} +

# rm -f plots/*

( cd src && make clean && make run )

echo "data generated; analyzing it..."

python3 analysis/calculate_mean.py

echo "making plots..."

python3 plotting/plot.py
