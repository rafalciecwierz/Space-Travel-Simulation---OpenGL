#version 430 core

uniform vec3 objectColor;
uniform vec3 lightDir;
uniform sampler2D texture;

in vec3 interpNormal;
in vec2 vertexTexNew;

void main()
{
	vec4 textureColor = texture2D(texture, -vertexTexNew*5);
	gl_FragColor = textureColor;
}
