#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include "abcg.hpp"
#include "model.hpp"
#include "camera.hpp"
#include <imgui.h>


class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_program{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  int m_mappingMode{};

  Model m_model;
  int m_trianglesToDraw{};

  Camera m_camera;
  float m_dollySpeed{0.0f};
  float m_truckSpeed{0.0f};
  float m_panSpeed{0.0f};

  int down = 0;
  int ActiveRoom = 1;
  int NoDoor;

  ImFont* m_font{};
  ImFont* m_font2{};

  char bufpass[64] = "PASSWORD";
  bool OpenBook = false;

  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka;
  glm::vec4 m_Kd;
  glm::vec4 m_Ks;
  float m_shininess{};

  glm::mat4 m_modelMatrix{1.0f};
  glm::mat4 m_viewMatrix{1.0f};

  bool first{true};

  void loadModel(std::string_view path, std::string_view path2);
  void update();
  bool porta();
  bool livro();
};

#endif