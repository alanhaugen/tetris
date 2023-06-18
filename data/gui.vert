#version 330 core
// es
//core

// ES requires setting precision qualifier
// Can be mediump or highp
precision highp float; // affects all floats (vec3, vec4 etc)

// Thanks to Gerdelan, Anton. Anton's OpenGL 4 Tutorials (p. 364). Kindle Edition.

layout(location = 0) in vec3 vVertex;	//object space vertex position
layout(location = 1) in vec4 vColor;	//per-vertex colour
layout(location = 2) in vec4 vNormal;	//per-vertex normals
layout(location = 3) in vec2 vTexcoord;	//per-vertex texcoord
layout(location = 6) in int  vGlyph;	//glyph per vertex

smooth out vec2 vSmoothTexcoord;

uniform vec2 pos;
uniform float scaleX;
uniform float scaleY;
uniform int width;
uniform int height;
uniform int totalWidth;
uniform int totalHeight;
uniform int index;
uniform int screenWidth;
uniform int screenHeight;

out float o_index;
out float o_width;
out float o_height;
out float o_totalwidth;
out float o_totalheight;

void main()
{
    float aspectRatio = float(screenWidth) / float(screenHeight);

    float w = float(width)  * float(scaleX);
    float h = float(height) * float(scaleY);

    // not sure if aspectRatio stuff makes any sense
    gl_Position = vec4((vVertex.x / float(screenWidth)) * w, (vVertex.y / float(screenHeight)) * h, 0.0, 1.0);
    vSmoothTexcoord = vTexcoord;

    float halfScreenWidth  = float(screenWidth)  / 2.0f;
    float halfScreenHeight = float(screenHeight) / 2.0f;

    gl_Position.x += -1.0 + (float(width) / 2.0f) / halfScreenWidth  + pos.x / halfScreenWidth;
    gl_Position.y -= -1.0 + (float(height) / 2.0f) / halfScreenHeight + pos.y / halfScreenHeight;// - 11)); // 11 is the top of a window on windows

    if (vGlyph == -1)
    {
        o_index = float(index);
    }
    else
    {
        o_index = float(vGlyph);
    }

    o_width = float(width);
    o_height = float(height);
    o_totalwidth = float(totalWidth);
    o_totalheight = float(totalHeight);
}
