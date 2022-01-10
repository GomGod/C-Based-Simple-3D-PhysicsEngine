#include"main.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;

bool firstMouse = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // y 좌표의 범위는 밑에서부터 위로가기 때문에 반대로 바꿉니다.
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.3f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void FramebufferSizeCallback(GLFWwindow* window , int width, int height)
{
	glViewport(0, 0, width, height); //창 크기가 변경되면 viewport를 창 크기에 맞춤
	cout << "called";
}


void ProcessInput(GLFWwindow* window)
{
	float cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

const char* vertexShaderPath = "vertexShader.vs";
const char* fragmentShaderPath = "fragmentShader.fs";
const char* lampFragmentShaderPath = "lampFragmentShader.fs";

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//OpenGL 3.3 버전을 사용한다는 의미
	//       _ => Major
	//         - => Minor
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//OpenGL Core_Profile 모드를 이용하여 개발	

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize GLAD\n";
		return -1;
	}

	glViewport(0, 0, 800, 600);

	//Shader compile with out shader class

	Shader ourShader(vertexShaderPath, fragmentShaderPath);
	Shader lampShader(vertexShaderPath, lampFragmentShaderPath);

	Sphere sphere;	

	unsigned int sphereVAO;
	glGenVertexArrays(1, &sphereVAO);
	glBindVertexArray(sphereVAO);

	unsigned int vertexVBO;
	glGenBuffers(1, &vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.getInterleavedVertexAttribSize(), sphere.getInterleavedVertexAttrib(), GL_STATIC_DRAW);

	unsigned int indexEBO;
	glGenBuffers(1, &indexEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.getIndexSize(), sphere.getIndicies(), GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // vertex
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float))); // normal
	glEnableVertexAttribArray(0);	
	glEnableVertexAttribArray(1);


	unsigned int lightSphereVAO;
	glGenVertexArrays(1, &lightSphereVAO);
	glBindVertexArray(lightSphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexEBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	
	glBindVertexArray(0);

	glm::vec3 lightPos(5.0f, 5.0f, 5.0f);
	ourShader.use();	
	ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	ourShader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
	
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	int indexCount = sphere.getIndexCount();
		
	//rendering loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); //Clear

		//Camera
		float radius = 20.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;


		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::mat4(1.0f);	

		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));	
		projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / 600, 0.1f, 100.0f);
		//Camera

		//Model
		ourShader.use(); // 사용 shader program	
		ourShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection);	

		glBindVertexArray(sphereVAO); // VAO 바인딩	
		glm::mat4 model = glm::mat4(1.0f);		
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));
		ourShader.setMat4("model", model);

		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)0);
		//Model

		//Lamp
		lampShader.use();
		glBindVertexArray(lightSphereVAO);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("model", model);

		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)0);
		//Lamp

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	
	glDeleteVertexArrays(1, &sphereVAO);
	glDeleteVertexArrays(1, &lightSphereVAO);
	glDeleteBuffers(1, &vertexVBO);	
	glDeleteBuffers(1, &indexEBO);

	glfwTerminate();
	return 0;
}
	

