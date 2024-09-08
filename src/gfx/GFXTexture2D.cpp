#include <gfx/GFXTexture2D.h>

#include <Engine.h>

static int gTexture2DIndexer = 0;

bool GFXTexture2D::init(const InitInfo &_info) {
  GLuint texture = INVALID_GL_HANDLE;
  glGenTextures(1, &texture);

  if (texture == INVALID_GL_HANDLE) {
    LOGGER.log("Unnable to create texture object");

    return false;
  }

  glBindTexture(GL_TEXTURE_2D, texture);

  for (const std::pair<GLenum, GLint> &param : _info.iParams) {
    glTexParameteri(GL_TEXTURE_2D, param.first, param.second);
  }

  glTexImage2D(GL_TEXTURE_2D, 0, _info.internalFormat, _info.width,
               _info.height, 0, _info.format, _info.type, _info.data);

  m_Handle = texture;
  m_ID.value = ++gTexture2DIndexer;

  return true;
}

void GFXTexture2D::shutdown() {
  if (m_Handle != INVALID_GL_HANDLE) {
    glDeleteTextures(1, &m_Handle);
  }
}

GLuint GFXTexture2D::getHandle() const { return m_Handle; }

const GFXTexture2DID &GFXTexture2D::getID() const { return m_ID; }

const GFXTexture2D::Info &GFXTexture2D::getInfo() const { return m_Info; }