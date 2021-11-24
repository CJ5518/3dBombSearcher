#version 330 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec4 fragTexCoordOffset;

//uniform vec4 objectColor;

out vec4 color;

uniform sampler2D tex;

void main(){
	float intervalSizeX = fragTexCoordOffset.y - fragTexCoordOffset.x;
	float intervalSizeY = fragTexCoordOffset.z - fragTexCoordOffset.w;
	vec2 scaledTexCoords = vec2((fragTexCoord.x * intervalSizeX) + fragTexCoordOffset.x, (fragTexCoord.y * intervalSizeY) + fragTexCoordOffset.z);
	vec4 tex1 = texture(tex, scaledTexCoords);
	color = tex1;
	//if (tex1.a < 0.1) discard;
}