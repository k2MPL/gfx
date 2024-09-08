#include <gfx/GFXShader.h>

#include <Engine.h>

static int gShaderIDIndexer = 0;

bool GFXShader::init(const InitInfo &_info) {
  const GLuint handle = glCreateShader(_info.type);

  if (handle == INVALID_GL_HANDLE) {
    LOGGER.log("Unnable to create shader object");

    return false;
  }

  const char *src = _info.src.c_str();
  glShaderSource(handle, 1, &src, nullptr);
  glCompileShader(handle);

  GLint status = 0;
  glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &status);

  if (status != 0) {
    char infoLog[1024] = {0};
    glGetShaderInfoLog(handle, 1024, nullptr, infoLog);

    LOGGER.log(infoLog);
  }

  status = GL_FALSE;
  glGetShaderiv(handle, GL_COMPILE_STATUS, &status);

  if (status == GL_FALSE) {
    glDeleteShader(handle);

    return false;
  }

  m_Type = _info.type;
  m_Handle = handle;
  m_ID.value = ++gShaderIDIndexer;

  return true;
}

void GFXShader::shutdown() {
  if (m_Handle != INVALID_GL_HANDLE) {
    glDeleteShader(m_Handle);

    m_Type = GL_INVALID_ENUM;
    m_Handle = INVALID_GL_HANDLE;
  }
}

GLenum GFXShader::getType() const { return m_Type; }

GLuint GFXShader::getHandle() const { return m_Handle; }

const GFXShaderID &GFXShader::getID() const { return m_ID; }

// ---------- Program ----------

static int gShaderProgramIDIndexer = 0;

bool GFXShaderProgram::init(const InitInfo &_info) {
  const GLuint handle = glCreateProgram();

  if (handle == INVALID_GL_HANDLE) {
    LOGGER.log("Unnable to create shader program object");

    return false;
  }

  for (const GFXShader *shader : _info.shaders) {
    glAttachShader(handle, shader->getHandle());
  }

  glLinkProgram(handle);

  GLint status = 0;
  glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &status);

  if (status != 0) {
    char infoLog[1024] = {0};
    glGetProgramInfoLog(handle, 1024, nullptr, infoLog);

    LOGGER.log(infoLog);
  }

  status = GL_FALSE;
  glGetProgramiv(handle, GL_LINK_STATUS, &status);

  for (GFXShader *shader : _info.shaders) {
    glDetachShader(handle, shader->getHandle());
  }

  if (status == GL_FALSE) {
    glDeleteProgram(handle);

    return false;
  }

  m_Handle = handle;
  m_ID.value = ++gShaderProgramIDIndexer;

  return true;
}

void GFXShaderProgram::shutdown() {
  if (m_Handle != INVALID_GL_HANDLE) {
    glDeleteProgram(m_Handle);
    m_Handle = INVALID_GL_HANDLE;
  }
}

GLuint GFXShaderProgram::getHandle() const { return m_Handle; }

const GFXShaderProgramID &GFXShaderProgram::getID() const { return m_ID; }