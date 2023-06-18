#version 330 core
//es

// ES requires setting precision qualifier
// Can be mediump or highp
precision highp float; // affects all floats (vec3, vec4 etc)

smooth in vec3 vSmoothTexcoord;
uniform samplerCube cube_texture;

out vec4 frag_colour;

void main()
{
    frag_colour = texture(cube_texture, vSmoothTexcoord);
}

