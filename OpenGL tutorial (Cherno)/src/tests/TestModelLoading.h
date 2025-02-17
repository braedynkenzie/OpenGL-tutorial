#pragma once

#include "Test.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Camera.h"

#include <memory>
#include <Model.h>


namespace test
{
	class TestModelLoading : public Test
	{
	private:
		static TestModelLoading* instance;
		GLFWwindow* m_MainWindow;
		bool modelLoaded;
		Model* m_BackpackModel;
		std::unique_ptr<VertexArray> m_VA;
		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr<IndexBuffer> m_IB;
		Shader* m_Shader;
		std::unique_ptr<Texture> m_GroundTexture;
		glm::vec3 m_CameraPos;
		glm::vec3 m_CameraFront;
		glm::vec3 m_CameraUp;
		Camera m_Camera;
		bool m_IsFlashlightOn;
		glm::vec3 m_FlashlightColour;
		glm::vec3 m_fl_diffuseIntensity;
		glm::vec3 m_fl_ambientIntensity;
		glm::vec3 m_fl_specularIntensity;
		glm::vec3 m_fl_diffuseColour;
		glm::vec3 m_fl_ambientColour;

	public:

		TestModelLoading(GLFWwindow*& mainWindow);
		~TestModelLoading();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnActivated() override;

		Camera* GetCamera() { return &m_Camera; }
		static TestModelLoading* GetInstance() { return instance; }
	};
}
