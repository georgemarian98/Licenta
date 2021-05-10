#pragma once

namespace SceneEditor{

	class Renderer{
	public:
		static void Initiliaze(glm::vec4 ClearColor = glm::vec4(0.3f));
		static void Clear( );

		static uint32_t GenerateID( ) { return m_Ids++; };
		static void Reset( ) { m_Ids = 0; };

		/// <summary>
		/// TODO Batch Rendering
		/// </summary>
		static void Submit( );

	private:
		static uint32_t m_Ids;
	};
}

