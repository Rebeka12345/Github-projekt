#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
using namespace glm;

// Ablak méret megadása, ezek határozzák meg a koordináta rendszert is
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;

// Enumok, hogy ne "varázs" számokkal dolgozzunk hanem nevesített értékekkel
enum eVertexArrayObject { VAO_CIRCLE, VAO_LINE, NumVAOs };
enum eBufferObject { VBO_CIRCLE, VBO_LINE, NumVBOs };

// OpenGL objektumok
GLuint vao[NumVAOs];
GLuint vbo[NumVBOs];

GLuint program; // shader program

// Kör
float cx = WINDOW_WIDTH / 2.0f;
float cy = WINDOW_HEIGHT / 2.0f;
float radius = 50.0f;
float speed = 150.0f;

// Vonal
float lineY = 0.0f;
float lineSpeed = 0.8f;

// Szín animáció
float colorBlend = 0.0f;
float colorTarget = 0.0f;

// Színmód
bool alternateColors = false;

// Shader uniformok
GLuint mvpLocation;
GLuint isLineLocation;
GLuint isIntersectingLocation;
GLuint colorModeLocation;

mat4 projection;
double lastTime = 0.0;

// Shaderek olvasása
string readFile(const char* path)
{
    ifstream f(path);
    if (!f)
    {
        cout << "Shader file not found: " << path << endl;
        exit(-1);
    }
    return string((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
}

// Shaderek betöltése
GLuint LoadShaders(const char* vsPath, const char* fsPath)
{
    string vsCode = readFile(vsPath);
    string fsCode = readFile(fsPath);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vsrc = vsCode.c_str();
    const char* fsrc = fsCode.c_str();

    glShaderSource(vs, 1, &vsrc, NULL);
    glShaderSource(fs, 1, &fsrc, NULL);

    glCompileShader(vs);
    glCompileShader(fs);

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return prog;
}

// Init
void init()
{
    program = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");

    glGenVertexArrays(NumVAOs, vao);
    glGenBuffers(NumVBOs, vbo);

    // Kör (négyzet)
    vec2 quad[6] = {
        vec2(-radius, -radius), vec2(radius, -radius), vec2(radius, radius),
        vec2(-radius, -radius), vec2(radius, radius), vec2(-radius, radius)
    };

    glBindVertexArray(vao[VAO_CIRCLE]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_CIRCLE]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Vonal
    float halfW = (WINDOW_WIDTH / 3.0f) / 2.0f;
    float halfH = 1.5f;

    vec2 line[6] = {
        vec2(-halfW,-halfH), vec2(halfW,-halfH), vec2(halfW,halfH),
        vec2(-halfW,-halfH), vec2(halfW,halfH), vec2(-halfW,halfH)
    };

    glBindVertexArray(vao[VAO_LINE]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[VBO_LINE]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    projection = ortho(0.0f, (float)WINDOW_WIDTH,
        0.0f, (float)WINDOW_HEIGHT);

    mvpLocation = glGetUniformLocation(program, "MVP");
    isLineLocation = glGetUniformLocation(program, "isLine");
    isIntersectingLocation = glGetUniformLocation(program, "isIntersecting");
    colorModeLocation = glGetUniformLocation(program, "alternateColors");
}

// Kör mozgás
void update(double dt)
{
    float nextX = cx + speed * dt;

    if (nextX + radius > WINDOW_WIDTH || nextX - radius < 0)
        speed = -speed;

    cx += speed * dt;
}

// Display
void display(double dt)
{
    update(dt);

    GLFWwindow* win = glfwGetCurrentContext();

    // A/D színmód váltás
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
        alternateColors = true;

    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        alternateColors = false;

    // Háttérszín
    if (alternateColors)
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    else
        glClearColor(1, 1, 0, 1);

    glClear(GL_COLOR_BUFFER_BIT);

    // Vonal mozgatás
    if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS)
        lineY += lineSpeed * dt;
    if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS)
        lineY -= lineSpeed * dt;

    float screenY = WINDOW_HEIGHT / 2 + lineY * (WINDOW_HEIGHT / 2);

    float halfLineHeight = 1.5f;
    float minScreenY = halfLineHeight;
    float maxScreenY = WINDOW_HEIGHT - halfLineHeight;

    if (screenY < minScreenY) screenY = minScreenY;
    if (screenY > maxScreenY) screenY = maxScreenY;

    lineY = (screenY - WINDOW_HEIGHT / 2) / (WINDOW_HEIGHT / 2);

    // Metszés
    float lineLength = WINDOW_WIDTH / 3.0f;
    float lineLeft = WINDOW_WIDTH / 2 - lineLength / 2;
    float lineRight = WINDOW_WIDTH / 2 + lineLength / 2;

    bool intersectX = (cx + radius >= lineLeft) && (cx - radius <= lineRight);
    bool intersectY = fabs(cy - screenY) <= (radius + halfLineHeight);
    bool intersect = intersectX && intersectY;

    colorTarget = intersect ? 0.0f : 1.0f;
    colorBlend += (colorTarget - colorBlend) * 3.0f * dt;

    glUseProgram(program);

    // shadernek küldjük a színmódot
    glUniform1i(colorModeLocation, alternateColors ? 1 : 0);

    // Vonal rajzolás
    mat4 modelLine = translate(mat4(1.0f),
        vec3(WINDOW_WIDTH / 2, screenY, 0));
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE,
        value_ptr(projection * modelLine));
    glUniform1i(isLineLocation, 1);

    glBindVertexArray(vao[VAO_LINE]);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Kör rajzolás
    mat4 modelCircle = translate(mat4(1.0f),
        vec3(cx, cy, 0));
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE,
        value_ptr(projection * modelCircle));
    glUniform1i(isLineLocation, 0);
    glUniform1f(isIntersectingLocation, colorBlend);

    glBindVertexArray(vao[VAO_CIRCLE]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

int main()
{
    if (!glfwInit()) return -1;

    GLFWwindow* window =
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
            "Circle", NULL, NULL);
    if (!window) return -1;

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) return -1;

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    cout << "=============================\n";
    cout << "Gombok:\n";
    cout << "A   ->   Szurke + Kek + Magenta\n";
    cout << "D   ->   Sarga + Zold + Piros\n";
    cout << "=============================\n";

    init();
    lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double t = glfwGetTime();
        double dt = t - lastTime;
        lastTime = t;

        display(dt);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}