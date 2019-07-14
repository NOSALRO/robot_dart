#ifndef NEW_GLSL
#define in attribute
#define out varying
#endif

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 0)
#endif
uniform highp mat4 transformationMatrix;

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 1)
#endif
uniform highp mat4 projectionMatrix;

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 2)
#endif
uniform mediump mat3 normalMatrix;

#ifdef EXPLICIT_ATTRIB_LOCATION
layout(location = POSITION_ATTRIBUTE_LOCATION)
#endif
in highp vec4 position;

#ifdef EXPLICIT_ATTRIB_LOCATION
layout(location = NORMAL_ATTRIBUTE_LOCATION)
#endif
in mediump vec3 normal;

#ifdef TEXTURED
#ifdef EXPLICIT_ATTRIB_LOCATION
layout(location = TEXTURECOORDINATES_ATTRIBUTE_LOCATION)
#endif
in mediump vec2 textureCoords;

out mediump vec2 interpolatedTextureCoords;
#endif

out mediump vec3 transformedNormal;
out highp vec3 cameraDirection;

void main() {
    /* Transformed vertex position */
    highp vec4 transformedPosition4 = transformationMatrix*position;
    highp vec3 transformedPosition = transformedPosition4.xyz/transformedPosition4.w;

    /* Transformed normal vector */
    transformedNormal = normalMatrix*normal;

    /* Direction to the camera */
    cameraDirection = -transformedPosition;

    /* Transform the position */
    gl_Position = projectionMatrix*transformedPosition4;

    #ifdef TEXTURED
    /* Texture coordinates, if needed */
    interpolatedTextureCoords = textureCoords;
    #endif
}
