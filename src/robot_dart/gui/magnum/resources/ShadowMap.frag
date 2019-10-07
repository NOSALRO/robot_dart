#ifndef NEW_GLSL
#define in varying
#define depth gl_FragDepth
#endif

#ifdef NEW_GLSL
layout(location = 0) out float depth;
#endif

void main() {
    depth = gl_FragCoord.z;
}