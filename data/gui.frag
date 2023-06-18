#version 330 core
//es
//core

// ES requires setting precision qualifier
// Can be mediump or highp
precision highp float; // affects all floats (vec3, vec4 etc)

//layout(location=0)

out vec4 vFragColor;	//fragment shader output

//input form the vertex shader
smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader
smooth in vec2 vSmoothTexcoord;
uniform sampler2D textureSampler;

in float o_index;
in float o_width;
in float o_height;
in float o_totalwidth;
in float o_totalheight;

void main ()
{
    vec4 final;

    vec2 coords = vSmoothTexcoord;

    // Calculate what area of the spritesheet to use
    // (or use entire sheet if width and height are equal to total width and height)
    float x, y, sum;
    x = coords.x * (o_width  / o_totalwidth) + o_index * (o_width  / o_totalwidth);
    y = coords.y * (o_height / o_totalheight);
    sum = x;

    // hack to make spritesheets scroll in the y dimension
    while (sum > 1.0f)
    {
        y += o_height / o_totalheight;
        sum--;
    }

    coords.x = x;
    coords.y = y;

    final = texture(textureSampler, coords);

    if (final.b > 0.4)
        discard;

    vFragColor = final;
}
