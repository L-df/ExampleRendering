!/usr/bin/env bash

pushd ./GLFW/

g++ -g -fconstexpr-loop-limit=400000 -DVULKAN_SEVERITY_THRESHOLD=VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT main.cpp DrawingPixels.h -o ExampleWindow.out -lglfw -lGL -lvulkan

g++ -g -fconstexpr-loop-limit=400000 -O0 -DPRINT_AVAILABLE_VULKAN_EXTENSIONS_AND_LAYERS=1 main.cpp DrawingPixels.h -o DebugExampleWindow.out -lglfw -lGL -lvulkan
popd
