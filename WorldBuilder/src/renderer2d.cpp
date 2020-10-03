#include "3DWBpch.h"
#include "renderer2d.h"


Renderer2D::Renderer2D(Shader &shader, WindowData *windowData, int maxBatchQuadCount)
    : m_Shader(shader), m_WindowData(windowData), m_MaxBatchVertexCount(maxBatchQuadCount * 4)
{
}

Renderer2D::~Renderer2D()
{
}

bool Renderer2D::Create()
{
    m_VertexData = new Renderer2DVertexData[m_MaxBatchVertexCount];
    m_VertexDataPointer = m_VertexData;

    glGenVertexArrays(1, &m_Vao);
    glBindVertexArray(m_Vao);

    glGenBuffers(1, &m_Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Renderer2DVertexData) * m_MaxBatchVertexCount, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Renderer2DVertexData), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Renderer2DVertexData), (void *)sizeof(glm::vec3));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    int *indices_data = new int[m_MaxBatchVertexCount / 4 * 6];

    int offset = 0;
    for (int i = 0; i < m_MaxBatchVertexCount; i += 4)
    {
        indices_data[offset] = i + 0;
        indices_data[offset + 1] = i + 1;
        indices_data[offset + 2] = i + 2;
        indices_data[offset + 3] = i + 2;
        indices_data[offset + 4] = i + 3;
        indices_data[offset + 5] = i + 0;

        offset += 6;
    }

    glGenBuffers(1, &m_Ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_MaxBatchVertexCount / 4 * 6, indices_data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    delete[] indices_data;

    return true;
}

void Renderer2D::Destroy()
{
    delete[] m_VertexData;

    glDeleteBuffers(1, &m_Vbo);
    glDeleteBuffers(1, &m_Ibo);
    glDeleteVertexArrays(1, &m_Vao);
}

void Renderer2D::Start()
{
    m_ProjectionMatrix = glm::ortho(0.0f, (float)m_WindowData->width, (float)m_WindowData->height, 0.0f, 0.0f, -1000.0f);

    m_Shader.Bind();
    glCheckError( );
    m_Shader.UploadUniformMat4("u_Projection", m_ProjectionMatrix);
    glCheckError( );
    m_Shader.Unbind();

    m_VertexCount = 0;
    m_VertexDataPointer = m_VertexData;

    m_VertexCount = 0;

    m_TotalVertexCount = 0;
    m_TotalBatchCount = 0;
}

void Renderer2D::End()
{
    DrawBatch();
}

void Renderer2D::DrawQuad(glm::vec2 position, glm::vec2 scale, glm::vec4 color)
{
    if (m_VertexCount >= m_MaxBatchVertexCount)
    {
        DrawBatch();
        m_VertexCount = 0;
        m_VertexDataPointer = m_VertexData;
    }

    m_VertexDataPointer->position = {position.x, position.y, 0.0};
    m_VertexDataPointer->color = color;
    m_VertexDataPointer++;
    m_VertexDataPointer->position = {position.x + scale.x, position.y, 0.0};
    m_VertexDataPointer->color = color;
    m_VertexDataPointer++;
    m_VertexDataPointer->position = {position.x + scale.x, position.y + scale.y, 0.0};
    m_VertexDataPointer->color = color;
    m_VertexDataPointer++;
    m_VertexDataPointer->position = {position.x, position.y + scale.y, 0.0};
    m_VertexDataPointer->color = color;
    m_VertexDataPointer++;

    m_VertexCount += 4;
}

void Renderer2D::DrawImGui()
{
//#ifdef DEBUG
    ImGui::Begin("Renderer 2D");
    ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
    ImGui::Text("Batch count: %d", m_TotalBatchCount);
    ImGui::Text("Quad count: %d", m_TotalVertexCount / 4);
    ImGui::End();
//#endif
}

void Renderer2D::DrawBatch()
{
    if (m_VertexCount == 0)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_VertexCount * sizeof(Renderer2DVertexData), m_VertexData);

    m_Shader.Bind();
    glBindVertexArray(m_Vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);

    glDrawElements(GL_TRIANGLES, m_VertexCount / 4 * 6, GL_UNSIGNED_INT, (void *)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
    m_Shader.Unbind();

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_TotalBatchCount++;
    m_TotalVertexCount += m_VertexCount;
}