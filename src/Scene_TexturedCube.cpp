#include <Scene_TexturedCube.h>

#include <Engine.h>
#include <GlobalRegistry.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <stb_image.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>

const std::string SceneTexturedCube::gSceneName = "Textured Cube";

const std::string &SceneTexturedCube::getName() const { return gSceneName; }

bool SceneTexturedCube::init() {
  if (initShaders() && initGeometry() && initTextures()) {
    glEnable(GL_DEPTH_TEST);

    return true;
  }

  return false;
}

void SceneTexturedCube::onUpdate(float _delta) {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(m_Program.getHandle());

  glm::mat4 worldMat = glm::mat4{1.0f};
  worldMat = glm::translate(worldMat, glm::vec3{0.0f, 1.0f, 0.0f});

  renderMesh(m_Cube, worldMat, m_BrickTexture);

  worldMat = glm::mat4{1.0f};
  worldMat = glm::scale(worldMat, glm::vec3{3.0f, 1.0f, 3.0f});

  renderMesh(m_Plane, worldMat, m_MossTexture);
}

void SceneTexturedCube::onShutdown() {}

bool SceneTexturedCube::initShaders() {
  const std::string vsSrc =
      FILE_SYSTEM.readTextFromFile("shaders/scene_textured_cube.vs");

  if (vsSrc.empty()) {
    //#TODO: Log
    return false;
  }

  const std::string fsSrc =
      FILE_SYSTEM.readTextFromFile("shaders/scene_textured_cube.fs");

  if (fsSrc.empty()) {
    //#TODO: Log
    return false;
  }

  struct ShaderScope {
    GFXShader *shader{nullptr};

    ~ShaderScope() {
      if (shader) {
        shader->shutdown();
      }
    }
  };

  GFXShader::InitInfo vsInitInfo;
  vsInitInfo.type = GL_VERTEX_SHADER;
  vsInitInfo.src = vsSrc;

  GFXShader vs;

  ShaderScope vsScope;
  vsScope.shader = &vs;

  if (!vs.init(vsInitInfo)) {
    return false;
  }

  GFXShader::InitInfo fsInitInfo;
  fsInitInfo.type = GL_FRAGMENT_SHADER;
  fsInitInfo.src = fsSrc;

  GFXShader fs;

  ShaderScope fsScope;
  fsScope.shader = &fs;

  if (!fs.init(fsInitInfo)) {
    return false;
  }

  GFXShaderProgram::InitInfo programInitInfo;
  programInitInfo.shaders.push_back(&vs);
  programInitInfo.shaders.push_back(&fs);

  if (!m_Program.init(programInitInfo)) {
    return false;
  }

  return true;
}

bool SceneTexturedCube::initGeometry() {
  if (loadMesh("data/geometry/cube.obj", m_Cube) &&
      loadMesh("data/geometry/plane.obj", m_Plane)) {
    int screenWidth = 0;
    int screenHeight = 0;

    GlobalRegistry::getScreenSize(screenWidth, screenHeight);

    glm::mat4 view =
        glm::lookAt(glm::vec3{3.0f, 3.0f, 3.0f}, glm::vec3{0.0f, 0.0f, 0.0f},
                    glm::vec3{0.0f, 1.0f, 0.0f});

    glm::mat4 projection = glm::perspective(
        glm::radians(60.0f),
        static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
        0.1f, 100.0f);

    m_VPMatrix = projection * view;

    return true;
  }

  return false;
}

bool SceneTexturedCube::initTextures() {
  return loadTexture("textures/moss.jpg", m_MossTexture) &&
         loadTexture("textures/brick.jpg", m_BrickTexture);
}

