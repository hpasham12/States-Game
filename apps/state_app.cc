// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "state_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include <../cmake-build-debug/_deps/nlohmann_json-src/single_include/nlohmann/json.hpp>
#include <cinder/gl/draw.h>
#include <cinder/gl/gl.h>
#include <iostream>
#include <cinder/Log.h>
#include <cinder/Color.h>
#include <algorithm>
#include <random>
#include <ctime>

namespace stateapp {

using cinder::app::KeyEvent;
using json = nlohmann::json;
using cinder::TextBox;
using cinder::ColorA;

const char kNormalFont[] = "Arial";

StateApp::StateApp() {
  std::ifstream i("C:/Users/hpash/cinder_0.9.2_vc2015/my-projects/final-project-hpasham12/resources/state_info.json");
  i >> json_obj;
  std::cout << json_obj["statesList"].at(0)["state"] << std::endl;
  int start_number = 1; //alaska
  int end_number = 10; //hawaii

  while (start_number == 1 || start_number == 10 || end_number == 1||end_number== 10 || start_number == end_number) {
    start_number = rand() % 50;
    end_number = rand() % 50;
  }

  start_state = json_obj["statesList"].at(start_number)["state"];
  end_state = json_obj["statesList"].at(end_number)["state"];

}

void StateApp::setup() {
  cinder::gl::enableDepthWrite();
  cinder::gl::enableDepthRead();

  CI_LOG_E( "Cinder Log test" );
  std::cout << "cout test" << std::endl;
  console() << "console test";

  auto img = loadImage( loadAsset( "map.png" ) );
  mTex = cinder::gl::Texture2d::create( img );
}

void StateApp::update() { }

void StateApp::draw() {
  //cinder::gl::enableAlphaBlending();
  cinder::gl::clear();
  cinder::gl::draw( mTex );
}

template <typename C>
void PrintText(const std::string& text, const C& color, const cinder::ivec2& size,
               const cinder::vec2& loc) {
  cinder::gl::color(color);

  auto box = TextBox()
      .alignment(TextBox::LEFT)
      .font(cinder::Font(kNormalFont, 30))
      .size(10, 10)
      .color(color)
      .backgroundColor(ColorA(0, 0, 0, 0))
      .text(text);

  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture);
}

void StateApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_UP: {
      PrintText("up arrow", cinder::Color::white(), {100, 100}, {50, 50});
      console() << "Up Arrow pressed" << std::endl;
      break;
    }
    case KeyEvent::KEY_RETURN: {
      std::string some_string;
      std::cin >> some_string; //somehow read in a string
      ReadInput(some_string);
      break;
    }
  }
}

void StateApp::ReadInput(std::string& statename) {
  int state_num = FindStateNum(statename);
  int starting_state_num = FindStateNum(start_state);

  if (state_num != -1 && starting_state_num != -1) {
    if (CheckBordering(starting_state_num, state_num)) {
      if (StringCompare(statename, end_state)) {
        std::cout << "you win! nice job!" << std::endl;
      } else {
        start_state = statename;
        DisplayStates(start_state, end_state);
      }
    } else {
      std::cout << "this state isn't a bordering state. try again" << std::endl;
    }
  } else {
    std::cout << "not a state. check the spelling? try again." << std::endl;
  }

}
int StateApp::FindStateNum(std::string& state) {
  // convert string to upper case
  transform(state.begin(), state.end(), state.begin(), ::toupper);
  int state_num = -1;

  try {
    state_num = json_obj["stateToNumber"][state];
  } catch (_exception e) {
    return state_num;
  }

  return state_num;
}
bool StateApp::CheckBordering(int start_num, int state_num) {
  std::vector<std::string> bordering = json_obj["statesList"].at(start_num)["borders"];
  std::string abbreviation = json_obj["statesList"].at(state_num)["abbreviation"];

  for (const auto & i : bordering) {
    if (i == abbreviation) {
      return true;
    }
  }

  return false;
}
void StateApp::DisplayStates(std::string starting, std::string ending) {
  PrintText(starting, cinder::Color::black(), {100, 100}, {50, 50});
  PrintText(ending, cinder::Color::white(), {300, 100}, {50, 50});
}
bool StateApp::StringCompare(std::string& str1, std::string str2) {
  //convert s1 and s2 into lower case strings
  transform(str1.begin(), str1.end(), str1.begin(), ::toupper);
  transform(str2.begin(), str2.end(), str2.begin(), ::toupper);
  if(str1.compare(str2) == 0) {
    return true;  // The strings are same
  }

  return false; //not matched
}

}  // namespace stateapp
