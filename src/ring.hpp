#ifndef SRC_RING_HPP
#define SRC_RING_HPP

#include <Arduino.h>

#include "constants.hpp"
#include "array.hpp"

#include <Adafruit_NeoPixel.h>

inline constexpr uint8_t  pulse_per_second = 1;
inline constexpr uint8_t  brightness_max   = 16;

class Ring {
public:
  Ring();

  void init();

  struct color_t {
    byte r;
    byte g;
    byte b;
    constexpr color_t operator*(const uint8_t s) const {
      return color_t { static_cast<byte>(static_cast<uint16_t>(r) * s / 255)
                     , static_cast<byte>(static_cast<uint16_t>(g) * s / 255)
                     , static_cast<byte>(static_cast<uint16_t>(b) * s / 255) };
    }
  };

  static constexpr color_t red   { 255,   0,   0 };
  static constexpr color_t green {   0, 255,   0 };
  static constexpr color_t blue  {   0,   0, 255 };

  void call_on_startup  () { setAll   (red  ); }
  void call_on_idle     () { pulse    (green); }
  void call_on_startPlay() { pulse    (red  ); }
  void call_on_play     () { rainbow  (); }
  void call_on_pause    () { /* simply stop rainbow */ }
  void call_on_admin    () { pulse    (blue ); }

private:

  void showStrip() { strip.show(); }
  void setPixel(int pixel, color_t color) { strip.setPixelColor(pixel, strip.Color(color.r, color.g, color.b)); }

  color_t wheel(byte wheelPos) const;

  void pulse(const color_t color);
  void rainbow();
  void setAll(const color_t color);
  void setAll(auto&& f);

  // for pulse()
  uint8_t brightness { 50 };
  uint8_t  brightness_inc { cycleTime*255/pulse_per_second/1000 };

  // for rainbow()
  unsigned int pixelCycle { 0 };  // Pattern Pixel Cycle

  Adafruit_NeoPixel strip;
};

void Ring::setAll(auto&& f) {
  for (uint8_t i = 0; i < neoPixelNumber; ++i) {
    setPixel(i, f(i));
  }
  showStrip();
}

#endif // SRC_RING_HPP
