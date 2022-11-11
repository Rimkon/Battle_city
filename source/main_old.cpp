//#include <glad/glad.h>
#include "../external/glad/include/glad/glad.h"
//#include "../external/glfw/include/GLFW/glfw3.h"
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std; 

GLfloat point[]=
{
    0.0f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f
};

GLfloat colors [] = 
{
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

const char * vertex_shader = 
"#version 410\n"
"layout (location = 0) in vec3 vertex_position;"
"layout (location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main(){"
"       color = vertex_color;"
"       gl_Position = vec4(vertex_position, 1.0);"
"};";


const char * fragment_shader = 
"#version 410\n"
"in vec3 color;"
"out vec4 fragment_color;"
"fragment_color =  vec4 (color, 1.0f);"
"};";


int g_windowSizeX = 640;
int g_windowSizeY = 480;

void glfwWindowsSizeCallback (GLFWwindow *pWindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;

    glViewport (0,0, g_windowSizeX, g_windowSizeY);
    cout<< g_windowSizeX <<' '<<g_windowSizeY<<endl;
}

void glfwKeyCallback (GLFWwindow * pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose (pWindow, GLFW_TRUE);
    }

}


int main(void)
{

    /* Initialize the library */
    if (!glfwInit())
    {
        cout<<"glfwInit failed!"<<endl;
        return -1;
    }

    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);// указываем необходимую версию OpenGL
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
     GLFWwindow *pWindow = glfwCreateWindow(g_windowSizeX,g_windowSizeY, "hello world", nullptr, nullptr);
    if (!pWindow)
    {
        cout<<"glfwCreateWindow failed"<<endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback (pWindow, glfwWindowsSizeCallback);
    glfwSetKeyCallback        (pWindow, glfwKeyCallback);


    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL ())
    {
        cout<<"eror glad"<<endl;
        return -1;
    }
    else
    {
        cout<<  "Render: "         <<glGetString(GL_RENDERER)<<endl;
        cout<<  "OpenGL version: " <<glGetString(GL_VERSION) <<endl;

        /* cout<< "version: " <<GLVersion.major << "." << GLVersion.minor<<endl; */
    }
    glClearColor (1,1,0,1);

    GLuint vs = glCreateShader  (GL_VERTEX_SHADER);                     // число обзначающие шейдер и получаем его из функции // какой хотим создать шейдер ветоксный
                glShaderSource  (vs, 1, &vertex_shader, nullptr);       // передать этому шейдеру исходный код
                glCompileShader (vs);                                   // для компиляции шейдера
    
    GLuint fs = glCreateShader  (GL_FRAGMENT_SHADER);
                glShaderSource  (fs, 1, &fragment_shader, nullptr);     // передать этому шейдеру исходный код
                glCompileShader (fs);                                   // для компиляции шейдера


    GLuint shader_program = glCreateProgram ();// для линковки                
    glAttachShader (shader_program, vs);
    glAttachShader (shader_program, fs);
    glLinkProgram  (shader_program);
    
    glDeleteShader (vs); 
    glDeleteShader (fs); 

    GLuint points_vbo = 0;
    glGenBuffers (1, &points_vbo);                                          // хотим сгенерировать только 1 буфер
    glBindBuffer (GL_ARRAY_BUFFER, points_vbo);                             // выбираем текущий буфер
    glBufferData (GL_ARRAY_BUFFER, sizeof (point), point, GL_STATIC_DRAW);  // заполняем буфер нашими позициями размер буфера в байтах, адрес на наш буфер, куда записать на карте эти данные


    GLuint color_vbo = 0;
    glGenBuffers (1, &color_vbo);                                          // хотим сгенерировать только 1 буфер
    glBindBuffer (GL_ARRAY_BUFFER, color_vbo);                             // выбираем текущий буфер
    glBufferData (GL_ARRAY_BUFFER, sizeof (colors), colors, GL_STATIC_DRAW);  // заполняем буфер нашими позициями размер буфера в байтах, адрес на наш буфер, куда записать на карте эти данные


    GLuint vao = 0;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);

    glEnableVertexAttribArray (0);
    glBindBuffer              (GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer     (0,3,GL_FLOAT,GL_FALSE, 0, nullptr);


    glEnableVertexAttribArray (1);
    glBindBuffer              (GL_ARRAY_BUFFER, color_vbo);
    glVertexAttribPointer     (1,3,GL_FLOAT,GL_FALSE, 0, nullptr);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram      (shader_program);
        glBindVertexArray (vao);
        glDrawArrays (GL_TRIANGLES, 0, 3);
        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
