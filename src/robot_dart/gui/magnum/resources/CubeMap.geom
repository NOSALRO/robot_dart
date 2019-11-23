layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 5)
#endif
uniform highp mat4 shadowMatrices[6];

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 3)
#endif
uniform int lightIndex;

out highp vec4 worldPosition;
flat out int lightNumber;

#ifdef TEXTURED
in mediump vec2 interTextureCoords[];
out mediump vec2 interpolatedTextureCoords;
#endif

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = lightIndex * 6 + face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            worldPosition = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * worldPosition;
            #ifdef TEXTURED
            /* Texture coordinates, if needed */
            interpolatedTextureCoords = interTextureCoords[i];
            #endif
            lightNumber = lightIndex;
            EmitVertex();
        }
        EndPrimitive();
    }
}