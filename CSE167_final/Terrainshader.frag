#version 330 core



in vec3 pos;
out vec4 color;
uniform float isOutline;


void main()
{
    if (isOutline == 1)
        color = vec4(0,0,0,1);
    else
        if (pos.y > 25)
        color = vec4(1,1,1,1);
    else if (pos.y > 14)
        color = vec4(0.32,0.35,0.143,1);
    else if (pos.y > 7)
        color = vec4(0.69,0.65,0.53,1);
    else
        color = vec4(0.15,0.33,0.47,1);
    
}


