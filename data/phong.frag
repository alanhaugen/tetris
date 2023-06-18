#version 330 core
//es

// ES requires setting precision qualifier
// Can be mediump or highp
precision highp float; // affects all floats (vec3, vec4 etc)

// Thanks to https://web.archive.org/web/20180816064924/http://www.sunandblackcat.com/tipFullView.php?l=eng&topicid=30

// data from vertex shader
in vec3 o_normal;
in vec3 o_toLight;
in vec3 o_toCamera;
in vec2 o_texcoords;

in vec4 o_colour;

// texture with diffuese color of the object
//layout(location = 0) uniform sampler2D u_diffuseTexture;
uniform sampler2D u_diffuseTexture;

// color for framebuffer
out vec4 resultingColor;

/////////////////////////////////////////////////////////

// parameters of the light and possible values
/*uniform vec3 u_lightAmbientIntensitys; // = vec3(0.6, 0.3, 0);
uniform vec3 u_lightDiffuseIntensitys; // = vec3(1, 0.5, 0);
uniform vec3 u_lightSpecularIntensitys; // = vec3(0, 1, 0);

// parameters of the material and possible values
uniform vec3 u_matAmbientReflectances; // = vec3(1, 1, 1);
uniform vec3 u_matDiffuseReflectances; // = vec3(1, 1, 1);
uniform vec3 u_matSpecularReflectances; // = vec3(1, 1, 1);
uniform float u_matShininess; // = 64;*/


// TODO: readd these !!!! as uniforms
/*
    uniform vec3 u_lightAmbientIntensity;
    uniform vec3 u_lightDiffuseIntensity;
    uniform vec3 u_lightSpecularIntensity;

    // parameters of the material and possible values
    uniform vec3 u_matAmbientReflectance;
    uniform vec3 u_matDiffuseReflectance;
    uniform vec3 u_matSpecularReflectance;
    uniform float u_matShininess;
*/
//TODO: remove these, use uniforms instead (see above)
    vec3 u_lightAmbientIntensity = vec3(0.6, 0.3, 0.0);
    vec3 u_lightDiffuseIntensity = vec3(1.0, 0.5, 0.0);
    vec3 u_lightSpecularIntensity = vec3(0.0, 1.0, 0.0);

    // parameters of the material and possible values
    vec3 u_matAmbientReflectance = vec3(1.0, 1.0, 1.0);
    vec3 u_matDiffuseReflectance = vec3(1.0, 1.0, 1.0);
    vec3 u_matSpecularReflectance = vec3(1.0, 1.0, 1.0);
    float u_matShininess = 64.0;

smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader


/////////////////////////////////////////////////////////

// returns intensity of reflected ambient lighting
vec3 ambientLighting()
{
   return u_matAmbientReflectance * u_lightAmbientIntensity;
}

float glsl_clamp(float i, float min, float max)
{
    if(i < min)
    {
        i = min;
    }
    else if(i > max)
    {
        i = max;
    }

    return i;
}

// returns intensity of diffuse reflection
vec3 diffuseLighting(in vec3 N, in vec3 L)
{
    // calculation as for Lambertian reflection
    float diffuseTerm = glsl_clamp(dot(N, L), 0.0f, 1.0f) ;
    return u_matDiffuseReflectance * u_lightDiffuseIntensity * diffuseTerm;
}

// returns intensity of specular reflection
vec3 specularLighting(in vec3 N, in vec3 L, in vec3 V)
{
    float specularTerm = 0.0f;

    // calculate specular reflection only if
    // the surface is oriented to the light source
    if(dot(N, L) > 0.0f)
    {
        // half vector
        vec3 H = normalize(L + V);
        specularTerm = pow(dot(N, H), u_matShininess);
    }
    return u_matSpecularReflectance * u_lightSpecularIntensity * specularTerm;
}

void main(void)
{
    // normalize vectors after interpolation
    vec3 L = normalize(o_toLight);
    vec3 V = normalize(o_toCamera);
    vec3 N = normalize(o_normal);

    // get Blinn-Phong reflectance components
    vec3 Iamb = ambientLighting();
    vec3 Idif = diffuseLighting(N, L);
    vec3 Ispe = specularLighting(N, L, V);

    // diffuse color of the object from texture
    vec3 diffuseColor = o_colour.rgb;//texture(u_diffuseTexture, o_texcoords).rgb;

    // combination of all components and diffuse color of the object
    resultingColor.xyz = diffuseColor * (Iamb + Idif + Ispe);
    resultingColor.a = 1.0f;
}
