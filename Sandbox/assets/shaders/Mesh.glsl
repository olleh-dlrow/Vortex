#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_TexCoord;
layout(location = 4) in vec3 a_Tangent;
layout(location = 5) in vec3 a_BiTangent;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_Color;
out vec2 v_TexCoord;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_ambient1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

void main()
{
    color = texture(texture_diffuse1, v_TexCoord);
}