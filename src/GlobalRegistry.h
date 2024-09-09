#pragma once

class GlobalRegistry {
public:
  static void setScreenSize(int _width, int _height);
  static void getScreenSize(int &_width, int &_height);
};