#version 330 core

uniform vec3 lightDir;
uniform vec3 cam_pos;

in vec3 normal;
in vec3 FragPos;

out vec4 color;




void main()
{
    // Discretized shading
    float intensity;
    intensity = dot(lightDir,normalize(normal));

    if (intensity > 0.95)
        color = vec4(1.0,0.5,0.5,1.0);
    else if (intensity > 0.5)
        color = vec4(0.6,0.3,0.3,1.0);
    else if (intensity > 0.25)
        color = vec4(0.4,0.2,0.2,1.0);
    else
        color = vec4(0.2,0.1,0.1,1.0);
    

    
    
    // Silhouette edge

    vec3 viewDir = cam_pos - FragPos;
    float edge = max(0, dot(viewDir,normalize(normal)));

    if(edge < 10){
        color = vec4(0.0,0.0,0.0,1.0);
    }



}



