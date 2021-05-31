//
//  SkyBox.hpp
//  Lab10(CubeMaps)
//
//  Created by CGIS on 16/12/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//

#pragma once

#include "OpenGL/Shader.h"

namespace SceneEditor{

	class SkyBox{
	public:
			SkyBox();
			~SkyBox( );

			void Load(const std::array<const GLchar*, 6>& CubeMapFaces);
			void Draw();

	private:
			GLuint LoadSkyBoxTextures(const std::array<const GLchar*, 6>& CubeMapFaces);

	private:
			VertexArray m_VertexBuffer;
			GLuint m_CubemapTexture = 0;
	};
}
