// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "state_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include <../cmake-build-debug/_deps/nlohmann_json-src/single_include/nlohmann/json.hpp>
#include <cinder/gl/draw.h>
#include <iostream>
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
//from https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
const std::string WHITESPACE = " \n\r\t\f\v";

StateApp::StateApp() {
  std::ifstream file("C:/Users/hpash/cinder_0.9.2_vc2015/my-projects/final-project-hpasham12/resources/state_info.json");
  file >> json_obj;

  std::vector<std::string> initial_states = FindStates();
  start_state = initial_states.at(0);
  end_state = initial_states.at(1);
  state_ = GameState::kStartState;
}

void StateApp::setup() {
  cinder::gl::enableDepthWrite();
  cinder::gl::enableDepthRead();

  auto img = loadImage( loadAsset( "map.png" ) );
  mTex = cinder::gl::Texture2d::create( img );
}


void StateApp::update() {
  if (state_ == GameState::kNewStateEntered) {
    user_state = TrimString(user_state);
    if (CheckRestart(user_state)) {
      RestartGame();
      state_ = GameState::kPlaying;
      user_state.clear();
    } else {
      ReadInput(user_state);
    }
  }
}

void StateApp::draw() {
  cinder::gl::enableAlphaBlending();
  cinder::gl::clear();
  cinder::gl::draw( mTex );
  PrintStates(start_state, end_state);

  if (state_ == GameState::kPlaying) {
    PrintUserState();
  }
  if (state_ == GameState::kInvalidState) {
    PrintText("Yikes! That's not a state. Maybe check your spelling? Try again!", cinder::Color(1, 0.5, 0.5), {400, 50}, {550, 850});
  }
  if (state_ == GameState::kInvalidBorder) {
    PrintText("That state isn't bordering the current one you're at. Try again!", cinder::Color(.68, 0.85, 0.9), {400, 50}, {550, 850});
  }
  if (state_ == GameState::kGameOver) {
    PrintText("WOOHOO YOU MADE IT!! NICE JOB!!", cinder::Color(.8, 0.8, 1), {500, 50}, {600, 850});
  }
}

void StateApp::keyDown(KeyEvent event) {
  if (event.getCode() == KeyEvent::KEY_RETURN) {
    state_ = GameState::kNewStateEntered;
  } else {
    state_ = GameState::kPlaying;
  }
  if (event.getCode() == KeyEvent::KEY_BACKSPACE) {
    if (!user_state.empty()) {
      user_state.pop_back();
    }
  } else if ((event.getCode() >= KeyEvent::KEY_a && event.getCode() <= KeyEvent::KEY_z) || event.getCode() == KeyEvent::KEY_SPACE) {
    user_state += event.getChar();
  }
}

//From snake lab
template <typename C>
void StateApp::PrintText(const std::string& text, const C& color, const cinder::ivec2& size,
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

void StateApp::ReadInput(std::string& state_name) {
  int state_num = FindStateNum(state_name);
  int starting_state_num = FindStateNum(start_state);

  if (state_num != -1 && starting_state_num != -1) {
    if (CheckBordering(starting_state_num, state_num)) {
      if (StringCompare(state_name, end_state)) {
        state_ = GameState::kGameOver;
        user_state.clear();
      } else {
        start_state = state_name;
        state_ = GameState::kPlaying;
        user_state.clear();
      }
    } else {
      state_ = GameState::kInvalidBorder;
      user_state.clear();
    }
  } else {
    state_ = GameState::kInvalidState;
    user_state.clear();
  }
}

int StateApp::FindStateNum(std::string& state) {
  transform(state.begin(), state.end(), state.begin(), ::toupper);
  int state_num;

  try {
    state_num = json_obj[state];
  } catch (...) {
    state_num = -1;
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
  transform(str1.begin(), str1.end(), str1.begin(), ::toupper);
  transform(str2.begin(), str2.end(), str2.begin(), ::toupper);

  return str1 == str2;
}

void StateApp::PrintStates(const std::string& starting, const std::string& ending) {
  PrintText("Current state:", cinder::Color::white(), {500, 50}, {250, 700});
  PrintText(starting, cinder::Color::white(), {500, 50}, {250, 750});
  PrintText("End state:", cinder::Color::white(), {500, 50}, {900, 700});
  PrintText(ending, cinder::Color::white(), {500, 50}, {900, 750});
  PrintText("Your text:", cinder::Color::white(), {500, 50}, {600, 800});
  //Instructions
  PrintText("Type in the name of a state that borders the current one and press the ENTER key. Try to get to the ending state!", cinder::Color::white(), {875, 50}, {450, 950});
  PrintText("Type 'RESET' or 'START OVER' to begin the game again", cinder::Color::white(), {900, 50}, {463, 1025});
}

void StateApp::PrintUserState() {
  PrintText(user_state, cinder::Color::white(), {500, 50}, {600, 850});
}

//trim whitespace from https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
std::string StateApp::TrimString(std::string& to_trim) {
  return rtrim(ltrim(to_trim));
}

//trim whitespace from https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
std::string StateApp::ltrim(const std::string& to_trim) {
  size_t start = to_trim.find_first_not_of(WHITESPACE);

  return (start == std::string::npos) ? "" : to_trim.substr(start);
}

//trim whitespace from https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
std::string StateApp::rtrim(const std::string& to_trim) {
  size_t end = to_trim.find_last_not_of(WHITESPACE);

  return (end == std::string::npos) ? "" : to_trim.substr(0, end + 1);
}

bool StateApp::CheckRestart(std::string input) {
  return StringCompare(input, "reset") || StringCompare(input, "start over");
}

void StateApp::RestartGame() {
  std::vector<std::string> initial_states = FindStates();
  start_state = initial_states.at(0);
  end_state = initial_states.at(1);
}

std::vector<std::string> StateApp::FindStates() {
  std::vector<std::string> states;
  int start_number = 1; //alaska
  int end_number = 10; //hawaii
  srand((unsigned) time(0));

  while (start_number == 1 || start_number == 10 || end_number == 1||end_number == 10 || start_number == end_number) {
    start_number = rand() % 50; // NOLINT(cert-msc30-c, cert-msc50-cpp)
    end_number = rand() % 50; // NOLINT(cert-msc30-c, cert-msc50-cpp)
  }
  states.push_back(json_obj["statesList"].at(start_number)["state"]);
  states.push_back(json_obj["statesList"].at(end_number)["state"]);

  return states;
}

}  // namespace stateapp