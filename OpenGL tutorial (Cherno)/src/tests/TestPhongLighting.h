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


namespace test
{
	struct PointLight {
		glm::vec3 Colour;
		glm::vec3 Position;
		glm::vec3 Direction;
		float Speed;
	};

	class TestPhongLighting : public Test
	{
	private:
		static TestPhongLighting* instance;
		GLFWwindow* m_MainWindow;
		std::vector<PointLight> m_PointLights;
		std::unique_ptr<VertexArray> m_VA_Ground;
		std::unique_ptr<VertexBuffer> m_VB_Ground;
		std::unique_ptr<IndexBuffer> m_IB_Ground;
		VertexArray* m_VA_PointLight;
		VertexBuffer* m_VB_PointLight;
		IndexBuffer* m_IB_PointLight;
		Shader* m_GroundShader;
		Shader* m_PointLightsShader;
		Texture* m_BrickGroundTexture;
		Texture* m_BrickGroundNormalMap;
		Texture* m_WoodenGroundTexture;
		glm::vec3 m_CameraPos;
		glm::vec3 m_CameraFront;
		glm::vec3 m_CameraUp;
		Camera m_Camera;
		// Flashlight properties
		bool m_IsFlashlightOn;
		glm::vec3 m_FlashlightColour;
		glm::vec3 m_FlashlightDiffuseIntensity;
		glm::vec3 m_FlashlightAmbientIntensity;
		glm::vec3 m_FlashlightSpecularIntensity;
		glm::vec3 m_FlashlightDiffuseColour;
		glm::vec3 m_FlashlightAmbientColour;
		// Point light properties
		glm::vec3 m_FloatingLightColour;
		glm::vec3 m_FloatingLightPos;
		glm::vec3 m_FloatingLightDiffuseIntensity;
		glm::vec3 m_FloatingLightAmbientIntensity;
		glm::vec3 m_FloatingLightSpecularIntensity;
		glm::vec3 m_FloatingLightDiffuseColour;
		glm::vec3 m_FloatingLightAmbientColour;
		// Lighting model modes
		bool m_BlinnPhongEnabled;
		bool m_WoodenGroundEnabled;
		// Skybox data
		Shader* m_SkyboxShader;
		Texture* m_SkyboxTexture;
		VertexArray* m_VA_Skybox;
		VertexBuffer* m_VB_Skybox;
		IndexBuffer* m_IB_Skybox;

	public:

		TestPhongLighting(GLFWwindow*& mainWindow);
		~TestPhongLighting();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnActivated() override;

		void NewProjectile();
		void ToggleBlinnPhong(bool flag);
		void ToggleGroundTexture(bool flag);

		Camera* GetCamera() { return &m_Camera; }
		static TestPhongLighting* GetInstance() { return instance; }
	};
}
