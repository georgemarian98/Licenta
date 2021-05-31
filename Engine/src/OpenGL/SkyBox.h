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

			void Bind( ) {
				glBindVertexArray(m_VAO);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);
			};
			void Unbind( ) { glBindVertexArray(0); };

	private:
			void InitSkyBox( );
			GLuint LoadSkyBoxTextures(const std::array<const GLchar*, 6>& CubeMapFaces);

	private:
			GLuint m_VAO = 0;
			GLuint m_VBO = 0;
			GLuint m_CubemapTexture = 0;
	};
}
