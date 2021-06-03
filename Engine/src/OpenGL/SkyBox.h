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

			void Draw();
			void Load(const std::array<std::string, 6>& CubeMapFaces);

			bool IsLoaded() { return m_LoadSkybox; };
	private:
			GLuint LoadSkyBoxTextures(const std::array<std::string, 6>& CubeMapFaces);

	private:
			bool m_LoadSkybox = false;

			VertexArray m_VertexBuffer;
			GLuint m_CubemapTexture = 0;

			std::array<std::string, 6> m_CubeTexturesPaths;
	};
}
