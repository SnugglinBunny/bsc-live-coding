#version 330 core

in vec2 textureCoordsOut;

out vec4 colour;

uniform sampler2D texture0;
uniform float factor;

vec4 Sepia( in vec4 color )
{
    return vec4(clamp(color.r * 0.393 + color.g * 0.769 + color.b * 0.189, 0.0, 1.0),
				clamp(color.r * 0.349 + color.g * 0.686 + color.b * 0.168, 0.0, 1.0),
				clamp(color.r * 0.272 + color.g * 0.534 + color.b * 0.131, 0.0, 1.0),
				color.a
    );
}

void main()
{
	vec4 textureColour = texture(texture0,textureCoordsOut);
	colour = mix(textureColour, Sepia(textureColour), 1.0f);
}
