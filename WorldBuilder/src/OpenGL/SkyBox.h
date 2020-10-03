//
//  SkyBox.hpp
//  Lab10(CubeMaps)
//
//  Created by CGIS on 16/12/2016.
//  Copyright © 2016 CGIS. All rights reserved.
//

#ifndef SkyBox_hpp
#define SkyBox_hpp

#include "OpenGL/Shader.h"

class SkyBox{
	public:
		SkyBox(const char* vertexPath, const char* fragmentPath);
		~SkyBox( );

		void Load(const std::vector<const GLchar*>& cubeMapFaces, const glm::mat4& projection);
		void Draw(const glm::mat4& viewMatrix);

		void SetProjection(const glm::mat4& projection) { m_Projection = projection; };

	private:
		void InitSkyBox( );
		GLuint LoadSkyBoxTextures(const std::vector<const GLchar*>& cubeMapFaces);

	private:
		GLuint m_VAO = 0;
		GLuint m_VBO = 0;
		GLuint m_CubemapTexture = 0;

		glm::mat4 m_Projection;
		Shader m_Shader;
	};
#endif /* SkyBox_hpp */
