#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_TexPos;

uniform sampler2D u_Texture;

void main()
{
	FragColor = texture(u_Texture, v_TexPos);
	// FragColor1 = texture(u_Texture, v_TexPos);
	// FragColor = vec4(v_TexPos, 0, 1);
}
