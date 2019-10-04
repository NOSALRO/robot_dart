layout(location = 0) out float gl_FragDepth;

void main() {
    gl_FragDepth = gl_FragCoord.z;
}