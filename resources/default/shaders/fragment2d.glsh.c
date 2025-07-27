
#version 330 core

in vec3 fragColor;

out vec4 scrColor;

void main() {
	scrColor = vec4(fragColor, 1.0);
}
