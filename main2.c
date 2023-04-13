#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int width = 800;
int height = 800;

void throw(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void loop() {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
}

char* readFile(const char* filaName)
{
    static int length = 8000;
    char *text = malloc(length);
    memset(text, 0, length);
    FILE *f;
    f = fopen(filaName, "rb");
    if (f) {
        fread(text, 1, length, f);
        fclose(f);
    }
    return text;
}

void createShaders()
{
//    char *text = readFile("/home/bublik/opengl/shader.frag");
//    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(shader, 1, &text, NULL);
//    glCompileShader(shader);
//    free(text);
//
//    GLuint oks;
//    GLchar log[2000];
//    glGetShaderiv(shader, GL_COMPILE_STATUS, &oks);
//    if (!oks) {
//        glGetShaderInfoLog(shader, 2000, NULL, log);
//        printf("%s\n", log);
//    }
}

int main() {

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        printf("Error: glfwInit error");
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow *window = glfwCreateWindow(width, height, "My Title", NULL, NULL);

    if (!window) {
        glfwTerminate();
        printf("Error: glfwInit error");
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    do {
        loop();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}