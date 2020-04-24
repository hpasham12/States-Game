// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>

#include "state_app.h"

using cinder::app::App;
using cinder::app::RendererGl;


namespace stateapp {

const int kSamples = 8;
const int kWidth = 985;
const int kHeight = 670;

void SetUp(App::Settings* settings) {
  settings->setWindowSize(kWidth, kHeight);
  settings->setTitle("CS 126 Final Project: State Paths");
}

}  // namespace stateapp


// This is a macro that runs the application.
CINDER_APP(stateapp::StateApp,
           RendererGl(RendererGl::Options().msaa(stateapp::kSamples)),
           stateapp::SetUp)
