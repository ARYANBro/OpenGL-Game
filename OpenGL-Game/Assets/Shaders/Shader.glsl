#shader vertex

#version 460

layout(location = 0) in vec4 a_VertexPosition;
layout(location = 1) in vec2 a_TextureCoord;

uniform mat4 u_ProjectionTransform;
uniform mat4 u_ModelTransform;

out vec2 v_TextureCoord;

void main()
{
	v_TextureCoord = a_TextureCoord;
	gl_Position = u_ProjectionTransform * u_ModelTransform * a_VertexPosition;
}

#shader fragment

#version 460

out vec4 v_FragColor;

in vec2 v_TextureCoord;

uniform sampler2D u_Texture;
uniform vec3 u_Color;

void main()
{
	v_FragColor = vec4(u_Color, 1.0f) * texture2D(u_Texture, v_TextureCoord);
}