# iDom Server OOP – Dokumentacja

> Serwer automatyki domowej dla Raspberry Pi  
> Język: C++20 | System budowania: CMake | Wynikowy plik: `iDom_server-CMAKE`

---

## Spis treści

1. [Opis projektu](#1-opis-projektu)
2. [Wymagania i zależności](#2-wymagania-i-zależności)
3. [Budowanie projektu](#3-budowanie-projektu)
4. [Konfiguracja](#4-konfiguracja)
5. [Uruchamianie](#5-uruchamianie)
6. [Architektura](#6-architektura)
7. [Struktura katalogów](#7-struktura-katalogów)
8. [Wątki i ich zadania](#8-wątki-i-ich-zadania)
9. [Protokoły komunikacyjne](#9-protokoły-komunikacyjne)
10. [API – polecenia TCP](#10-api--polecenia-tcp)
11. [Integrowane systemy](#11-integrowane-systemy)
12. [Baza danych InfluxDB](#12-baza-danych-influxdb)
13. [Wzorce projektowe](#13-wzorce-projektowe)
14. [Biblioteki pomocnicze (libs/)](#14-biblioteki-pomocnicze-libs)
15. [Pliki robocze i logi](#15-pliki-robocze-i-logi)
16. [Stany programu i graceful shutdown](#16-stany-programu-i-graceful-shutdown)

---

## 1. Opis projektu

**iDom** (inteligentny Dom) to wielowątkowy serwer automatyki domowej pisany w C++20, działający na Raspberry Pi jako centralny punkt sterowania inteligentnym domem.

Główne funkcje:
- Serwer TCP z autoryzacją i szyfrowaniem dla klientów mobilnych/zewnętrznych
- Obsługa oświetlenia smart (żarówki Zigbee/Shelly) przez MQTT
- Integracja z centralą alarmową **Satel Integra 32**
- Integracja z kotłem grzewczym **Buderus** (przez EMS-ESP/MQTT)
- Sterowanie **rekuperatorem** (wentylacja mechaniczna) przez MQTT
- Odbiór i nadawanie sygnałów **433MHz** (pilot, czujniki) przez RFLink
- Sterowanie odtwarzaczem muzycznym **MPD**
- Harmonogramowanie zadań **CRON** (plik YAML)
- Wyświetlacz **LCD 16×2 I2C** z menu
- Komunikacja z **Arduino** przez RS-232
- Notyfikacje push przez **Viber API**
- Zapis danych historycznych do **InfluxDB** i **ThingSpeak**

---

## 2. Wymagania i zależności

### System
- Raspberry Pi z systemem Linux (Raspberry Pi OS / Debian)
- Zainstalowany i zamontowany ramdysk tmpfs na `/mnt/ramdisk`
- Plik konfiguracyjny w `/etc/config/iDom_SERVER/iDom_serverConfig.json`

### Biblioteki systemowe (do zainstalowania)

```bash
sudo apt-get install \
    libcurl4-openssl-dev \
    libmpdclient-dev \
    libmosquitto-dev \
    libi2c-dev \
    cmake \
    git
```

| Biblioteka | Zastosowanie |
|---|---|
| `libcurl` | HTTP (ThingSpeak, InfluxDB, Viber API, ip-api.com) |
| `libmpd` (libmpdclient) | Klient Music Player Daemon |
| `libmosquitto` | Klient MQTT 3.1/3.1.1 |
| `libi2c` | Komunikacja I2C (LCD, czujniki) |
| `libpthread` | Wątki POSIX |
| `librt` | POSIX real-time (zegary) |

### Biblioteki dołączone w projekcie

| Biblioteka | Katalog | Zastosowanie |
|---|---|---|
| nlohmann/json | `json2/` | Parsowanie JSON |
| LiquidCrystal I2C | `liquidcrystal_i2c-for-ardupi/` | Sterownik LCD |
| ftplibpp | `ftplibpp/` | Upload przez FTP |
| yaml.hpp | `libs/yaml/` | Parser YAML (CRON) |

---

## 3. Budowanie projektu

Projekt używa CMake z katalogiem budowania in-source (budowanie odbywa się w katalogu projektu).

```bash
# Konfiguracja (jeśli jeszcze nie wykonana)
cd /home/pi/programowanie/malina/iDom_server_OOP
cmake .

# Kompilacja (automatycznie wykrywa liczbę rdzeni)
make -j$(nproc)

# Wynikowy plik wykonywalny:
# ./iDom_server-CMAKE
```

### Flagi kompilatora
- `-O3` – maksymalna optymalizacja
- `-DNOSSL` – wyłączenie SSL
- Standard: **C++20**
- W binarce embed-owane są: hash commita git i nazwa brancha (`GIT_COMMIT_HASH`, `GIT_BRANCH`)

---

## 4. Konfiguracja

Główny plik konfiguracyjny: `/etc/config/iDom_SERVER/iDom_serverConfig.json`

### Przykładowa struktura JSON

```json
{
  "server": {
    "PORT": 12345,
    "SERVER_IP": "0.0.0.0",
    "MPD_IP": "127.0.0.1",
    "ID_server": 1,
    "TS_KEY": "THINGSPEAK_KEY",
    "TS_KEY2": "THINGSPEAK_KEY2",
    "radio433MHzConfigFile": "/etc/config/iDom_SERVER/radio433.json",
    "saveFilePath": "/mnt/ramdisk/iDom_state.json",
    "keyDatabasePath": "/etc/config/iDom_SERVER/keys.json",
    "encrypted": true,
    "viber_notification": true,
    "lightningApiURL": "https://api.example.com/lightning"
  },
  "runThread": {
    "RFLink": true,
    "MPD": true,
    "CRON": true,
    "RS232": true,
    "DUMMY": false,
    "MQTT": true,
    "SATEL": true,
    "INFLUX": true,
    "threadPoolSize": 14
  },
  "rs232": {
    "portRS232": "/dev/ttyAMA0",
    "BaudRate": 115200
  },
  "rflink": {
    "RFLinkPort": "/dev/ttyUSB0",
    "RFLinkBaudRate": 57600
  },
  "mqtt_broker": {
    "host": "localhost",
    "topicSubscribe": "iDom",
    "topicPublish": "iDom",
    "port": 1883,
    "qos": 2
  },
  "satel_integra": {
    "host": "192.168.1.100",
    "port": 7094,
    "pin": "1234",
    "outdoor_siren_lights_id": 5,
    "partitionID": 1
  },
  "database": {
    "ip": "127.0.0.1",
    "port": 8086,
    "token": "INFLUX_TOKEN",
    "org": "home",
    "bucket": "iDom"
  },
  "recuperation": {
    "MQTT_CONTROL_TOPIC": "home/recuperator/control",
    "MQTT_SENSOR_TOPIC": "home/recuperator/sensor"
  },
  "fb_viber": {
    "viberToken": "VIBER_TOKEN",
    "viberSender": "iDomServer",
    "viberReceiver": ["USER_ID_1"],
    "viberAvatar": "https://example.com/avatar.jpg"
  }
}
```

### Dodatkowe pliki konfiguracyjne

| Plik | Zastosowanie |
|---|---|
| `/etc/config/iDom_SERVER/bulb_config.json` | Konfiguracja żarówek smart i pokoi |
| `/etc/config/iDom_SERVER/button_config.json` | Mapowanie przycisków 433MHz na komendy |
| `/etc/config/iDom_SERVER/radio433.json` | Konfiguracja urządzeń 433MHz (RFLink) |
| Plik YAML (w konfiguracji serwera) | Harmonogram CRON |

---

## 5. Uruchamianie

```bash
# Uruchomienie bezpośrednie
./iDom_server-CMAKE

# Uruchomienie jako usługa systemd (zalecane)
sudo systemctl start iDom
sudo systemctl enable iDom  # autostart po restarcie
```

### Wymagania przed uruchomieniem
```bash
# Ramdysk musi być zamontowany
sudo mount -t tmpfs -o size=64M tmpfs /mnt/ramdisk

# Plik konfiguracyjny musi istnieć
ls /etc/config/iDom_SERVER/iDom_serverConfig.json
```

---

## 6. Architektura

### Przepływ inicjalizacji

```
main()
  └─ iDom_main()
       ├── catchSigInt()               SIGTERM → graceful shutdown
       │                               SIGSEGV/SIGABRT → backtrace do logu
       ├── Zapis PID → /mnt/ramdisk/pid-iDom.txt
       ├── Odczyt konfiguracji JSON
       ├── Tworzenie thread_context    centralny agregator stanu
       ├── ThreadPool (domyślnie 14 wątków, tryb Blocking)
       ├── Odczyt zapisanego stanu (saveFilePath)
       ├── Inicjalizacja MQTT, LCD, Menu, iDomTools
       ├── Inicjalizacja Buderus, house_room_handler, Recuperator
       ├── Uruchamianie opcjonalnych wątków (RFLink, RS232, MQTT,
       │   CRON, InfluxDB, Satel, MPD, node)
       ├── Notyfikacja Viber: "iDom server wystartował"
       ├── Przywracanie stanu z pliku
       ├── MQTT publish: odpytanie żarówek o stan
       └── useful_F::startServer()     ← blokująca pętla TCP
```

### Centralny obiekt: `thread_context`

Przekazywany wskaźnikiem do wszystkich wątków i handlerów. Zawiera:

| Pole | Typ | Opis |
|---|---|---|
| `server_settings` | `CONFIG_JSON*` | Wskaźnik na konfigurację |
| `m_threadPool` | `unique_ptr<ThreadPool>` | Pula wątków |
| `mqttHandler` | `unique_ptr<MQTT_mosquitto>` | Klient MQTT |
| `main_house_room_handler` | `shared_ptr<house_room_handler>` | Żarówki i pokoje |
| `main_iDomTools` | `shared_ptr<iDomTOOLS_INTERFACE>` | Fasada narzędziowa |
| `main_Rs232` | `unique_ptr<SerialPi>` | Komunikacja RS-232 |
| `main_RFLink` | `shared_ptr<RFLinkHandler>` | Moduł RFLink 433MHz |
| `main_REC` | `unique_ptr<RADIO_EQ_CONTAINER>` | Konfiguracja urządzeń 433MHz |
| `ptr_buderus` | `shared_ptr<BUDERUS_IF>` | Kocioł grzewczy Buderus |
| `satelIntegraHandler` | `SATEL_INTEGRA_HANDLER_INTERFACE*` | Centrala alarmowa Satel |
| `m_recuperator` | `unique_ptr<RecuperatorInterface>` | Rekuperator |
| `main_iDomStatus` | `unique_ptr<iDomSTATUS>` | Rejestr stanów obiektów |
| `main_key_menu_handler` | `unique_ptr<KEY_HANDLER_BASE>` | Menu LCD |
| `m_keyHandler` | `unique_ptr<iDomKEY_ACCESS>` | Zarządzanie kluczami dostępu |
| `ptr_MPD_info` | `MPD_info*` | Aktualny stan MPD |
| `myEventHandler` | `event_counters_handler` | Liczniki zdarzeń |
| `iDomAlarm` | `Alarm` | System alarmów wewnętrznych |
| `iDomProgramState` | `iDomStateEnum` | Stan programu |
| `lusina` | `LUSINA` | Dane z szopy (shed) |
| `start` | `time_t` | Czas startu serwera |

---

## 7. Struktura katalogów

```
iDom_server_OOP/
├── src/
│   ├── main.cpp                        Punkt wejścia
│   ├── iDom_server_OOP.h/.cpp          Główna logika, wątki, struktury
│   ├── idom_api.h                      Interfejs rejestru API (wzorzec registry)
│   │
│   ├── command/                        Wzorzec Command – API serwera TCP
│   │   ├── command.h/.cpp              Klasa bazowa polecenia
│   │   ├── commandhandler.cpp          Handler domyślny (użytkownicy)
│   │   ├── commandhandlerroot.cpp      Handler ROOT (pełny dostęp)
│   │   ├── commandhandlergateway.cpp   Handler GATEWAY (bramy)
│   │   ├── commandhandlermqtt.cpp      Handler wiadomości MQTT
│   │   └── commandClass/              Implementacje konkretnych poleceń (~25 klas)
│   │
│   ├── HOUSE/                          Smart lighting – żarówki i pokoje
│   ├── CRON/                           Harmonogramowanie zadań
│   ├── TASKER/                         Routing wiadomości MQTT
│   ├── SATEL_INTEGRA/                  Centrala alarmowa Satel Integra 32
│   ├── buderus/                        Kocioł grzewczy Buderus
│   ├── recuperator/                    Rekuperator (wentylacja mechaniczna)
│   ├── 433MHz/                         Protokół RFLink 433MHz
│   ├── RADIO_433_eq/                   Konfiguracja urządzeń 433MHz
│   ├── MENU/                           Menu na wyświetlaczu LCD
│   ├── LCD_c/                          Sterownik LCD I2C
│   ├── SerialPi/                       Komunikacja RS-232
│   ├── c_connection/                   Obsługa połączeń TCP (szyfrowanie, autoryzacja)
│   ├── functions/                      Funkcje pomocnicze (startServer, konfiguracja)
│   ├── HTTP/                           Parser HTTP
│   ├── iDomTools/                      Fasada narzędziowa (MPD, sensory, notyfikacje)
│   ├── iDomStatus/                     Rejestr stanów obiektów
│   ├── iDomKEY_ACCESS/                 Zarządzanie kluczami dostępu
│   ├── iDomSaveState/                  Persystencja stanu (JSON)
│   ├── THERMOMETER_CONTAINER/          Historia temperatur
│   ├── dbClient/                       Klient bazy InfluxDB (HTTP API)
│   ├── logger/                         Thread-safe logger
│   └── blockQueue/                     Kolejka blokująca
│
├── libs/                               Biblioteki pomocnicze (osobne repo git)
│   ├── ThreadPool/                     Pula wątków
│   ├── alarm/                          Alarmy wewnętrzne (liczniki błędów)
│   ├── circular_buffer/                Bufor cykliczny
│   ├── config_parser/                  Parser konfiguracji
│   ├── emoji/                          Emoji w odpowiedziach i logach
│   ├── event_counters/                 Liczniki zdarzeń
│   ├── generator/                      Generator losowych ciągów
│   ├── sunrise-sunset/                 Obliczanie wschodu/zachodu słońca
│   ├── useful/                         Narzędzia (httpPost, ipCountry, ...)
│   ├── viberAPI/                       Klient Viber API
│   ├── Statistic/                      Statystyki numeryczne (min/max/avg)
│   ├── backTrace/                      Handler SIGSEGV/SIGABRT z backtrace
│   └── yaml/                           Parser YAML (CRON)
│
├── mqtt_mosquitto/                     Wrapper C++ nad libmosquitto
├── liquidcrystal_i2c-for-ardupi/       Sterownik LCD I2C (port z Arduino)
├── json2/                              nlohmann/json
├── ftplibpp/                           Biblioteka FTP
├── docker/                             Pliki Docker (deploy)
└── CMakeLists.txt                      System budowania
```

---

## 8. Wątki i ich zadania

Wszystkie wątki działają w `ThreadPool`. Uruchamianie każdego wątku jest kontrolowane flagami w sekcji `runThread` konfiguracji JSON.

### Pętla główna TCP (wątek główny)
Funkcja: `useful_F::startServer()`  
- Tworzy non-blocking socket TCP
- W każdej iteracji wywołuje `TASKER::runTasker()` (obsługa kolejki MQTT)
- Przyjmuje przychodzące połączenia TCP i deleguje je do ThreadPool
- Opóźnienie: 10ms gdy są wiadomości MQTT, 256ms gdy kolejka pusta

### Wątek obsługi połączenia TCP
Funkcja: `useful_F::Server_connectivity_thread()`  
- Tworzony dynamicznie dla każdego klienta TCP
- Autoryzacja → rozpoznanie poziomu dostępu → pętla recv/analyze/send

### Wątek RFLink (433MHz)
Funkcja: `RFLinkHandlerRUN()`  
- Inicjalizacja modułu RFLink przez RS-232
- Pętla odbierania ramek 433MHz → mapowanie na komendy
- Pingowanie modułu (`10;PING;`) przy starcie

### Wątek RS-232 (Arduino)
Funkcja: `Send_Recieve_rs232_thread()`  
- Dwukierunkowa komunikacja z Arduino
- Sterowanie LED: `LED_POWER`, `LED_AT`, `LED_CLOCK`

### Wątek MQTT Subscriber
Funkcja: `f_master_mqtt()`  
- Połączenie z brokerem Mosquitto
- Subskrypcja tematów i wypełnianie kolejki wiadomości

### Wątek CRON
Funkcja: `f_master_CRON()`  
- Parsowanie harmonogramu z pliku YAML
- Wywoływanie akcji w interwałach: co 1min, 5min, 15min, 30min, 1h
- Akcje o wschodzie i zachodzie słońca

### Wątek InfluxDB
Funkcja: `f_master_influx()`  
- Konsumuje kolejkę blokującą `m_bulbStatus`
- Wysyła stany żarówek do InfluxDB przez HTTP API
- Retry z TTL=100 przy błędach; sleep 1min przy braku internetu

### Wątek Satel Integra 32
Funkcja: `f_satelIntegra32()`  
- Cykliczne wywołania `checkSatel()` i `checkAlarm()`
- Monitorowanie stref i wyjść centrali alarmowej

### Wątek MPD
Funkcja: `main_mpd_cli()`  
- Pobieranie aktualnego stanu MPD (tytuł, radio, głośność)
- Aktualizacja `ptr_MPD_info`

---

## 9. Protokoły komunikacyjne

### TCP – własny protokół iDom

Port: konfigurowalny (`server.PORT`)

```
Klient                          Serwer
  |                               |
  |-- [connect] ----------------> |
  |                               |  Sprawdzenie IP
  |<-- [recv: rozmiar klucza] --- |
  |                               |
  |-- [send: RSHash(czas+klucz)] >|  Autoryzacja
  |<-- [OK / FAIL] -------------- |
  |                               |
  |-- [send: ROOT/GATEWAY/user] ->|  Poziom dostępu
  |<-- [OK you are ROOT] -------- |
  |                               |
  |-- [send: komenda] ----------->|  Pętla komend
  |<-- [send: odpowiedź] -------- |
  |        ...                    |
  |-- [exit] -------------------> |
```

**Autoryzacja RSHash:**  
Klucz = hash z aktualnego czasu (format `MM:HH:weekday`) + klucza konfiguracyjnego.  
Okno czasowe: ~1 minuta (zależy od synchronizacji zegarów).

**Szyfrowanie:** XOR z kluczem RSHash (opcjonalne, wyłączalne per-IP).

**Poziomy dostępu:**
- `ROOT` – pełny dostęp do wszystkich poleceń
- `GATEWAY` – podzbiór poleceń dla podłączonych bram
- domyślny – podstawowe polecenia użytkownika

**Limity:**
- `MAX_CONNECTION = 10` jednoczesnych połączeń
- Bufor: 32768 bajtów

### HTTP (pasywny, na tym samym porcie)

Jeśli klient wyśle żądanie HTTP zamiast klucza iDom, serwer obsługuje je jako HTTP GET/POST.

### MQTT

Broker: `localhost:1883` (domyślnie)  
QoS: 2 (exactly once)

| Kierunek | Temat | Opis |
|---|---|---|
| Subscribe | `iDom` | Główny temat |
| Subscribe | `iDom-client/buderus/ems-esp/heating_active` | Stan grzania |
| Subscribe | `iDom-client/buderus/ems-esp/boiler_data` | Dane kotła |
| Subscribe | `iDom-client/buderus/ems-esp/boiler_data_ww` | Dane ciepłej wody |
| Subscribe | `iDom-client/buderus/ems-esp/thermostat_data` | Dane termostatu |
| Subscribe | `iDom-client/shed/put` | Dane z szopy |
| Subscribe | `iDom-client/command` | Komendy ogólne |
| Subscribe | `<MQTT_SENSOR_TOPIC>` | Dane rekuperatora |
| Publish | `iDom/command` | Odpowiedzi na komendy |
| Publish | `<temat żarówki>` | Sterowanie żarówką |
| Publish | `<MQTT_CONTROL_TOPIC>` | Sterowanie rekuperatorem |

### RS-232 (SerialPi / WiringSerial)

Komunikacja z Arduino na porcie z konfiguracji (`portRS232`).

Przykładowe komendy:
```
LED_POWER:1;   ← włącz LED zasilania
LED_AT:1;      ← sygnalizacja aktywnego połączenia TCP
LED_CLOCK:1;   ← LED zegara
```

### RFLink 433MHz

Komunikacja przez RS-232 z modułem RFLink.  
Format komendy RFLink: `10;<protokół>;<dane>;`

```
10;PING;                    ← ping modułu
10;NewKaku;ID=12345;SWITCH=1;CMD=ON;   ← włączenie gniazdka
```

### I2C

Wyświetlacz LCD 16×2 pod adresem `0x27`.  
Biblioteka: `liquidcrystal_i2c-for-ardupi`.

### HTTP zewnętrzne (libcurl)

| Serwis | Zastosowanie |
|---|---|
| ThingSpeak | Upload temperatur i danych sensorycznych |
| InfluxDB v2 | Upload stanów żarówek (token auth) |
| Viber API | Notyfikacje push |
| ip-api.com | Geolokalizacja IP przy nieautoryzowanych połączeniach |
| Lightning API | Dane o wyładowaniach atmosferycznych |

---

## 10. API – polecenia TCP

Format: spacja jako separator tokenów, np. `MPD start 3`

### Polecenia dostępne dla wszystkich (commandHandler)

| Polecenie | Składnia | Opis |
|---|---|---|
| `test` | `test` | Test połączenia |
| `uptime` | `uptime` | Czas działania serwera |
| `clock` | `clock` | Aktualny czas serwera |
| `hello` | `hello` | Odpowiedź powitalna |
| `help` | `help` | Lista dostępnych poleceń |
| `ok` | `ok` | Potwierdzenie (ping) |
| `big` | `big` | Wyświetlenie dużego tekstu na LCD |
| `exit` | `exit` | Zamknięcie połączenia |
| `log` | `log` | Operacje na logach |
| `show` | `show log` | Wyświetlenie całego logu |
| `show` | `show log <string>` | Linie logu zawierające `<string>` |
| `show` | `show log no <string>` | Linie logu niezawierające `<string>` |
| `show` | `show thread` | Lista aktywnych wątków |
| `show` | `show iDom key` | Lista kluczy dostępu |
| `put` | `put temperature` | Wyślij temperaturę do ThingSpeak |
| `put` | `put influx` | Wyślij dane do InfluxDB |
| `put` | `put recuperator` | Wyślij dane rekuperatora do InfluxDB |
| `shed` | `shed put <json>` | Aktualizacja danych z szopy (Arduino) |
| `recuperator` | `recuperator set speed <speed>` | Ustaw prędkość: `away/low/medium/high` |
| `recuperator` | `recuperator set away [<czas> <jednostka>]` | Tryb away |
| `recuperator` | `recuperator set boost <czas> <jednostka>` | Tryb boost |

### Polecenia ROOT (commandHandlerRoot) – dodatkowe

| Polecenie | Składnia | Opis |
|---|---|---|
| `MPD` | `MPD start` | Uruchomienie muzyki |
| `MPD` | `MPD start <id>` | Uruchomienie konkretnego utworu/radia |
| `MPD` | `MPD stop` | Zatrzymanie |
| `MPD` | `MPD pause` | Pauza |
| `MPD` | `MPD next` | Następny utwór |
| `MPD` | `MPD prev` | Poprzedni utwór |
| `MPD` | `MPD volume up/down` | Zmiana głośności |
| `MPD` | `MPD volume set <val>` | Ustawienie głośności |
| `MPD` | `MPD list` | Lista dostępnych stacji/utworów |
| `light` | `light info` | JSON z informacjami o wszystkich żarówkach |
| `light` | `light info on` | JSON z włączonymi żarówkami |
| `light` | `light bulb on <id>` | Włącz żarówkę o danym ID |
| `light` | `light bulb on <id> priority` | Włącz z priorytetem |
| `light` | `light bulb off <id>` | Wyłącz żarówkę |
| `light` | `light bulb change <id>` | Przełącz stan żarówki |
| `light` | `light room on <pokój>` | Włącz wszystkie w pokoju |
| `light` | `light room off <pokój>` | Wyłącz wszystkie w pokoju |
| `light` | `light room stats` | Statystyki pokojów |
| `light` | `light clean` | Wyłącz nieoczekiwanie włączone żarówki |
| `iDom` | `iDom speakers ON/OFF` | Sterowanie głośnikami |
| `iDom` | `iDom sunset` | Godzina zachodu słońca |
| `iDom` | `iDom sunrise` | Godzina wschodu słońca |
| `iDom` | `iDom sun` | Wschód, zachód i długość dnia |
| `iDom` | `iDom day lenght` | Długość dnia |
| `iDom` | `iDom sysinfo` | Informacje o systemie Linux |
| `iDom` | `iDom temperature` | Wszystkie temperatury |
| `iDom` | `iDom temperature stats <nazwa>` | Statystyki termometru |
| `state` | `state all` | Stan wszystkich obiektów |
| `state` | `state <nazwa>` | Stan konkretnego obiektu |
| `buderus` | `buderus print` | Wszystkie dane kotła |
| `buderus` | `buderus circPomp` | Uruchomienie pompy cyrkulacyjnej |
| `buderus` | `buderus heating_active 0/1` | Ustawienie stanu grzania |
| `buderus` | `buderus tapwater_active 0/1` | Ustawienie stanu CWU |
| `433MHz` | `433MHz show all` | Lista skonfigurowanych urządzeń |
| `433MHz` | `433MHz show switch` | Lista przełączników |
| `433MHz` | `433MHz show aether` | Ostatnie ramki z eteru |
| `433MHz` | `433MHz show config` | Wyświetl plik konfiguracyjny |
| `433MHz` | `433MHz send <ramka>` | Wyślij ramkę RFLink |
| `433MHz` | `433MHz switch <nazwa> ON/OFF/15s/change` | Sterowanie przełącznikiem |
| `ardu` | `ardu show` | Dane ze stacji pogodowej |
| `cmd` | `cmd <polecenie_linux>` | Wykonaj polecenie systemowe Linux |
| `event` | `event` | Historia zdarzeń (połączenia, komendy, 433MHz) |
| `program` | `program` | Informacje o wersji programu |
| `RS232` | `RS232 <komenda>` | Wyślij komendę bezpośrednio na RS-232 |
| `gateway` | `gateway` | Operacje z bramami |
| `sleep` | `sleep` | Tryb uśpienia |
| `iDom` | `iDom close` | Zamknięcie serwera |
| `iDom` | `iDom reload` | Przeładowanie konfiguracji |
| `iDom` | `iDom reboot` | Restart Raspberry Pi |

---

## 11. Integrowane systemy

### Smart Lighting (HOUSE/)

Żarówki zarządzane przez MQTT (Zigbee2MQTT / Shelly).

Konfiguracja: `/etc/config/iDom_SERVER/bulb_config.json`

Klasy:
- `light_bulb` – reprezentacja żarówki (ID, nazwa, stan, flaga `lighting_priority`)
- `ROOM` – zbiór żarówek tworzący pokój, statystyki użycia
- `house_room_handler` – główny manager (mapa żarówek, mapa pokoi, mapa sensorów Satel)

Integracja z Satel Integra: aktywacja sensora alarmowego może automatycznie sterować oświetleniem w danej strefie.

### Satel Integra 32 (SATEL_INTEGRA/)

Komunikacja TCP z modułem ETHM-1 centrali alarmowej.

Możliwości:
- Uzbrajanie/rozbrajanie partycji
- Włączanie/wyłączanie wyjść
- Monitorowanie stanu uzbrojenia
- Integracja ze światłami (czujniki ruchu → oświetlenie)

Konfiguracja: sekcja `satel_integra` w JSON.

### Buderus / EMS-ESP (buderus/)

Dane z kotła przychodzą przez MQTT (gateway EMS-ESP).

Dostępne dane:
- Temperatura zewnętrzna, wewnętrzna, kotła, przepływu
- Stan grzania i ciepłej wody użytkowej (CWU)
- Sterowanie pompą cyrkulacyjną

Tematy MQTT:
```
iDom-client/buderus/ems-esp/heating_active
iDom-client/buderus/ems-esp/boiler_data
iDom-client/buderus/ems-esp/boiler_data_ww
iDom-client/buderus/ems-esp/thermostat_data
```

### Rekuperator (recuperator/)

Sterowanie wentylacją mechaniczną przez MQTT.

Tryby pracy:
- `away` – tryb nieobecności (zmniejszony przepływ)
- `low` / `medium` / `high` – prędkości wentylatora
- `boost` – tryb intensywny na czas określony (`<czas> <jednostka>`)

Jednostki czasu: `min`, `h`, `d` (minuty, godziny, dni)

### RFLink 433MHz (433MHz/ + RADIO_433_eq/)

Moduł RFLink podłączony przez RS-232 umożliwia odbiór i nadawanie sygnałów 433MHz.

Obsługiwane typy urządzeń (konfiguracja w JSON):
- `RADIO_SWITCH` – gniazdka/przełączniki zdalnie sterowane
- `RADIO_BUTTON` – przyciski, piloty
- `RADIO_WEATHER_STATION` – stacje pogodowe (temperatura, wilgotność, ciśnienie)

Ramki RFLink: `20;<protokół>;ID=<hex>;CMD=<cmd>;`

### MPD – Music Player Daemon (iDomTools/)

Sterowanie przez libmpdclient. Integracja z głośnikami (włączanie wzmacniacza przed odtwarzaniem).

Stan przechowywany w `MPD_info`:
```cpp
struct MPD_info {
    std::string title;    // tytuł aktualnego utworu
    std::string radio;    // nazwa stacji
    std::string artist;   // wykonawca
    int volume;           // głośność 0-100
    bool isPlay;          // czy gra
    int currentSongID;    // ID aktualnego utworu
    std::vector<std::string> songList;  // lista stacji/plików
};
```

### Wyświetlacz LCD (LCD_c/ + MENU/)

LCD 16×2 przez I2C (adres `0x27`). Menu obsługiwane przez maszynę stanów `MENU_STATE_MACHINE`.

Przyciski (`KEY_PAD`):
```
POWER, OK, RES, UP, MENU, RIGHT, DOWN, LEFT, EPG, REBOOT, OFF_LCD, TIMEOUT
```

### Viber API (libs/viberAPI/)

Notyfikacje wysyłane przy:
- Starcie/zatrzymaniu serwera
- Nieudanej autoryzacji TCP
- Podejrzanej aktywności połączeń

---

## 12. Baza danych InfluxDB

Wątek `f_master_influx` asynchronicznie wysyła dane do InfluxDB v2 przez HTTP API.

Dane zapisywane:
- Stany żarówek (`BULB_STATUS`: nazwa, stan on/off, timestamp)
- Temperatury (przez `put influx`)
- Dane rekuperatora (przez `put recuperator`)

Mechanizm retry: każdy element kolejki ma TTL=100. Przy błędzie HTTP TTL jest dekrementowany i element wraca do kolejki. Wątek śpi 1 minutę przy braku internetu.

Konfiguracja:
```json
"database": {
    "ip": "127.0.0.1",
    "port": 8086,
    "token": "TOKEN",
    "org": "ORGANIZACJA",
    "bucket": "BUCKET"
}
```

---

## 13. Wzorce projektowe

### Command Pattern
Hierarchia handlerów z mapą `string → unique_ptr<command>`.  
Każde polecenie TCP to tokenizowany string, np. `MPD start 3`.

```
commandHandler (bazowy, ~13 poleceń)
└── commandHandlerRoot (~13 dodatkowych poleceń ROOT)
└── commandHandlerGATEWAY (podzbiór dla bram)
commandHandlerMQTT (niezależny, do wiadomości MQTT)
```

### Interface / Abstract Base Class
Wszystkie główne podsystemy za interfejsami:
- `BUDERUS_IF` – kocioł Buderus
- `RecuperatorInterface` – rekuperator
- `SATEL_INTEGRA_INTERFACE` – centrala alarmowa
- `iDomTOOLS_INTERFACE` – fasada narzędziowa
- `KEY_HANDLER_BASE` – handler klawiszy/menu

### Thread Pool
Własna implementacja `ThreadPool` z kolejką blokującą i trybem `Blocking` (enqueue blokuje gdy kolejka pełna).

### Registry Pattern
`iDom_API` z statyczną mapą `m_map_iDom_API` – używana przez polecenie `show dump` do dumpowania stanu wszystkich zarejestrowanych obiektów.

### Blocking Queue
`BlockingQueue<BULB_STATUS>` między wątkiem głównym a wątkiem InfluxDB – bezpieczna asynchroniczna komunikacja.

### Context Object
`thread_context` przekazywany wskaźnikiem – unika globalnych zmiennych, agreguje cały stan systemu.

---

## 14. Biblioteki pomocnicze (libs/)

Katalog `libs/` to osobne repozytorium git (podmoduł lub kopia).

| Biblioteka | Opis |
|---|---|
| `ThreadPool` | Pula wątków z trybami Blocking/NonBlocking |
| `alarm` | Wewnętrzny system alarmów (licznik błędów z ID i opisem) |
| `circular_buffer` | Bufor cykliczny dla historii sensorów Satel |
| `config_parser` | Parser plików konfiguracyjnych |
| `emoji` | Emoji w logach i odpowiedziach TCP (np. ⚠️ dla błędów) |
| `event_counters` | Liczniki zdarzeń według typów (connections, 433MHz, commands, MPD...) |
| `generator` | Generator pseudolosowych ciągów |
| `sunrise-sunset` | Obliczanie wschodu/zachodu słońca na podstawie współrzędnych geograficznych |
| `useful` | Narzędzia ogólne: httpPost, ipCountry, split, sleep, runLinuxCommand |
| `viberAPI` | Klient REST API Viber (wiadomości, zdjęcia, URL) |
| `Statistic` | Statystyki numeryczne (min, max, avg, count) dla pomiarów float |
| `backTrace` | Handler SIGSEGV/SIGABRT z zrzutem backtrace do logu |
| `yaml` | Minimalny parser YAML (używany przez CRON) |
| `blocking_queue` | Kolejka blokująca (używana przez house_room_handler → InfluxDB) |

---

## 15. Pliki robocze i logi

| Ścieżka | Opis |
|---|---|
| `/mnt/ramdisk/iDom_log.log` | Główny plik logów (ramdysk) |
| `/mnt/ramdisk/pid-iDom.txt` | PID procesu serwera |
| `/mnt/ramdisk/command.txt` | Nieznane polecenia (diagnostyka) |
| `/mnt/ramdisk/cmd2` | Kod zakończenia dla watchdoga (zakomentowany main) |
| `<saveFilePath>` | Zapisany stan serwera (JSON, ścieżka z konfiguracji) |

### Poziomy logowania

| Makro | Znaczenie |
|---|---|
| `INFO` | Informacje ogólne (start, konfiguracja) |
| `DEBUG` | Debugowanie (wersja, detale) |
| `CRITICAL` | Błędy wymagające uwagi (wyjątki, błędy autoryzacji) |

---

## 16. Stany programu i graceful shutdown

Program może zakończyć się z różnymi stanami:

| Stan (`iDomStateEnum`) | Opis | Kod wyjścia (watchdog) |
|---|---|---|
| `CLOSE` | Normalne zamknięcie | 0 |
| `RELOAD` | Przeładowanie konfiguracji (miękki restart) | pętla restart |
| `HARD_RELOAD` | Twardy restart procesu | 3 |
| `RASPBERRY_RELOAD` | Restart całego Raspberry Pi | `shutdown -r now` |
| `ERROR` | Zamknięcie z błędem | 1 |
| `WORKING` | Normalny stan pracy | – |

### Sygnały systemowe

| Sygnał | Akcja |
|---|---|
| `SIGTERM` | Wywołanie `close_iDomServer()` → graceful shutdown |
| `SIGABRT` | Zapisanie backtrace do logu → zakończenie |
| `SIGSEGV` | Zapisanie backtrace do logu → zakończenie |

### Watchdog (zakomentowany w main.cpp)

Rozbudowana wersja `main()` (aktualnie zakomentowana) implementuje watchdoga:
- Automatyczny restart przy `RELOAD`
- Limit 13 szybkich restartów (< 10s) – po przekroczeniu: twarde zamknięcie
- Zapis kodu wyjścia do `/mnt/ramdisk/cmd2` dla zewnętrznego skryptu

---

*Dokumentacja wygenerowana na podstawie analizy kodu źródłowego.*  
*Projekt: iDom_server_OOP | Język: C++20 | Platforma: Raspberry Pi / Linux*
