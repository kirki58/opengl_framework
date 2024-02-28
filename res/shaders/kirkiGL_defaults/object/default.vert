#version 330 core
layout (location = 0) in vec3 Vertex_Pos;
layout (location = 1) in vec3 Vertex_Normal;
layout (location = 2) in vec2 Vertex_TexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(Vertex_Pos, 1.0));
    Normal = mat3(transpose(inverse(model))) * Vertex_Normal;  
    TexCoords = Vertex_TexCoords;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}