#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 0)
#endif
uniform highp mat4 transformationMatrix;

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 1)
#endif
uniform highp mat4 projectionMatrix;

#ifdef EXPLICIT_ATTRIB_LOCATION
layout(location = POSITION_ATTRIBUTE_LOCATION)
#endif
in highp vec4 position;

void main() {
    /* Transformed vertex position */
    highp vec4 transformedPosition4 = transformationMatrix*position;
    /* Transform the position */
    gl_Position = projectionMatrix*transformedPosition4;
}