bool SceneTexturedCube::loadTexture(const std::string &_path,
                                    GFXTexture2D &_texture) {

  const std::vector<unsigned char> buffer =
      FILE_SYSTEM.readBinaryFromFile(_path);

  if (buffer.empty()) {
    LOGGER.log("Unnable to read texture");

    return false;
  }

  int width = 0;
  int height = 0;
  int channels = 0;

  stbi_uc *textureData = stbi_load_from_memory(buffer.data(), buffer.size(),
                                               &width, &height, &channels, 0);

  assert(channels == 3 && "Expected RGB texture");

  if (!textureData) {
    LOGGER.log("Unnable to process texture data");

    return false;
  }

  GFXTexture2D::InitInfo info;
  info.data = textureData;
  info.width = width;
  info.height = height;
  info.internalFormat = GL_RGB;
  info.format = GL_RGB;
  info.type = GL_UNSIGNED_BYTE;
  info.iParams.push_back({GL_TEXTURE_WRAP_S, GL_REPEAT});
  info.iParams.push_back({GL_TEXTURE_WRAP_T, GL_REPEAT});
  info.iParams.push_back({GL_TEXTURE_MIN_FILTER, GL_LINEAR});
  info.iParams.push_back({GL_TEXTURE_MAG_FILTER, GL_LINEAR});

  const bool result = _texture.init(info);
  stbi_image_free(textureData);

  return result;
}

bool SceneTexturedCube::loadMesh(const std::string &_path, Mesh &_mesh) {
  // https://community.khronos.org/t/how-to-import-3d-model-file-from-memory-as-a-file-stream-to-assimp-importer/107190/11
  // https://ogldev.org/www/tutorial22/tutorial22.html

  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile(
      _path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);

  if (!scene) {
    //#TODO: Log

    return false;
  }

  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
  };

  std::vector<Vertex> vtxData;
  std::vector<unsigned int> idxData;

  for (int i = 0; i < scene->mNumMeshes; ++i) {
    const aiMesh *mesh = scene->mMeshes[i];

    for (int j = 0; j < mesh->mNumVertices; ++j) {
      const aiVector3D &position = mesh->mVertices[j];
      const aiVector3D &normal = mesh->mNormals[j];

      Vertex vtx;
      vtx.position = glm::vec3{position.x, position.y, position.z};
      vtx.normal = glm::vec3{normal.x, normal.y, normal.z};

      if (mesh->HasTextureCoords(0)) {
        const aiVector3D &texCoords = mesh->mTextureCoords[0][j];
        vtx.texCoords = glm::vec2{texCoords.x, texCoords.y};
      }

      vtxData.push_back(vtx);
    }

    for (int j = 0; j < mesh->mNumFaces; ++j) {
      const aiFace &face = mesh->mFaces[j];

      idxData.push_back(face.mIndices[0]);
      idxData.push_back(face.mIndices[1]);
      idxData.push_back(face.mIndices[2]);
    }
  }

  GLuint vao = INVALID_GL_HANDLE;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo = INVALID_GL_HANDLE;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vtxData.size(), vtxData.data(),
               GL_STATIC_DRAW);

  GLuint ebo = INVALID_GL_HANDLE;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * idxData.size(),
               idxData.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)(sizeof(glm::vec3::value_type) * 3));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)(sizeof(glm::vec3::value_type) * 6));

  _mesh.vao = vao;
  _mesh.vbo = vbo;
  _mesh.ebo = ebo;
  _mesh.idxCount = idxData.size();

  return true;
}

void SceneTexturedCube::renderMesh(const Mesh &_mesh,
                                   const glm::mat4 &_worldMat,
                                   const GFXTexture2D &_texture) {
  glBindVertexArray(_mesh.vao);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _texture.getHandle());

  glm::mat4 mvp = m_VPMatrix * _worldMat;

  m_Program.setUniform("uMVP", mvp);
  m_Program.setUniform("uTexture", 0);

  glDrawElements(GL_TRIANGLES, _mesh.idxCount, GL_UNSIGNED_INT, nullptr);
}

// ---------- Factory ----------

const std::string &SceneTexturedCubeFactory::getName() const {
  return SceneTexturedCube::gSceneName;
}

std::unique_ptr<Scene> SceneTexturedCubeFactory::instantiate() {
  std::unique_ptr<SceneTexturedCube> scene{new SceneTexturedCube{}};

  if (scene->init()) {
    return std::move(scene);
  }

  return nullptr;
}