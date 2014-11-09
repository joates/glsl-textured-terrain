#!/bin/bash

cd .build
make clean && make
cd ..
./demo

