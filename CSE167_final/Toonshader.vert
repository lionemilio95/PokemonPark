#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


uniform mat4 projection;
uniform mat4 modelview;


out vec3 normal;
out vec3 FragPos;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * modelview * vec4(aPos, 1.0);
    //normal = aNormal;
    FragPos = vec3(modelview * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(modelview))) * aNormal;
}





