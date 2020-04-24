// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_STATEAPP_H_
#define FINALPROJECT_APPS_STATEAPP_H_

#include <cinder/app/App.h>
#include <cinder/gl/Texture.h>

#include <../cmake-build-debug/_deps/nlohmann_json-src/single_include/nlohmann/json.hpp>

namespace stateapp {

using json = nlohmann::json;

class StateApp : public cinder::app::App {
 public:
  StateApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;

  cinder::gl::Texture2dRef mTex;

 private:
  json json_obj;

};

}  // namespace stateapp

#endif  // FINALPROJECT_APPS_STATEAPP_H_
