#version 330 core

in vec2 fragPos;    // vertexShader-bõl

uniform int isLine; // 1: vonal, 0: nem vonal
uniform float isIntersecting;   // színátmenet vezérlése
uniform int alternateColors;    // színek változtatása

out vec4 outColor;

void main()
{
    if(isLine == 1)
    {
        outColor = vec4(0,0,1,1);   // vonal színe: kék
        return;
    }

    // A négyzet körré alakítása
    float r = 50.0;
    float dist = length(fragPos);   // Távolság a középponttól

    if(dist > r) discard;   // ha kívül esik, akkor a pixel eldobása

    // A szín logikája, két szín közötti interpolálás és plusz színek
    vec3 red;
    vec3 green;

    if(alternateColors == 1)
    {
        red   = vec3(1.0,0.0,1.0);
        green = vec3(0.6,0.8,1.0);
    }
    else
    {
        red   = vec3(1.0,0.0,0.0);
        green = vec3(0.0,1.0,0.0);
    }

    vec3 c1 = (isIntersecting < 0.5) ? red : green;
    vec3 c2 = (isIntersecting < 0.5) ? green : red;

    vec3 color = mix(c1, c2, dist / r); // Sugár menti átmenet

    outColor = vec4(color,1);
}