#version 430 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 color;
in vec2 textCoords;

uniform sampler2D text; 

void main() {
	FragColor = texture(text, textCoords) * vec4(color, 1.0f);
}