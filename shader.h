
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

typedef GLuint ShaderProgram;

char *load_file(char *filename);

ShaderProgram shprog_new(char* vertexPath, char* fragmentPath);

void shprog_use(ShaderProgram p);

#endif // SHADER_H

