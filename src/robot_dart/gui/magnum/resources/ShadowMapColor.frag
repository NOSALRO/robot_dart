#ifdef EXPLICIT_ATTRIB_LOCATION
layout(location = 0)
#endif
out lowp vec4 color;

#ifdef TEXTURED
#ifdef EXPLICIT_TEXTURE_LAYER
layout(binding = 1)
#endif
uniform lowp sampler2D diffuseTexture;
#endif

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 2)
#endif
uniform lowp vec4 diffuseColor
    #if !defined(GL_ES) && defined(TEXTURED)
    = vec4(1.0)
    #endif
    ;

#if defined(TEXTURED)
in mediump vec2 interpolatedTextureCoords;
#endif

void main() {
    lowp vec4 finalDiffuseColor =
        #ifdef TEXTURED
        texture(diffuseTexture, interpolatedTextureCoords)*
        #endif
        diffuseColor;
    /* Ignore opaque pixels */
    if(finalDiffuseColor.a == 1.)
        discard;

    color = finalDiffuseColor;
    color.rgb *= (1. - finalDiffuseColor.a);
}