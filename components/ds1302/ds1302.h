/*
 * Copyright (c) 2022 Tomoyuki Sakurai <y@trombik.org>
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

#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/time/real_time_clock.h"

/*
 * https://datasheets.maximintegrated.com/en/ds/DS1302.pdf
 *
 * RST - CE, must be high when reading or writing
 *
 * Address/Command Byte (LSB first)
 *
 * | 7         | 6                        | 5  | 4  | 3  | 2  | 1  | 0             |
 * | WRITE (1) | RAM (1) / Clock data (0) | A4 | A3 | A2 | A1 | A0 | R (1) / W (0) |
 */

namespace esphome {
namespace ds1302 {

class DS1302Component : public time::RealTimeClock {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void read_time();
  void write_time();
  void set_cs_pin(InternalGPIOPin *pin) { cs_pin_ = pin; }
  void set_clk_pin(InternalGPIOPin *pin) { clk_pin_ = pin; }
  void set_dio_pin(InternalGPIOPin *pin) { dio_pin_ = pin; }

 protected:
  uint8_t read_byte_();
  void write_byte_(uint8_t byte);
  uint8_t read_register_(uint8_t reg);
  void write_register_(uint8_t reg, uint8_t val);
  void enable_();
  void disable_();
  void toggle_clk_();
  uint8_t bcd2dec_(uint8_t val);
  uint8_t dec2bcd_(uint8_t val);
  void start_(bool start);
  void update_register_(uint8_t reg, uint8_t mask, uint8_t val);
  void set_write_protect_(bool wp);
  void write_rtc_(uint8_t reg, uint8_t *src, uint8_t len);
  void burst_read_(uint8_t reg, uint8_t *dst, uint8_t len);
  bool read_rtc_();

  InternalGPIOPin *cs_pin_;
  InternalGPIOPin *clk_pin_;
  InternalGPIOPin *dio_pin_;
  struct tm *time;
  union DS1302Reg {
    struct {
      uint8_t second : 4;
      uint8_t second_10 : 3;
      bool ch : 1;

      uint8_t minute : 4;
      uint8_t minute_10 : 3;
      uint8_t unused_1 : 1;

      uint8_t hour : 4;
      uint8_t hour_10 : 2;
      uint8_t unused_2 : 2;

      uint8_t day : 4;
      uint8_t day_10 : 2;
      uint8_t unused_4 : 2;

      uint8_t month : 4;
      uint8_t month_10 : 1;
      uint8_t unused_5 : 3;

      uint8_t weekday : 3;
      uint8_t unused_3 : 5;

      uint8_t year : 4;
      uint8_t year_10 : 4;

      uint8_t unused_7 : 7;
      uint8_t wp : 1;

      uint8_t rs : 2;
      uint8_t ds : 2;
      uint8_t tcs : 4;
    } reg;
    mutable uint8_t raw[sizeof(reg)];
  } ds1302_;
};

template<typename... Ts> class WriteAction : public Action<Ts...>, public Parented<DS1302Component> {
 public:
  void play(Ts... x) override { this->parent_->write_time(); }
};

template<typename... Ts> class ReadAction : public Action<Ts...>, public Parented<DS1302Component> {
 public:
  void play(Ts... x) override { this->parent_->read_time(); }
};
}  // namespace ds1302
}  // namespace esphome
