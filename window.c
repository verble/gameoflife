

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "shader.h"
#include "board.h"
#include "board_graphics.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint WIDTH = 500;
const GLuint HEIGHT = 500;

GLFWwindow* init_graphics() {

    // init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Game of Life", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, WIDTH, HEIGHT);

    return window;
}

void terminate_graphics() {

    glfwTerminate();
}

void run_window() {

    GLFWwindow* window = init_graphics();

    Board b = board_generate(5, 5, 1, "0000000000011100000000000");
    GLfloat* positions = generate_pos_buffer(b);
    GLfloat* colors = generate_col_buffer(b);

    // shader
    ShaderProgram shprog = shprog_new("vertex_shader.glsl", "fragment_shader.glsl");

    // vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);

    // vertex buffers
    GLuint position_buffer;
    glGenBuffers(1, &position_buffer);

    GLuint color_buffer;
    glGenBuffers(1, &color_buffer);

    // copy data
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glBufferData(GL_ARRAY_BUFFER, (25 * 2 * 3 * 2 * sizeof(GLfloat)), positions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, (25 * 2 * 3 * 3 * sizeof(GLfloat)), colors, GL_STATIC_DRAW);

    // register attributes
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // graphics loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shprog_use(shprog);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, (25 * 3 * 2));
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &position_buffer);
    glDeleteBuffers(1, &color_buffer);

    free(positions);
    free(colors);
    board_destroy(b);

    terminate_graphics();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main() {
    run_window();
    return 0;
}
