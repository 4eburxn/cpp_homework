#!/bin/bash
cmake -S . -B build && cmake --build build -j16 
find ./build -maxdepth 1 -executable -type f -exec ln -s {} {}_link \;
mv ./build/*_link ./
