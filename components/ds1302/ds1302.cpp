/*
 * Copyright (c) 2022 Tomoyuki Sakurai <y@trombik.org>
 * Copyright (c) 2016 Ruslan V. Uss <unclerus@gmail.com>
 * Copyright (c) 2016 Pavel Merzlyakov <merzlyakovpavel@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of itscontributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* ported from:
 * https://github.com/UncleRus/esp-idf-lib/tree/master/components/ds1302
 */

#include "ds1302.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

#define DS1302_CH_REG 0x80
#define DS1302_WP_REG 0x8e

#define DS1302_CH_BIT (1 << 7)
#define DS1302_WP_BIT (1 << 7)

#define DS1302_CH_MASK ((uint8_t)(~DS1302_CH_BIT))
#define DS1302_WP_MASK ((uint8_t)(~DS1302_WP_BIT))

#define DS1302_CLOCK_BURST 0xbe

namespace esphome {
namespace ds1302 {

static const char *const TAG = "ds1302";

void DS1302Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up DS1302...");
  if (cs_pin_ == nullptr || dio_pin_ == nullptr || clk_pin_ == nullptr) {
    this->mark_failed();
    return;
  }
  this->cs_pin_ = cs_pin_;
  this->clk_pin_ = clk_pin_;
  this->dio_pin_ = dio_pin_;

