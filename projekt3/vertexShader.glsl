#version 330
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

out vec3 varyingFragmentPosition;
out vec3 varyingNormal;
out vec2 varyingTexCoord;

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat3 inverseTransposeMatrix;

void main(void) {
    varyingFragmentPosition = vec3(matModel * vec4(aPosition, 1.0));
    varyingNormal = inverseTransposeMatrix * aNormal;
    
    // Gömbi UV generálás
    float pi = 3.14159265359;
    float u = atan(aPosition.z, aPosition.x) / (2.0 * pi) + 0.5;
    float v = asin(aPosition.y / 0.5) / pi + 0.5;
    varyingTexCoord = vec2(u, v); 
    
    gl_Position = matProjection * matView * vec4(varyingFragmentPosition, 1.0);
}