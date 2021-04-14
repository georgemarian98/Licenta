#pragma once
class Renderer{

public:
	static void Initiliaze(glm::vec4 ClearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	static void Clear( );

	/// <summary>
	/// TODO Batch Rendering
	/// </summary>
	static void Submit( );
};

