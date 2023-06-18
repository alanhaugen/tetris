#version 330 core
//es

// ES requires setting precision qualifier
// Can be mediump or highp
precision highp float; // affects all floats (vec3, vec4 etc)

// Thanks to https://web.archive.org/web/20180816064924/http://www.sunandblackcat.com/tipFullView.php?l=eng&topicid=30

// attributes
/*layout(location = 0) in vec3 i_position; // xyz - position
layout(location = 1) in vec3 i_normal; // xyz - normal
layout(location = 2) in vec2 i_texcoord0; // xy - texture coords*/

layout(location = 0) in vec3 i_position;        // object space vertex position
layout(location = 1) in vec4 vColor;	        // per-vertex colour
layout(location = 2) in vec3 i_normal;	        // per-vertex normals
layout(location = 3) in vec2 i_texcoord0;	// per-vertex texcoord

// matrices
uniform mat4 u_modelMat;
uniform mat3 u_normalMat;

// position of light and camera
uniform vec3 u_lightPosition;
uniform vec3 u_cameraPosition;

// data for fragment shader
out vec3 o_normal;
out vec3 o_toLight;
out vec3 o_toCamera;
out vec2 o_texcoords;

uniform vec4 colour;
out vec4 o_colour;
uniform mat4 MVP;	// combined modelview projection matrix

///////////////////////////////////////////////////////////////////

void main(void)
{
    // position in world space
    vec4 worldPosition = u_modelMat * vec4(i_position, 1);

    // normal in world space
    o_normal = normalize(u_normalMat * i_normal);

    // direction to light
    o_toLight = normalize(u_lightPosition - worldPosition.xyz);

    // direction to camera
    o_toCamera = normalize(u_cameraPosition - worldPosition.xyz);

    // texture coordinates to fragment shader
    o_texcoords = i_texcoord0;

    // screen space coordinates of the vertex
    gl_Position = MVP * worldPosition;

    // Set colour
    o_colour = colour;
}
