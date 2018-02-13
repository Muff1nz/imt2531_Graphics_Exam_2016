#version 410

	layout(location = 0) in vec3 vp;
	layout(location = 1) in vec3 normal;
	layout(location = 2) in vec3 ka;
	layout(location = 3) in vec3 kd;
	layout(location = 4) in vec3 ks;
	layout(location = 5) in float ns;
	layout(location = 6) in vec2 uv;

	const int maxLights = 10; 
	
	uniform mat4 ProjectionMatrix, ViewMatrix, ModelMatrix;

	uniform int lightCount;
	uniform mat4 lightVP[maxLights];
	
	out vec4 shadowST[maxLights];
	out vec3 posEye, normalEye;
	out float specularExponent;
	out vec3 surfaceAmbient, surfaceDiffuse, surfaceSpecular;
	out vec2 fragmentUV;

	void main () {
		posEye = vec3(ViewMatrix * ModelMatrix * vec4(vp, 1));
		normalEye = vec3(ViewMatrix * ModelMatrix * vec4(normal, 0));
		gl_Position = ProjectionMatrix * vec4(posEye, 1.0);	
		
		fragmentUV = uv;
		
		//The position as viewed from the light
		//Used to work out shadow texture coordinates
		for(int i = 0; i < lightCount; i++){
			shadowST[i] = lightVP[i] * ModelMatrix * vec4(vp, 1);
		}
		
		surfaceAmbient = ka; surfaceDiffuse = kd; surfaceSpecular = ks;
		specularExponent = ns;
	};