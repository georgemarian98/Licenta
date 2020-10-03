#include "3DWBpch.h"

#include "engine.h"
#include "core/Window.h"
#include "OpenGL/Shader.h"
#include "renderer2d.h"

#pragma warning(disable:4098)



int main()
{
    Window window("OpenGL Boilerplate!!!", 960, 540);
    window.SetVsync(true);

    Shader shader("shaders/basic.vert", "shaders/basic.frag");

    WindowData data
    {
        "OpenGL Boilerplate!!!",
        960,
        540,
        false
    };
    Renderer2D renderer(shader, &data, 1000);
    renderer.Create();

    // Setup ImGui

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    bool show_demo_window = false;

    while (window.ShouldClose() == false)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        renderer.Start();
        for (int i = 0; i < 10000; i++) {
            float x = (float)rand()/(float)(RAND_MAX);
            float y = (float)rand()/(float)(RAND_MAX);
            renderer.DrawQuad(glm::vec2(x * window.GetWindowSize().x, y * window.GetWindowSize().y), glm::vec2(10.0f, 10.0f), glm::vec4(x, y * 0.5f, 0.2f, 1.0f));
        }

        renderer.End();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderer.DrawImGui();

        //ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.Update();
    }

    renderer.Destroy();
}