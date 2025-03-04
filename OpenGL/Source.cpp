#define STB_IMAGE_IMPLEMENTATION

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Shader/Shader.h"
#include "camera.h"
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
const unsigned int SC_WIDTH = 800;
const unsigned int SC_HEIGHT = 600;

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = SC_WIDTH / 2.0f;
float lastY = SC_HEIGHT / 2.0f;
bool firstMouse = true;

//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(0.0f, 0.5f, 2.0f);

float vertices[] =
{
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

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
};// 六个面，每个面两个三角形六个顶点

unsigned int indices[] =
{
	 0, 1, 3,
	 1, 2, 3
};

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


	GLFWwindow* window = glfwCreateWindow(SC_WIDTH, SC_HEIGHT, "Hello", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create"<<std::endl;
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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);// 开启深度测试

	Shader LightingShader("./Shader/color_vs.shader", "./Shader/color_fs.shader");
	Shader LightCubeShader("./Shader/lightcube_vs.shader", "./Shader/lightcube_fs.shader");
	std::cout << "Shader compile success" << std::endl;



	unsigned int VBO, cubeVAO;

    glGenVertexArrays(1, &cubeVAO);//vertex buffer array object, storage multiple VBO
	glGenBuffers(1, &VBO); //use glGenBuffers function generate a VBO with bufferID
	//glGenBuffers(1, &EBO); 

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind VBO to vertexbuffer"GL_ARRAY_BUFFER"
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
	//把用户定义的数据复制到当前绑定缓冲

	glBindVertexArray(cubeVAO);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//Bind EBO
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//将索引复制到缓冲里


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);//顶点属性，顶点属性大小vec3，数据类型，数据是否标准化，步长，参数类型
	glEnableVertexAttribArray(0);
	//Tell OpenGL how to analysis vertex data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));//normal attribute
	glEnableVertexAttribArray(1);

	unsigned int lightcubeVAO;
	glGenVertexArrays(1, &lightcubeVAO);
	glBindVertexArray(lightcubeVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	/*
	// Load and create texture
	unsigned int texture1, texture2;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // All GL_TEXTURE_2D operation have effect on texture
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// wrapping

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Filter


	//load image, create texture, generate mipmap
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);// tell stb_image.h to flip loaded texture's on the y-axis.


	unsigned char* data = stbi_load("./Texture/container.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "load texture1 success" << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture1" << std::endl;
	}
	
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	stbi_set_flip_vertically_on_load(true);// tell stb_image.h to flip loaded texture's on the y-axis.

	data = stbi_load("./Texture/awesomeface.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "load texture2 success" << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture2" << std::endl;
	}
	stbi_image_free(data);
	

	ourShader.use();
	ourShader.SetInt("texture1", 0);
	ourShader.SetInt("texture2", 1);

    */
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/*input*/
		processinput(window);    //key"esc" event monitor

		/*render command*/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //use custom color clear screen
		glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);         //clear buffer color  

		LightingShader.use();
		LightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		LightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		LightingShader.setVec3("lightPos", lightPos);
		LightingShader.setVec3("viewPos", camera.Position);

		/*
		//bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.use();
		*/

		// create transformations
		glm::mat4 model       = glm::mat4(1.0f);
		glm::mat4 view        = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		projection = glm::perspective(glm::radians(camera.Zoom), (float)SC_WIDTH / (float)SC_HEIGHT, 0.1f, 100.0f);// 声明一个投影矩阵
		LightingShader.setMat4("projection", projection);//pass to shader

		view = camera.GetViewMatrix();
		LightingShader.setMat4("view", view);
			
		// pass them to the shaders		
		LightingShader.setMat4("model", model);
		
		
		/*
		//update uniform color
		float TimeValue = glfwGetTime();
		float GreenValue = sin(TimeValue) / 2.0f + 0.5f;
		int VertexColorLocation = glGetUniformLocation(programshader, "OurColor");
		glUniform4f(VertexColorLocation, 0.0f, GreenValue, 0.0f, 1.0f);*/

		// get matrix's uniform location and set matrix
		

		glBindVertexArray(cubeVAO); //if we have multiple VAO, need to bind it everytime	
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		LightCubeShader.use();
		LightCubeShader.setMat4("projection", projection);
		LightCubeShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		LightCubeShader.setMat4("model", model);

		glBindVertexArray(lightcubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/*check and call events, swap buffers*/
		glfwSwapBuffers(window); //storage pixel color buffer 
		glfwPollEvents();        //check if poll events eg.mouse events
	
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightcubeVAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	
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