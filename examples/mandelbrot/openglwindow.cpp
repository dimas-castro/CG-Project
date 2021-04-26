#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>

#include "abcg.hpp"

void OpenGLWindow::handleEvent(SDL_Event &event) {
    double scale_per;
    if (event.type == SDL_KEYDOWN) {
        scale_per = 0.1F * mandelbrotData.scale;
        if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
            mandelbrotData.y += scale_per;
        if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
            mandelbrotData.y -= scale_per;
        if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
            mandelbrotData.x -= scale_per;
        if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
            mandelbrotData.x += scale_per;
        if (event.key.keysym.sym == SDLK_n)
            if (mandelbrotData.max_iterations > 30) --mandelbrotData.max_iterations;
		if (event.key.keysym.sym == SDLK_m)
            ++mandelbrotData.max_iterations;
        if (event.key.keysym.sym == SDLK_r)
            mandelbrotData = {1.0F, 0.0F, 0.0F, 30};
        if (event.key.keysym.sym == SDLK_z) {
            mandelbrotData.scale -= scale_per;
            ++mandelbrotData.max_iterations;
		}
        if (event.key.keysym.sym == SDLK_x) {
            mandelbrotData.scale += scale_per;
            if (mandelbrotData.max_iterations > 30) --mandelbrotData.max_iterations;
		}
    }

    if(event.type == SDL_MOUSEWHEEL) {
        scale_per = 0.1F * mandelbrotData.scale;
        if(event.wheel.y > 0) {
            mandelbrotData.scale -= scale_per;
            ++mandelbrotData.max_iterations;
        }
        else if(event.wheel.y < 0) {
            mandelbrotData.scale += scale_per;
            if (mandelbrotData.max_iterations > 30) --mandelbrotData.max_iterations;
        }
    }
}

void OpenGLWindow::initializeGL() {

    auto windowSettings{getWindowSettings()};

    m_program = createProgramFromFile(getAssetsPath() + "shader.vert",
                                           getAssetsPath() + "shader.frag");

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
}

void OpenGLWindow::paintGL() {
    setupModel();
    glUseProgram(m_program);
    glBindVertexArray(m_vao);
		
	GLint areaWLoc = glGetUniformLocation(m_program, "area_w");
    GLint areaHLoc = glGetUniformLocation(m_program, "area_h");   
    GLint maxItLoc = glGetUniformLocation(m_program, "max_iterations");   
        
    glUniform2f(areaWLoc, -2.0F * mandelbrotData.scale + mandelbrotData.x,
        1.0F * mandelbrotData.scale + mandelbrotData.x);
    glUniform2f(areaHLoc, -1.0F * mandelbrotData.scale + mandelbrotData.y,
        1.0F * mandelbrotData.scale + mandelbrotData.y);
    glUniform1ui(maxItLoc, mandelbrotData.max_iterations); 

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glBindVertexArray(0);
    glUseProgram(0);
}

void OpenGLWindow::setupModel() {

    constexpr GLfloat m_P[]{

            -1.0F, -1.0F, 0.0F,

             1.0F, -1.0F, 0.0F,

             1.0F,  1.0F, 0.0F,

             1.0F,  1.0F, 0.0F,

            -1.0F,  1.0F, 0.0F,

            -1.0F, -1.0F, 0.0F

    };

    // Release previous VBO and VAO
    glDeleteBuffers(1, &m_vboVertices);
    glDeleteVertexArrays(1, &m_vao);

    // Generate a new VBO and get the associated ID
    glGenBuffers(1, &m_vboVertices);
    // Bind VBO in order to use it
    glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
    // Upload data to VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_P), m_P, GL_STATIC_DRAW);
    // Unbinding the VBO is allowed (data can be released now)
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create VAO
    glGenVertexArrays(1, &m_vao);

    // Bind vertex attributes to current VAO
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboVertices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // End of binding to current VAO
    glBindVertexArray(0);
}

void OpenGLWindow::terminateGL() {
  // Release shader program, VBO and VAO
  glDeleteProgram(m_program);
  glDeleteBuffers(1, &m_vboVertices);
  glDeleteVertexArrays(1, &m_vao);
}