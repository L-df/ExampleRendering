!/usr/bin/env bash

pushd GLFW/
g++ -g -fconstexpr-loop-limit=400000 main.cpp DrawingPixels.h -o ExampleWindow.out -lglfw -lGL -lvulkan
popd
