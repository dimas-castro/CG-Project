#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
    void handleEvent(SDL_Event& event) override;
    void initializeGL() override;
    void paintGL() override;
    void terminateGL() override;

    GLuint m_program{};
    GLuint m_vao{};
    GLuint m_vboVertices{};

    struct MandelbrotData {
        double scale;
        float x;
        float y;
        unsigned max_iterations;
    };

    MandelbrotData mandelbrotData{1.0F, 0.0F, 0.0F, 30};

    void setupModel();
};

#endif