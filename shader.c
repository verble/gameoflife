
#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

typedef GLuint ShaderProgram;

char *load_file(char *filename) {

    char *buffer;
    long length;
    FILE *f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = malloc((length + 1) * sizeof(char));
    fread(buffer, sizeof(char), length, f);
    fclose(f);
    buffer[length] = '\0';

    return buffer;
}

ShaderProgram shprog_new(char* vertexPath, char* fragmentPath) {

    GLuint vertex, fragment, program;
    GLint success;
    GLchar infoLog[512];

    // load source files

    char* vShaderCode = load_file(vertexPath);
    char* fShaderCode = load_file(fragmentPath);

    // compile vertex shader

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const GLchar *const *) &vShaderCode, NULL);
    glCompileShader(vertex);
    free(vShaderCode);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        puts(infoLog);
        puts("vertex failed!");
    }

    // compile fragment shader

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const GLchar *const *) &fShaderCode, NULL);
    glCompileShader(fragment);
    free(fShaderCode);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        printf("Message: %s\n", infoLog);
        puts("fragment failed!");
    }

    // link program

    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}


void shprog_use(ShaderProgram p) {
    glUseProgram(p);
}

