// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <cinder/Rand.h>
#include <mylibrary/example.h>

#include <../cmake-build-debug/_deps/nlohmann_json-src/single_include/nlohmann/json.hpp>
#include <catch2/catch.hpp>

#include "../apps/state_app.h"

using json = nlohmann::json;
using stateapp::StateApp;

TEST_CASE("Random sanity test", "[random]") {
  const float random = cinder::randFloat();
  REQUIRE(0. <= random);
  REQUIRE(random <= 1.);
}

TEST_CASE("json deserialization", "[json]") {
  std::ifstream i("C:/Users/hpash/cinder_0.9.2_vc2015/my-projects/final-project-hpasham12/resources/state_info.json");
  json json_obj;
  i >> json_obj;

  SECTION("statesList size") {
    REQUIRE(json_obj["statesList"].size() == 50);
  }
  SECTION("state name") {
    REQUIRE(json_obj["statesList"].at(0)["state"] == "ALABAMA");
  }
  SECTION("state abbreviation") {
    REQUIRE(json_obj["statesList"].at(1)["abbreviation"] == "AK");
  }
  SECTION("state borders") {
    REQUIRE(json_obj["statesList"].at(2)["borders"].size() == 5);
  }
}

TEST_CASE("stateapp method 1", "[state number]") {
  std::ifstream i(
      "C:/Users/hpash/cinder_0.9.2_vc2015/my-projects/final-project-hpasham12/resources/state_info.json");
  json json_obj;
  i >> json_obj;

  SECTION("valid state") {
    std::string state = "VIRGINIA";
    transform(state.begin(), state.end(), state.begin(), ::toupper);
    int state_num = -1;
    try {
      state_num = json_obj[state];
    } catch (...) {
      state_num = -2;
    }

    REQUIRE(state_num == 45);
  }

  SECTION("valid state case insensitive") {
    std::string state = "alabama";
    transform(state.begin(), state.end(), state.begin(), ::toupper);
    int state_num = -1;
    try {
      state_num = json_obj[state];
    } catch (...) {
      state_num = -2;
    }

    REQUIRE(state_num == 0);
  }

  SECTION("valid state w/ space") {
    std::string state = "north carolina";
    transform(state.begin(), state.end(), state.begin(), ::toupper);
    int state_num = -1;
    try {
      state_num = json_obj[state];
    } catch (...) {
      state_num = -2;
    }

    REQUIRE(state_num == 32);
  }

  SECTION("invalid state") {
    std::string state = "alaba";
    transform(state.begin(), state.end(), state.begin(), ::toupper);
    int state_num = -1;
    try {
      state_num = json_obj[state];
    } catch (...) {
      state_num = -2;
    }

    REQUIRE(state_num == -2);
  }
}

TEST_CASE("stateapp method 2", "[check bordering]") {
  std::ifstream file(
      "C:/Users/hpash/cinder_0.9.2_vc2015/my-projects/final-project-hpasham12/resources/state_info.json");
  json json_obj;
  file >> json_obj;

  SECTION("not bordering") {
    bool isBorder;
    int state_num = 1; //alaska
    int start_num = 12; //illinois
    std::vector<std::string> bordering = json_obj["statesList"].at(start_num)["borders"];
    std::string abbreviation = json_obj["statesList"].at(state_num)["abbreviation"];

    for (const auto & i : bordering) {
      if (i == abbreviation) {
        isBorder = true;
      }
    }
    isBorder = false;

    REQUIRE(!isBorder);
  }

  SECTION("bordering") {
    bool isBorder = false;
    int state_num = 2; //arizona
    int start_num = 4; //california
    std::vector<std::string> bordering = json_obj["statesList"].at(start_num)["borders"];
    std::string abbreviation = json_obj["statesList"].at(state_num)["abbreviation"];

    for (const auto & i : bordering) {
      if (i == abbreviation) {
        isBorder = true;
      }
    }

    REQUIRE(isBorder);
  }

  SECTION("bordering") {
    bool isBorder = false;
    int state_num = 47; //arizona
    int start_num = 34; //california
    std::vector<std::string> bordering = json_obj["statesList"].at(start_num)["borders"];
    std::string abbreviation = json_obj["statesList"].at(state_num)["abbreviation"];

    for (const auto & i : bordering) {
      if (i == abbreviation) {
        isBorder = true;
      }
    }

    REQUIRE(isBorder);
  }
}

TEST_CASE("stateapp method 3", "[string compare]") {

  SECTION("same string") {
    bool same = false;
    std::string str1 = "hello";
    std::string str2 = "hello";

    //convert s1 and s2 into lower case strings
    transform(str1.begin(), str1.end(), str1.begin(), ::toupper);
    transform(str2.begin(), str2.end(), str2.begin(), ::toupper);
    if(str1 == str2) {
      same = true;  // The strings are same
    }

    REQUIRE(same);
  }

  SECTION("case insensitive") {
    bool same = false;
    std::string str1 = "hello";
    std::string str2 = "HeLLo";

    //convert s1 and s2 into lower case strings
    transform(str1.begin(), str1.end(), str1.begin(), ::toupper);
    transform(str2.begin(), str2.end(), str2.begin(), ::toupper);
    if(str1 == str2) {
      same = true;  // The strings are same
    }

    REQUIRE(same);
  }

  SECTION("not equal") {
    bool same = false;
    std::string str1 = "hello";
    std::string str2 = "what";

    //convert s1 and s2 into lower case strings
    transform(str1.begin(), str1.end(), str1.begin(), ::toupper);
    transform(str2.begin(), str2.end(), str2.begin(), ::toupper);
    if(str1 == str2) {
      same = true;  // The strings are same
    }

    REQUIRE(!same);
  }

  SECTION("same w/ spaces") {
    bool same = false;
    std::string str1 = "my name is harshita";
    std::string str2 = "MY name IS harshitA";

    //convert s1 and s2 into lower case strings
    transform(str1.begin(), str1.end(), str1.begin(), ::toupper);
    transform(str2.begin(), str2.end(), str2.begin(), ::toupper);
    if(str1 == str2) {
      same = true;  // The strings are same
    }

    REQUIRE(same);
  }
}