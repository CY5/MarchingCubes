
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "src/Renderer.h"
#include "src/VertexBuffer.h"
#include "src/IndexBuffer.h"
#include "src/VertexArray.h"
#include "src/VertexBufferLayout.h"
#include "src/Shader.h"
#include "src/Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "src/Vendor/imgui/imgui.h"
#include "src/Vendor/imgui/imgui_impl_glfw.h"
#include "src/Vendor/imgui/imgui_impl_opengl3.h"
#include "src/helper/Render.h"

void reshape(GLFWwindow* window, int w, int h)
{
    glViewport(0.0, 0.0, (GLsizei) w, (GLsizei) h);
   // std::cout << diffX << " " << diffY << std::endl;
    std::cout << h << " " << w << std::endl;
}

static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
    GLFWwindow* window;
    /* Register error callback first */
    glfwSetErrorCallback(ErrorCallback);

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Marching Cube", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    int scrWidth, scrHeight;
    glfwGetWindowSize(window, &scrWidth, &scrHeight);

    float aspect = float(scrWidth) / float(scrHeight);

    glViewport(0,0, scrWidth,scrHeight);

    glfwSwapInterval(1);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

    }

    fprintf(stdout, "Status: Using OpenGl %s\n", glGetString(GL_VERSION));
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));


    std::vector<glm::vec3> position;
    std::vector<glm::vec3> normal;
    std::vector<glm::vec3> points;
    std::vector<glm::vec2> cube_texcoords;
    std::vector<unsigned int> indices;
    std::vector<float> values;

    // number of cubes along a side
	int size = 40;
	int axisMin = -20;
	int axisMax =  20;
	int axisRange = axisMax - axisMin;
    float isoLevel = 0.0;

	// Generate a list of 3D points and values at those points
	for (int k = 0; k < size; k++)
	for (int j = 0; j < size; j++)
	for (int i = 0; i < size; i++)
	{
		// actual values
		int x = axisMin + axisRange * i / (size - 1);
		int y = axisMin + axisRange * j / (size - 1);
		int z = axisMin + axisRange * k / (size - 1);
		points.push_back( glm::vec3(x,y,z));
		float value = x*x + y*y - z*z - 25;
	    values.push_back( value );
	}

    Render::MarchingCubes(points, values, position, normal, 
                          indices, cube_texcoords, size, isoLevel);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
    /*GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_CULL_FACE));
    GLCall(glCullFace(GL_BACK));
    GLCall(glFrontFace(GL_CCW));*/
    //GLCall(glCullFace(GL_BACK));
    VertexArray va;
    VertexBuffer vb_vertices(position.data(), static_cast<unsigned int>(
        ((position.size()) * sizeof(glm::vec3))));
    VertexBuffer vb_texcord(cube_texcoords.data(), static_cast<unsigned int>(
        (cube_texcoords.size()) * sizeof(glm::vec2)));
    VertexBuffer vb_normal(normal.data(), static_cast<unsigned int>(
        ((normal.size()) * sizeof(glm::vec3))));
    VertexBufferLayout layout;
    layout.Push<float>(0,3);
    layout.Push<float>(1,2);
    layout.Push<float>(2,3);
    auto elements = layout.GetElements();
    va.AddBuffer(vb_vertices, elements[0]);
    va.AddBuffer(vb_texcord, elements[1]);
    va.AddBuffer(vb_normal, elements[2]);

    /* Indexed Buffer */
   IndexBuffer ib(&indices[0], indices.size());
    /*******************/

    //glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect,
                                      0.001f,200.0f);
    //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100,0,0));
    glm::mat4 view = glm::lookAt(
            glm::vec3(1.0f, 1.0f, 50.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
    );

    Shader shader("res/shaders/Basic.shader");
    shader.Bind();

    Texture texture("res/textures/montecarlo_simulation.png");
    texture.Bind(0);
    shader.SetUniform1i("u_Texture", 0);
    //shader.SetUniformMat4f("trans",  trans);

    va.UnBind();
    vb_vertices.UnBind();
    vb_texcord.UnBind();
    vb_normal.UnBind();
    ib.UnBind();
    shader.UnBind();

    Renderer renderer;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    glm::vec3 translation(-0.8, -0.3, -0.2);
    glm::vec3 rotation(1.0,1.0,1.0);

    {
        glfwSetKeyCallback(window, keyboard);
        glfwSetWindowSizeCallback(window, reshape);
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            renderer.Clear();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();



            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);

            model = glm::rotate(model, glm::radians(rotation.z),
                                          glm::vec3(0.0,0.0,1.0f));

            model = glm::rotate(model, glm::radians(rotation.x),
                                          glm::vec3(1.0,0.0,0.0f));

            model = glm::rotate(model, glm::radians(rotation.y),
                                          glm::vec3(0.0,1.0,0.0f));

            glm::mat4 mvp = proj * view * model;

            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);
            shader.SetUniformMat4f("u_Model", model);
            shader.SetUniformMat4f("u_View", view);
            //shader.SetUniformMat4f("u_Proj", proj);

            renderer.Draw(va, ib, shader);


            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
            {
                ImGui::SliderFloat3("Translation", &translation.x, 100.0f, -100.0f);// Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::SliderFloat3("Rotation", &rotation.x, 0.0f, 360.0f);// Edit 1 float using a slider from 0.0f to 360.0f
                //ImGui::SliderFloat1("Zoom", &rotation.x, 0.0f, 360.0f);// Edit 1 float using a slider from 0.0f to 360.0f

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            }



            // Render dear imgui into screen
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // glDeleteProgram(source);
    glfwTerminate();
    return 0;
}
