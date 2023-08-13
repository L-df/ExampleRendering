!/usr/bin/env bash

pushd GLFW/
g++ -g -fconstexpr-loop-limit=400000 main.cpp DrawingPixels.h -o ExampleWindow.out -lglfw -lGL -lvulkan
g++ -g -fconstexpr-loop-limit=400000 -O0 main.cpp DrawingPixels.h -o DebugExampleWindow.out -lglfw -lGL -lvulkan
popd
