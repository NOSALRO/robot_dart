layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 4)
#endif
uniform highp mat4 shadowMatrices[6];

#ifdef EXPLICIT_UNIFORM_LOCATION
layout(location = 3)
#endif
uniform highp int index;

out highp vec4 worldPosition; // FragPos from GS (output per emitvertex)

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = index * 6 + face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            worldPosition = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * worldPosition;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  