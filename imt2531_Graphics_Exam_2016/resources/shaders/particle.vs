#version 410

	layout (location = 0) in vec3 startPos;
	layout (location = 1) in float startTime; //between 0 and 10

	uniform mat4 ProjectionMatrix, ViewMatrix;
	uniform float time; //Time since program started
	
	out vec3 pos;
	
	void main() {
		vec3 velocity = vec3(0.0, -10.0, 0.0);	
		float t = time - startTime;
		t = mod(t, 10.0); //hardcoded 10 as particle lifetime
	
		
		pos = startPos + velocity * t;		
		
		gl_Position = ProjectionMatrix * ViewMatrix * vec4(pos, 1);;
		gl_PointSize = 20.0;		
	}
	