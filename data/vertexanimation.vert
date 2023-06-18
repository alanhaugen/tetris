#version 330 core

layout(location = 0) in vec3 vVertex;	// object space vertex position
layout(location = 1) in vec4 vColor;	// per-vertex colour
layout(location = 2) in vec4 vNormal;	// per-vertex normals
layout(location = 3) in vec2 vTexcoord;	// per-vertex texcoord

//uniform
uniform mat4 MVP;	//combined modelview projection matrix
uniform vec4 colour;
uniform uint frame;
uniform uint verticesQuantity;

//uniform mat4 pose[120];
//uniform mat4 invBindPose[120];

//output from the vertex shader
smooth out vec4 vSmoothColor;		//smooth colour to fragment shader
smooth out vec2 vSmoothTexcoord;

void main()
{
    // assign the per-vertex colour to vSmoothColor varying
    vSmoothColor = colour;//vec4(vColor) * colorTint;
    vSmoothTexcoord = vTexcoord;

    //get the clip space position by multiplying the combined MVP matrix with the object space
    //vertex position
    if (gl_VertexID < int(verticesQuantity + (frame * verticesQuantity)) && gl_VertexID >= int(verticesQuantity * frame))
    {
        gl_Position = MVP * vec4(vVertex, 1.0);
    }
}
