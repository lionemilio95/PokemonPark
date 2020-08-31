#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec2 TexCoords;


// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
    color = texture(texture_diffuse1, TexCoords);
    

    
}


