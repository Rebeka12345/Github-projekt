enum eVertexArrayObject {
	VAOVerticesData,
	VAOSphere,
	VAOCount
};
enum eBufferObject {
	VBOVerticesData,
	VBOSphere,
	EBOSphere,      // ← ÚJ index buffer
	BOCount
};
enum eProgram {
	QuadScreenProgram,
	ProgramCount
};
enum eTexture {
	NoTexture,
	TextureCount
};

#include "common.cpp"

GLchar	windowTitle[] = "Tóth Rebeka";
GLuint	cameraPositionLoc;
float   radius = 9.0f;
float   cameraAngle = 0.0f;
float   cameraHeight = 0.0f;
bool    lightingOn = true;
float   lightAngle = 0.0f;
float   dsphere = 0.5f;
vec3 diffuseLightColor = vec3(1.0f, 0.0f, 0.0f);
vec3    cameraPosition = vec3(radius, 0.0f, 0.0f);
vec3    cameraTarget = vec3(0.0f, 0.0f, 0.0f);
vec3    cameraUpVector = vec3(0.0f, 0.0f, 1.0f);
GLuint  lightPositionLoc;
vec3    lightPosition;
GLuint  inverseTransposeMatrixLoc;
mat3    inverseTransposeMatrix;
GLuint  sunTexture;
std::vector<GLfloat> sphereVertices;
std::vector<GLuint>  sphereIndices;

