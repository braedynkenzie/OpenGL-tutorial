#include "TestFPSCamera.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <vendor\stb_image\stb_image.h>
#include <tests\TestClearColour.h>

#include "Globals.h"

namespace test
{
	// Function declarations
	void mouse_callbackCameraTest(GLFWwindow* window, double xpos, double ypos);
	void scroll_callbackCameraTest(GLFWwindow* window, double xOffset, double yOffset);
	void processInputCameraTest(GLFWwindow* window);

	// Init static variable
	TestFPSCamera* TestFPSCamera::instance;

	TestFPSCamera::TestFPSCamera(GLFWwindow*& mainWindow)
		: m_MainWindow(mainWindow), 
		m_CameraPos(glm::vec3(0.0f, 0.0f, 3.0f)), 
		m_CameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), 
		m_CameraUp(glm::vec3(0.0f, 1.0f, 0.0f)), 
		m_Camera(Camera(m_CameraPos, 110.0f))
	{
		instance = this;

		// Mouse cursor should start in the middle of the window
		lastCursorX = SCREEN_WIDTH / 2.0f;
		lastCursorY = SCREEN_HEIGHT / 2.0f;

		// Callback function for mouse cursor movement
		glfwSetCursorPosCallback(m_MainWindow, mouse_callbackCameraTest); // todo callback
		// Callback function for scrolling zoom
		glfwSetScrollCallback(m_MainWindow, scroll_callbackCameraTest);

		// Create vertice positions
		float vertices[] = {
		 //       positions     --   tex coords 
			  -80.0, -10.0, -80.0,    0.0, 1.0, // Floor
			   80.0, -10.0,  80.0,    1.0, 0.0,
			  -80.0, -10.0,  80.0,    0.0, 0.0,
			   80.0, -10.0, -80.0,    1.0, 1.0,
		};

		unsigned int indices[]{
			1, 0, 2,
			3, 0, 1,
		};

		m_VA = std::make_unique<VertexArray>();

		// Init Vertex Buffer and bind to Vertex Array (m_VA)
		m_VB = std::make_unique<VertexBuffer>(vertices, 5 * 4 * sizeof(float));

		// Create and associate the layout (Vertex Attribute Pointer)
		VertexBufferLayout layout;
		layout.Push<float>(3); // Vertex position vec3
		layout.Push<float>(2); // Texture coordinates vec2
		m_VA->AddBuffer(*m_VB, layout);

		// Init index buffer and bind to Vertex Array (m_VA)
		m_IB = std::make_unique<IndexBuffer>(indices, 6);

		// Load shader
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");

		// Flip texture along y axis before loading
		stbi_set_flip_vertically_on_load(true);

		// Bind shader program and set uniforms
		m_Shader->Bind();
		m_Texture = std::make_unique<Texture>("res/textures/high_res_world_map_texture.png");
		m_Texture->Bind(0); // make sure this texture slot is the same as the one set in the next line, which tells the shader where to find the Sampler2D data
		m_Shader->SetUniform1i("u_Texture0", 0);

		// Unbind everything
		m_VA->Unbind();
		m_VB->Unbind();
		m_IB->Unbind();
		m_Shader->Unbind();
	}

	TestFPSCamera::~TestFPSCamera()
	{
	}

	void TestFPSCamera::OnUpdate(float deltaTime)
	{
		// Hide and capture mouse cursor
		glfwSetInputMode(m_MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void TestFPSCamera::OnRender()
	{
		// Calculate deltaTime
		float currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		// Process WASD keyboard camera movement
		processInputCameraTest(m_MainWindow);

		float* clearColour = test::TestClearColour::GetClearColour();
		float darknessFactor = 2.0f;
		GLCall(glClearColor(clearColour[0] / darknessFactor, clearColour[1] / darknessFactor,
			clearColour[2] / darknessFactor, clearColour[3] / darknessFactor));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Renderer renderer;

		// Bind shader and set its per frame uniforms
		m_Shader->Bind();
		//
		// Create model, view, projection matrices 
		// Send combined MVP matrix to shader
		glm::mat4 modelMatrix = glm::mat4(1.0);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(50.0, 0.0, 36.0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0));
		glm::mat4 proj = glm::perspective(glm::radians(m_Camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 200.0f);
		glm::mat4 view = m_Camera.GetViewMatrix();
		glm::mat4 MVP_matrix = proj * view * modelMatrix;
		m_Shader->SetMatrix4f("u_MVP", MVP_matrix);
		renderer.DrawTriangles(*m_VA, *m_IB, *m_Shader);
	}

	void TestFPSCamera::OnImGuiRender()
	{
		// ImGui interface
		ImGui::Text("PRESS 'BACKSPACE' TO EXIT");
		ImGui::Text("- Use WASD keys to move camera");
		ImGui::Text("- Use scroll wheel to change FOV");
		ImGui::Text("- Avg %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	void TestFPSCamera::OnActivated()
	{
		// Bind shader program and reset any uniforms
		m_Shader->Bind();
		m_Texture = std::make_unique<Texture>("res/textures/high_res_world_map_texture.png");
		m_Texture->Bind(0); // make sure this texture slot is the same as the one set in the next line, which tells the shader where to find the Sampler2D data
		m_Shader->SetUniform1i("u_Texture0", 0);

		// Enable OpenGL z-buffer depth comparisons
		glEnable(GL_DEPTH_TEST);
		// Render only those fragments with lower depth values
		glDepthFunc(GL_LESS);
		// Enable blending
		GLCall(glEnable(GL_BLEND));
		//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glBlendFunc(GL_ONE, GL_ZERO));

		// Reset all callbacks
		// Callback function for keyboard inputs
		processInputCameraTest(m_MainWindow);
		// Callback function for mouse cursor movement
		glfwSetCursorPosCallback(m_MainWindow, mouse_callbackCameraTest);
		// Callback function for scrolling zoom
		glfwSetScrollCallback(m_MainWindow, scroll_callbackCameraTest);
	}

	void scroll_callbackCameraTest(GLFWwindow* window, double xOffset, double yOffset)
	{
		test::TestFPSCamera* cameraTest = test::TestFPSCamera::GetInstance();
		Camera* activeCamera = cameraTest->GetCamera();
		activeCamera->ProcessMouseScroll(yOffset);
	}

	void mouse_callbackCameraTest(GLFWwindow* window, double xpos, double ypos)
	{
		// Fixes first mouse cursor capture by OpenGL window
		if (firstMouseCapture)
		{
			lastCursorX = xpos;
			lastCursorY = ypos;
			firstMouseCapture = false;
		}
		float xOffset = xpos - lastCursorX;
		float yOffset = lastCursorY - ypos; // reverse the y-coordinates
		float cursorSensitivity = 0.08f;
		xOffset *= cursorSensitivity;
		yOffset *= cursorSensitivity;
		yaw += xOffset;
		pitch += yOffset;
		lastCursorX = xpos;
		lastCursorY = ypos;

		test::TestFPSCamera* cameraTest = test::TestFPSCamera::GetInstance();
		Camera* activeCamera = cameraTest->GetCamera();
		activeCamera->ProcessMouseMovement(xOffset, yOffset);
	}

	void processInputCameraTest(GLFWwindow* window) {
		test::TestFPSCamera* test = test::TestFPSCamera::GetInstance();
		Camera* camera = test->GetCamera();

		// Camera position movement
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera->ProcessKeyboardForMapView(FORWARD, deltaTime, -0.0f, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera->ProcessKeyboardForMapView(BACKWARD, deltaTime, -0.0f, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera->ProcessKeyboardForMapView(LEFT, deltaTime, -0.0f, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera->ProcessKeyboardForMapView(RIGHT, deltaTime, -0.0f, 0.0f);
	}
}
