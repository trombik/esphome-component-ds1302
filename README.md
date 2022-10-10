# `esphome-component-ds1302`

A `ds1302` component for `esphome`. This is a drop-in replacement of `ds1307`.
The driver was ported from
[esp-idf-lib](https://github.com/UncleRus/esp-idf-lib).

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
[I][wifi:442]: - 'makers' (50:C4:DD:47:2A:37) ▂▄▆█
[D][wifi:444]:     Channel: 9
[D][wifi:445]:     RSSI: -67 dB
[I][wifi:442]: - 'makers' (50:C4:DD:47:2D:B5) ▂▄▆█
[D][wifi:444]:     Channel: 11
[D][wifi:445]:     RSSI: -82 dB
[I][wifi:442]: - 'makers' (18:C2:BF:D2:E7:87) ▂▄▆█
[D][wifi:444]:     Channel: 4
[D][wifi:445]:     RSSI: -89 dB
[D][wifi:447]: - 'Babel Floor 1' (88:DC:96:58:80:40) ▂▄▆█
[D][wifi:447]: - 'Happy Guest House' (80:2A:A8:7B:63:11) ▂▄▆█
[D][wifi:447]: - 'Happy Guest House' (80:2A:A8:AD:81:30) ▂▄▆█
[D][wifi:447]: - 'CCTV ROOM' (90:F6:52:F0:B9:70) ▂▄▆█
[D][wifi:447]: - 'Sweet Dreams 1' (4A:D9:E7:A5:EB:38) ▂▄▆█
[D][wifi:447]: - 'Babel Guesthouse' (3C:84:6A:F8:D4:2E) ▂▄▆█
[D][wifi:447]: - 'Happy Guest House' (80:2A:A8:AD:82:CA) ▂▄▆█
[D][wifi:447]: - 'Happy Guest House' (80:2A:A8:7B:63:2B) ▂▄▆█
[D][wifi:447]: - 'Happy Guest House' (80:2A:A8:7B:5E:04) ▂▄▆█
[D][wifi:447]: - 'KC' (74:4D:28:CF:C9:F2) ▂▄▆█
[D][wifi:447]: - 'ONLINE' (6C:3B:6B:5E:CF:BF) ▂▄▆█
[I][wifi:255]: WiFi Connecting to 'makers'...
[I][wifi:516]: WiFi Connected!
[C][wifi:360]:   Local MAC: 48:3F:DA:0C:B6:8D
[C][wifi:361]:   SSID: 'makers'
[C][wifi:362]:   IP Address: 192.168.99.29
[C][wifi:363]:   BSSID: 50:C4:DD:47:2A:37
[C][wifi:365]:   Hostname: 'lobby-clock-1'
[C][wifi:367]:   Signal strength: -72 dB ▂▄▆█
[C][wifi:371]:   Channel: 9
[C][wifi:372]:   Subnet: 255.255.255.0
[C][wifi:373]:   Gateway: 192.168.99.254
[C][wifi:374]:   DNS1: 192.168.99.251
[C][wifi:375]:   DNS2: 0.0.0.0
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
