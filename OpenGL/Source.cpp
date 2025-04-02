
#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "Shader/Shader.h"
#include "camera.h"
#include "model.h"

#include "iostream"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processinput(GLFWwindow* window);

// Window size
const unsigned int SC_WIDTH = 1600;
const unsigned int SC_HEIGHT = 1200;

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SC_WIDTH / 2.0f;
float lastY = SC_HEIGHT / 2.0f;
bool firstMouse = true;

//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.5f, 1.0f, 2.0f);


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Tell GLFW the OpenGL version is 3.3 and using CORE_PROFILE mode.

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


	GLFWwindow* window = glfwCreateWindow(SC_WIDTH, SC_HEIGHT, "3D_GraphicRender", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create" << std::endl;
		glfwTerminate();
		return -1;
	}
	//create window


	glfwMakeContextCurrent(window);
	//通知GLFW将我们窗口的上下文设置为当前线程的主上下文

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//call "framebuffer_size_callback" function when adjust the of window

	glfwSetCursorPosCallback(window, mouse_callback);
	//active mouse callback event

	glfwSetScrollCallback(window, scroll_callback);
	//active mouse scroll callback

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// tell GLFW to capture mouse cursor

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);// 开启深度测试
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK); // 剔除背面三角形


	Shader ourShader("./Shader/model_vs.shader", "./Shader/model_fs.shader");
	std::cout << "Shader compile success" << std::endl;

	//load models
	Model ourModel("./model/backpack.obj");

	
	//render line
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/*input*/
		processinput(window);    //key"esc" event monitor

		/*render command*/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //use custom color clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear buffer color

		ourShader.use();

		// create transformations
		
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		//view && projection transform
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SC_WIDTH / (float)SC_HEIGHT, 0.1f, 100.0f);// 声明一个投影矩阵
		view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);//pass to shader
		ourShader.setMat4("view", view);

		// pass them to the shaders
		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// scale
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);


		/*check and call events, swap buffers*/
		glfwSwapBuffers(window); //storage pixel color buffer 
		glfwPollEvents();        //check if poll events eg.mouse events

	}

	glfwTerminate();
	return 0;
}




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
//each frame callback to change window size

void processinput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	float cameraSpeed = static_cast<float>(2.5 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
}
//press esc close the window
//press WASD control camera

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
