#!/usr/bin/env bash
set -e

ph="/Instances/Solomon/c101.txt"
cmake -S . -B build
cmake --build build
./build/main $ph
./build/unitTest