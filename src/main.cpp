//Encabezados y shaders--------------------------------------------------------------------------------------
#include <glad/gl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

// Se ejecuta una vez por vertice.
const char* vertexShaderSource = R"glsl(
#version 450 core

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)glsl";

// Se ejecuta una vez por fragmento.
const char* fragmentShaderSource = R"glsl(
#version 450 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)glsl";

//Compilacion y enlazado de shaders-----------------------------------------------------------------------------------

void errorGLFW(int codigo, const char* mensaje)
{
    std::cerr << "Error GLFW " << codigo
              << ": " << mensaje << '\n';
}

GLuint compilarShader(GLenum tipo, const char* fuente)
{
    GLuint shader = glCreateShader(tipo);

    glShaderSource(shader, 1, &fuente, nullptr);
    glCompileShader(shader);

    GLint correcto = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &correcto);

    if (correcto != GL_TRUE)
    {
        GLint longitud = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &longitud);

        std::cerr << "Error al compilar el shader:\n";

        if (longitud > 0)
        {
            std::string log(
                static_cast<std::size_t>(longitud),
                '\0'
            );

            glGetShaderInfoLog(
                shader,
                longitud,
                nullptr,
                log.data()
            );

            std::cerr << log << '\n';
        }

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint crearPrograma()
{
    GLuint vertexShader = compilarShader(
        GL_VERTEX_SHADER,
        vertexShaderSource
    );

    if (vertexShader == 0)
    {
        return 0;
    }

    GLuint fragmentShader = compilarShader(
        GL_FRAGMENT_SHADER,
        fragmentShaderSource
    );

    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        return 0;
    }

    GLuint programa = glCreateProgram();

    glAttachShader(programa, vertexShader);
    glAttachShader(programa, fragmentShader);

    glLinkProgram(programa);

    GLint correcto = GL_FALSE;
    glGetProgramiv(programa, GL_LINK_STATUS, &correcto);

    // El programa enlazado conserva lo que necesita.
    glDetachShader(programa, vertexShader);
    glDetachShader(programa, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (correcto != GL_TRUE)
    {
        GLint longitud = 0;
        glGetProgramiv(programa, GL_INFO_LOG_LENGTH, &longitud);

        std::cerr << "Error al enlazar el programa:\n";

        if (longitud > 0)
        {
            std::string log(
                static_cast<std::size_t>(longitud),
                '\0'
            );

            glGetProgramInfoLog(
                programa,
                longitud,
                nullptr,
                log.data()
            );

            std::cerr << log << '\n';
        }

        glDeleteProgram(programa);
        return 0;
    }

    return programa;
}

//Ventana y bucle principal-----------------------------------------------------------------------------------
int main()
{
    // 1. Iniciar GLFW.
    glfwSetErrorCallback(errorGLFW);

    if (!glfwInit())
    {
        return 1;
    }

    // Misma version que las filminas.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* ventana = glfwCreateWindow(
        800,
        600,
        "Practico 01 - Triangulo",
        nullptr,
        nullptr
    );

    if (ventana == nullptr)
    {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(ventana);

    // 2. Cargar las funciones OpenGL mediante GLAD.
    if (!gladLoadGL(glfwGetProcAddress) || !GLAD_GL_VERSION_4_5)
    {
        std::cerr << "No se pudo cargar OpenGL 4.5.\n";

        glfwDestroyWindow(ventana);
        glfwTerminate();

        return 1;
    }

    glfwSwapInterval(1);

    std::cout << "OpenGL: " << glGetString(GL_VERSION) << '\n';

    // 3. Decidir los datos: tres posiciones, con X, Y y Z.
    const float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // 4. Crear el programa de shaders.
    GLuint programa = crearPrograma();

    if (programa == 0)
    {
        glfwDestroyWindow(ventana);
        glfwTerminate();

        return 1;
    }

    // 5. Crear los recursos para almacenar e interpretar los vertices.
    GLuint vao = 0;
    GLuint vbo = 0;

    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);

    // Copiar los bytes del arreglo a un buffer de la GPU.
    glNamedBufferData(
        vbo,
        static_cast<GLsizeiptr>(sizeof(vertices)),
        vertices,
        GL_STATIC_DRAW
    );

    // Conectar el VBO al punto de enlace 0 del VAO.
    glVertexArrayVertexBuffer(
        vao,
        0,
        vbo,
        0,
        static_cast<GLsizei>(3 * sizeof(float))
    );

    // El atributo 0 contiene tres numeros float: X, Y y Z.
    glVertexArrayAttribFormat(
        vao,
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0
    );

    // El atributo 0 toma sus datos del punto de enlace 0.
    glVertexArrayAttribBinding(vao, 0, 0);

    // Habilitar el atributo.
    glEnableVertexArrayAttrib(vao, 0);

    glClearColor(0.10f, 0.12f, 0.16f, 1.0f);

    // 6. Bucle de dibujo.
    while (!glfwWindowShouldClose(ventana))
    {
        glfwPollEvents();

        if (glfwGetKey(ventana, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(ventana, GLFW_TRUE);
        }

        int ancho = 0;
        int alto = 0;

        glfwGetFramebufferSize(ventana, &ancho, &alto);
        glViewport(0, 0, ancho, alto);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programa);
        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(ventana);
    }

    // 7. Liberar recursos mientras el contexto sigue vivo.
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(programa);

    glfwDestroyWindow(ventana);
    glfwTerminate();

    return 0;
}