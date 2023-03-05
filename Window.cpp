#include <iostream>
#include "GLEW/glew.h" //must come before other gl libraries
#include "GLFW/glfw3.h"
#include <fstream>
#include <string>
#include <sstream>
#include "Square.h"
#include <cmath>

void error_callback(int, const char*);

static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = &source[0];
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cerr<<"Failed to compile shader\n";
        std::cerr<<message<<std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

static ShaderProgramSource ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType {NONE = -1, VERTEX = 0, FRAGMENT = 1};

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while(getline(stream, line))
    {
        if(line.find("#shader") != std::string::npos)
        {
            if(line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if(line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

int main() 
{
    //GLFW and other misc. setup
    if(!glfwInit())
    {
        std::cerr<<"Error intitializing glfw"<<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetErrorCallback(error_callback);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if(!window)
    {
        std::cerr<<"Error creating window"<<std::endl;
    }
    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK)
    {
        std::cerr<<"issue with glew"<<std::endl;
    }

    Square testSquare(Vector2(0, 0));
    Vector2* vertices = testSquare.getVertices();

    float positions[8] = {
        vertices[0].x, vertices[0].y,
        vertices[1].x, vertices[1].y,
        vertices[2].x, vertices[2].y,
        vertices[3].x, vertices[3].y,
        };

    unsigned int index[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_DYNAMIC_DRAW);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), index, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    ShaderProgramSource source = ParseShader("res/shaders/basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    unsigned int colorLoc = glGetUniformLocation(shader, "u_Color");
    glUniform4f(colorLoc, 1.0, 0.0, 0.0, 1.0);
    //glfwSetCursorPosCallback(window, cursorPosCallback);

    int count = 0;
    //Window Loop
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        testSquare.SetPos(Vector2((xpos/320)-1, ypos/-240+1));
        vertices = testSquare.getVertices();
        for(int i=0; i<4; i++)
        {
            positions[2*i] = vertices[i].x;
            positions[2*i+1] = vertices[i].y;
        }
        //glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(float), positions);
        glUniform4f(colorLoc, sin(++count/M_PI*180.0)+1.0, sin(count/M_PI*147)+1.0, sin(count/M_PI*123)+1.0, 1.0);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader);
}

void error_callback(int error, const char* description)
{
    std::cerr<<description<<std::endl;
}

static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) 
{
    return;
}