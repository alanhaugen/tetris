#version 330 core
//es

// ES requires setting precision qualifier
// Can be mediump or highp
precision highp float; // affects all floats (vec3, vec4 etc)

layout(location = 0) in vec3 vVertex;	// object space vertex position
layout(location = 1) in vec4 vColor;	// per-vertex colour
//layout(location = 2) in vec4 vNormal;	// per-vertex normals
layout(location = 3) in vec2 vTexcoord;	// per-vertex texcoord

uniform mat4 MVP;	                //combined modelview projection matrix

smooth out vec3 vSmoothTexcoord;

void main()
{
    vSmoothTexcoord = vVertex;
    gl_Position = MVP * vec4(vVertex, 1.0);
}

