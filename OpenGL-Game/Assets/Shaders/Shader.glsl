#shader vertex

#version 460

layout(location = 0) in vec4 a_VertexPosition;
layout(location = 1) in vec2 a_TextureCoord;

out vec2 v_TextureCoord;

void main()
{
	v_TextureCoord = a_TextureCoord;
	gl_Position = a_VertexPosition;
}

#shader fragment

#version 460

out vec4 v_FragColor;

in vec2 v_TextureCoord;

uniform sampler2D u_Texture;

void main()
{
	// v_FragColor = vec4(0.9f, 01f, 0.2f, 1.0f);
	v_FragColor = texture2D(u_Texture, v_TextureCoord);
}