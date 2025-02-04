#include "Shrink.hpp"

#include "Log.hpp"
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/desktop/Window.hpp>
#include <hyprland/src/managers/AnimationManager.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>

void CShrink::init(HANDLE pHandle, std::string animationName) {
  IFocusAnimation::init(pHandle, "shrink");
  addConfigValue(pHandle, "shrink_percentage", Hyprlang::FLOAT{0.5f});
}

void CShrink::setup(HANDLE pHandle, std::string animationName) {
  // IFocusAnimation::setup(pHandle, animationName);
  // static const auto *shrinkPercentage =
  //     (Hyprlang::FLOAT *const *)(getConfigValue(pHandle, "shrink_percentage")
  //                                    ->getDataStaticPtr());
  // g_fShrinkPercentage = **shrinkPercentage;
  // hyprfocus_log(LOG, "Shrink percentage: {}", g_fShrinkPercentage);
}

void CShrink::onWindowFocus(PHLWINDOW pWindow, HANDLE pHandle) {
  std::string currentAnimStyle =
      pWindow->m_vRealSize->getConfig()->internalStyle;
  hyprfocus_log(LOG, "Current animation style: {}", currentAnimStyle);
  if ((currentAnimStyle == "popout" || currentAnimStyle == "popin") &&
      pWindow->m_vRealSize->isBeingAnimated()) {
    hyprfocus_log(LOG, "Shrink: Window is already being animated, skipping");
    return;
  }

  IFocusAnimation::onWindowFocus(pWindow, pHandle);

  pWindow->m_vRealSize->setConfig(
      Hyprutils::Memory::makeShared<
          Hyprutils::Animation::SAnimationPropertyConfig>(
          m_sFocusOutAnimConfig));
  pWindow->m_vRealPosition->setConfig(
      Hyprutils::Memory::makeShared<
          Hyprutils::Animation::SAnimationPropertyConfig>(
          m_sFocusOutAnimConfig));

  // m_sShrinkAnimation.registerVar();
  //  m_sShrinkAnimation.create(1.0f, &m_sFocusInAnimConfig, AVARDAMAGE_ENTIRE);

  m_sShrinkAnimation.create(
      1, g_pAnimationManager.get(),
      Hyprutils::Memory::makeShared<
          Hyprutils::Animation::CGenericAnimatedVariable<float,
                                                         SAnimationContext>>(),
      1.0f);
  static const auto *shrinkPercentage =
      (Hyprlang::FLOAT *const *)(getConfigValue(pHandle, "shrink_percentage")
                                     ->getDataStaticPtr());
  hyprfocus_log(LOG, "Shrink percentage: {}", **shrinkPercentage);
  m_sShrinkAnimation = **shrinkPercentage;

  m_sShrinkAnimation.setUpdateCallback(
      [this, pWindow](Hyprutils::Memory::CWeakPointer<
                      Hyprutils::Animation::CBaseAnimatedVariable>
                          weakPtr) {
        const auto GOALPOS = pWindow->m_vRealPosition->goal();
        const auto GOALSIZE = pWindow->m_vRealSize->goal();

        auto PANIMATION = weakPtr.lock(); // Convert weakPtr to shared pointer
        if (!PANIMATION)
          return; // Ensure it’s still valid

        auto *animatedVar =
            static_cast<Hyprutils::Animation::CGenericAnimatedVariable<
                float, SAnimationContext> *>(PANIMATION.get());

        pWindow->m_vRealSize->setValue(GOALSIZE * animatedVar->value());
        pWindow->m_vRealPosition->setValue(GOALPOS + GOALSIZE / 2.f -
                                           pWindow->m_vRealSize->value() / 2.f);
      });

  m_sShrinkAnimation.setCallbackOnEnd(
      [this, pWindow](Hyprutils::Memory::CWeakPointer<
                      Hyprutils::Animation::CBaseAnimatedVariable>
                          weakPtr) {
        auto sharedPtr = weakPtr.lock();
        if (!sharedPtr)
          return; // Ensure it is still valid

        sharedPtr->resetAllCallbacks();
      });
}
