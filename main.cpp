#include <iostream>
#include <vector>
//bunları değiştir
#include "glm/glm.hpp"
#include "glm/common.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "opengl_utilities.h"
#include "mesh_generation.h"


#define GLFW_KEY_Q 81
#define GLFW_KEY_W 87
#define GLFW_KEY_E 69
#define GLFW_KEY_R 82
#define GLFW_KEY_T 84
#define GLFW_KEY_Y 89



/* Keep the global state inside this struct */
static struct {
    glm::dvec2 mouse_position;
    glm::ivec2 screen_dimensions = glm::ivec2(960, 960);
    int key_pressed;
} Globals;

/* GLFW Callback functions */
static void ErrorCallback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}

static void CursorPositionCallback(GLFWwindow* window, double x, double y)
{
    Globals.mouse_position.x = x;
    Globals.mouse_position.y = y;
}

static void WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    Globals.screen_dimensions.x = width;
    Globals.screen_dimensions.y = height;

    glViewport(0, 0, width, height);
}

static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Globals.key_pressed = key;
}


int main(int argc, char* argv[])
{
    /* Set GLFW error callback */
    glfwSetErrorCallback(ErrorCallback);

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(
        Globals.screen_dimensions.x, Globals.screen_dimensions.y,
        "Deniz Cangı", NULL, NULL
    );
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    /* Move window to a certain position [do not change] */
    glfwSetWindowPos(window, 10, 50);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    /* Enable VSync */
    glfwSwapInterval(1);

    /* Load OpenGL extensions with GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Set GLFW Callbacks */
    glfwSetCursorPosCallback(window, CursorPositionCallback);
    glfwSetWindowSizeCallback(window, WindowSizeCallback);
    glfwSetKeyCallback(window, KeyCallBack);

    /* Configure OpenGL */
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);

    /* Creating Meshes */
    VAO triangleVAO(
        {
            {-0.5, -0.5, 0},
            {+0.5, -0.5, 0},
            {+0.0, +0.5, 0},
        },
        {},
        {
            0, 1, 2
        }
        );
    
    /* Creating OpenGL objects */
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<GLuint> indices;

    GenerateParametricShapeFrom2D(positions, normals, indices, ParametricHalfCircle, 16, 16);
    VAO sphereVAO(positions, normals, indices);
    
    positions.clear();
    normals.clear();
    indices.clear();
    GenerateParametricShapeFrom2D(positions, normals, indices, ParametricCircle, 16, 16);
    VAO torusVAO(positions, normals, indices);
    
    positions.clear();
    normals.clear();
    indices.clear();
    GenerateParametricShapeFrom2D(positions, normals, indices, ParametricSpikes, 64, 64);
    VAO spikesVAO(positions, normals, indices);
    
    positions.clear();
    normals.clear();
    indices.clear();
    GenerateParametricShape(positions, normals, indices, ParametricSpikes, 48, 48);
    VAO flowerVAO(positions, normals, indices);
    
    positions.clear();
    normals.clear();
    indices.clear();
    GenerateParametricShape_2(positions, normals, indices, ParametricCircle_2, 48, 48);
    VAO newshapeVAO(positions, normals, indices);
    
    positions.clear();
    normals.clear();
    indices.clear();
    GenerateParametricShapeFrom2D(positions, normals, indices, ParametricHalfCircle_2, 16, 16);
    VAO denemeVAO(positions, normals, indices);
    
    positions.clear();
    normals.clear();
    indices.clear();
    GenerateParametricShapeFrom2D(positions, normals, indices, ParametricHalfCircle_3, 16, 16);
    VAO deneme2VAO(positions, normals, indices);
    
    positions.clear();
    normals.clear();
    indices.clear();
    GenerateParametricShapeFrom2D(positions, normals, indices, ParametricHalfCircle_4, 16, 16);
    VAO deneme3VAO(positions, normals, indices);
    
    positions.clear();
    normals.clear();
    indices.clear();
    GenerateParametricShapeFrom2D(positions, normals, indices, ParametricHalfCircle_5, 16, 16);
    VAO deneme4VAO(positions, normals, indices);


    /* Creating Programs */
    GLuint program_1 = CreateProgramFromSources(
        R"VERTEX(
        #version 330 core

        layout(location = 0) in vec3 a_position;
        layout(location = 1) in vec3 a_normal;

        uniform mat4 u_transform;

        out vec3 vertex_position;
        out vec3 vertex_normal;

        void main()
        {
            gl_Position = u_transform * vec4(a_position, 1);
            vertex_normal = (u_transform * vec4(a_normal, 0)).xyz;
            vertex_position = gl_Position.xyz;
        }
                )VERTEX",

                R"FRAGMENT(
        #version 330 core

        in vec3 vertex_position;
        in vec3 vertex_normal;

        out vec4 out_color;

        void main()
        {
            vec3 color = vec3(0);
                                                
            vec3 surface_color = vec3(1);
            vec3 surface_poition = vertex_position;
            vec3 surface_normal = normalize(vertex_normal);
                                                
            vec3 ambient_color = vec3(1);
            color += ambient_color * surface_color;
                                                
            out_color = vec4(color, 1);
        }
                )FRAGMENT");
    if (program_1 == NULL)
    {
        glfwTerminate();
        return -1;
    }
    
    GLuint program_2 = CreateProgramFromSources(
        R"VERTEX(
        #version 330 core

        layout(location = 0) in vec3 a_position;
        layout(location = 1) in vec3 a_normal;

        uniform mat4 u_transform;

        out vec3 vertex_position;
        out vec3 vertex_normal;

        void main()
        {
            gl_Position = u_transform * vec4(a_position, 1);
            vertex_normal = (u_transform * vec4(a_normal, 0)).xyz;
            vertex_position = gl_Position.xyz;
        }
                )VERTEX",

                R"FRAGMENT(
        #version 330 core

        in vec3 vertex_position;
        in vec3 vertex_normal;

        out vec4 out_color;

        void main()
        {
            vec3 color = vec3(0);
                                                
            vec3 surface_poition = vertex_position;
            vec3 surface_normal = normalize(vertex_normal);

            color += surface_normal;
                                                
            out_color = vec4(color, 1);
        }
                )FRAGMENT");
    if (program_2 == NULL)
    {
        glfwTerminate();
        return -1;
    }
    
    GLuint program_3 = CreateProgramFromSources(
        R"VERTEX(
        #version 330 core

        layout(location = 0) in vec3 a_position;
        layout(location = 1) in vec3 a_normal;

        uniform mat4 u_transform;

        out vec3 vertex_position;
        out vec3 vertex_normal;

        void main()
        {
            gl_Position = u_transform * vec4(a_position, 1);
            vertex_normal = (u_transform * vec4(a_normal, 0)).xyz;
            vertex_position = gl_Position.xyz;
        }
                )VERTEX",

                R"FRAGMENT(
        #version 330 core

        in vec3 vertex_position;
        in vec3 vertex_normal;

        out vec4 out_color;

        void main()
        {
            vec3 color = vec3(0);
            
            vec3 surface_color = vec3(0.5,0.5,0.5);
            vec3 surface_position = vertex_position;
            vec3 surface_normal = normalize(vertex_normal);

            vec3 ambient_color = vec3(0.5,0.5,0.5);
                                            
            vec3 light_direction = normalize(vec3(1,1,-1));
            vec3 light_color = vec3(0.4,0.4,0.4);
                                            
            float diffuse_intensity = max(0,dot(light_direction, surface_normal));

            vec3 view_dir = normalize(vec3(vertex_position.xy,-1)-surface_position);
            vec3 halfway_dir = normalize(view_dir + light_direction);
            float shininess =  64;
            float specular_intensity = max(0, dot(halfway_dir, surface_normal));
                                                
            color += ambient_color * surface_color + diffuse_intensity * light_color * surface_color + pow(specular_intensity, shininess) * light_color;
                                    
                                                
            out_color = vec4(color, 1);
        }
                )FRAGMENT");
    if (program_3 == NULL)
    {
        glfwTerminate();
        return -1;
    }
    
    GLuint program_4 = CreateProgramFromSources(
        R"VERTEX(
        #version 330 core

        layout(location = 0) in vec3 a_position;
        layout(location = 1) in vec3 a_normal;

        uniform mat4 u_transform;

        out vec3 vertex_position;
        out vec3 vertex_normal;

        void main()
        {
            gl_Position = u_transform * vec4(a_position, 1);
            vertex_normal = (u_transform * vec4(a_normal, 0)).xyz;
            vertex_position = gl_Position.xyz;
        }
                )VERTEX",

                R"FRAGMENT(
        #version 330 core
        
       uniform vec2 u_mouse_position;
       uniform vec3 u_color;
       uniform float u_shininess;

        in vec3 vertex_position;
        in vec3 vertex_normal;

        out vec4 out_color;

        void main()
        {
            vec3 color = vec3(0);
            
            vec3 surface_color = u_color;
            vec3 surface_position = vertex_position;
            vec3 surface_normal = normalize(vertex_normal);

            vec3 ambient_color = vec3(0.5,0.5,0.5);
            color += ambient_color * surface_color;
                                            
            vec3 light_direction = normalize(vec3(1,1,-1));
            vec3 light_color = vec3(0.4,0.4,0.4);
            float diffuse_intensity = max(0,dot(light_direction, surface_normal));
            color += diffuse_intensity * light_color * surface_color;

            vec3 view_dir = normalize(vec3(vertex_position.xy,-1)-surface_position);
            vec3 halfway_dir = normalize(view_dir + light_direction);
            float shininess =  u_shininess;
            float specular_intensity = max(0, dot(halfway_dir, surface_normal));
            color += pow(specular_intensity, shininess) * light_color;
                                                
            vec3 point_light_position = vec3(u_mouse_position,-1);
            vec3 point_light_color = vec3(0.5,0.5,0.5);
            vec3 to_point_light = normalize(point_light_position - surface_position);
            diffuse_intensity = max(0,dot(to_point_light, surface_normal));
            color += diffuse_intensity * point_light_color * surface_color;
                                                
            view_dir = normalize(vec3(vertex_position.xy,-1)-surface_position);
            halfway_dir = normalize(view_dir + to_point_light);
            shininess =  u_shininess;
            specular_intensity = max(0, dot(halfway_dir, surface_normal));
            color += pow(specular_intensity, shininess) * point_light_color;
                                                
                                    
                                                
            out_color = vec4(color, 1);
        }
                )FRAGMENT");
    
    if (program_4 == NULL)
    {
        glfwTerminate();
        return -1;
    }
    
    GLuint program_5 = CreateProgramFromSources(
     R"VERTEX(
     #version 330 core

     layout(location = 0) in vec3 a_position;
     layout(location = 1) in vec3 a_normal;

     uniform mat4 u_transform;

     out vec3 vertex_position;
     out vec3 vertex_normal;

     void main()
     {
         gl_Position = u_transform * vec4(a_position, 1);
         vertex_normal = (u_transform * vec4(a_normal, 0)).xyz;
         vertex_position = gl_Position.xyz;
     }
             )VERTEX",

             R"FRAGMENT(
     #version 330 core
                                                
     uniform vec3 u_color;
                                                
     in vec3 vertex_position;
     in vec3 vertex_normal;

     out vec4 out_color;

     void main()
     {
         vec3 color = vec3(0);
         
         vec3 surface_color = u_color;
         vec3 surface_position = vertex_position;
         vec3 surface_normal = normalize(vertex_normal);

         vec3 ambient_color = vec3(0.5,0.5,0.5);
                                         
         vec3 light_direction = normalize(vec3(1,1,-1));
         vec3 light_color = vec3(0.4,0.4,0.4);
                                         
         float diffuse_intensity = max(0,dot(light_direction, surface_normal));

         vec3 view_dir = normalize(vec3(vertex_position.xy,-1)-surface_position);
         vec3 halfway_dir = normalize(view_dir + light_direction);
         float shininess =  64;
         float specular_intensity = max(0, dot(halfway_dir, surface_normal));
                                             
         color += ambient_color * surface_color + diffuse_intensity * light_color * surface_color + pow(specular_intensity, shininess) * light_color;
                                      
         out_color = vec4(color, 1);
     }
             )FRAGMENT");
    
    if (program_5 == NULL)
    {
        glfwTerminate();
        return -1;
    }
    
    GLuint program_6 = CreateProgramFromSources(
     R"VERTEX(
     #version 330 core

     layout(location = 0) in vec3 a_position;
     layout(location = 1) in vec3 a_normal;

     uniform mat4 u_transform;

     out vec3 vertex_position;
     out vec3 vertex_normal;

     void main()
     {
         gl_Position = u_transform * vec4(a_position, 1);
         vertex_normal = (u_transform * vec4(a_normal, 0)).xyz;
         vertex_position = gl_Position.xyz;
     }
             )VERTEX",

             R"FRAGMENT(
     #version 330 core
                                                
     uniform vec3 u_color;
                                                
     in vec3 vertex_position;
     in vec3 vertex_normal;

     out vec4 out_color;

     void main()
     {
         vec3 color = vec3(0);
         
         vec3 surface_color = u_color;
         vec3 surface_position = vertex_position;
         vec3 surface_normal = normalize(vertex_normal);

         vec3 ambient_color = vec3(0.5,0.5,0.5);
         color += ambient_color * surface_color;
                                         
         vec3 light_direction = normalize(vec3(1,1,-1));
         vec3 light_color = vec3(0.4,0.4,0.4);
         float diffuse_intensity = max(0,dot(light_direction, surface_normal));
         color += diffuse_intensity * light_color * surface_color;

         vec3 view_dir = normalize(vec3(vertex_position.xy,-1)-surface_position);
         vec3 halfway_dir = normalize(view_dir + light_direction);
         float shininess =  16;
         float specular_intensity = max(0, dot(halfway_dir, surface_normal));
         color += pow(specular_intensity, shininess) * light_color;

                           
         out_color = vec4(color, 1);
     }
             )FRAGMENT");
    
    if (program_6 == NULL)
    {
        glfwTerminate();
        return -1;
    }
    
    auto u_transform_location_scene1 = glGetUniformLocation(program_1, "u_transform");
    auto u_transform_location_scene2 = glGetUniformLocation(program_2, "u_transform");
    auto u_transform_location_scene3 = glGetUniformLocation(program_3, "u_transform");
    auto u_transform_location_scene4 = glGetUniformLocation(program_4, "u_transform");
    auto u_transform_location_scene5 = glGetUniformLocation(program_5, "u_transform");
    auto u_transform_location_scene6 = glGetUniformLocation(program_6, "u_transform");
    auto mouse_position_location = glGetUniformLocation(program_4, "u_mouse_position");
    auto mouse_position_location_scene5 = glGetUniformLocation(program_5, "u_mouse_position");
    auto color_scene4 = glGetUniformLocation(program_4, "u_color");
    auto color_scene5 = glGetUniformLocation(program_5, "u_color");
    auto color_scene6 = glGetUniformLocation(program_6, "u_color");
    auto shininess_scene4 = glGetUniformLocation(program_4, "u_shininess");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        auto key_pressed = Globals.key_pressed;
        
        
        if(key_pressed == GLFW_KEY_Q || key_pressed == 0){
        
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            glUseProgram(program_1);
            
            glBindVertexArray(sphereVAO.id);
            
            auto scale = glm::scale(glm::vec3(0.4));
            auto rotate= glm::rotate(glm::radians(float(glfwGetTime() * 10)), glm::vec3(1, 1, 0));
            
            auto translate_sphere = glm::translate(glm::vec3(-0.5,0.5,0));
            auto transform_sphere = translate_sphere*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene1, 1, GL_FALSE, glm::value_ptr(transform_sphere));
            glDrawElements(GL_TRIANGLES, sphereVAO.element_array_count, GL_UNSIGNED_INT, 0);
            
            
            glBindVertexArray(torusVAO.id);
            
            auto translate_torus = glm::translate(glm::vec3(0.5,0.5,0));
            auto transform_torus = translate_torus*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene1, 1, GL_FALSE, glm::value_ptr(transform_torus));
            glDrawElements(GL_TRIANGLES, torusVAO.element_array_count, GL_UNSIGNED_INT, 0);
            
            
            glBindVertexArray(spikesVAO.id);
            
            auto translate_spikes = glm::translate(glm::vec3(-0.5,-0.5,0));
            auto transform_spikes = translate_spikes*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene1, 1, GL_FALSE, glm::value_ptr(transform_spikes));
            glDrawElements(GL_TRIANGLES, spikesVAO.element_array_count, GL_UNSIGNED_INT, 0);
            
            
            glBindVertexArray(flowerVAO.id);
            
            auto translate_flower = glm::translate(glm::vec3(0.5,-0.5,0));
            auto transform_flower = translate_flower*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene1, 1, GL_FALSE, glm::value_ptr(transform_flower));
            glDrawElements(GL_TRIANGLES, flowerVAO.element_array_count, GL_UNSIGNED_INT, 0);
        }
        
        else if(key_pressed == GLFW_KEY_W)
        {
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glUseProgram(program_2);
            
            glBindVertexArray(sphereVAO.id);
            
            auto scale = glm::scale(glm::vec3(0.4));
            auto rotate= glm::rotate(glm::radians(float(glfwGetTime() * 10)), glm::vec3(1, 1, 0));
            
            
            auto translate_sphere = glm::translate(glm::vec3(-0.5,0.5,0));
            auto transform_sphere = translate_sphere*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene2, 1, GL_FALSE, glm::value_ptr(transform_sphere));
            glDrawElements(GL_TRIANGLES, sphereVAO.element_array_count, GL_UNSIGNED_INT, 0);
            
            
            glBindVertexArray(torusVAO.id);
            
            auto translate_torus = glm::translate(glm::vec3(0.5,0.5,0));
            auto transform_torus = translate_torus*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene2, 1, GL_FALSE, glm::value_ptr(transform_torus));
            glDrawElements(GL_TRIANGLES, torusVAO.element_array_count, GL_UNSIGNED_INT, 0);
            
            
            glBindVertexArray(spikesVAO.id);
            
            auto translate_spikes = glm::translate(glm::vec3(-0.5,-0.5,0));
            auto transform_spikes = translate_spikes*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene2, 1, GL_FALSE, glm::value_ptr(transform_spikes));
            glDrawElements(GL_TRIANGLES, spikesVAO.element_array_count, GL_UNSIGNED_INT, 0);
            
            
            glBindVertexArray(flowerVAO.id);
            
            auto translate_flower = glm::translate(glm::vec3(0.5,-0.5,0));
            auto transform_flower = translate_flower*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene2, 1, GL_FALSE, glm::value_ptr(transform_flower));
            glDrawElements(GL_TRIANGLES, flowerVAO.element_array_count, GL_UNSIGNED_INT, 0);
            
        }
        
        else if(key_pressed == GLFW_KEY_E)
        {
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glUseProgram(program_3);
            
            glBindVertexArray(sphereVAO.id);
            
            auto scale = glm::scale(glm::vec3(0.4));
            auto rotate= glm::rotate(glm::radians(float(glfwGetTime() * 10)), glm::vec3(1, 1, 0));
            
            
            auto translate_sphere = glm::translate(glm::vec3(-0.5,0.5,0));
            auto transform_sphere = translate_sphere*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene3, 1, GL_FALSE, glm::value_ptr(transform_sphere));
            glDrawElements(GL_TRIANGLES, sphereVAO.element_array_count, GL_UNSIGNED_INT, 0);
            
            
            glBindVertexArray(torusVAO.id);
            
            auto translate_torus = glm::translate(glm::vec3(0.5,0.5,0));
            auto transform_torus = translate_torus*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene3, 1, GL_FALSE, glm::value_ptr(transform_torus));
            glDrawElements(GL_TRIANGLES, torusVAO.element_array_count, GL_UNSIGNED_INT, 0);
            
            
            glBindVertexArray(spikesVAO.id);
            
            auto translate_spikes = glm::translate(glm::vec3(-0.5,-0.5,0));
            auto transform_spikes = translate_spikes*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene3, 1, GL_FALSE, glm::value_ptr(transform_spikes));
            glDrawElements(GL_TRIANGLES, spikesVAO.element_array_count, GL_UNSIGNED_INT, 0);
            
            
            glBindVertexArray(flowerVAO.id);
            
            auto translate_flower = glm::translate(glm::vec3(0.5,-0.5,0));
            auto transform_flower = translate_flower*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene3, 1, GL_FALSE, glm::value_ptr(transform_flower));
            glDrawElements(GL_TRIANGLES, flowerVAO.element_array_count, GL_UNSIGNED_INT, 0);
            
        }
        
        else if(key_pressed == GLFW_KEY_R){
            
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glUseProgram(program_4);
            
            auto mouse_position = Globals.mouse_position / glm::dvec2(Globals.screen_dimensions);
            mouse_position.y = 1. - mouse_position.y;
            mouse_position = mouse_position * 2. - 1.;

            glUniform2fv(mouse_position_location, 1, glm::value_ptr(glm::vec2(mouse_position)));
            
            glBindVertexArray(sphereVAO.id);
            
            auto scale = glm::scale(glm::vec3(0.4));
            auto rotate= glm::rotate(glm::radians(float(glfwGetTime() * 10)), glm::vec3(1, 1, 0));
            
            
            auto translate_sphere = glm::translate(glm::vec3(-0.5,0.5,0));
            auto transform_sphere = translate_sphere*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene4, 1, GL_FALSE, glm::value_ptr(transform_sphere));
            glUniform3fv(color_scene4,1,glm::value_ptr(glm::vec3(0.5)));
            glUniform1f(shininess_scene4, 128);
            glDrawElements(GL_TRIANGLES, sphereVAO.element_array_count, GL_UNSIGNED_INT, 0);
            
            
            glBindVertexArray(torusVAO.id);
            
            auto translate_torus = glm::translate(glm::vec3(0.5,0.5,0));
            auto transform_torus = translate_torus*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene4, 1, GL_FALSE, glm::value_ptr(transform_torus));
            glUniform3fv(color_scene4,1,glm::value_ptr(glm::vec3(1,0,0)));
            glUniform1f(shininess_scene4, 32);
            glDrawElements(GL_TRIANGLES, torusVAO.element_array_count, GL_UNSIGNED_INT, 0);
            
            
            glBindVertexArray(spikesVAO.id);
            
            auto translate_spikes = glm::translate(glm::vec3(-0.5,-0.5,0));
            auto transform_spikes = translate_spikes*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene4, 1, GL_FALSE, glm::value_ptr(transform_spikes));
            glUniform3fv(color_scene4,1,glm::value_ptr(glm::vec3(0,1,0)));
            glUniform1f(shininess_scene4, 32);
            glDrawElements(GL_TRIANGLES, spikesVAO.element_array_count, GL_UNSIGNED_INT, 0);
            
            
            glBindVertexArray(flowerVAO.id);
            
            auto translate_flower = glm::translate(glm::vec3(0.5,-0.5,0));
            auto transform_flower = translate_flower*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene4, 1, GL_FALSE, glm::value_ptr(transform_flower));
            glUniform3fv(color_scene4,1,glm::value_ptr(glm::vec3(0,0,1)));
            glUniform1f(shininess_scene4, 32);
            glDrawElements(GL_TRIANGLES, flowerVAO.element_array_count, GL_UNSIGNED_INT, 0);
        }
        
        else if(key_pressed == GLFW_KEY_T){
            
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glUseProgram(program_5);
            
            auto mouse_position = Globals.mouse_position / glm::dvec2(Globals.screen_dimensions);
            mouse_position.y = 1. - mouse_position.y;
            mouse_position = mouse_position * 2. - 1.;
            
            glUniform2fv(mouse_position_location_scene5, 1, glm::value_ptr(glm::vec2(mouse_position)));
            
            glBindVertexArray(sphereVAO.id);
            
            auto scale = glm::scale(glm::vec3(0.3));
            auto translate_sphere = glm::translate(glm::vec3(mouse_position, 0));
            auto transform_sphere = translate_sphere*scale;

            glUniformMatrix4fv(u_transform_location_scene5, 1, GL_FALSE, glm::value_ptr(transform_sphere));
            
            
            glm::dvec2 chasing_pos;
            
            chasing_pos = glm::mix(mouse_position, chasing_pos, 0.99);
            double dist = distance(mouse_position, chasing_pos);
            
            if (dist > 0.6){
                glUniform3fv(color_scene5,1,glm::value_ptr(glm::vec3(0,1,0)));
                glDrawElements(GL_TRIANGLES, sphereVAO.element_array_count, GL_UNSIGNED_INT, 0);
            }
            else{
                glUniform3fv(color_scene5,1,glm::value_ptr(glm::vec3(1,0,0)));
                glDrawElements(GL_TRIANGLES, sphereVAO.element_array_count, GL_UNSIGNED_INT, 0);
            }

            auto translate_sphere_2 = glm::translate(glm::vec3(chasing_pos,0));
            auto transform_sphere_2 = translate_sphere_2*scale;

            glUniformMatrix4fv(u_transform_location_scene5, 1, GL_FALSE, glm::value_ptr(transform_sphere_2));
            glUniform3fv(color_scene5,1,glm::value_ptr(glm::vec3(0.5)));
            glDrawElements(GL_TRIANGLES, sphereVAO.element_array_count, GL_UNSIGNED_INT, 0);
        }
        
        else if(key_pressed == GLFW_KEY_Y){
            
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            glUseProgram(program_6);
            
            glBindVertexArray(sphereVAO.id);
            
            auto scale = glm::scale(glm::vec3(0.15));
            auto rotate= glm::rotate(glm::radians(float(glfwGetTime() * 30)), glm::vec3(1, 1, 0));
            
            auto translate_sphere = glm::translate(glm::vec3(0,0,0));
            auto transform_sphere = translate_sphere*scale*rotate;

            glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(transform_sphere));
            glm::vec3 color1 = glm::vec3(72.0/255,61.0/255,139.0/255);
            glUniform3fv(color_scene6, 1, glm::value_ptr(color1));
            glDrawElements(GL_TRIANGLES, sphereVAO.element_array_count, GL_UNSIGNED_INT, 0);
            
            auto rotate2= glm::rotate(glm::radians(float(glfwGetTime() * 30)), glm::vec3(1, 1, 0));
            
            auto translate_sphere2 = glm::translate(glm::vec3(0,0,0));
            auto transform_sphere2 = translate_sphere2*scale*rotate2;

            glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(transform_sphere2));
            glm::vec3 color2 = glm::vec3(65.0/255,105.0/255,225.0/255);
            glUniform3fv(color_scene6, 1, glm::value_ptr(color2));
            glDrawElements(GL_TRIANGLES, sphereVAO.element_array_count, GL_UNSIGNED_INT, 0);
            
            
            glBindVertexArray(denemeVAO.id);
            
            for (int i = 0; i<2; i++){
                int a;
                if (i%2 == 0){
                    a=-1;
                }
                else {
                    a=1;
                }
            
                auto translate1 = glm::translate(glm::vec3(a*0.5, 0, 0));
                auto rotation1 = glm::rotate(glm::radians(float(glfwGetTime()) * 60), glm::vec3(0, 1, 0));
                auto planet1_transform = rotation1 * translate1 * scale;
                glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(planet1_transform));
                glm::vec3 color1 = glm::vec3(123.0/255,104.0/255,238.0/255);
                glUniform3fv(color_scene6, 1, glm::value_ptr(color1));
                glDrawElements(GL_TRIANGLES, denemeVAO.element_array_count, GL_UNSIGNED_INT, NULL);
            
            }
            
            for (int i = 0; i<2; i++){
                int d;
                if (i%2 == 0){
                    d=-1;
                }
                else {
                    d=1;
                }
            
                auto translate1 = glm::translate(glm::vec3(d*0.5, 0, 0));
                auto rotation1 = glm::rotate(glm::radians(float(glfwGetTime()) * 60), glm::vec3(0, 1, 0));
                auto planet1_transform = rotation1 * translate1 * scale;
                glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(planet1_transform));
                glm::vec3 color1 = glm::vec3(173.0/255,216.0/255,230.0/255);
                glUniform3fv(color_scene6, 1, glm::value_ptr(color1));
                glDrawElements(GL_TRIANGLES, denemeVAO.element_array_count, GL_UNSIGNED_INT, NULL);
            
            }

            /*
            auto translate1 = glm::translate(glm::vec3(-0.5, 0, 0));
            auto rotation1 = glm::rotate(glm::radians(float(glfwGetTime()) * 30), glm::vec3(0, 1, 0));
            auto planet1_transform = rotation1 * translate1 * scale;
            glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(planet1_transform));
            glm::vec3 color1 = glm::vec3(30.0/255,144.0/255,255.0/255);
            glUniform3fv(color_scene6, 1, glm::value_ptr(color1));
            glDrawElements(GL_TRIANGLES, sphereVAO.element_array_count, GL_UNSIGNED_INT, NULL);*/
            
            
            
            glBindVertexArray(deneme4VAO.id);
            auto new_scale = glm::scale(glm::vec3(0.1));
            
            for(int i = 0; i<2;i++){
                
                int b;
                if (i%2 == 0){
                    b=-1;
                }
                else {
                    b=1;
                }
                
                auto translate3 = glm::translate(glm::vec3(0, b*0.5, 0));
                auto rotation3 = glm::rotate(glm::radians(float(glfwGetTime()) * 60), glm::vec3(1, 0, 0));
                auto planet3_transform = rotation3 * translate3 * new_scale;
                glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(planet3_transform));
                glm::vec3 color3 = glm::vec3(123.0/255,104.0/255,238.0/255);
                glUniform3fv(color_scene6, 1, glm::value_ptr(color3));
                glDrawElements(GL_TRIANGLES, deneme4VAO.element_array_count, GL_UNSIGNED_INT, NULL);
                
                
            }
            
            for(int i = 0; i<2;i++){
                
                int c;
                if (i%2 == 0){
                    c=-1;
                }
                else {
                    c=1;
                }
                
                auto translate4 = glm::translate(glm::vec3(0, c*0.5, 0));
                auto rotation4 = glm::rotate(glm::radians(float(glfwGetTime()) * 60), glm::vec3(1, 0, 0));
                auto planet4_transform = rotation4 * translate4 * new_scale;
                glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(planet4_transform));
                glm::vec3 color4 = glm::vec3(173.0/255,216.0/255,230.0/255);
                glUniform3fv(color_scene6, 1, glm::value_ptr(color4));
                glDrawElements(GL_TRIANGLES, deneme4VAO.element_array_count, GL_UNSIGNED_INT, NULL);
                
                
            }

            /*
            auto translate3 = glm::translate(glm::vec3(0, 0.5, 0));
            auto rotation3 = glm::rotate(glm::radians(float(glfwGetTime()) * 50), glm::vec3(1, 0, 0));
            auto planet3_transform = rotation3 * translate3 * scale;
            glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(planet3_transform));
            glm::vec3 color3 = glm::vec3(173.0/255,216.0/255,230.0/255);
            glUniform3fv(color_scene6, 1, glm::value_ptr(color3));
            glDrawElements(GL_TRIANGLES, spikesVAO.element_array_count, GL_UNSIGNED_INT, NULL);
            
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glBindVertexArray(sphereVAO.id);*/
            
            glBindVertexArray(deneme2VAO.id);
            
            for(int i = 0; i<2;i++){
                
                int e;
                if (i%2 == 0){
                    e=-1;
                }
                else {
                    e=1;
                }
                
                auto translate4 = glm::translate(glm::vec3(-0.6*e, 0.6*e, 0));
                auto rotation4 = glm::rotate(glm::radians(float(glfwGetTime()) * 60), glm::vec3(1, 1, 0));
                auto planet4_transform = rotation4 * translate4 * scale;

                glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(planet4_transform));
                glm::vec3 color4 = glm::vec3(147.0/255,112.0/255,219.0/255);
                glUniform3fv(color_scene6, 1, glm::value_ptr(color4));
                glDrawElements(GL_TRIANGLES, deneme2VAO.element_array_count, GL_UNSIGNED_INT, NULL);
                
                
            }
            
            for(int i = 0; i<2;i++){
                
                int f;
                if (i%2 == 0){
                    f=-1;
                }
                else {
                    f=1;
                }
                
                auto translate4 = glm::translate(glm::vec3(-0.6*f, 0.6*f, 0));
                auto rotation4 = glm::rotate(glm::radians(float(glfwGetTime()) * 60), glm::vec3(1, 1, 0));
                auto planet4_transform = rotation4 * translate4 * scale;

                glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(planet4_transform));
                glm::vec3 color4 = glm::vec3(100.0/255,149.0/255,237.0/255);
                glUniform3fv(color_scene6, 1, glm::value_ptr(color4));
                glDrawElements(GL_TRIANGLES, deneme2VAO.element_array_count, GL_UNSIGNED_INT, NULL);
                
                
            }
            glBindVertexArray(deneme3VAO.id);
            for(int i = 0; i<2;i++){
                
                int e;
                if (i%2 == 0){
                    e=-1;
                }
                else {
                    e=1;
                }
                
                auto translate4 = glm::translate(glm::vec3(0.6*e, 0.6*e, 0));
                auto rotation4 = glm::rotate(glm::radians(float(glfwGetTime()) * 60), glm::vec3(1, -1, 0));
                auto planet4_transform = rotation4 * translate4 * scale;

                glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(planet4_transform));
                glm::vec3 color4 = glm::vec3(147.0/255,112.0/255,219.0/255);
                glUniform3fv(color_scene6, 1, glm::value_ptr(color4));
                glDrawElements(GL_TRIANGLES, deneme3VAO.element_array_count, GL_UNSIGNED_INT, NULL);
                
                
            }
            
            for(int i = 0; i<2;i++){
                
                int f;
                if (i%2 == 0){
                    f=-1;
                }
                else {
                    f=1;
                }
                
                auto translate4 = glm::translate(glm::vec3(0.6*f, 0.6*f, 0));
                auto rotation4 = glm::rotate(glm::radians(float(glfwGetTime()) * 60), glm::vec3(1, -1, 0));
                auto planet4_transform = rotation4 * translate4 * scale;

                glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(planet4_transform));
                glm::vec3 color4 = glm::vec3(100.0/255,149.0/255,237.0/255);
                glUniform3fv(color_scene6, 1, glm::value_ptr(color4));
                glDrawElements(GL_TRIANGLES, deneme3VAO.element_array_count, GL_UNSIGNED_INT, NULL);
                
                
            }

            
            /*
            
            auto translate2 = glm::translate(glm::vec3(-0.5, 0.5, 0));
            auto rotation2 = glm::rotate(glm::radians(float(glfwGetTime()) * 70), glm::vec3(1, 1, 0));
            auto planet2_transform = rotation2 * translate2 * scale;

            glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(planet2_transform));
            glm::vec3 color2 = glm::vec3(0,191.0/255,255.0/255);
            glUniform3fv(color_scene6, 1, glm::value_ptr(color2));
            glDrawElements(GL_TRIANGLES, sphereVAO.element_array_count, GL_UNSIGNED_INT, NULL);
            
            glBindVertexArray(torusVAO.id);
            
            auto translate4 = glm::translate(glm::vec3(0.25, 0.5, 0));
            auto rotation4 = glm::rotate(glm::radians(float(glfwGetTime()) * 100), glm::vec3(1, -0.5, 0));
            auto planet4_transform = rotation4 * translate4 * scale;

            glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(planet4_transform));
            glm::vec3 color4 = glm::vec3(70.0/255,130.0/255,180.0/255);
            glUniform3fv(color_scene6, 1, glm::value_ptr(color4));
            glDrawElements(GL_TRIANGLES, torusVAO.element_array_count, GL_UNSIGNED_INT, NULL);*/
            
            /*
            glBindVertexArray(deneme3VAO.id);
            
            
            for(int i = 0; i<2;i++){
                
                int g;
                if (i%2 == 0){
                    g=-1;
                }
                else {
                    g=1;
                }
                
                auto rotation5_first = glm::rotate(glm::radians(90.f),glm::vec3(1,0,0));
                auto translate5 = glm::translate(glm::vec3(0.25*g, 0, 0));
                auto rotation5 = glm::rotate(glm::radians(float(glfwGetTime()) * 60), glm::vec3(0, -1, 0));
                auto planet5_transform = rotation5 * translate5 * scale * rotation5_first;

                glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(planet5_transform));
                glm::vec3 color5 = glm::vec3(65.0/255,105.0/255,225.0/255);
                glUniform3fv(color_scene6, 1, glm::value_ptr(color5));
                glDrawElements(GL_TRIANGLES, deneme3VAO.element_array_count, GL_UNSIGNED_INT, NULL);
                
                
            }

            for(int i = 0; i<2;i++){
                
                int f;
                if (i%2 == 0){
                    f=-1;
                }
                else {
                    f=1;
                }
                
                auto rotation5_first = glm::rotate(glm::radians(90.f),glm::vec3(1,0,0));
                auto translate5 = glm::translate(glm::vec3(0.25*f, 0, 0));
                auto rotation5 = glm::rotate(glm::radians(float(glfwGetTime()) * 60), glm::vec3(0, -1, 0));
                auto planet5_transform = rotation5 * translate5 * scale * rotation5_first;

                glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(planet5_transform));
                glm::vec3 color4 = glm::vec3(72.0/255,61.0/255,139.0/255);
                glUniform3fv(color_scene6, 1, glm::value_ptr(color4));
                glDrawElements(GL_TRIANGLES, deneme3VAO.element_array_count, GL_UNSIGNED_INT, NULL);
                
                
            }*/
            
            /*
            
            auto rotation5_first = glm::rotate(glm::radians(90.f),glm::vec3(1,0,0));
            auto translate5 = glm::translate(glm::vec3(0.25, 0, 0));
            auto rotation5 = glm::rotate(glm::radians(float(glfwGetTime()) * 60), glm::vec3(0, -1, 0));
            auto planet5_transform = rotation5 * translate5 * scale * rotation5_first;

            glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(planet5_transform));
            glm::vec3 color5 = glm::vec3(255.0/255,133.0/255,154.0/255);
            glUniform3fv(color_scene6, 1, glm::value_ptr(color5));
            glDrawElements(GL_TRIANGLES, denemeVAO.element_array_count, GL_UNSIGNED_INT, NULL);
            
            glBindVertexArray(flowerVAO.id);
            
            auto rotation6_first = glm::rotate(glm::radians(90.f),glm::vec3(1,0,0));
            auto translate6 = glm::translate(glm::vec3(0.25, 0, 0));
            auto rotation6 = glm::rotate(glm::radians(float(glfwGetTime()) * 50), glm::vec3(0, -1, 0));
            auto planet6_transform = rotation6 * translate6 * scale*rotation6_first;

            glUniformMatrix4fv(u_transform_location_scene6, 1, GL_FALSE, glm::value_ptr(planet6_transform));
            glm::vec3 color6 = glm::vec3(255.0/255,114.0/255,138.0/255);
            glUniform3fv(color_scene6, 1, glm::value_ptr(color6));
            glDrawElements(GL_TRIANGLES, flowerVAO.element_array_count, GL_UNSIGNED_INT, NULL);*/
            

            
            
            
        }
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