  this->cs_pin_->pin_mode(gpio::FLAG_OUTPUT);
  this->clk_pin_->pin_mode(gpio::FLAG_OUTPUT);
  this->dio_pin_->pin_mode(gpio::FLAG_OUTPUT);
  this->set_write_protect_(false);
  this->start_(true);
}

void DS1302Component::update() { this->read_time(); }

void DS1302Component::dump_config() {
  ESP_LOGCONFIG(TAG, "DS1302:");
  LOG_PIN("  Pin CS: ", this->cs_pin_);
  LOG_PIN("  Pin CLK: ", this->clk_pin_);
  LOG_PIN("  Pin DIO: ", this->dio_pin_);
  ESP_LOGCONFIG(TAG, "  Timezone: '%s'", this->timezone_.c_str());
}

float DS1302Component::get_setup_priority() const { return setup_priority::DATA; }

void DS1302Component::read_time() {
  this->read_rtc_();
  if (ds1302_.reg.ch) {
    ESP_LOGW(TAG, "RTC halted, not syncing to system clock.");
    return;
  }
  time::ESPTime rtc_time{.second = uint8_t(ds1302_.reg.second + 10 * ds1302_.reg.second_10),
                         .minute = uint8_t(ds1302_.reg.minute + 10u * ds1302_.reg.minute_10),
                         .hour = uint8_t(ds1302_.reg.hour + 10u * ds1302_.reg.hour_10),
                         .day_of_week = uint8_t(ds1302_.reg.weekday),
                         .day_of_month = uint8_t(ds1302_.reg.day + 10u * ds1302_.reg.day_10),
                         .day_of_year = 1,  // ignored by recalc_timestamp_utc(false)
                         .month = uint8_t(ds1302_.reg.month + 10u * ds1302_.reg.month_10),
                         .year = uint16_t(ds1302_.reg.year + 10u * ds1302_.reg.year_10 + 2000)};
  rtc_time.recalc_timestamp_utc(false);
  if (!rtc_time.is_valid()) {
    ESP_LOGE(TAG, "Invalid RTC time, not syncing to system clock.");
    return;
  }
  time::RealTimeClock::synchronize_epoch_(rtc_time.timestamp);
}

void DS1302Component::write_time() {
  auto now = time::RealTimeClock::utcnow();
  if (!now.is_valid()) {
    ESP_LOGE(TAG, "Invalid system time, not syncing to RTC.");
    return;
  }
  ds1302_.reg.year = (now.year - 2000) % 10;
  ds1302_.reg.year_10 = (now.year - 2000) / 10 % 10;
  ds1302_.reg.month = now.month % 10;
  ds1302_.reg.month_10 = now.month / 10;
  ds1302_.reg.day = now.day_of_month % 10;
  ds1302_.reg.day_10 = now.day_of_month / 10;
  ds1302_.reg.weekday = now.day_of_week;
  ds1302_.reg.hour = now.hour % 10;
  ds1302_.reg.hour_10 = now.hour / 10;
  ds1302_.reg.minute = now.minute % 10;
  ds1302_.reg.minute_10 = now.minute / 10;
  ds1302_.reg.second = now.second % 10;
  ds1302_.reg.second_10 = now.second / 10;
  ds1302_.reg.ch = false;

  this->write_rtc_(DS1302_CLOCK_BURST, ds1302_.raw, sizeof(ds1302_.raw));
}

bool DS1302Component::read_rtc_() {
  this->burst_read_(DS1302_CLOCK_BURST, ds1302_.raw, sizeof(ds1302_.raw));
  ESP_LOGD(TAG, "Read  %0u%0u:%0u%0u:%0u%0u 20%0u%0u-%0u%0u-%0u%0u  CH:%s RS:%0u DS:%s TCS:%0u", ds1302_.reg.hour_10,
           ds1302_.reg.hour, ds1302_.reg.minute_10, ds1302_.reg.minute, ds1302_.reg.second_10, ds1302_.reg.second,
           ds1302_.reg.year_10, ds1302_.reg.year, ds1302_.reg.month_10, ds1302_.reg.month, ds1302_.reg.day_10,
           ds1302_.reg.day, ONOFF(ds1302_.reg.ch), ds1302_.reg.rs, ONOFF(ds1302_.reg.ds), ds1302_.reg.tcs);

  return true;
}

void DS1302Component::write_rtc_(uint8_t reg, uint8_t *src, uint8_t len) {
  this->enable_();
  this->write_byte_(reg);
  for (uint8_t i = 0; i < len; i++, src++) {
    this->write_byte_(*src);
  }
  ESP_LOGD(TAG, "Write %0u%0u:%0u%0u:%0u%0u 20%0u%0u-%0u%0u-%0u%0u  CH:%s RS:%0u DS:%s TCS:%0u", ds1302_.reg.hour_10,
           ds1302_.reg.hour, ds1302_.reg.minute_10, ds1302_.reg.minute, ds1302_.reg.second_10, ds1302_.reg.second,
           ds1302_.reg.year_10, ds1302_.reg.year, ds1302_.reg.month_10, ds1302_.reg.month, ds1302_.reg.day_10,
           ds1302_.reg.day, ONOFF(ds1302_.reg.ch), ds1302_.reg.rs, ONOFF(ds1302_.reg.ds), ds1302_.reg.tcs);
  this->disable_();
}

uint8_t DS1302Component::read_byte_() {
  uint8_t byte = 0;
  this->dio_pin_->pin_mode(gpio::FLAG_INPUT);
  for (uint8_t i = 0; i < 8; i++) {
    bool val = this->dio_pin_->digital_read();
    byte |= (val << i);
    this->toggle_clk_();
  }
  return byte;
}

void DS1302Component::write_byte_(uint8_t byte) {
  this->dio_pin_->pin_mode(gpio::FLAG_OUTPUT);
  for (uint8_t i = 0; i < 8; i++) {
    this->dio_pin_->digital_write((byte >> i) & 1);
    this->toggle_clk_();
  }
}

void DS1302Component::start_(bool start) {
  this->update_register_(DS1302_CH_REG, DS1302_CH_MASK, start ? 0 : DS1302_CH_BIT);
}

void DS1302Component::enable_() {
  this->cs_pin_->digital_write(false);
  this->cs_pin_->digital_write(true);
  delayMicroseconds(4);
}

void DS1302Component::disable_() {
  this->cs_pin_->digital_write(true);
  this->cs_pin_->digital_write(false);
}

void DS1302Component::toggle_clk_() {
  this->clk_pin_->digital_write(true);
  delayMicroseconds(1);
  this->clk_pin_->digital_write(false);
  delayMicroseconds(1);
}

uint8_t bcd2dec_(uint8_t val) { return (val >> 4) * 10 + (val & 0x0f); }

uint8_t dec2bcd_(uint8_t val) { return ((val / 10) << 4) + (val % 10); }

uint8_t DS1302Component::read_register_(uint8_t reg) {
  uint8_t val = 0;
  this->enable_();
  this->write_byte_(reg | 0x01);
  val = this->read_byte_();
  this->disable_();
  return val;
}

void DS1302Component::write_register_(uint8_t reg, uint8_t val) {
  this->enable_();
  this->write_byte_(reg);
  this->write_byte_(val);
  this->disable_();
}

void DS1302Component::update_register_(uint8_t reg, uint8_t mask, uint8_t val) {
  uint8_t r;
  r = this->read_register_(reg);
  this->write_register_(reg, ((r & mask) | val));
}

void DS1302Component::set_write_protect_(bool wp) {
  this->update_register_(DS1302_WP_REG, DS1302_WP_MASK, wp ? DS1302_WP_BIT : 0);
}

void DS1302Component::burst_read_(uint8_t reg, uint8_t *dst, uint8_t len) {
  this->enable_();
  this->write_byte_(reg | 0x01);
  for (uint8_t i = 0; i < len; i++, dst++) {
    *dst = this->read_byte_();
  }
  this->disable_();
}

uint8_t DS1302Component::bcd2dec_(uint8_t val) { return (val >> 4) * 10 + (val & 0x0f); }

uint8_t DS1302Component::dec2bcd_(uint8_t val) { return ((val / 10) << 4) + (val % 10); }

}  // namespace ds1302
}  // namespace esphome
