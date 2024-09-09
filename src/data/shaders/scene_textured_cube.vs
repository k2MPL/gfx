#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 uMVP;

out vec2 vTexCoords;

void main() {
  vTexCoords = aTexCoords;
  gl_Position = uMVP * vec4(aPosition, 1.0);
}