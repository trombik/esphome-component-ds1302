# `esphome-component-ds1302`

A `ds1302` component for `esphome`. This is a drop-in replacement of `ds1307`.
The driver was ported from
[esp-idf-lib](https://github.com/UncleRus/esp-idf-lib).

## Usage

See [Time](https://esphome.io/components/time.html) component.

```yaml
# import the component from GitHub
external_components:
  - source:
      type: git
      url: https://github.com/trombik/esphome-component-ds1302
    components:
      - ds1302

esphome:
  name: example-esp8266
  on_boot:
    then:
      ds1302.read_time:

time:
  - platform: ds1302
    id: ds1302_time
    # on some modules, CS is labeled as RST
    cs_pin: D8
    dio_pin: D7
    clk_pin: D5
    update_interval: never

  # a time source is required
  - platform: sntp
    id: sntp_time
    on_time_sync:
      then:
        ds1302.write_time:
```

## Status

* Works on `esp8266` and `esp32`

## Examples

See examples under [config](config).

## Logs

### `esp8266`


```console
[I][logger:243]: Log initialized
[17:59:11][C][ota:465]: There have been 0 suspected unsuccessful boot attempts.
[17:59:11][I][app:029]: Running through setup()...
[17:59:11][C][ds1302:055]: Setting up DS1302...
[17:59:11][D][ds1302:131]: Read  10:58:54 2022-10-11  CH:OFF RS:0 DS:OFF TCS:0
[17:59:11][D][time:041]: Synchronized time: 2022-10-11 17:58:54
[17:59:11][C][display.tm1637:128]: Setting up TM1637...
[17:59:11][C][wifi:037]: Setting up WiFi...

... snipped WiFi logs ...

[17:59:20][C][ota:089]: Over-The-Air Updates:
[17:59:20][C][ota:090]:   Address: example-esp8266.local:8266
[17:59:20][C][ota:093]:   Using Password.
[17:59:20][C][api:025]: Setting up Home Assistant API server...
[17:59:20][C][sntp:025]: Setting up SNTP...
[17:59:20][I][app:062]: setup() finished successfully!
[17:59:20][D][sntp:074]: Synchronized time: 2022-10-11 17:59:02
[17:59:20][D][ds1302:145]: Write 10:59:02 2022-10-11  CH:OFF RS:0 DS:OFF TCS:0
[17:59:20][I][app:102]: ESPHome version 2022.9.1 compiled on Oct 11 2022, 17:50:36
[17:59:20][C][wifi:502]: WiFi:

... snipped WiFi logs ...

[17:59:20][C][logger:275]: Logger:
[17:59:20][C][logger:276]:   Level: DEBUG
[17:59:20][C][logger:277]:   Log Baud Rate: 115200
[17:59:20][C][logger:278]:   Hardware UART: UART0
[17:59:20][C][ds1302:074]: DS1302:
[17:59:20][C][ds1302:075]:   Pin CS: GPIO15
[17:59:20][C][ds1302:076]:   Pin CLK: GPIO14
[17:59:20][C][ds1302:077]:   Pin DIO: GPIO13
[17:59:20][C][ds1302:078]:   Timezone: '<+07>-7'
[17:59:20][C][display.tm1637:138]: TM1637:
[17:59:20][C][display.tm1637:139]:   Intensity: 5
[17:59:20][C][display.tm1637:140]:   Inverted: 0
[17:59:20][C][display.tm1637:141]:   Length: 4
[17:59:20][C][display.tm1637:142]:   CLK Pin: GPIO5
[17:59:20][C][display.tm1637:143]:   DIO Pin: GPIO4
[17:59:20][C][display.tm1637:144]:   Update Interval: 0.5s
[17:59:20][C][mdns:094]: mDNS:
[17:59:20][C][mdns:095]:   Hostname: example-esp8266
[17:59:20][C][ota:089]: Over-The-Air Updates:
[17:59:20][C][ota:090]:   Address: example-esp8266.local:8266
[17:59:20][C][ota:093]:   Using Password.
[17:59:20][C][api:138]: API Server:
[17:59:20][C][api:139]:   Address: example-esp8266.local:6053
[17:59:20][C][api:143]:   Using noise encryption: NO
[17:59:20][C][sntp:050]: SNTP Time:
[17:59:20][C][sntp:051]:   Server 1: '0.pool.ntp.org'
[17:59:20][C][sntp:052]:   Server 2: '1.pool.ntp.org'
[17:59:20][C][sntp:053]:   Server 3: '2.pool.ntp.org'
[17:59:20][C][sntp:054]:   Timezone: '<+07>-7'
[17:59:20][C][debug:049]: Debug component:
[17:59:20][D][debug:061]: ESPHome version 2022.9.1
[17:59:20][D][debug:065]: Free Heap Size: 40008 bytes
[17:59:20][D][debug:093]: Flash Chip: Size=4096kB Speed=40MHz Mode=DOUT
[17:59:20][D][debug:250]: Chip ID: 0x000CB68D
[17:59:20][D][debug:251]: SDK Version: 2.2.2-dev(38a443e)
[17:59:20][D][debug:252]: Core Version: 3.0.2
[17:59:20][D][debug:253]: Boot Version=6 Mode=1
[17:59:20][D][debug:254]: CPU Frequency: 80
[17:59:20][D][debug:255]: Flash Chip ID=0x0016405E
[17:59:20][D][debug:256]: Reset Reason: External System
[17:59:20][D][debug:257]: Reset Info: External System
```

## Notes

With `esphome` version 2022.9.1 as of this writing, `esphome compile` fails
when the MCU is `esp8266`. See a workaround at
[Issue #3348](https://github.com/esphome/issues/issues/3348).
As such, `esphome/components/time/automation.cpp` must be patched. See
[how](https://github.com/trombik/esphome-component-ds1302/blob/main/tox.ini#L26)
in the `tox.ini`.
