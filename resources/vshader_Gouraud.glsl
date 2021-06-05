#version 330

const int MAX_LIGHTS = 5;
const float EPSILON = 0.00001f;

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

vec4 blinnPhong() {
    vec4 componentAmbient = ambientLight.Ia*material.Ka;
    vec4 componentDifosa = vec4(0,0,0,1);
    vec4 componentEspecular = vec4(0,0,0,1);
    // Posem l'ultima coord a 0 per si algun punt tenia l'ultima coord malament
    vec4 V = obs - vPosition; V.w = 0.0f;
    V = normalize(V);

    for (int i = 0; i < MAX_LIGHTS; i++) {
        vec4 L;
        float atten;
        float a = lights[i].coeficients.x;
        float b = lights[i].coeficients.y;
        float c = lights[i].coeficients.z;

        // Cas PointLight
        if (lights[i].lightType == 0) {
            // Posem l'ultima coord a 0 per si algun punt tenia l'ultima coord malament
            L = lights[i].position - vPosition; L.w = 0.0f;
            float d = length(L);
            L = normalize(L);
            if (abs(a) < EPSILON && abs(b) < EPSILON && abs(c) < EPSILON) {
                atten = 1.0f;
            } else {
                atten = 1.0f/(a + b*d + c*d*d);
            }

        // Cas DirectionalLight
        } else if (lights[i].lightType == 1) {
            L = normalize(-lights[i].direction);
            atten = 1.0f;
        }

        // Cas SpotLight
        else if (lights[i].lightType == 2) {
            // Posem l'ultima coord a 0 per si algun punt tenia l'ultima coord malament
            L = lights[i].position - vPosition; L.w = 0.0f;
            float d = length(L);
            L = normalize(L);
            float cos_angle = dot(-L, normalize(lights[i].direction));
            // Ens assegurem que el cosinus estigui entre -1 i 1 (per errors d'arrodoniment)
            if(cos_angle > 1){
                cos_angle = 1;
            }
            else if (cos_angle < -1){
               cos_angle = -1;
            }
            float angle = acos(cos_angle); // ja tenim els dos vectors normalitzats
            if (angle < lights[i].angle) {
                if (abs(a) < EPSILON && abs(b) < EPSILON && abs(c) < EPSILON) {
                    // Si tots els coeficients son 0 considerem que no hi ha atenuacio
                    atten = 1.0f;
                } else {
                    // Calculem l'atenuacio
                    atten = 1.0f/(c*d*d + b*d + a);
                }
            }
            else{
                atten = 0.0f;
            }
        }
        vec4 H = normalize(L + V);
        componentAmbient += lights[i].Ia * material.Ka;
        componentDifosa += atten*lights[i].Id*material.Kd*max(0.0f, dot(L, normalize(vNormal)));
        componentEspecular += atten*lights[i].Is*material.Ks*pow(max(dot(normalize(vNormal), H), 0.0f), material.shininess);
    }
    vec4 result = componentAmbient + componentDifosa + componentEspecular;
    return vec4(result.x, result.y, result.z, 1.0f);
}


void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;
    // Retornem el calcul de Blinn-Phong
    color = blinnPhong();
}
