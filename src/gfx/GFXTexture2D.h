#pragma once

#include <gfx/GFXCore.h>

#include <string>
#include <utility>
#include <vector>

#include <ProtectedID.h>

class GFXTexture2D;
using GFXTexture2DID = ProtectedID<GFXTexture2D, int, -1>;

class GFXTexture2D final {
public:
  struct InitInfo {
    std::vector<std::pair<GLenum, GLint>> iParams;

    GLint internalFormat;

    GLenum format;
    GLenum type;

    GLsizei width{0};
    GLsizei height{0};

    const void *data{nullptr};

    std::string name;
    std::string path;
  };

  bool init(const InitInfo &_info);
  void shutdown();

  GLuint getHandle() const;
  const GFXTexture2DID &getID() const;

  struct Info {
    GLsizei width;
    GLsizei height;

    std::string name;
    std::string path;
  };

  const Info &getInfo() const;

private:
  GLuint m_Handle{INVALID_GL_HANDLE};

  GFXTexture2DID m_ID;

  Info m_Info;
};