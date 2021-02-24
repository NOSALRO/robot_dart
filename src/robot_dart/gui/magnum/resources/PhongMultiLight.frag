#extension GL_ARB_texture_cube_map_array : require

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
    highp vec4 worldPosition;
    bool castsShadows;
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

#ifdef EXPLICIT_TEXTURE_LAYER
layout(binding = 3)
#endif
uniform sampler2DArrayShadow shadowTextures;

#ifdef EXPLICIT_TEXTURE_LAYER
layout(binding = 4)
#endif
uniform samplerCubeArrayShadow cubeMapTextures;

#ifdef EXPLICIT_TEXTURE_LAYER
layout(binding = 5)
#endif
uniform sampler2DArray shadowColorTextures;

#ifdef EXPLICIT_TEXTURE_LAYER
layout(binding = 6)
#endif
uniform samplerCubeArray cubeMapColorTextures;

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 8)
#endif
uniform float farPlane;

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 9)
#endif
uniform bool isShadowed;

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 10)
#endif
uniform bool drawTransparentShadows;

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 11)
#endif
uniform float specularStrength = 0.5;

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 12)
#endif
uniform lightSource lights[LIGHT_COUNT];

in mediump vec3 transformedNormal;
in highp vec3 cameraDirection;
in highp vec3 worldPosition;
in highp vec4 lightSpacePositions[LIGHT_COUNT];

#if defined(AMBIENT_TEXTURE) || defined(DIFFUSE_TEXTURE) || defined(SPECULAR_TEXTURE)
in mediump vec2 interpolatedTextureCoords;
#endif

#ifdef NEW_GLSL
out lowp vec4 color;
#endif

float visibilityCalculation(int index, float bias)
{
    vec4 fragPosLightSpace = lightSpacePositions[index];
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    // projCoords = projCoords * 0.5 + 0.5;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    if(currentDepth > 1. || projCoords.x < 0. || projCoords.x >= 1. || projCoords.y < 0. || projCoords.y >= 1.)
        return 1.;
    // float visibility = texture(shadowTextures, vec4(projCoords.xy, index, currentDepth - bias));
    float visibility = 0.;
    vec2 texelSize = 0.5 / textureSize(shadowTextures, 0).xy;
    for(int x = -2; x <= 2; ++x)
        for(int y = -2; y <= 2; ++y)
            visibility += texture(shadowTextures, vec4(projCoords.xy + vec2(x, y) * texelSize, index, currentDepth - bias));
    visibility /= 16.;
    visibility = clamp(visibility, 0., 1.);

    // return 1. - 0.7 * (1. - visibility);
    return visibility;
}

vec3 shadowColorCalculation(int index)
{
    vec4 fragPosLightSpace = lightSpacePositions[index];
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    vec3 colorShadow = vec3(0.);
    vec2 texelSize = 0.5 / textureSize(shadowColorTextures, 0).xy;
    for(int x = -2; x <= 2; ++x)
        for(int y = -2; y <= 2; ++y)
            colorShadow += texture(shadowColorTextures, vec3(projCoords.xy + vec2(x, y) * texelSize, index)).rgb;
    colorShadow /= 16.;
    colorShadow = clamp(colorShadow, vec3(0.), vec3(1.));

    return colorShadow;
}

float visibilityCalculationPointLight(int index, float bias)
{
    vec3 direction = worldPosition - lights[index].worldPosition.xyz;
    float depth = length(direction) / farPlane;
    if(depth > 1.)
        return 1.;
    float visibility = 0.;

    vec3 sampleOffsetDirections[20] = vec3[]
    (
        vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
        vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
        vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
        vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
        vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );

    float diskRadius = (1.0 + (length(cameraDirection) / farPlane)) / 400.0;//0.01;
    for(int i = 0; i < 20; ++i)
        visibility += texture(cubeMapTextures, vec4(normalize(direction) + sampleOffsetDirections[i] * diskRadius, index), depth - bias);
    visibility /= 20.;

    // float samples = 4.0;
    // float offset = 0.005;
    // for(float x = -offset; x < offset; x += offset / (samples * 0.5))
    // {
    //     for(float y = -offset; y < offset; y += offset / (samples * 0.5))
    //     {
    //         for(float z = -offset; z < offset; z += offset / (samples * 0.5))
    //         {
    //             visibility += texture(cubeMapTextures, vec4(normalize(direction) + vec3(x, y, z), index), depth - bias);
    //         }
    //     }
    // }
    // visibility /= (samples * samples * samples);

    visibility = clamp(visibility, 0., 1.);
    // return 1. - 0.7 * (1. - visibility);
    return visibility;
}

