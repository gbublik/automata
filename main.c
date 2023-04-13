#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <math.h>

GLFWwindow* window;
int width = 2000;
int height = 1000;

double zoom = 1;
double minZoom = 1;
GLsizei zoomWidth, zoomHeight; //Zoomed resolution

bool isMoveMode = false; //Is shift mode on
bool isPause = true;
double vx = 0; //Shift x
double vy = 0; //Shift y
double mx, my; //Mouse coordinate;
int Ww, Wh; // Размер окна

/**
 * Shaders program
 */
GLuint prog; // Шейдер фрейм калькуляции буфера
GLuint prog2; // Шейдер масштабирования и смещения
GLuint prog1;

/**
 * Буферы
 */
GLenum status;
GLuint colorBufferID;
GLuint colorBufferID2;
GLuint colorBufferID1;
GLuint currentBufferID;

GLuint framebuffer;
GLuint framebuffer2;
GLuint framebuffer1;

struct image {
    unsigned char r, g, b, a;
};


void throw(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
    exit(EXIT_FAILURE);
}

void scroll_callback(GLFWwindow *window2, double xoffset, double yoffset) {
    if (yoffset > 0) {
        zoom += 0.1f;
    } else if (yoffset < 0) {
        zoom -= 0.1f;
    } else {
        return;
    }
    if (zoom < minZoom) zoom = minZoom;

    double oldWidth = zoomWidth;
    double oldHeight = zoomHeight;
    zoomWidth = (GLsizei) (width * pow(zoom, 3));
    zoomHeight = (GLsizei) (height * pow(zoom, 3));

    glfwGetWindowSize (window, &Ww, &Wh);

    double percentX = (mx + vx) / oldWidth * 100;
    double newPositionX = (double)zoomWidth / 100.0f * percentX;
    vx += newPositionX - (mx + vx);
    //printf("%f - %f - %f - %f - %d\n", vx, mx, percentX,  oldWidth, zoomWidth);

    double rMy = height - (my - (Wh - height));
    double percentY = (rMy + vy) / oldHeight * 100;
    double newPositionY = (double)zoomHeight / 100.0f * percentY;
    vy += newPositionY - (rMy + vy);
    //printf("%f - %f - %f - %f - %d\n",  vy, rMy, percentY,  oldHeight, zoomHeight);
}

void mouse_button_callback(GLFWwindow *window2, int button, int action, int mods) {

    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {

        } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
            isMoveMode = true;
        }
    } else {
        if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
            isMoveMode = false;
        }
    }
}

void cursor_position_callback(GLFWwindow *window2, double xpos, double ypos) {
    if (isMoveMode) {
        vx += (mx - xpos);
        vy += (my - ypos) * -1;
    }
    mx = xpos;
    my = ypos;
}

void key_callback(GLFWwindow *window2, int key, int scancode, int action, int kmode) {
    if (key == GLFW_KEY_SPACE && action == 1) {
        isPause = !isPause;
    }
}

void loop() {
    glBegin(GL_QUADS);
    //glTexCoord2f(0,0);
    glVertex2f(-1, -1);

    //glTexCoord2f(0,1);
    glVertex2f(-1, 1);

    //glTexCoord2f(1,1);
    glVertex2f(1, 1);

    //glTexCoord2f(1,0);
    glVertex2f(1, -1);
    glEnd();
}

char *readFile(const char *filaName) {
    static int length = 12000;
    char *text = malloc(length);
    memset(text, 0, length);
    FILE *f;
    f = fopen(filaName, "rb");
    if (f) {
        fread(text, 1, length, f);
        fclose(f);
    } else {
        return NULL;
    }
    return text;
}

GLuint loadShader(const char *fileName, GLuint type) {
    char *text = readFile(fileName);
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar *const *) &text, NULL);
    glCompileShader(shader);
    free(text);

    GLsizei oks;
    GLchar log[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &oks);
    if (!oks) {
        glGetShaderInfoLog(shader, 1024, NULL, log);
        char error[1024] = "Shader compile error: ";
        strcat(error, log);
        throw(1, error);
    }
    return shader;
}

void createShaders() {
    GLsizei oks;
    GLchar log[1024];
    GLuint vertexShader = loadShader("/home/bublik/CLionProjects/automat/shader.vert", GL_VERTEX_SHADER);


    GLuint fragShader = loadShader("/home/bublik/CLionProjects/automat/shader.frag", GL_FRAGMENT_SHADER);
    prog = glCreateProgram();
    glAttachShader(prog, vertexShader);
    glAttachShader(prog, fragShader);
    glLinkProgram(prog);
    glGetProgramiv(prog, GL_LINK_STATUS, &oks);
    if (!oks) {
        glGetProgramInfoLog(prog, 1024, NULL, log);
        char error[1024] = "Shader error: ";
        strcat(error, log);
        throw(1, error);
    }

    GLuint fragShader2 = loadShader("/home/bublik/CLionProjects/automat/shader2.frag", GL_FRAGMENT_SHADER);
    prog2 = glCreateProgram();
    glAttachShader(prog2, vertexShader);
    glAttachShader(prog2, fragShader2);
    glLinkProgram(prog2);
    glGetProgramiv(prog2, GL_LINK_STATUS, &oks);
    if (!oks) {
        glGetProgramInfoLog(prog2, 1024, NULL, log);
        char error[1024] = "Shader2 error: ";
        strcat(error, log);
        throw(1, error);
    }

    GLuint fragShader1 = loadShader("/home/bublik/CLionProjects/automat/shader1.frag", GL_FRAGMENT_SHADER);
    prog1 = glCreateProgram();
    glAttachShader(prog1, vertexShader);
    glAttachShader(prog1, fragShader1);
    glLinkProgram(prog1);
    glGetProgramiv(prog1, GL_LINK_STATUS, &oks);
    if (!oks) {
        glGetProgramInfoLog(prog1, 1024, NULL, log);
        char error[1024] = "Shader1 error: ";
        strcat(error, log);
        throw(1, error);
    }
}

