//
//  SkyBox.hpp
//  Lab10(CubeMaps)
//
//  Created by CGIS on 16/12/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//

#pragma once

#include "OpenGL/Shader.h"
#include <array>

namespace SceneEditor{

	class SkyBox{
	public:
			SkyBox();
			~SkyBox( );

			void Draw();
			void LoadTextures(const std::array<std::string, 6>& CubeMapFaces);

			bool IsLoaded() { return m_LoadSkybox; };
			void ClearSkybox();

			std::array<std::string, 6> GetTexturePaths() { return m_CubeTexturesPaths; };
	private:
			GLuint LoadSkyBoxTextures(const std::array<std::string, 6>& CubeMapFaces);

	private:
			bool m_LoadSkybox = false;

			Texture m_CubemapTexture;
			VertexArray m_VertexBuffer;

			std::array<std::string, 6> m_CubeTexturesPaths;
	};
}
