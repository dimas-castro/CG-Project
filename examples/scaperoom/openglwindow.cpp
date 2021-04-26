#include "openglwindow.hpp"

#include <imgui.h>

#include <string.h>

#include <cppitertools/itertools.hpp>

#include <glm/gtc/matrix_inverse.hpp>

void OpenGLWindow::handleEvent(SDL_Event& ev) {
  if (ev.type == SDL_KEYDOWN) {
    if (ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w)
      m_dollySpeed = 0.3f;
    if (ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s)
      m_dollySpeed = -0.3f;
    if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_q) m_truckSpeed = -0.3f;
    if (ev.key.keysym.sym == SDLK_e) m_truckSpeed = 0.3f;
    if (ev.key.keysym.sym == SDLK_LSHIFT && down == 0) {
      if(ActiveRoom == 1 || ActiveRoom == 3) {
        m_camera.m_eye[1] = m_camera.m_eye[1] - 0.1;
        m_camera.m_at[1] = m_camera.m_at[1] - 0.1;
      } else {
        m_camera.m_eye[1] = m_camera.m_eye[1] - 0.05;
        m_camera.m_at[1] = m_camera.m_at[1] - 0.05;
      }
      down++;
    }
    porta();
    livro();
  }
  if (ev.type == SDL_KEYUP) {
    if ((ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_q && m_truckSpeed < 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_e && m_truckSpeed > 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_SPACE && porta()) {
      if (ActiveRoom == 1) {
        if (NoDoor == 1) {
          m_camera.m_eye[0] = 0;
          m_camera.m_eye[1] = -0.12;
          m_camera.m_eye[2] = 0;
          m_camera.m_at[1] = -0.12;
          ActiveRoom = 2;
          loadModel(getAssetsPath() + "model1.obj", getAssetsPath() + "maps/tex_u1_v2.jpg");
        }
      } else if (ActiveRoom == 2) {
        if (NoDoor == 1) {
          m_camera.m_eye[0] = 0;
          m_camera.m_eye[1] = -0.22;
          m_camera.m_eye[2] = 0;
          m_camera.m_at[1] = -0.22;
          ActiveRoom = 1;
          loadModel(getAssetsPath() + "model.obj", getAssetsPath() + "maps/tex_u1_v1.jpg");
        } else if (NoDoor == 2) {
          m_camera.m_at[0] = 0.16;
          m_camera.m_at[2] = -0.13;
          ActiveRoom = 3;
          loadModel(getAssetsPath() + "model2.obj", getAssetsPath() + "maps/tex_u1_v3.jpg");
        }
      } else if (ActiveRoom == 3) {
        m_camera.m_eye[0] = 0;
        m_camera.m_eye[1] = -0.12;
        m_camera.m_eye[2] = 0;
        m_camera.m_at[1] = -0.12;
        ActiveRoom = 2;
        loadModel(getAssetsPath() + "model1.obj", getAssetsPath() + "maps/tex_u1_v2.jpg");
      }
      
    }
    if (ev.key.keysym.sym == SDLK_SPACE && livro()) {
      OpenBook = true;
    }
    if (ev.key.keysym.sym == SDLK_LSHIFT) {
      if(ActiveRoom == 1 || ActiveRoom == 3) {
        m_camera.m_eye[1] = m_camera.m_eye[1] + 0.1;
        m_camera.m_at[1] = m_camera.m_at[1] + 0.1;
      } else {
        m_camera.m_eye[1] = m_camera.m_eye[1] + 0.05;
        m_camera.m_at[1] = m_camera.m_at[1] + 0.05;
      }
      down--;
    }
  }
}

void OpenGLWindow::initializeGL() {

  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "Classic Story.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 30.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }
  auto filename2{getAssetsPath() + "Stranger.ttf"};
  m_font2 = io.Fonts->AddFontFromFileTTF(filename2.c_str(), 40.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "shaders/texture.vert",
                                    getAssetsPath() + "shaders/texture.frag");

  // Load model
  loadModel(getAssetsPath() + "model.obj", getAssetsPath() + "maps/tex_u1_v1.jpg");
  m_mappingMode = 3;

}

