{ pkgs ? import <nixpkgs> {} }:
	pkgs.mkShell {
	 nativeBuildInputs = with pkgs; [
		gcc
		glfw glfw2 glfw3 glew
		vulkan-headers
		vulkan-tools
	    gdb	
	];
	}
