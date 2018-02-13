#version 410

	const int maxLights = 10; 
	uniform int lightCount;
	
	uniform int lightType[maxLights];
	uniform vec3 lightDirection[maxLights];
	uniform float lightArc[maxLights];
	
	uniform vec3 lightPos[maxLights];
	
	uniform sampler2D textureBuffer;

	in vec3 pos;
	
	out vec4 clr;

	void main() {
		bool visible = false;
		//Check if particle can be seen by any light
		for(int i = 0; i < lightCount; i++){
			if (lightType[i] == 1){
				//Calculate direction factor
				vec3 directionToSurface = normalize(pos - lightPos[i]);
				float directionDot = dot(directionToSurface, lightDirection[i]);
				if (directionDot > cos(lightArc[i])){
					visible = true;
				}
			}else if (lightType[i] == 2){
				visible = true;
			}
		}
		
		clr = texture(textureBuffer, gl_PointCoord);
		
		if (!visible){
			clr = vec4(0, 0, 0, clr.a);
		}
	}