GLFWwindow* initGL() {
    glfwSetErrorCallback(throw);
    if (!glfwInit()) {
        throw(1, "Error: glfwInit error");
    }

    //glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(width, height, "My Title", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw(1, "Error: glfwCreateWindow error");
    }
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

    //minZoom = (double)mode->width / (double)width;
    //zoom = (double)mode->width / (double)width;
    //minZoom = zoom;
    zoom = (double)mode->height / (double)height;


    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw(1, "Failed to initialize OpenGL context");
    }
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_MULTISAMPLE);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    return window;
}

void initBuffers( struct image *data) {
    glGenTextures(1, &colorBufferID);
    glBindTexture(GL_TEXTURE_2D, colorBufferID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, (GLuint) framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferID, 0);
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        throw(1, "Buffer create filed");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenTextures(1, &colorBufferID2);
    glBindTexture(GL_TEXTURE_2D, colorBufferID2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &framebuffer2);
    glBindFramebuffer(GL_FRAMEBUFFER, (GLuint) framebuffer2);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferID2, 0);
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        throw(1, "Buffer2 create filed");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    glGenTextures(1, &currentBufferID);
    glBindTexture(GL_TEXTURE_2D, currentBufferID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &framebuffer1);
    glBindFramebuffer(GL_FRAMEBUFFER, (GLuint) framebuffer1);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, currentBufferID, 0);
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        throw(1, "Buffer2 create filed");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void calculateBuffers(int frame) {
    glUseProgram(prog);
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glUniform2f(glGetUniformLocation(prog, "resolution"), (float) width, (float) height);
    if (frame % 2 == 0) {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
        glBindTexture(GL_TEXTURE_2D, colorBufferID);
        glUniform1i(glGetUniformLocation(prog, "SourceTexture"), 0);
        loop();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //currentBufferID = colorBufferID;
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glBindTexture(GL_TEXTURE_2D, colorBufferID2);
        glUniform1i(glGetUniformLocation(prog, "SourceTexture"), 0);
        loop();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //currentBufferID = colorBufferID2;
    }
    glUseProgram(0);

    glUseProgram(prog1);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer1);
    //glBindTexture(GL_TEXTURE_2D, colorBufferID);
    glUniform2f(glGetUniformLocation(prog1, "resolution"), (float) width, (float) height);
    glUniform1i(glGetUniformLocation(prog1, "SourceTexture"), 0);
    loop();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(0);
}

void showCurrentBuffer() {
    glLoadIdentity();

    glUseProgram(prog2);
    glBindTexture(GL_TEXTURE_2D, currentBufferID);
    glUniform2f(glGetUniformLocation(prog2, "resolution"), (GLfloat) zoomWidth, (GLfloat) zoomHeight);
    glUniform1i(glGetUniformLocation(prog2, "SourceTexture"), 0);
    glUniform2f(glGetUniformLocation(prog2, "shift"), (GLfloat) vx, (GLfloat) vy);
    loop();
    glUseProgram(0);
}

void dispose() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteProgram(prog);
    glDeleteProgram(prog2);
    glfwDestroyWindow(window);
    glfwTerminate();
}

int main() {
    window = initGL();
    createShaders();

    struct image data[width][height];
    memset(data, 0, sizeof(data));
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            data[i][j].r = 255;
            data[i][j].g = 255;
            data[i][j].b = 255;

            if (i % 3 != 0 && j % 3 != 0) {
                data[i][j].a = 255;
//            } else if (i >500 && j > 500) {
//                data[i][j].r = 255;
//            } else if (i % 5 != 0 && j % 5 != 0) {
//                data[i][j].r = 255;
            } else {
                data[i][j].a = 0;
            }
            //data[i][j].a = 0;
        }
    }

    data[0][4].a = 255;
    data[0][5].a = 255;
    data[1][4].a = 255;
    data[1][6].a = 255;
    data[2][5].a = 255;

    data[17][2].a = 255;
    data[17][1].a = 255;
    data[18][0].a = 255;
    data[18][2].a = 255;
    data[19][1].a = 255;

    data[15][7].a = 255;
    data[16][6].a = 255;
    data[16][8].a = 255;
    data[17][7].a = 255;
    data[17][6].a = 255;

    data[10][7].a = 255;
    data[11][6].a = 255;
    data[11][8].a = 255;
    data[12][7].a = 255;
    data[12][8].a = 255;


    data[rand() % width][rand() % height].a = 255;
    data[rand() % width][rand() % height].a = 255;
    data[rand() % width][rand() % height].a = 255;
    data[rand() % width][rand() % height].a = 255;
    data[rand() % width][rand() % height].a = 255;

    initBuffers( *data);

    int i = 0;
    zoomWidth = (GLsizei)(width * zoom);
    zoomHeight =  (GLsizei)(height * zoom);
    //currentBufferID = colorBufferID;

    do {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        if (isPause != true) {
            calculateBuffers(i);
            i++;
        }

        glViewport(0, 0, zoomWidth, zoomHeight);
        showCurrentBuffer();

        glfwSwapBuffers(window);
        if (i % 100 == 0 && isPause == false) {
            printf("%d\n", i);
        }
        //usleep(100000);
        //sleep(1);

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    dispose();
    return 0;
}