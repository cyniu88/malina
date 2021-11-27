// for development purposes copy "config.h.sample" into "config.h" file or use link:
// - Windows 10 (PowerShell) as admin: Start-Process -FilePath "$env:comspec" -ArgumentList "/k", "mklink", "config.h","config.h.sample" -Verb RunAs
// For production you can customize "config.h" or use link to "config.h.local":
// - Start-Process -FilePath "$env:comspec" -ArgumentList "/k", "mklink", "config.h","config.h.local" -Verb RunAs
// Alternative version:
// Start-Process -Verb RunAs -FilePath "powershell" -ArgumentList "-NoExit","-command","New-Item -Path '$(Get-Location)\include\config.h' -ItemType SymbolicLink -Value '$(Get-Location)\include\config.h.sample'"

// For mono-stable buttons it will trigger state: LOW - when button is pressed, HIGH - when button is released
const uint8_t MONO_STABLE_TRIGGER = LOW;

// time interval for RELAY_IMPULSE type relay, ignored when button type is DING_DONG or REED_SWITCH
const unsigned long RELAY_IMPULSE_INTERVAL = 1500UL;

// debounce interval in milliseconds
const unsigned long BUTTON_DEBOUNCE_INTERVAL = 50;

// double-click interval in milliseconds
const unsigned long BUTTON_DOUBLE_CLICK_INTERVAL = 350;

// long-press interval in milliseconds
const unsigned long BUTTON_LONG_PRESS_INTERVAL = 800;

#ifdef USE_EXPANDER
uint8_t expanderAddresses[] = {0, 1, 2, 3};
#endif

const char MULTI_RELAY_DESCRIPTION[] = "Multi Relay";

constexpr int VIRTUAL_________RELAY = 10;
constexpr int NOT_USED = -1;

constexpr int TARAS_GNIAZDKO_____99 = 99;
constexpr int TARAS_OSWIETLENIE_103 = 103;

constexpr int DZWONEK____________88 = 88;

constexpr int KORYTARZ__________117 = 117;

constexpr int PODBITKA_WEJSCIE__107 = 107;
constexpr int PODBITKA_TARAS____104 = 104;

constexpr int SPIZARKA__________105 = 105;

constexpr int LAZIENKA_MALA_____129 = 129;

constexpr int KANCIAPA__________123 = 123;
constexpr int WIATROLAP_________130 = 130;
constexpr int WEJSCIE___________106 = 106;

constexpr int LAZIENKA_WANNA____102 = 102;
constexpr int LAZIENKA_WEJSCIE__113 = 113;

constexpr int POKOJ_W_JEDNA_____109 = 109;
constexpr int POKOJ_W_DWIE______110 = 110;
// Relays config - ID and PIN MUST BE UNIQUE (can't be repeated)!
// Row params: relay ID - [0-254] sensor ID reported on MySensor Gateway
//             relay pin - pin used for relay, can be expander pin via "E(x, y)" macro
//             relay options - [RELAY_TRIGGER_LOW|RELAY_TRIGGER_HIGH] {RELAY_STARTUP_ON|RELAY_STARTUP_OFF} {RELAY_IMPULSE}
//             relay description - reported on MySensor Gateway, can help identify device on initial configuration in Home Automation App, can be empty ("")
const RelayConfigDef gRelayConfig[] = {
    {100, E(0, 0), RELAY_TRIGGER_LOW, ""},
    {101, E(0, 1), RELAY_TRIGGER_LOW, "salon_kinkiet"},
    {LAZIENKA_WANNA____102, E(0, 2), RELAY_TRIGGER_LOW, "lazienka wanna"},
    {TARAS_OSWIETLENIE_103, E(0, 3), RELAY_TRIGGER_LOW, "taras oswietlenie"},
    {PODBITKA_TARAS____104, E(0, 4), RELAY_TRIGGER_LOW, " podbitka taras"},
    {SPIZARKA__________105, E(0, 5), RELAY_TRIGGER_LOW, "spizarka"},
    {WEJSCIE___________106, E(0, 6), RELAY_TRIGGER_LOW, "wejscie"},
    {PODBITKA_WEJSCIE__107, E(0, 7), RELAY_TRIGGER_LOW, "podbitk wejscie"},
    {108, E(0, 8), RELAY_TRIGGER_LOW, ""},
    {POKOJ_W_JEDNA_____109, E(0, 9), RELAY_TRIGGER_LOW, "pokoj_W_jedna"},
    {POKOJ_W_DWIE______110, E(0, 0xa), RELAY_TRIGGER_LOW, "pokoj_W_dwie"},
    {111, E(0, 0xb), RELAY_TRIGGER_LOW, "sypialnia"},
    {112, E(0, 0xc), RELAY_TRIGGER_LOW, ""},
    {LAZIENKA_WEJSCIE__113, E(0, 0xd), RELAY_TRIGGER_LOW, "lazienka_wejscie"},
    {114, E(0, 0xe), RELAY_TRIGGER_LOW, ""},
    {115, E(0, 0xf), RELAY_TRIGGER_LOW, ""},
    {116, E(1, 0), RELAY_TRIGGER_LOW, "kuchnia_bar"},
    {KORYTARZ__________117, E(1, 1), RELAY_TRIGGER_LOW, "korytarz"},
    {118, E(1, 2), RELAY_TRIGGER_LOW, ""},
    {119, E(1, 3), RELAY_TRIGGER_LOW, ""},
    {120, E(1, 4), RELAY_TRIGGER_LOW, ""},
    {121, E(1, 5), RELAY_TRIGGER_LOW, "salon_nad_stolem"},
    {122, E(1, 6), RELAY_TRIGGER_LOW, ""},
    {KANCIAPA__________123, E(1, 7), RELAY_TRIGGER_LOW, "kanciapa"},
    {124, E(1, 8), RELAY_TRIGGER_LOW, ""},
    {125, E(1, 9), RELAY_TRIGGER_LOW, "pokoj_E"},
    {126, E(1, 0xa), RELAY_TRIGGER_LOW, "kuchnia_kinkiet"},
    {127, E(1, 0xb), RELAY_TRIGGER_LOW, ""},
    {128, E(1, 0xc), RELAY_TRIGGER_LOW, "pokoj_N"},
    {LAZIENKA_MALA_____129, E(1, 0xd), RELAY_TRIGGER_LOW, "lazienka_mala"},
    {WIATROLAP_________130, E(1, 0xe), RELAY_TRIGGER_LOW, "wiatrolap"},
    {131, E(1, 0xf), RELAY_TRIGGER_LOW, ""},
    {DZWONEK____________88, 16, RELAY_TRIGGER_LOW | RELAY_IMPULSE_INTERVAL, "dzwonek"},
    {TARAS_GNIAZDKO_____99, 38, RELAY_TRIGGER_LOW, "taras gniazdko"},
    {VIRTUAL_________RELAY, 10, RELAY_TRIGGER_LOW, "wirtulny"}

};

