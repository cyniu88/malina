// for development purposes copy "config.h.sample" into "config.h" file or use link:
// - Windows 10 (PowerShell) as admin: Start-Process -FilePath "$env:comspec" -ArgumentList "/k", "mklink", "config.h","config.h.sample" -Verb RunAs
// For production you can customize "config.h" or use link to "config.h.local":
// - Start-Process -FilePath "$env:comspec" -ArgumentList "/k", "mklink", "config.h","config.h.local" -Verb RunAs
// Alternative version:
// Start-Process -Verb RunAs -FilePath "powershell" -ArgumentList "-NoExit","-command","New-Item -Path '$(Get-Location)\include\config.h' -ItemType SymbolicLink -Value '$(Get-Location)\include\config.h.sample'"

// For mono-stable buttons it will trigger state: LOW - when button is pressed, HIGH - when button is released
const uint8_t MONO_STABLE_TRIGGER = LOW;

// time interval for RELAY_IMPULSE type relay, ignored when button type is DING_DONG or REED_SWITCH
const unsigned long RELAY_IMPULSE_INTERVAL = 250;

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

// Relays config - ID and PIN MUST BE UNIQUE (can't be repeated)!
// Row params: relay ID - [0-254] sensor ID reported on MySensor Gateway
//             relay pin - pin used for relay, can be expander pin via "E(x, y)" macro
//             relay options - [RELAY_TRIGGER_LOW|RELAY_TRIGGER_HIGH] {RELAY_STARTUP_ON|RELAY_STARTUP_OFF} {RELAY_IMPULSE}
//             relay description - reported on MySensor Gateway, can help identify device on initial configuration in Home Automation App, can be empty ("")
const RelayConfigDef gRelayConfig[] = {
    {100, E(0, 0), RELAY_TRIGGER_LOW, ""},
    {101, E(0, 1), RELAY_TRIGGER_LOW, "salon_kinkiet"},
    {102, E(0, 2), RELAY_TRIGGER_LOW, "lazienka wanna"},
    {103, E(0, 3), RELAY_TRIGGER_LOW, ""},
    {104, E(0, 4), RELAY_TRIGGER_LOW, ""},
    {105, E(0, 5), RELAY_TRIGGER_LOW, "spizarka"},
    {106, E(0, 6), RELAY_TRIGGER_LOW, ""},
    {107, E(0, 7), RELAY_TRIGGER_LOW, "wejscie"},
    {108, E(0, 8), RELAY_TRIGGER_LOW, ""},
    {109, E(0, 9), RELAY_TRIGGER_LOW, "pokoj_W_jedna"},
    {110, E(0, 0xa), RELAY_TRIGGER_LOW, "pokoj_W_dwie"},
    {111, E(0, 0xb), RELAY_TRIGGER_LOW, "sypialnia"},
    {112, E(0, 0xc), RELAY_TRIGGER_LOW, ""},
    {113, E(0, 0xd), RELAY_TRIGGER_LOW, "lazienka_wejscie"},
    {114, E(0, 0xe), RELAY_TRIGGER_LOW, ""},
    {115, E(0, 0xf), RELAY_TRIGGER_LOW, ""},
    {116, E(1, 0), RELAY_TRIGGER_LOW, "kuchnia_bar"},
    {117, E(1, 1), RELAY_TRIGGER_LOW, "korytarz"},
    {118, E(1, 2), RELAY_TRIGGER_LOW, ""},
    {119, E(1, 3), RELAY_TRIGGER_LOW, ""},
    {120, E(1, 4), RELAY_TRIGGER_LOW, ""},
    {121, E(1, 5), RELAY_TRIGGER_LOW, "salon_nad_stolem"},
    {122, E(1, 6), RELAY_TRIGGER_LOW, ""},
    {123, E(1, 7), RELAY_TRIGGER_LOW, "kanciapa"},
    {124, E(1, 8), RELAY_TRIGGER_LOW, ""},
    {125, E(1, 9), RELAY_TRIGGER_LOW, "pokoj_E"},
    {126, E(1, 0xa), RELAY_TRIGGER_LOW, "kuchnia_kinkiet"},
    {127, E(1, 0xb), RELAY_TRIGGER_LOW, ""},
    {128, E(1, 0xc), RELAY_TRIGGER_LOW, "pokoj_N"},
    {129, E(1, 0xd), RELAY_TRIGGER_LOW, ""},
    {130, E(1, 0xe), RELAY_TRIGGER_LOW, "wiatrolap"},
    {131, E(1, 0xf), RELAY_TRIGGER_LOW, ""},
    {88, 16, RELAY_TRIGGER_LOW, "dzwonek"},
    {99, 38, RELAY_TRIGGER_LOW, "taras"}

};

