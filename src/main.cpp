#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "Shader.hpp"

const int width  = 1280;
const int height = 720;

void resize_window(GLFWwindow* window, int width, int height);
void windowInput(GLFWwindow* window);

std::vector<float> gridVerts;
std::vector<unsigned int> gridInds;

glm::mat4 view;
glm::vec3 cameraPos   = glm::vec3(0.0f, 2.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
float yaw = -90.0f;

float dt;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Game", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, resize_window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glViewport(0, 0, width, height);
    
    Shader shader("shaders/main.vs", "shaders/main.fs");

    int count = 0;
    int j = 200;
    while(count < 400) {
    	int i = 200;
    	gridVerts.push_back((i * 1));
    	gridVerts.push_back(0);
    	gridVerts.push_back(-(j * 1));
    	gridVerts.push_back(-(i * 1));
    	gridVerts.push_back(0);
    	gridVerts.push_back(-(j * 1));
    	j--;
    	count++;
    }
    j = 200;
    count = 0;
    while(count < 400) {
    	int i = 200;
    	gridVerts.push_back(-(j * 1));
    	gridVerts.push_back(0);
    	gridVerts.push_back(( - i * 1));
    	gridVerts.push_back(-(j * 1));
    	gridVerts.push_back(0);
    	gridVerts.push_back((i * 1));
    	j--;
    	count++;
    }

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, gridVerts.size() * sizeof(float), gridVerts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f,0.0f, 0.0f));

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.01f, 80.0f);
    shader.Use();
    shader.SetMat4("projection", projection);

    cameraFront.y = -sin(glm::radians(45.0f));

    float lastFrame = 0;
    while(!glfwWindowShouldClose(window))
    {
        float newFrame = glfwGetTime();
        dt = lastFrame - newFrame;
        lastFrame = newFrame;
        windowInput(window);

        glClearColor(0.03f, 0.03f, 0.03f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();
        shader.SetMat4("model", model);
        shader.SetMat4("view", view);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, gridVerts.size() * 4);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shader.Delete();
}

void resize_window(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    glClearColor(0.03f, 0.03f, 0.03f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void windowInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float cameraSpeed = 5.0f;
    const float cameraRotateSpeed = 50.0f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront * dt;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront * dt;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * dt;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * dt;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos.y -= cameraSpeed * dt;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos.y += cameraSpeed * dt;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        yaw -= cameraRotateSpeed * dt;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        yaw += cameraRotateSpeed * dt;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw));
    direction.y = 0;
    direction.z = sin(glm::radians(yaw));
    cameraFront = glm::normalize(direction);
    std::cout << "X: " << cameraPos.x << "\n";
    std::cout << "Y: " << cameraPos.y << "\n";
    std::cout << "Z: " << cameraPos.z << "\n";
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