void OpenGLWindow::paintGL() {
  update();

  // Clear color buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  glUseProgram(m_program);

  // Get location of uniform variables (could be precomputed)
  GLint viewMatrixLoc{glGetUniformLocation(m_program, "viewMatrix")};
  GLint projMatrixLoc{glGetUniformLocation(m_program, "projMatrix")};
  GLint modelMatrixLoc{glGetUniformLocation(m_program, "modelMatrix")};
  GLint normalMatrixLoc{glGetUniformLocation(m_program, "normalMatrix")};
  GLint shininessLoc{glGetUniformLocation(m_program, "shininess")};
  GLint IaLoc{glGetUniformLocation(m_program, "Ia")};
  GLint IdLoc{glGetUniformLocation(m_program, "Id")};
  GLint IsLoc{glGetUniformLocation(m_program, "Is")};
  GLint KaLoc{glGetUniformLocation(m_program, "Ka")};
  GLint KdLoc{glGetUniformLocation(m_program, "Kd")};
  GLint KsLoc{glGetUniformLocation(m_program, "Ks")};
  GLint diffuseTexLoc{glGetUniformLocation(m_program, "diffuseTex")};
  GLint mappingModeLoc{glGetUniformLocation(m_program, "mappingMode")};

  // Set uniform variables used by every scene object
  glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);
  glUniform1i(diffuseTexLoc, 0);
  glUniform1i(mappingModeLoc, m_mappingMode);

  glUniform4fv(IaLoc, 1, &m_Ia.x);
  glUniform4fv(IdLoc, 1, &m_Id.x);
  glUniform4fv(IsLoc, 1, &m_Is.x);

  // Set uniform variables of the current object
  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);

  auto modelViewMatrix{glm::mat3(m_viewMatrix * m_modelMatrix)};
  glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  glUniform1f(shininessLoc, m_shininess);
  glUniform4fv(KaLoc, 1, &m_Ka.x);
  glUniform4fv(KdLoc, 1, &m_Kd.x);
  glUniform4fv(KsLoc, 1, &m_Ks.x);

  m_model.render(m_trianglesToDraw);

  glUseProgram(0);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    auto size{ImVec2(300, 40)};
    auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                         (m_viewportHeight - size.y) - 5)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar | 
                           ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoScrollbar};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (ActiveRoom == 1 && porta() && NoDoor == 2) {
      ImGui::InputText("", bufpass, 64);
      ImGui::SameLine(220);
      if (ImGui::Button("Ok")) {
        if (strcmp("I LOVE CG", bufpass) == 0) {
          ActiveRoom = 4;
          glClearColor(173, 234, 234, 1);
          m_program = createProgramFromFile(getAssetsPath() + "shaders/normal.vert",
                                            getAssetsPath() + "shaders/normal.frag");
          m_camera.m_eye[0] = -0.486;
          m_camera.m_eye[2] = 1.0;
          m_camera.m_eye[1] = 0;
          m_camera.m_at[1] = 0;
          m_camera.m_at[0] = -0.24;
          m_camera.m_at[2] = 0.03;
          loadModel(getAssetsPath() + "youwin.obj", getAssetsPath() + "maps/tex_u1_v4.jpg");
        }
      }
    } else if (ActiveRoom == 4) {
      if(ImGui::Button("Restart")) {
        m_program = createProgramFromFile(getAssetsPath() + "shaders/texture.vert",
                                            getAssetsPath() + "shaders/texture.frag");
        m_camera.m_at[0] = 1;
        m_camera.m_at[1] = -0.22;
        m_camera.m_at[2] = 0;
        m_camera.m_eye[0] = 0;
        m_camera.m_eye[1] = -0.22;
        m_camera.m_eye[2] = 0;
        loadModel(getAssetsPath() + "model.obj", getAssetsPath() + "maps/tex_u1_v1.jpg");
        ActiveRoom = 1;
      }
    } else if (porta()) {
      ImGui::Text("Press space to change to another room");
    } else if (livro()) {
      ImGui::Text("Press space to open the book");
    } else if (ActiveRoom == 1) {
      ImGui::Text("The Smoaking Room");
    } else if (ActiveRoom == 2) {
      ImGui::Text("The Greating Drawing Room");
    } else if (ActiveRoom == 3) {
      ImGui::Text("The Small Drawing Room");
    } 
    
    ImGui::PopFont();
    ImGui::End();

  }

  {
    if (livro() && OpenBook) {
      auto widgetSize{ImVec2(222, 244)};
      ImGui::SetNextWindowPos(ImVec2((m_viewportWidth - widgetSize.x) / 2,
                                    (m_viewportHeight - widgetSize.y) / 2));
      ImGui::SetNextWindowSize(widgetSize);
      ImGui::Begin("Senha", &OpenBook);

      ImGui::PushFont(m_font2);
      ImGui::Text("Password: I LOVE CG");
      ImGui::PopFont();
      ImGui::End();
    }
  }

  {
    if(first) {
      auto widgetSize{ImVec2(800, 222)};
      ImGui::SetNextWindowPos(ImVec2((m_viewportWidth - widgetSize.x) / 2,
                                    (m_viewportHeight - widgetSize.y) / 2));
      ImGui::SetNextWindowSize(widgetSize);
      ImGui::Begin("Instruções", &first);

      ImGui::Text("Bem-vindo");
      ImGui::Text("Para ganhar o jogo você deve procurar nas salas a senha que abre a porta de saída");
      ImGui::Text("Comandos:");
      ImGui::Text("Girar a câmera: A e D");
      ImGui::Text("Andar: W, S, Q e E");
      ImGui::Text("Shift: Agachar");
      ImGui::Text("Espaço: Interagir");

      ImGui::End();
    }
    
  }
  

  /*if (porta()) {
    auto widgetSizeB{ImVec2(222, 100)};
    // Slider to control light properties
    ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - widgetSizeB.x - 50,
                                   m_viewportHeight - widgetSizeB.y - 50));
    ImGui::SetNextWindowSize(widgetSizeB);
    ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoDecoration);
    ImGui::Text("%f", m_camera.m_eye[0]);
    ImGui::Text("%f", m_camera.m_eye[1]);
    ImGui::Text("%f", m_camera.m_eye[2]);
    
    ImGui::End();
  }*/
    
}