GLfloat verticesData[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

void generateSphere(float radius, int sectors, int stacks) {
	sphereVertices.clear();
	sphereIndices.clear();

	for (int i = 0; i <= stacks; i++) {
		float phi = M_PI / 2.0f - i * (M_PI / stacks);
		float y = radius * sin(phi);
		float xz = radius * cos(phi);

		for (int j = 0; j <= sectors; j++) {
			float theta = j * (2.0f * M_PI / sectors);
			float x = xz * cos(theta);
			float z = xz * sin(theta);

			sphereVertices.push_back(x);
			sphereVertices.push_back(y);
			sphereVertices.push_back(z);
			sphereVertices.push_back(x / radius);
			sphereVertices.push_back(y / radius);
			sphereVertices.push_back(z / radius);
		}
	}

	for (int i = 0; i < stacks; i++) {
		for (int j = 0; j < sectors; j++) {
			int first = i * (sectors + 1) + j;
			int second = first + sectors + 1;

			sphereIndices.push_back(first);
			sphereIndices.push_back(second);
			sphereIndices.push_back(first + 1);

			sphereIndices.push_back(second);
			sphereIndices.push_back(second + 1);
			sphereIndices.push_back(first + 1);
		}
	}
}

void initSphere() {
	generateSphere(0.5f, 36, 18);

	glBindVertexArray(VAO[VAOSphere]);

	glBindBuffer(GL_ARRAY_BUFFER, BO[VBOSphere]);
	glBufferData(GL_ARRAY_BUFFER,
		sphereVertices.size() * sizeof(GLfloat),
		sphereVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BO[EBOSphere]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sphereIndices.size() * sizeof(GLuint),
		sphereIndices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

void initShaderProgram() {
	ShaderInfo shader_info[ProgramCount][3] = { {
		{ GL_VERTEX_SHADER,   "./vertexShader.glsl" },
		{ GL_FRAGMENT_SHADER, "./fragmentShader.glsl" },
		{ GL_NONE, nullptr } } };

	for (int programItem = 0; programItem < ProgramCount; programItem++) {
		program[programItem] = LoadShaders(shader_info[programItem]);
		locationMatModel = glGetUniformLocation(program[programItem], "matModel");
		locationMatView = glGetUniformLocation(program[programItem], "matView");
		locationMatProjection = glGetUniformLocation(program[programItem], "matProjection");
	}

	glBindVertexArray(VAO[VAOVerticesData]);
	glBindBuffer(GL_ARRAY_BUFFER, BO[VBOVerticesData]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glUseProgram(program[QuadScreenProgram]);

	projectionType = Perspective;
	matModel = mat4(1.0);
	matView = lookAt(cameraPosition, cameraTarget, cameraUpVector);

	glUniformMatrix4fv(locationMatModel, 1, GL_FALSE, value_ptr(matModel));
	glUniformMatrix4fv(locationMatView, 1, GL_FALSE, value_ptr(matView));
	glUniformMatrix4fv(locationMatProjection, 1, GL_FALSE, value_ptr(matProjection));

	inverseTransposeMatrixLoc = glGetUniformLocation(program[QuadScreenProgram], "inverseTransposeMatrix");
	lightPositionLoc = glGetUniformLocation(program[QuadScreenProgram], "lightPosition");
	cameraPositionLoc = glGetUniformLocation(program[QuadScreenProgram], "cameraPosition");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	sunTexture = SOIL_load_OGL_texture(
		"sun.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
	);
	if (sunTexture == 0) {
		cout << "HIBA: A textura nem toltheto be! Indok: " << SOIL_last_result() << endl;
	}
	initSphere();
	glEnable(GL_DEPTH_TEST);
}

void computeModelMatrix(vec3 translation) {
	matModel = translate(mat4(1.0f), translation);
	glUniformMatrix4fv(locationMatModel, 1, GL_FALSE, value_ptr(matModel));
	inverseTransposeMatrix = mat3(inverseTranspose(matModel));
	glUniformMatrix3fv(inverseTransposeMatrixLoc, 1, GL_FALSE, value_ptr(inverseTransposeMatrix));
}

void computeCameraMatrix() {
	cameraPosition.x = radius * cos(cameraAngle);
	cameraPosition.y = radius * sin(cameraAngle);
	cameraPosition.z = cameraHeight;
	matView = lookAt(cameraPosition, cameraTarget, cameraUpVector);
	glUniformMatrix4fv(locationMatView, 1, GL_FALSE, value_ptr(matView));
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static GLdouble lastFrame = 0.0;
	GLdouble deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;

	// 1. KAMERA
	if (keyboard[GLFW_KEY_LEFT])  cameraAngle -= 2.0f * (float)deltaTime;
	if (keyboard[GLFW_KEY_RIGHT]) cameraAngle += 2.0f * (float)deltaTime;
	if (keyboard[GLFW_KEY_UP])    cameraHeight += 5.0f * (float)deltaTime;
	if (keyboard[GLFW_KEY_DOWN])  cameraHeight -= 5.0f * (float)deltaTime;
	computeCameraMatrix();

	// 2. FÉNY MOZGATÁSA
	lightAngle += 1.0f * (float)deltaTime;
	lightPosition.x = (2.0f * radius) * cos(lightAngle);
	lightPosition.y = (2.0f * radius) * sin(lightAngle);
	lightPosition.z = 0.0f;

	// 3. PROGRAM + UNIFORMOK
	glUseProgram(program[QuadScreenProgram]);
	glUniform3fv(lightPositionLoc, 1, value_ptr(lightPosition));
	glUniform3fv(cameraPositionLoc, 1, value_ptr(cameraPosition));
	glUniform1i(glGetUniformLocation(program[QuadScreenProgram], "lightingOn"), lightingOn);
	glUniform3fv(glGetUniformLocation(program[QuadScreenProgram], "diffuseLightColor"), 1, value_ptr(diffuseLightColor));

	// 4. KOCKÁK RAJZOLÁSA
	glUniform1i(glGetUniformLocation(program[QuadScreenProgram], "isLightSource"), false);
	glUniform3fv(glGetUniformLocation(program[QuadScreenProgram], "objectColor"),
		1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));

	glBindVertexArray(VAO[VAOVerticesData]);
	computeModelMatrix(vec3(0.0f, 0.0f, 0.0f));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	computeModelMatrix(vec3(0.0f, 0.0f, 2.0f));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	computeModelMatrix(vec3(0.0f, 0.0f, -2.0f));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// 5. NAP RAJZOLÁSA GÖMBBEL + TEXTÚRA
	glUniform1i(glGetUniformLocation(program[QuadScreenProgram], "isLightSource"), true);
	glUniform3fv(glGetUniformLocation(program[QuadScreenProgram], "objectColor"),
		1, value_ptr(diffuseLightColor));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sunTexture);
	glUniform1i(glGetUniformLocation(program[QuadScreenProgram], "tex"), 0);

	matModel = translate(mat4(1.0f), lightPosition);
	glUniformMatrix4fv(locationMatModel, 1, GL_FALSE, value_ptr(matModel));
	inverseTransposeMatrix = mat3(inverseTranspose(matModel));
	glUniformMatrix3fv(inverseTransposeMatrixLoc, 1, GL_FALSE, value_ptr(inverseTransposeMatrix));

	glBindVertexArray(VAO[VAOSphere]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BO[EBOSphere]); // ← ÚJ SOR
	glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);

	// VISSZAÁLLÍTÁS
	glUniform1i(glGetUniformLocation(program[QuadScreenProgram], "isLightSource"), false);
	glBindVertexArray(VAO[VAOVerticesData]);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	windowWidth = glm::max(width, 1);
	windowHeight = glm::max(height, 1);
	GLfloat aspectRatio = (GLfloat)windowWidth / (GLfloat)windowHeight;
	glViewport(0, 0, windowWidth, windowHeight);
	if (projectionType == Orthographic)
		if (windowWidth < windowHeight)
			matProjection = ortho(-worldSize, worldSize, -worldSize / aspectRatio, worldSize / aspectRatio, -100.0, 100.0);
		else
			matProjection = ortho(-worldSize * aspectRatio, worldSize * aspectRatio, -worldSize, worldSize, -100.0, 100.0);
	else
		matProjection = perspective(radians(55.0f), aspectRatio, 0.1f, 100.0f);
	glUniformMatrix4fv(locationMatProjection, 1, GL_FALSE, value_ptr(matProjection));
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	modifiers = mods;
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (action == GLFW_PRESS)   keyboard[key] = GL_TRUE;
	else if (action == GLFW_RELEASE) keyboard[key] = GL_FALSE;

	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		lightingOn = !lightingOn;
		cout << "Vilagitas: " << (lightingOn ? "BE" : "KI") << endl;
	}
	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		projectionType = Orthographic;
		framebufferSizeCallback(window, windowWidth, windowHeight);
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		projectionType = Perspective;
		framebufferSizeCallback(window, windowWidth, windowHeight);
	}
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {}

int main(void) {
	init(3, 3, GLFW_OPENGL_COMPAT_PROFILE);
	initShaderProgram();
	framebufferSizeCallback(window, windowWidth, windowHeight);
	setlocale(LC_ALL, "hu_HU");
	cout << "Billentyűk:" << endl;
	cout << "ESC  - kilépés" << endl;
	cout << "L    - fény ki/be" << endl;
	cout << "O    - ortografikus nézet" << endl;
	cout << "P    - perspektív nézet" << endl;
	cout << "nyilak - kamera mozgatás" << endl;

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	cleanUpScene(EXIT_SUCCESS);
	return EXIT_SUCCESS;
}