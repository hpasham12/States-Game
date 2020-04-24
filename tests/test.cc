// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <cinder/Rand.h>
#include <../cmake-build-debug/_deps/nlohmann_json-src/single_include/nlohmann/json.hpp>

#include <mylibrary/example.h>

using json = nlohmann::json;


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