// Buttons config
// Row params: button pin - pin used for button, can be expander pin (but NOT RECOMMENDED) via "E(x, y)" macro
//             button type - [MONO_STABLE|BI_STABLE|DING_DONG|REED_SWITCH]
//             click action relay ID - MUST be defined in gRelayConfig
//             long-press action relay ID - ignored for BI_STABLE, DING_DONG, REED_SWITCH
//             double-click action relay ID - ignored for DING_DONG, REED_SWITCH
//             button description - debug only, can be empty ("")
const ButtonConfigDef gButtonConfig[] = {
    {29, MONO_STABLE, 101, -1, -1, "z korytarz"},
    {28, MONO_STABLE, 101, -1, -1, "z korytarz temporarny"},
    {36, MONO_STABLE, 101, -1, -1, "z taras"},
    {37, MONO_STABLE, 101, -1, -1, "z taras"},
    {26, MONO_STABLE, 101, -1, -1, "z kuchni"},
    {27, MONO_STABLE, 101, -1, -1, "z kuchni"},
    {41, MONO_STABLE, 102, -1, -1, "wanna"},
    {24, MONO_STABLE, 121, -1, -1, "z kuchni"},
    {30, MONO_STABLE, 121, -1, -1, "z korytarza"},
    {25, MONO_STABLE, 116, -1, -1, "z kuchni"},
    {31, MONO_STABLE, 116, -1, -1, "z korytarza"},
    {22, MONO_STABLE, 126, -1, -1, "z kuchni temporarny"},
    {23, MONO_STABLE, 126, -1, -1, "z kuchni"},
    {33, MONO_STABLE, 126, -1, -1, "z kuchni"},
    {32, MONO_STABLE, 126, -1, -1, "z korytarza temporarny"},
    {45, MONO_STABLE, 105, -1, -1, "z spizarka"},
    {20, MONO_STABLE, 107, -1, -1, "wejscie"},
    {47, MONO_STABLE, 109, -1, -1, "pokoj W jedna"},
    {46, MONO_STABLE, 110, -1, -1, "pokoj W dwie"},
    {42, MONO_STABLE, 111, -1, -1, "sypialnia"},
    {43, MONO_STABLE, 111, -1, -1, "sypialnia"},
    {40, MONO_STABLE, 113, -1, -1, "wejscie"},
    {52, MONO_STABLE, 117, -1, -1, "korytarz od salonu"},
    {53, MONO_STABLE, 117, -1, -1, "korytarz od salonu"},
    {44, MONO_STABLE, 117, -1, -1, "korytarz od sypialni"},
    {21, MONO_STABLE, 123, -1, -1, "kanciapa"},
    {49, MONO_STABLE, 125, -1, -1, "pokoj E"},
    {48, MONO_STABLE, 125, -1, -1, "pokoj E"},
    {50, MONO_STABLE, 128, -1, -1, "pokoj N"},
    {51, MONO_STABLE, 128, -1, -1, "pokoj N"},
    {35, MONO_STABLE, 130, -1, -1, "wiatrolap z salonu"},
    {19, MONO_STABLE, 130, -1, -1, "wiatrolap z wiatrolapu"},
    {34, MONO_STABLE, 130, -1, -1, "wiatrolap z salonu temporarny"},
    {17, DING_DONG, 88, -1, -1, "dzwonek"}

};
