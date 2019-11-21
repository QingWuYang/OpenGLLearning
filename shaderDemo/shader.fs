#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）
in vec3 positionColor;

void main()
{
//    FragColor = vertexColor;
    FragColor = vec4(positionColor, 1.0f);
}
