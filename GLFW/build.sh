#!/usr/bin/env bash

pushd GLFW/
g++ -g main.cpp -o ExampleWindow.out -lglfw
popd
