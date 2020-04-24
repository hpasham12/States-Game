// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "state_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include <../cmake-build-debug/_deps/nlohmann_json-src/single_include/nlohmann/json.hpp>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>

namespace stateapp {

using cinder::app::KeyEvent;
using json = nlohmann::json;

StateApp::StateApp() {
  std::ifstream i("C:/Users/hpash/cinder_0.9.2_vc2015/my-projects/final-project-hpasham12/resources/state_info.json");
  i >> json_obj;
  std::cout << json_obj["statesList"].at(0)["state"] << std::endl;
}

void StateApp::setup() {
  cinder::gl::enableDepthWrite();
  cinder::gl::enableDepthRead();

  std::ifstream i("C:/Users/hpash/cinder_0.9.2_vc2015/my-projects/final-project-hpasham12/resources/state_info.json");
  i >> json_obj;

  auto img = loadImage( loadAsset( "map.png" ) );
  mTex = cinder::gl::Texture2d::create( img );
}

void StateApp::update() { }

void StateApp::draw() {
  //cinder::gl::enableAlphaBlending();
  cinder::gl::clear();

  cinder::gl::draw( mTex );
}

void StateApp::keyDown(KeyEvent event) { }

}  // namespace stateapp
