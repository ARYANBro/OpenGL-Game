#shader vertex

#version 460

layout(location = 0) in vec4 a_VertexPosition;

void main()
{
	gl_Position = a_VertexPosition;
}

#shader fragment

#version 460

out vec4 v_FragColor;

void main()
{
	v_FragColor = vec4(0.9f, 01f, 0.2f, 1.0f);
}