#!/bin/bash

# Copy the scs files and shaders, res folders recursively

rm -rf ./shaders ./scs ./res
cp -f ../scs ./
cp -rf ../shaders ./shaders
cp -rf ../res ./res

# Run the Python script
python3 test.py