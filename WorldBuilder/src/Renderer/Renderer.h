#pragma once

namespace SceneEditor{

	class Renderer{
	public:
		static void Initiliaze(glm::vec4 ClearColor = glm::vec4(0.3f));
		static void Clear( );

		/// <summary>
		/// TODO Batch Rendering
		/// </summary>
		static void Submit( );
	};
}

