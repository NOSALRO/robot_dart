#extension GL_ARB_texture_cube_map_array : require

in highp vec4 worldPosition;
flat in int lightNumber;

#ifdef EXPLICIT_ATTRIB_LOCATION
layout(location = 0)
#endif
out lowp vec4 color;

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 1)
#endif
uniform highp vec3 lightPosition;

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 2)
#endif
uniform highp float farPlane;

#ifdef TEXTURED
#ifdef EXPLICIT_TEXTURE_LAYER
layout(binding = 1)
#endif
uniform lowp sampler2D diffuseTexture;
#endif

#ifdef EXPLICIT_TEXTURE_LAYER
layout(binding = 2)
#endif
uniform samplerCubeArrayShadow cubeMapTextures;

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 4)
#endif
uniform lowp vec4 diffuseColor
    #if !defined(GL_ES) && defined(TEXTURED)
    = vec4(1.0)
    #endif
    ;

#if defined(TEXTURED)
in mediump vec2 interpolatedTextureCoords;
#endif

void main()
{
    lowp vec4 finalDiffuseColor =
        #ifdef TEXTURED
        texture(diffuseTexture, interpolatedTextureCoords)*
        #endif
        diffuseColor;

    /* Ignore opaque pixels */
    if(finalDiffuseColor.a == 1.)
        discard;

    // get distance between object and light source
    vec3 direction = worldPosition.xyz - lightPosition;
    float lightDistance = length(direction);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / farPlane;

    float visibility = texture(cubeMapTextures, vec4(normalize(direction), lightNumber), lightDistance);
    // Ignore non fully visible pixels
    if(visibility == 0.)
        discard;

    color = finalDiffuseColor;
    color.rgb *= (1. - finalDiffuseColor.a);

    // write this as modified depth
    gl_FragDepth = lightDistance;
}