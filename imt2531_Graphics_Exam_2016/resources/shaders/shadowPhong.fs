#version 410
	

	const int maxLights = 10;	
	int currentLight = 0;
	
	uniform int lightCount;
	
	uniform int lightType[maxLights];
	uniform vec3 lightDirection[maxLights];
	uniform float lightArc[maxLights];
	
	uniform vec3 lightPos[maxLights];
	uniform vec3 lightSpecular[maxLights];
	uniform vec3 lightDiffuse[maxLights];
	uniform vec3 lightAmbient[maxLights];
	
	uniform sampler2D shadowTexture[maxLights]; //Its really a depth map
	
	uniform mat4 ViewMatrix;
	
	in vec4 shadowST[maxLights];
	in vec3 posEye, normalEye;		
	in float specularExponent;	
	in vec3 surfaceAmbient, surfaceDiffuse, surfaceSpecular;
	in vec2 fragmentUV;
	
	out vec4 fragment_colour;
	
	float calcShadow (in vec4 texcoods) {
		// constant that you can use to slightly tweak the depth comparison
		float epsilon;
		if (lightType[currentLight] == 1){
			epsilon = 0.0009;
		}else if (lightType[currentLight] == 2){
			epsilon = 0.0005;
		}		
	
		if (texcoods.x > 1.0 || texcoods.x < 0.0 || texcoods.y > 1.0 || texcoods.y < 0.0 || texcoods.w < 0.0) {
			return 1.0; // do not add shadow/ignore
		}
		
		float shadow = texture (shadowTexture[currentLight], texcoods.xy).r;
		
		if (shadow + epsilon < texcoods.z) {
			return 0.0; // shadowed
		}
		return 1.0; // not shadowed
	}
	
	vec3 spotlight(){
		//Calculate direction factor
		vec3 lightPosEye = vec3(ViewMatrix * vec4(lightPos[currentLight], 1));
		vec3 directionToSurfaceEye = normalize(posEye - lightPosEye);
		vec3 lightDirectionEye = vec3(ViewMatrix * vec4(lightDirection[currentLight], 0));
		float directionDot = dot(directionToSurfaceEye, lightDirectionEye);
		float directionFactor = 1;
		if (directionDot < cos(lightArc[currentLight]))
			directionFactor = 0;

		//Ambient light
		vec3 ambient = surfaceAmbient * lightAmbient[currentLight];
	
		//Diffuse light
		vec3 distanceToLightEye = lightPosEye - posEye;
		vec3 directionToLightEye = normalize(distanceToLightEye);
		float dotProd = dot(directionToLightEye, normalEye);
		dotProd = max(dotProd, 0.0);
		vec3 diffuse = lightDiffuse[currentLight] * surfaceDiffuse * dotProd  * directionFactor;
		
		//Specular light
		vec3 reflectionEye = reflect(-directionToLightEye, normalEye);
		vec3 surfaceToViewerEye = normalize(-posEye);
		float dotProdSpecular = dot(reflectionEye, surfaceToViewerEye);
		dotProdSpecular = max(dotProdSpecular, 0.0);
		float specularFactor = pow(dotProdSpecular, specularExponent);
		vec3 specular = lightSpecular[currentLight] * surfaceSpecular * specularFactor * directionFactor;
		
		//Shadows
		vec4 shadowCoord = shadowST[currentLight];			
		//convert coordinates to texture coordinates
		shadowCoord.xyz /= shadowCoord.w;
		shadowCoord.xyz += 1.0;
		shadowCoord.xyz *= 0.5;		
		float shadowFactor = calcShadow(shadowCoord);
		
		//final light
		return ambient + (diffuse + specular) * shadowFactor;
	}
	
	vec3 directionalLight(){
		//Ambient light
		vec3 ambient = surfaceAmbient * lightAmbient[currentLight];
				
		//Diffuse light
		vec3 lightDirectionEye = vec3(ViewMatrix * vec4(lightDirection[currentLight], 0));
		float dotProd = dot(-lightDirectionEye, normalEye);
		dotProd = max(dotProd, 0.0);
		vec3 diffuse = lightDiffuse[currentLight] * surfaceDiffuse * dotProd;
		
		//Specular light			
		vec3 reflectionEye = reflect(lightDirectionEye, normalEye);
		vec3 surfaceToViewerEye = normalize(-posEye);
		float dotProdSpecular = dot(reflectionEye, surfaceToViewerEye);
		dotProdSpecular = max(dotProdSpecular, 0.0);
		float specularFactor = pow(dotProdSpecular, specularExponent);
		vec3 specular = lightSpecular[currentLight] * surfaceSpecular * specularFactor;
		
		//Shadows
		vec4 shadowCoord = shadowST[currentLight];			
		//convert coordinates to texture coordinates
		shadowCoord.xyz /= shadowCoord.w;
		shadowCoord.xyz += 1.0;
		shadowCoord.xyz *= 0.5;		
		float shadowFactor = calcShadow(shadowCoord);
		
		//final light
		return ambient + (diffuse + specular) * shadowFactor;
	}
	
	void main () {	
		vec3 totalLight = vec3(0, 0, 0);
		
		for(currentLight = 0; currentLight < lightCount; currentLight++){ //do light calculations for each light
			int type = lightType[currentLight];
			if(type == 1){
				totalLight += spotlight();
			}else if (type == 2){
				totalLight += directionalLight();
			}			
		}
		//final colour
		fragment_colour = vec4(totalLight, 1);
	}