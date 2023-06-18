#version 330 core
//es

// ES requires setting precision qualifier
// Can be mediump or highp
precision highp float; // affects all floats (vec3, vec4 etc)

layout(location=0) out vec4 vFragColor;	//fragment shader output

//input form the vertex shader
smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader
smooth in vec2 vSmoothTexcoord;
uniform sampler2D textureSampler;
uniform bool uEnableTexture;

void main()
{

    vec4 final = vec4(1.0, 0.0, 0.0, 1.0);
    if (uEnableTexture)
    {
        final.x = vSmoothTexcoord.x;
        final.y = vSmoothTexcoord.y;
        final = texture(textureSampler, vSmoothTexcoord);
    } else {
        //set the interpolated colour as the shader output
        final = vSmoothColor;
    }
    vFragColor = final;
}

