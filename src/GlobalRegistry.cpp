#include <GlobalRegistry.h>

static int gScreenWidth = 0;
static int gScreenHeight = 0;

void GlobalRegistry::setScreenSize(int _width, int _height) {
  gScreenWidth = _width;
  gScreenHeight = _height;
}

void GlobalRegistry::getScreenSize(int &_width, int &_height) {
  _width = gScreenWidth;
  _height = gScreenHeight;
}