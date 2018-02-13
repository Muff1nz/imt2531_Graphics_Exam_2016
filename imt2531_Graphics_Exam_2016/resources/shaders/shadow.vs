#version 410

layout(location = 0) in vec3 vp;
uniform mat4 lightVP, ModelMatrix;

void main () {
  gl_Position = lightVP * ModelMatrix * vec4(vp, 1);
}