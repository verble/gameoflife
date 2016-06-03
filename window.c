

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "shader.h"
#include "board.h"
#include "board_graphics.h"
#include "millitime.h"


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
    BoardGraphics g = board_graphics_create(b);

    // timer
    int previous;
    int now;
    int delta;
    int ticker = 0;

    previous = millitime();

    // graphics loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        now = millitime();
        delta = now - previous;
        ticker += delta;
        if (ticker >= 500) {
            board_evolve(&b);
            board_graphics_update(&g, b);
            ticker = 0;
        }
        previous = now;

        board_graphics_draw(g);
        glfwSwapBuffers(window);
    }

    board_graphics_destroy(g);
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
