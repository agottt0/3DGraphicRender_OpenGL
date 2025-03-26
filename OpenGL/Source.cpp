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
unsigned int loadTexture(char const* path);

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

float vertices[] =
{
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};// 六个面，每个面两个三角形六个顶点

// positions all containers
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 pointlightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
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

	glEnable(GL_DEPTH_TEST);// 开启深度测试
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK); // 剔除背面三角形


	Shader LightingShader("./Shader/color_vs.shader", "./Shader/color_fs.shader");
	Shader LightCubeShader("./Shader/lightcube_vs.shader", "./Shader/lightcube_fs.shader");
	std::cout << "Shader compile success" << std::endl;



	unsigned int VBO, cubeVAO;

	glGenVertexArrays(1, &cubeVAO);//vertex buffer array object, storage multiple VBO
	glGenBuffers(1, &VBO); //use glGenBuffers function generate a VBO with bufferID

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind VBO to vertexbuffer"GL_ARRAY_BUFFER"
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//把用户定义的数据复制到当前绑定缓冲

	glBindVertexArray(cubeVAO);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);//顶点属性，顶点属性大小vec3，数据类型，数据是否标准化，步长，参数类型
	glEnableVertexAttribArray(0);//里面编号对应shader中的layout(0)
	//Tell OpenGL how to analysis vertex data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//normal attribute
	glEnableVertexAttribArray(1);//layout(1)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));//texture attribute
	glEnableVertexAttribArray(2);

	unsigned int lightcubeVAO;
	glGenVertexArrays(1, &lightcubeVAO);
	glBindVertexArray(lightcubeVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int diffuseMap = loadTexture("C:/Users/john/Desktop/OpenGL/3DGraphicRender_OpenGL/OpenGL/Texture/container2.png");
	unsigned int specularMap = loadTexture("C:/Users/john/Desktop/OpenGL/3DGraphicRender_OpenGL/OpenGL/Texture/container2_specular.png");
	
	//shader configuration
	LightingShader.use();
	LightingShader.SetInt("material.diffuse", 0);
	LightingShader.SetInt("material.specular", 1);
	

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

		LightingShader.use();
		//materials properties
		LightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		LightingShader.SetFloat("material.shininess", 64.0f);

		// directional light
		LightingShader.setVec3("dirlight.direction", -0.2f, -1.0f, -0.3f);
		LightingShader.setVec3("dirlight.ambient", 0.05f, 0.05f, 0.05f);
		LightingShader.setVec3("dirlight.diffuse", 0.4f, 0.4f, 0.4f);
		LightingShader.setVec3("dirlight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		LightingShader.setVec3("pointlights[0].position", pointlightPositions[0]);
		LightingShader.setVec3("pointlights[0].ambient", 0.05f, 0.05f, 0.05f);
		LightingShader.setVec3("pointlights[0].diffuse", 0.8f, 0.8f, 0.8f);
		LightingShader.setVec3("pointlights[0].specular", 1.0f, 1.0f, 1.0f);
		LightingShader.SetFloat("pointlights[0].constant", 1.0f);
		LightingShader.SetFloat("pointlights[0].linear", 0.09f);
		LightingShader.SetFloat("pointlights[0].quadratic", 0.032f);
		// point light 2
		LightingShader.setVec3("pointlights[1].position", pointlightPositions[1]);
		LightingShader.setVec3("pointlights[1].ambient", 0.05f, 0.05f, 0.05f);
		LightingShader.setVec3("pointlights[1].diffuse", 0.8f, 0.8f, 0.8f);
		LightingShader.setVec3("pointlights[1].specular", 1.0f, 1.0f, 1.0f);
		LightingShader.SetFloat("pointlights[1].constant", 1.0f);
		LightingShader.SetFloat("pointlights[1].linear", 0.09f);
		LightingShader.SetFloat("pointlights[1].quadratic", 0.032f);
	
		// spotLight
		LightingShader.setVec3("spotlight.position", camera.Position);
		LightingShader.setVec3("spotlight.direction", camera.Front);
		LightingShader.setVec3("spotlight.ambient", 0.0f, 0.0f, 0.0f);
		LightingShader.setVec3("spotlight.diffuse", 1.0f, 1.0f, 1.0f);
		LightingShader.setVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);
		LightingShader.SetFloat("spotlight.constant", 1.0f);
		LightingShader.SetFloat("spotlight.linear", 0.09f);
		LightingShader.SetFloat("spotlight.quadratic", 0.032f);
		LightingShader.SetFloat("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
		LightingShader.SetFloat("spotlight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// create transformations
		
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		//view && projection transform
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SC_WIDTH / (float)SC_HEIGHT, 0.1f, 100.0f);// 声明一个投影矩阵
		view = camera.GetViewMatrix();
		LightingShader.setMat4("projection", projection);//pass to shader
		LightingShader.setMat4("view", view);

		// pass them to the shaders
		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		LightingShader.setMat4("model", model);

		//bind texture(diffuseMap&&specularMap)
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		//render cube
		glBindVertexArray(cubeVAO); //if we have multiple VAO, need to bind it everytime	
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			LightingShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		//LightCubeShader.use();
		//LightCubeShader.setMat4("projection", projection);
		//LightCubeShader.setMat4("view", view);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		//LightCubeShader.setMat4("model", model);

		//glBindVertexArray(lightcubeVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

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

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		glDeleteTextures(1, &textureID); // 删除无效纹理
		textureID = 0; // 标记为无效ID
	}

	return textureID;

}