// Buttons config
// Row params: button pin - pin used for button, can be expander pin (but NOT RECOMMENDED) via "E(x, y)" macro
//             button type - [MONO_STABLE|BI_STABLE|DING_DONG|REED_SWITCH]
//             click action relay ID - MUST be defined in gRelayConfig
//             long-press action relay ID - ignored for BI_STABLE, DING_DONG, REED_SWITCH
//             double-click action relay ID - ignored for DING_DONG, REED_SWITCH
//             button description - debug only, can be empty ("")
const ButtonConfigDef gButtonConfig[] = {
    {29, MONO_STABLE, 101, VIRTUAL_________RELAY, TARAS_OSWIETLENIE_103, "z korytarz"},
    {28, MONO_STABLE, 101, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "z korytarz temporarny"},
    {36, MONO_STABLE, 101, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "z taras"},
    {37, MONO_STABLE, TARAS_OSWIETLENIE_103, VIRTUAL_________RELAY, PODBITKA_TARAS____104, "z salonu"},
    {26, MONO_STABLE, 101, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "z kuchni"},
    {27, MONO_STABLE, 101, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "z kuchni"},
    {41, MONO_STABLE, LAZIENKA_WANNA____102, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "wanna"},
    {24, MONO_STABLE, 121, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "z kuchni"},
    {30, MONO_STABLE, 121, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "z korytarza"},
    {25, MONO_STABLE, 116, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "z kuchni"},
    {31, MONO_STABLE, 116, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "z korytarza"},
    {22, MONO_STABLE, 126, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "z kuchni temporarny"},
    {23, MONO_STABLE, 126, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "z kuchni"},
    {33, MONO_STABLE, 126, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "z kuchni"},
    {32, MONO_STABLE, 126, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "z korytarza temporarny"},
    {45, MONO_STABLE, SPIZARKA__________105, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "z spizarka"},
    {20, MONO_STABLE, WEJSCIE___________106, VIRTUAL_________RELAY, PODBITKA_WEJSCIE__107, "wejscie"},
    {47, MONO_STABLE, POKOJ_W_JEDNA_____109, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "pokoj W jedna"},
    {46, MONO_STABLE, POKOJ_W_DWIE______110, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "pokoj W dwie"},
    {42, MONO_STABLE, 111, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "sypialnia"},
    {43, MONO_STABLE, 111, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "sypialnia"},
    {40, MONO_STABLE, LAZIENKA_WEJSCIE__113, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "wejscie"},
    {A12, MONO_STABLE, KORYTARZ__________117, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "korytarz od salonu"},
    {44, MONO_STABLE, KORYTARZ__________117, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "korytarz od sypialni"},
    {A13, MONO_STABLE, LAZIENKA_MALA_____129, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "lazenka mala"},
    {21, MONO_STABLE, KANCIAPA__________123, WIATROLAP_________130, VIRTUAL_________RELAY, "kanciapa"},
    {49, MONO_STABLE, 125, VIRTUAL_________RELAY, KORYTARZ__________117, "pokoj E"},
    {48, MONO_STABLE, 125, VIRTUAL_________RELAY, KORYTARZ__________117, "pokoj E"},
    {A15, MONO_STABLE, 128, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "pokoj N"},
    {A14, MONO_STABLE, 128, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "pokoj N"},
    {35, MONO_STABLE, WIATROLAP_________130, VIRTUAL_________RELAY, WEJSCIE___________106, "wiatrolap z salonu"},
    {19, MONO_STABLE, WIATROLAP_________130, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "wiatrolap z wiatrolapu"},
    {34, MONO_STABLE, WIATROLAP_________130, VIRTUAL_________RELAY, VIRTUAL_________RELAY, "lazienka mala"},
    {17, MONO_STABLE, DZWONEK____________88, VIRTUAL_________RELAY, WEJSCIE___________106, "dzwonek"}};


// MQTT
constexpr int mqttPort = 1883;
const String mqttBrokerIP = "192.168.13.181";
const String publicTopic = "iDom-client/command";
const String subTopic = "swiatlo/output/#";
//ethernet
String ip = "192.168";
String MAC = "";
uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};