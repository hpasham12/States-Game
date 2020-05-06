// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_STATEAPP_H_
#define FINALPROJECT_APPS_STATEAPP_H_

#include <cinder/app/App.h>
#include <cinder/gl/Texture.h>

#include <../cmake-build-debug/_deps/nlohmann_json-src/single_include/nlohmann/json.hpp>

namespace stateapp {

enum class GameState {
  kStartState,
  kNewStateEntered,
  kPlaying,
  kInvalidState,
  kInvalidBorder,
  kGameOver,
};

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
  std::string start_state;
  std::string end_state;
  GameState state_;
  std::string mUserState = "";

  void ReadInput(std::string& state);
  int FindStateNum(std::string& basicString);
  bool CheckBordering(int start_num, int state_num);
  void DisplayStates(std::string starting, std::string ending);
  static bool StringCompare(std::string& str1, std::string str2);
  static void PrintStates(std::string starting, std::string ending);
  void PrintUserState();
};

}  // namespace stateapp

#endif  // FINALPROJECT_APPS_STATEAPP_H_
