#pragma once

namespace SceneEditor{

	enum class DrawMode	{
		Normal = GL_FILL,
		Line = GL_LINE,
		Point = GL_POINT
	};

	class Renderer{
	public:
		static void Initiliaze(glm::vec4 ClearColor = glm::vec4(0.3f));
		static void Clear( );

		static uint32_t GenerateID( ) { return m_Ids++; };
		static void Reset( ) { m_Ids = 0; };

		static void SetDrawMode(DrawMode Mode) { glPolygonMode(GL_FRONT_AND_BACK, (int)Mode); };

		/// <summary>
		/// TODO Batch Rendering
		/// </summary>
		static void Submit( );

	private:
		static uint32_t m_Ids;
	};
}

