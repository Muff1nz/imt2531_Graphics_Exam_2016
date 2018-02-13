#version 410
	
	
	uniform samplerCube textureBuffer;	
	
	in vec3 texCoords;
	
	out vec4 fragment_colour;
	
	
	void main () {	
		//modelspace vertex positions = texCoords, since im applying a cubemap to a cube
		fragment_colour = texture(textureBuffer, texCoords);
	}