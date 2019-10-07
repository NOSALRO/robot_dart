in highp vec4 worldPosition;

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 1)
#endif
uniform highp vec3 lightPosition;

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 2)
#endif
uniform highp float farPlane;

// layout(location = 0) out float depth;

void main()
{
    // get distance between object and light source
    float lightDistance = length(worldPosition.xyz - lightPosition);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / farPlane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}  