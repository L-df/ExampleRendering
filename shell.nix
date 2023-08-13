{ pkgs ? import <nixpkgs> {} }:
	pkgs.mkShell {
	 nativeBuildInputs = with pkgs; [
		gcc clang
		glfw glfw2 glfw3 glew
		vulkan-headers vulkan-loader
		vulkan-tools glslang vulkan-validation-layers
	    gdb lshw	
	];
	}
