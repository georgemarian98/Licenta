//
//  SkyBox.cpp
//  Lab10(CubeMaps)
//
//  Created by CGIS on 16/12/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//
#include "pch.h"

#include "SkyBox.h"

namespace SceneEditor{

	static std::array<GLfloat, 108> skyboxVertices = {
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
	};

	void SkyBox::LoadTextures(const std::array<std::string, 6>& CubeMapFaces)
	{
		if (m_LoadSkybox == true) {
			glDeleteTextures(1, &m_CubemapTexture.id);
		}

		m_CubeTexturesPaths = CubeMapFaces;
		m_CubemapTexture.id = LoadSkyBoxTextures(CubeMapFaces);
	}

	void SkyBox::ClearSkybox()
	{
		glDeleteTextures(1, &m_CubemapTexture.id);
		m_LoadSkybox = false;
	}

	void SkyBox::Draw( )
	{
		m_VertexBuffer.Bind( );

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture.id);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		m_VertexBuffer.Unbind( );
	}

	GLuint SkyBox::LoadSkyBoxTextures(const std::array<std::string, 6>& SkyBoxFaces)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

		int width, height, nrChannels;

		for(uint32_t i = 0; i < 6; i++){
			uint8_t* data = stbi_load(SkyBoxFaces[i].c_str(), &width, &height, &nrChannels, 0);
			if(data){
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			else{
				std::cerr << "Cubemap texture failed to load at path: " << SkyBoxFaces[i] << std::endl;
			}
			stbi_image_free(data);
		}

		glCheckError();
		m_LoadSkybox = true;

		return textureID;
	}


	SkyBox::SkyBox() : m_VertexBuffer{ skyboxVertices }, m_CubemapTexture{0}{	}

	SkyBox::~SkyBox( )
	{
		glDeleteTextures(1, &m_CubemapTexture.id);
	}
}
