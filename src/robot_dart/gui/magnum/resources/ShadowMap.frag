#ifdef EXPLICIT_ATTRIB_LOCATION
layout(location = 0)
#endif
out float depth;

void main() {
    depth = gl_FragCoord.z;
}