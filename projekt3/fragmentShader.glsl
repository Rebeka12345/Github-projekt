#version 330

in vec3         varyingFragmentPosition;
in vec3         varyingNormal;
in vec2         varyingTexCoord;

uniform vec3    lightPosition;
uniform vec3    cameraPosition;
uniform vec3    diffuseLightColor;
uniform vec3    objectColor;        // ? ÚJ
uniform bool    lightingOn;
uniform bool    isLightSource;
uniform sampler2D tex;

out vec4        outColor;

void main(void) {
    // Ha a Napot rajzoljuk - fényforrás színe, megvilágítás nélkül
  if (isLightSource) {
    vec4 texColor = texture(tex, varyingTexCoord);
    outColor = mix(texColor, vec4(diffuseLightColor, 1.0), 0.3);
    return;
}
    // Ambient
    vec3 ambient = vec3(0.3, 0.3, 0.3);

    if (lightingOn) {
        vec3 n = normalize(varyingNormal);
        vec3 l = normalize(lightPosition - varyingFragmentPosition);
        float diff = max(dot(n, l), 0.0);
        vec3 diffuse = diff * diffuseLightColor * objectColor;
        
        vec3 v = normalize(cameraPosition - varyingFragmentPosition);
        vec3 r = reflect(-l, n);
        float spec = pow(max(dot(v, r), 0.0), 32);
        vec3 specular = spec * diffuseLightColor;
        
        outColor = vec4(ambient * objectColor + diffuse + specular, 1.0);
    } else {
        outColor = vec4(ambient * objectColor, 1.0);
    }
}