#!/usr/bin/env bash

pushd GLFW/
g++ -g -fconstexpr-loop-limit=400000 main.cpp -o ExampleWindow.out -lglfw -lGL
popd
