#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 instanceTexCoord;
layout(location = 4) in mat4 instanceMatrix;


uniform mat4 model;
uniform mat4 pv;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoord;
out vec4 fragTexCoordOffset;
flat out int fragInstanceID;

void main(){
	mat4 usingModel = instanceMatrix;
	gl_Position = pv * usingModel * vec4(vertexPos, 1.0f);
	fragPos = vec3(usingModel * vec4(vertexPos, 1.0f));
	fragNormal = mat3(transpose(inverse(usingModel))) * normal;
	fragTexCoord = texCoord;
	fragTexCoordOffset = instanceTexCoord;
	fragInstanceID = gl_InstanceID;
}
