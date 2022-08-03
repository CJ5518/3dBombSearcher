#version 330 core
//Input things
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;
//Tells us where we are on our special minesweeper texture
in vec4 fragTexCoordOffset;

//I've forgotten what flat means
flat in int fragInstanceID;

uniform int objectColor;
uniform int selectedID;

out vec4 color;

uniform sampler2D tex;

void main(){
	float intervalSizeX = fragTexCoordOffset.y - fragTexCoordOffset.x;
	float intervalSizeY = fragTexCoordOffset.z - fragTexCoordOffset.w;
	vec2 scaledTexCoords = vec2((fragTexCoord.x * intervalSizeX) + fragTexCoordOffset.x, (fragTexCoord.y * intervalSizeY) + fragTexCoordOffset.z);
	vec4 tex1 = texture(tex, scaledTexCoords);
	//Get selection stuff
	float selectionModifier = 1.5f - ((1.0f - float(int(abs(fragInstanceID - selectedID)) == 0)) / 2.0f);
	color = tex1 * selectionModifier;
	if (color.a < 0.9) discard;
}