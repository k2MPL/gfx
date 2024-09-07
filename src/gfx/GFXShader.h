#pragma once

#include <gfx/GFXCore.h>

#include <string>
#include <vector>

class GFXShader {
public:
  struct InitInfo {
    GLenum type{GL_INVALID_ENUM};

    std::string src;
    std::string name;
    std::string path;
  };

  bool init(const InitInfo &_info);
  void shutdown();

  GLenum getType() const;
  GLuint getHandle() const;

  struct Info {
    std::string src;
    std::string name;
    std::string path;
  };

  const Info &getInfo() const;

private:
  GLenum m_Type{INVALID_GL_HANDLE};
  GLuint m_Handle{INVALID_GL_HANDLE};

  Info m_Info;
};

class GFXShaderProgram {
public:
  struct InitInfo {
    std::vector<GFXShader *> shaders;
  };

  bool init(const InitInfo &_info);
  void shutdown();

  GLuint getHandle() const;

  template <typename T> bool setUniform(const std::string &_name, T _value);

  template <> bool setUniform(const std::string &_name, int _value) {
    const GLint location = glGetUniformLocation(m_Handle, _name.c_str());

    if (location == -1) {
      //#TODO: Log
      return false;
    }

    glUniform1i(location, _value);

    return true;
  }

private:
  GLuint m_Handle{INVALID_GL_HANDLE};
};