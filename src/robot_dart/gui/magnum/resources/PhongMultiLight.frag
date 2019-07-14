#ifndef NEW_GLSL
#define in varying
#define color gl_FragColor
#define texture texture2D
#endif

#ifndef RUNTIME_CONST
#define const
#endif

#define M_PI 3.1415926535897932384626433832795

struct lightSource
{
    highp vec4 position;
    highp vec4 ambient;
    highp vec4 diffuse;
    highp vec4 specular;
    highp vec3 spotDirection;
    highp float spotExponent;
    highp float spotCutoff;
    highp float intensity;
    highp float constantAttenuation;
    highp float linearAttenuation;
    highp float quadraticAttenuation;
};

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 3)
#endif
uniform mediump float shininess
    #ifndef GL_ES
    = 80.0
    #endif
    ;

#ifdef AMBIENT_TEXTURE
#ifdef EXPLICIT_TEXTURE_LAYER
layout(binding = 0)
#endif
uniform lowp sampler2D ambientTexture;
#endif

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 4)
#endif
uniform lowp vec4 ambientColor
    #ifndef GL_ES
    #ifndef AMBIENT_TEXTURE
    = vec4(0.0, 0.0, 0.0, 1.0)
    #else
    = vec4(1.0)
    #endif
    #endif
    ;

#ifdef DIFFUSE_TEXTURE
#ifdef EXPLICIT_TEXTURE_LAYER
layout(binding = 1)
#endif
uniform lowp sampler2D diffuseTexture;
#endif

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 5)
#endif
uniform lowp vec4 diffuseColor
    #if !defined(GL_ES) && defined(DIFFUSE_TEXTURE)
    = vec4(1.0)
    #endif
    ;

#ifdef SPECULAR_TEXTURE
#ifdef EXPLICIT_TEXTURE_LAYER
layout(binding = 2)
#endif
uniform lowp sampler2D specularTexture;
#endif

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 6)
#endif
uniform lowp vec4 specularColor
    #ifndef GL_ES
    = vec4(1.0)
    #endif
    ;

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 7)
#endif
uniform lightSource lights[LIGHT_COUNT];

in mediump vec3 transformedNormal;
in highp vec3 cameraDirection;

#if defined(AMBIENT_TEXTURE) || defined(DIFFUSE_TEXTURE) || defined(SPECULAR_TEXTURE)
in mediump vec2 interpolatedTextureCoords;
#endif

#ifdef NEW_GLSL
out lowp vec4 color;
#endif

void main() {
    lowp const vec4 finalAmbientColor =
        #ifdef AMBIENT_TEXTURE
        texture(ambientTexture, interpolatedTextureCoords)*
        #endif
        ambientColor;
    lowp const vec4 finalDiffuseColor =
        #ifdef DIFFUSE_TEXTURE
        texture(diffuseTexture, interpolatedTextureCoords)*
        #endif
        diffuseColor;
    lowp const vec4 finalSpecularColor =
        #ifdef SPECULAR_TEXTURE
        texture(specularTexture, interpolatedTextureCoords)*
        #endif
        specularColor;

    /* Ambient color */
    color = finalAmbientColor;

    mediump vec3 normalizedTransformedNormal = normalize(transformedNormal);

    for(int i = 0; i != LIGHT_COUNT; ++i) {
        highp vec3 lightDirection;
        highp float attenuation;

        /* Directional light */
        if(lights[i].position.w == 0.0) {
            attenuation = 1.0;
            lightDirection = normalize(-vec3(lights[i].position));
        }
        /* Pointlight or Spotlight */
        else {
            /* cameraDirection = -transformedPosition */
            lightDirection = vec3(lights[i].position) + cameraDirection;

            highp float distance = length(lightDirection);

            attenuation = lights[i].intensity / (lights[i].constantAttenuation + 
                                                    distance * (lights[i].linearAttenuation + distance * lights[i].quadraticAttenuation));

            lightDirection = normalize(lightDirection);

            /* Spotlight */
            if(lights[i].spotCutoff < M_PI/2.0) {
                float clampedCosine = max(0.0, dot(-lightDirection, lights[i].spotDirection));
                /* Outsise of spotlight cone? */
                if(clampedCosine < cos(lights[i].spotCutoff)) {
                    attenuation = 0.0;
                }
                else {
                    attenuation = attenuation * pow(clampedCosine, lights[i].spotExponent);
                }
            }
        }

        highp float intensity = dot(normalizedTransformedNormal, lightDirection);

        /* Diffuse color */
        highp vec4 diffuseReflection = attenuation * lights[i].diffuse * finalDiffuseColor * max(0.0, intensity);  

        highp vec4 specularReflection = vec4(0.0, 0.0, 0.0, 1.0);

        /* Specular color if needed */
        if(intensity > 0.0) {
            highp vec3 reflection = reflect(-lightDirection, normalizedTransformedNormal);
            highp float specularity = pow(max(0.0, dot(normalize(cameraDirection), reflection)), shininess + 1e-8);
            specularReflection = attenuation * lights[i].specular * finalSpecularColor * specularity;
        }

        color += diffuseReflection + specularReflection;
    }
}
