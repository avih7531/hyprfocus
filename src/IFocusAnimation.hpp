#pragma once

#include <hyprlang.hpp>
#include <hyprutils/animation/AnimationConfig.hpp>
#define WLR_USE_UNSTABLE

#include <hyprland/src/config/ConfigManager.hpp>

using Hyprutils::Animation::SAnimationPropertyConfig;

class IFocusAnimation {
public:
  virtual void onWindowFocus(PHLWINDOW pWindow, HANDLE pHandle);
  virtual void init(HANDLE pHandle, std::string animationName);
  virtual void setup(HANDLE pHandle, std::string animationName);

  void addConfigValue(HANDLE pHandle, std::string name,
                      Hyprlang::CConfigValue sValue);
  Hyprlang::CConfigValue *getConfigValue(HANDLE pHandle, std::string name);

public:
  SAnimationPropertyConfig m_sFocusInAnimConfig;
  SAnimationPropertyConfig m_sFocusOutAnimConfig;

  std::string m_szAnimationName;

  std::string configPrefix() {
    return std::string("plugin:hyprfocus:") + m_szAnimationName + ":";
  }
};
