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
    # read time from RTC before WiFi starts so that the display shows the time
    # even when WiFi is not available. any value more than 200.0 (before WiFi)
    # should work.
    priority: 600.0
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

* Works on `esp8266`

## Examples

See [config/esp8266.yml](config/esp8266.yml).

## Logs

### `esp8266`

```console
[I][logger:243]: Log initialized
[C][ota:465]: There have been 0 suspected unsuccessful boot attempts.
[I][app:029]: Running through setup()...
[D][ds1302:128]: Read  09:43:12 2022-10-10  CH:OFF RS:0 DS:ON TCS:5
[D][time:041]: Synchronized time: 2022-10-10 16:43:12
[C][ds1302:051]: Setting up DS1302...
[C][display.tm1637:128]: Setting up TM1637...
[C][wifi:037]: Setting up WiFi...
[D][wifi:384]: Starting scan...
[D][wifi:399]: Found networks:

... snipped WiFi logs ...

[C][ota:089]: Over-The-Air Updates:
[C][ota:090]:   Address: lobby-clock-1.local:8266
[C][ota:093]:   Using Password.
[C][api:025]: Setting up Home Assistant API server...
[C][sntp:025]: Setting up SNTP...
[I][app:062]: setup() finished successfully!
[D][sntp:074]: Synchronized time: 2022-10-10 16:43:20
[D][ds1302:142]: Write 09:43:20 2022-10-10  CH:OFF RS:0 DS:ON TCS:5
[I][app:102]: ESPHome version 2022.9.1 compiled on Oct 10 2022, 15:56:45
[C][wifi:502]: WiFi:
[C][wifi:360]:   Local MAC: xx:xx:xx:xx:xx:xx

... snipped WiFi logs ...

[C][logger:275]: Logger:
[C][logger:276]:   Level: DEBUG
[C][logger:277]:   Log Baud Rate: 115200
[C][logger:278]:   Hardware UART: UART0
[C][ds1302:070]: DS1302:
[C][ds1302:071]:   Pin CS: GPIO15
[C][ds1302:072]:   Pin CLK: GPIO14
[C][ds1302:073]:   Pin DIO: GPIO13
[C][ds1302:074]:   Timezone: '<+07>-7'
[C][display.tm1637:138]: TM1637:
[C][display.tm1637:139]:   Intensity: 5
[C][display.tm1637:140]:   Inverted: 0
[C][display.tm1637:141]:   Length: 4
[C][display.tm1637:142]:   CLK Pin: GPIO5
[C][display.tm1637:143]:   DIO Pin: GPIO4
[C][display.tm1637:144]:   Update Interval: 0.5s
[C][mdns:094]: mDNS:
[C][mdns:095]:   Hostname: lobby-clock-1
[C][ota:089]: Over-The-Air Updates:
[C][ota:090]:   Address: lobby-clock-1.local:8266
[C][ota:093]:   Using Password.
[C][api:138]: API Server:
[C][api:139]:   Address: lobby-clock-1.local:6053
[C][api:143]:   Using noise encryption: NO
[C][sntp:050]: SNTP Time:
[C][sntp:051]:   Server 1: '0.pool.ntp.org'
[C][sntp:052]:   Server 2: '1.pool.ntp.org'
[C][sntp:053]:   Server 3: '2.pool.ntp.org'
[C][sntp:054]:   Timezone: '<+07>-7'
[C][debug:049]: Debug component:
[D][debug:061]: ESPHome version 2022.9.1
[D][debug:065]: Free Heap Size: 39672 bytes
[D][debug:093]: Flash Chip: Size=4096kB Speed=40MHz Mode=DOUT
[D][debug:250]: Chip ID: 0x000CB68D
[D][debug:251]: SDK Version: 2.2.2-dev(38a443e)
[D][debug:252]: Core Version: 3.0.2
[D][debug:253]: Boot Version=6 Mode=1
[D][debug:254]: CPU Frequency: 80
[D][debug:255]: Flash Chip ID=0x0016405E
[D][debug:256]: Reset Reason: External System
[D][debug:257]: Reset Info: External System
```

## Notes

With `esphome` version 2022.9.1 as of this writing, `esphome compile` fails.
See a workaround at
[Issue #3348](https://github.com/esphome/issues/issues/3348).
