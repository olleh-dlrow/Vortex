// do nothing

#type vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}  


#type fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float hdr_exposure;
uniform bool hdr;
uniform bool gamma_open;

void main()
{
    const float gamma = 2.2;
    vec3 col = texture(screenTexture, TexCoords).rgb;
    vec3 mapped = col;
    if(hdr)
    {
        //mapped = col / (col + vec3(1.0));
        mapped = vec3(1.0) - exp(-col * hdr_exposure);
    }
    if(gamma_open)
    {
        mapped = pow(mapped, vec3(1.0 / gamma));
    }
    FragColor = vec4(mapped, 1.0);
}