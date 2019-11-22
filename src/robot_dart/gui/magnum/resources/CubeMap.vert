#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 0)
#endif
uniform highp mat4 transformationMatrix;

#ifdef EXPLICIT_ATTRIB_LOCATION
layout(location = POSITION_ATTRIBUTE_LOCATION)
#endif
in highp vec4 position;

#ifdef TEXTURED
#ifdef EXPLICIT_ATTRIB_LOCATION
layout(location = TEXTURECOORDINATES_ATTRIBUTE_LOCATION)
#endif
in mediump vec2 textureCoords;

out mediump vec2 interTextureCoords;
#endif

void main() {
    /* Transform the position */
    gl_Position = transformationMatrix * position;

    #ifdef TEXTURED
    /* Texture coordinates, if needed */
    interTextureCoords = textureCoords;
    #endif
}