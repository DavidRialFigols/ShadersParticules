#version 330

in vec4 normal;
in vec4 position;
out vec4 colorOut;

struct Material {
    vec4 Ka;
    vec4 Kd;
    vec4 Ks;
    float shininess;
};
uniform Material material;
uniform vec4 obs;
uniform vec4 toonLightDirection;

void main()
{
    float intensity;
    vec3 direction = normalize(-vec3(toonLightDirection));
    intensity = dot(direction,normalize(vec3(normal)));

    if (intensity > 0.95) {
        colorOut = material.Kd; colorOut.w = 1.0f;//vec4(1.0,0.5,0.5,1.0);
    }
    else if (intensity > 0.5) {
        colorOut = material.Kd*0.5f; colorOut.w =1.0f;//vec4(0.6,0.3,0.3,1.0);
    }
    else if (intensity > 0.25) {
        colorOut = material.Kd*0.3f; colorOut.w = 1.0f;//vec4(0.4,0.2,0.2,1.0);
    }
    else {
        colorOut = material.Kd*0.2f; colorOut.w = 1.0f;//vec4(0.2,0.1,0.1,1.0);
    }

    //Fem l'emfasi de siluetes
    colorOut = colorOut * (1-dot(normalize(vec3(position) - vec3(obs)), normalize(vec3(normal))));
}
