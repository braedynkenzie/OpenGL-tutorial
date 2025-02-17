#pragma once

#include "Test.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"

#include <memory>

namespace test
{
	class TestTexture2D : public Test
	{
	private:
		GLFWwindow* m_MainWindow;
		std::unique_ptr<VertexArray> m_VA;
		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr<IndexBuffer> m_IB;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Textures[2];
		int m_ActiveTexture;
		glm::vec3 m_ModelTranslation;
		float m_ModelRotationX, m_ModelRotationY, m_ModelRotationZ, m_ModelScale;
		const int m_SCREEN_WIDTH, m_SCREEN_HEIGHT;
	public: 
		TestTexture2D(GLFWwindow*& mainWindow);
		~TestTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnActivated() override;
	};
}