void OpenGLWindow::loadModel(std::string_view path, std::string_view path2) {
  m_model.loadDiffuseTexture(path2);
  m_model.loadFromFile(path);
  m_model.setupVAO(m_program);
  m_trianglesToDraw = m_model.getNumTriangles();

  // Use material properties from the loaded model
  m_Ka = m_model.getKa();
  m_Kd = m_model.getKd();
  m_Ks = m_model.getKs();
  m_shininess = m_model.getShininess();
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() { 
  glDeleteProgram(m_program); 
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}

bool OpenGLWindow::porta() {
  if (m_camera.m_eye[0] < -0.13 && m_camera.m_eye[0] > -0.4 && m_camera.m_eye[2] < -0.23 && m_camera.m_eye[2] > -0.35 && ActiveRoom == 1) {
    NoDoor = 1;
    return true;
  } else if (((m_camera.m_eye[0] < -0.36 && m_camera.m_eye[0] > -0.54) || (m_camera.m_eye[0] < -0.32 && m_camera.m_eye[0] > -0.46)) && ((m_camera.m_eye[2] < 0.28 && m_camera.m_eye[2] > 0.06) || (m_camera.m_eye[2] < 0.21 && m_camera.m_eye[2] > 0.06)) && (ActiveRoom == 2)) {
    NoDoor = 1;
    return true;
  } else if (((m_camera.m_eye[0] < 0.24 && m_camera.m_eye[0] > 0.12) || (m_camera.m_eye[0] < 0.19 && m_camera.m_eye[0] > 0.1)) && ((m_camera.m_eye[2] < 0.24 && m_camera.m_eye[2] > 0.15) || (m_camera.m_eye[2] < 0.2 && m_camera.m_eye[2] > 0.11)) && (ActiveRoom == 2)) {
    NoDoor = 2;
    return true;
  } else if (((m_camera.m_eye[0] < -0.01 && m_camera.m_eye[0] > -0.22) || (m_camera.m_eye[0] < 0.025 && m_camera.m_eye[0] > -0.21)) && ((m_camera.m_eye[2] < -0.29 && m_camera.m_eye[2] > -0.37) || (m_camera.m_eye[2] < -0.15 && m_camera.m_eye[2] > -0.26)) && (ActiveRoom == 3)) {
    return true;
  } else if (m_camera.m_eye[0] < 0.7 && m_camera.m_eye[0] > 0.45 && m_camera.m_eye[2] < 0.05 && m_camera.m_eye[2] > -0.2 && ActiveRoom == 1) {
    NoDoor = 2;
    return true;
  } else {
    return false;
  }
}

bool OpenGLWindow::livro() {
  if (((m_camera.m_eye[0] < 0.149 && m_camera.m_eye[0] > 0.127) || (m_camera.m_eye[0] < 0.197 && m_camera.m_eye[0] > 0.182)) && ((m_camera.m_eye[2] < 0.02 && m_camera.m_eye[2] > -0.078) || (m_camera.m_eye[2] < -0.02 && m_camera.m_eye[2] > -0.09)) && (ActiveRoom == 3)) {
    return true;
  } else {
    OpenBook = false;
    return false;
  }
}