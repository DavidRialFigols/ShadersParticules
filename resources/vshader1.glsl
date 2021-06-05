#version 330

const int MAX_LIGHTS = 5;

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vNormal;

struct Material {
    vec4 Ka;
    vec4 Kd;
    vec4 Ks;
    float shininess;
};

uniform Material material;
uniform mat4 model_view;
uniform mat4 projection;

out vec4 color;

struct Light
{
    vec4 Ia;
    vec4 Id;
    vec4 Is;

    vec3 coeficients;

    int lightType;

    vec4 position;
    vec4 direction;
    float angle;
};
uniform Light lights[MAX_LIGHTS];

struct AmbientLight
{
    vec4 Ia;
};
uniform AmbientLight ambientLight;

uniform vec4 obs;

void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;
    // Canviar la variable color per comprovar si es passen correctament les components del material
    color = material.Kd;
}
