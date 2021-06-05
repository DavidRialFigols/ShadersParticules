#version 330
const int MAX_LIGHTS = 5;
const float EPSILON = 0.00001f;
const float PI = 3.14159;

in vec4 normal;
in vec4 position;
in vec2 v_texcoord;
out vec4 colorOut;

struct Material {
    vec4 Ka;
    vec4 Kd;
    vec4 Ks;
    float shininess;
};
uniform Material material;

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

uniform sampler2D textureMap;

uniform vec4 center; // Centre de l'objecte

uniform bool isTextMapDirect; //if true es fa mapeig directe, if false es fa paeig indirecte
uniform bool isTextureActivated; //if true utilitzara la textura

vec4 calculateDiffuse() {
    if (!isTextureActivated) {
        return material.Kd;
    }

    if(isTextMapDirect) {
        vec4 color_Kd = vec4(texture(textureMap, v_texcoord).rgb*0.75 + vec3(material.Kd)*0.25, 1.0f);
        return color_Kd;
    } else {
        vec3 centerNormalized = normalize(vec3(position - center));
        float u = 0.5 - atan(centerNormalized.z, centerNormalized.x)/(2.*PI);
        float v = 0.5 - asin(centerNormalized.y)/PI;
        return vec4(texture(textureMap, vec2(u, v)).rgb*0.75 + vec3(material.Kd)*0.25, 1.0f);
    }


}

vec4 blinnPhong() {
    vec4 componentAmbient = ambientLight.Ia*material.Ka;
    vec4 componentDifosa = vec4(0,0,0,1);
    vec4 componentEspecular = vec4(0,0,0,1);
    // Posem l'ultima coord a 0 per si algun punt tenia l'ultima coord malament
    vec4 V = obs - position; V.w = 0.0f;
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
            L = lights[i].position - position; L.w = 0.0f;
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
            L = lights[i].position - position; L.w = 0.0f;
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
            float angle = acos(cos_angle); //ja tenim els dos vectors normalitzats
            if (angle < lights[i].angle) {
                if (abs(a) < EPSILON && abs(b) < EPSILON && abs(c) < EPSILON) {
                    //Si tots els coeficients son 0 considerem que no hi ha atenuacio
                    atten = 1.0f;
                } else {
                    //Calculem la distancia entre el punt i la posicio de la llum
                    atten = 1.0f/(c*d*d + b*d + a);
                }
            }
            else{
                atten = 0.0f;
            }
        }
        vec4 H = normalize(L + V);
        componentAmbient += lights[i].Ia * material.Ka;
        componentDifosa += atten*lights[i].Id*calculateDiffuse()*max(0.0f, dot(L, normalize(normal)));
        componentEspecular += lights[i].Is*material.Ks*pow(max(dot(normalize(normal), H), 0.0f), material.shininess);
    }
    vec4 result = componentAmbient + componentDifosa + componentEspecular;
    return vec4(result.x, result.y, result.z, 1);
}


void main()
{
    // Retornem el calcul de Blinn-Phong
    colorOut = blinnPhong();
}
