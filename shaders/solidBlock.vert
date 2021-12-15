#version 430 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in float aMode;

layout (location = 2) in vec2 inTextCoords;

// Outputs the color for the Fragment Shader
out vec3 color;

out vec2 textCoords;


// Imports the camera matrix from the main function
uniform mat4 camMatrix;




void main() {
	textCoords = inTextCoords;

	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(aPos, 1.0);
	
	// Assigns the colors from the Vertex Data to "color"
	if(aMode == 0.f) { color = vec3(0.8,0.2,0.2);}
	else if(aMode == 1.f) { color = vec3(0.4,0.3,0.7);}
	else if(aMode == 2.f) { color = vec3(0,1,0);}
	else if(aMode == 3.f) { color = vec3(1,0,1);}
	else if(aMode == 4.f) { color = vec3(1,0,0);}
	else if(aMode == 5.f) { color = vec3(0,1,1);}
	else if(aMode == 6.f) { color = vec3(1,0.6,0);}
	else if(aMode == 7.f) { color = vec3(1,0,0.2);}
	else if(aMode == 8.f) { color = vec3(0.5,0,1);}

}