vec3 shadowColorCalculationPointLight(int index)
{
    vec3 direction = worldPosition - lights[index].worldPosition.xyz;
    vec3 colorShadow = vec3(0.);

    vec3 sampleOffsetDirections[20] = vec3[]
    (
        vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
        vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
        vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
        vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
        vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );

    float diskRadius = (1.0 + (length(cameraDirection) / farPlane)) / 400.0;//0.01;
    for(int i = 0; i < 20; ++i)
        colorShadow += texture(cubeMapColorTextures, vec4(normalize(direction) + sampleOffsetDirections[i] * diskRadius, index)).rgb;
    colorShadow /= 20.;
    colorShadow = clamp(colorShadow, vec3(0.), vec3(1.));

    return colorShadow;
}

void main() {
    lowp vec4 finalAmbientColor =
        #ifdef AMBIENT_TEXTURE
        texture(ambientTexture, interpolatedTextureCoords)*
        #endif
        ambientColor;
    lowp vec4 finalDiffuseColor =
        #ifdef DIFFUSE_TEXTURE
        texture(diffuseTexture, interpolatedTextureCoords)*
        #endif
        diffuseColor;
    lowp vec4 finalSpecularColor =
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
        bool spec = any(greaterThan(lights[i].specular.rgb, vec3(0.0))) && specularStrength > 0.0;
        bool isPoint = false;

         if(!any(greaterThan(lights[i].diffuse.rgb, vec3(0.0))) && !spec)
            continue;

        /* Directional light */
        if(lights[i].position.w == 0.0) {
            attenuation = lights[i].intensity;
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
            else
                isPoint = true;
        }

        highp float intensity = dot(normalizedTransformedNormal, lightDirection);
        float visibility = 1.;
        vec3 colorShadow = vec3(1.);
        if(isShadowed && lights[i].castsShadows) {
            float bias = 0.0005;//max(0.0001, 0.0005*tan(acos(intensity)));//0.001;// max(0.05 * (1.0 - intensity), 0.005);
            if(!isPoint) {
                visibility = visibilityCalculation(i, bias);

                if(drawTransparentShadows && visibility > 0. && finalDiffuseColor.a == 1.) {
                    colorShadow = shadowColorCalculation(i);
                }
            }
            else {
                bias = 0.003;
                visibility = visibilityCalculationPointLight(i, bias);

                if(drawTransparentShadows && visibility > 0. && finalDiffuseColor.a == 1.) {
                    colorShadow = shadowColorCalculationPointLight(i);
                }
            }
        }

        /* Diffuse color */
        highp vec3 diffuseReflection = attenuation * lights[i].diffuse.rgb * finalDiffuseColor.rgb * max(0.0, intensity);

        highp vec3 specularReflection = vec3(0.0);

        /* Specular color if needed */
        if(spec && intensity > 0.0) {
            highp vec3 reflection = reflect(-lightDirection, normalizedTransformedNormal);
            highp float specularity = pow(max(0.0, dot(normalize(cameraDirection), reflection)), shininess + 1e-8);
            specularReflection = attenuation * lights[i].specular.rgb * finalSpecularColor.rgb * specularity;
        }

        color.rgb += (diffuseReflection + specularStrength * specularReflection) * visibility * colorShadow;
    }

    color.a = finalDiffuseColor.a;
}
