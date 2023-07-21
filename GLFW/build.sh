#!/usr/bin/env bash

pushd GLFW/
g++ main.cpp -o ExampleWindow.out -lglfw
popd
