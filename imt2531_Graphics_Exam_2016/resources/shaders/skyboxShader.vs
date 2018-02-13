#version 410

	layout(location = 0) in vec3 vp;

	const int maxLights = 10; 
	
	uniform mat4 ProjectionMatrix, ViewMatrix, ModelMatrix;

	out vec3 texCoords;

	void main () {
	
		gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vp, 1);
		
		texCoords = vp;
	};