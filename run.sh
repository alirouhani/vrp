#!/usr/bin/env bash
set -e

ph=Instances/A/A-n32-k5.vrp
cmake -S . -B build
cmake --build build
./build/main "$ph"
./build/unitTest