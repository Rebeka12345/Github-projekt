#version 330 core

layout(location = 0) in vec2 aPos;  // Bemenet vertex pozíció VBO-ból

uniform mat4 MVP;   // Model-View-Projection mátrix

out vec2 fragPos;   // A fragmentShader-nek átadott adat

void main()
{
    fragPos = aPos; // Az eredeti lokális koordináta
    gl_Position = MVP * vec4(aPos, 0.0, 1.0);   // A vertex átalakítása képernyõ koordinátába
}