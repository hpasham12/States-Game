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
  std::ifstream file("C:/Users/hpash/cinder_0.9.2_vc2015/my-projects/final-project-hpasham12/resources/state_info.json");
  file >> json_obj;
  std::cout << json_obj["statesList"].at(0)["state"] << std::endl;
  int start_number = 1; //alaska
  int end_number = 10; //hawaii
  srand((unsigned) time(0));

  while (start_number == 1 || start_number == 10 || end_number == 1||end_number== 10 || start_number == end_number) {

    start_number = rand() % 50;
    end_number = rand() % 50;
  }

  std::string start = json_obj["statesList"].at(start_number)["state"];
  std::string end = json_obj["statesList"].at(end_number)["state"];
  start_state = start;
  end_state = end;
  state_ = GameState::kStartState;
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

//From snake lab
template <typename C>
void PrintText(const std::string& text, const C& color, const cinder::ivec2& size,
               const cinder::vec2& loc) {
  cinder::gl::color(color);

  auto box = TextBox()
      .alignment(TextBox::LEFT)
      .font(cinder::Font(kNormalFont, 30))
      .size(size)
      .color(color)
      .backgroundColor(ColorA(0, 0, 0, 0))
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x / 2, loc.y - box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}

void StateApp::update() {
  if (state_ == GameState::kNewStateEntered) {
    ReadInput(mUserState);
  }
//  if (state_ == GameState::kPlaying) {
//    PrintUserState();
//  }
}

void StateApp::draw() {
  cinder::gl::enableAlphaBlending();
  cinder::gl::clear();
  PrintStates(start_state, end_state);

  if (state_ == GameState::kPlaying) {
    PrintUserState();
  }
  if (state_ == GameState::kInvalidState) {
    PrintText("that's not a state. try again!", cinder::Color(.75, 0, 0), {500, 50}, {600, 850});
  }
  if (state_ == GameState::kInvalidBorder) {
    PrintText("that state isn't bordering the current one you're at. try again!", cinder::Color(1, 0, 0), {500, 50}, {600, 850});
  }
  if (state_ == GameState::kGameOver) {
    PrintText("WOOHOO YOU WON! NICE JOB!", cinder::Color(0, 0, 1), {500, 50}, {600, 850});
  }

  cinder::gl::draw( mTex );
}

void StateApp::keyDown(KeyEvent event) {
  if (event.getCode() == KeyEvent::KEY_RETURN) {
    state_ = GameState::kNewStateEntered;
  } else {
    state_ = GameState::kPlaying;
  }

  if (event.getCode() == KeyEvent::KEY_BACKSPACE) {
    mUserState.pop_back();
  } else if ((event.getCode() >= KeyEvent::KEY_a && event.getCode() <= KeyEvent::KEY_z) || event.getCode() == KeyEvent::KEY_SPACE) {
    mUserState = mUserState + event.getChar();
  }
}

void StateApp::ReadInput(std::string& statename) {
  int state_num = FindStateNum(statename);
  int starting_state_num = FindStateNum(start_state);

  if (state_num != -1 && starting_state_num != -1) {
    if (CheckBordering(starting_state_num, state_num)) {
      if (StringCompare(statename, end_state)) {
        state_ = GameState::kGameOver;
      } else {
        start_state = statename;
        mUserState = "";
        state_ = GameState::kPlaying;
      }
    } else {
      state_ = GameState::kInvalidBorder;
      mUserState = "";
    }
  } else {
    state_ = GameState::kInvalidState;
    mUserState = "";
  }

}
int StateApp::FindStateNum(std::string& state) {
  // convert string to upper case
  transform(state.begin(), state.end(), state.begin(), ::toupper);
  int state_num = -1;

  try {
    state_num = json_obj[state];
  } catch (...) {
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

bool StateApp::StringCompare(std::string& str1, std::string str2) {
  //convert s1 and s2 into lower case strings
  transform(str1.begin(), str1.end(), str1.begin(), ::toupper);
  transform(str2.begin(), str2.end(), str2.begin(), ::toupper);

  return str1 == str2;
}

void StateApp::PrintStates(std::string starting, std::string ending) {
  PrintText("Start state:", cinder::Color::white(), {500, 50}, {250, 700});
  PrintText("End state:", cinder::Color::white(), {500, 50}, {900, 700});
  PrintText(starting, cinder::Color::white(), {500, 50}, {250, 750});
  PrintText(ending, cinder::Color::white(), {500, 50}, {900, 750});
  PrintText("Your text:", cinder::Color::white(), {500, 50}, {600, 800});
}

void StateApp::PrintUserState() {
  PrintText(mUserState, cinder::Color::white(), {500, 50}, {600, 850});
}

}  // namespace stateapp
