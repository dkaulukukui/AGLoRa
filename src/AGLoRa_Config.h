#ifndef AGLoRa_CONFIG
    #define AGLoRa_CONFIG

    #include "Arduino.h"

     /*
    Project AGLoRa (abbreviation of Arduino + GPS + LoRa)
    Tiny and chip LoRa GPS tracker

    Copyright © 2021-2024 Eugeny Shlyagin. Contacts: <shlyagin@gmail.com>
    License: http://opensource.org/licenses/MIT

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    Modules used:
    - Arduino UNO/Nano (ATMEGA328P, not ATmega168)
    - GPS NMEA Module (Generic)
    - LoRa EBYTE E220-900T22D (868 MHz) or EBYTE E32-E433T30D (433 MHz)
    - Bluetooth BLE AT-09 or HC-05
    */

    /* 
    HOW THIS SKETCH WORKS :

    Most of the time the tracker works in listening mode. 

    After receiving the LoRa package from another tracker - 
    this data is transferred to the bluetooth serial port.

    Once every three minutes, the tracker switches to 
    the GPS receiver for a few seconds to get coordinates. 
    Then sends a LoRa data packet.

    NOTE: GPS is valid, if LED_BUILTIN is HIGH
    */

    // HOW TO SETUP:
    // ========================================
    // ==== Settings LEVEL 1 (required) =======
    // ========================================



    // ...or, if you use PlatformIO, add to "platformio.ini":
    //  lib_deps = 
    //  	mikalhart/TinyGPSPlus@^1.0.3
    //  	xreef/EByte LoRa E220 library@^1.0.8

    // The first thing you need is to set up a tracker name and modules connections:

    // // ========== NAME =======================
     #define NAME_LENGTH 12             // The same value for all devices
    const char NAME[NAME_LENGTH] = "Rick";               // Name of current tracker, NAME_LENGTH characters
    // Example:
    // #define NAME = "Morty"; // All names length should be no longer than NAME_LENGTH
    // ========== WIRING =====================
    //UART LORA
    #define LORA_PIN_RX 2
    #define LORA_PIN_TX 3
    #define LORA_PIN_M0 4
    #define LORA_PIN_M1 5
    #define LORA_PIN_AX 6

    //UART GPS
    #define GPS_PIN_RX 7
    #define GPS_PIN_TX 8

    // Leds
    #define LORA_LED LED_BUILTIN
    #define GPS_LED LED_BUILTIN
    #define BLE_LED LED_BUILTIN
    #define MEMORY_LED LED_BUILTIN
    // ========== MODULES SETTING=============
    #define GPS_SPEED 9600
    #define LORA_SPEED 9600

    // Then install "EByte LoRa E220 by Renzo Mischianty" library
    // and "TinyGPSPlus by Mikal Hart" library
    // from Arduino IDE library manager ("Tools" -> "Manage Libraries")

    // Now you can upload this sketch to Arduino
    // and turn on the app "AGLoRa" on your phone

    // If something went wrong you can enable
    // "debug mode" through the serial port.
    // Don't forget to disconnect the bluetooth module.
    // Then open “Tools” -> ”Serial monitor” in Arduino IDE.
    #define DEBUG_MODE true  // change "false" to "true" to enable
    // Next, logs levels for comfortable deallbugging, 
    // if DEBUG_MODE == false, logs level are not important 
    #define DEBUG_BLE false  // bluetooth low energy
    #define DEBUG_GPS true  // print GPS logs
    #define DEBUG_LORA true  // print GPS logs
    #define DEBUG_MEMORY true  // print GPS logs
    #define DEBUG_AGLORA true  // print GPS logs


    // ========================================
    // ==== Settings LEVEL 2 (optional) =======
    // ========================================

    //Moved to 

    

    // ========================================
    // ==== Settings LEVEL 3 (nightmare) ======
    // ========================================
    #define USE_EEPROM_MEMORY false  // "false" by default
    // set "false" to use SRAM memory, "true" to use EEPROM
    // EEPROM is permanent memory, data is not lost even 
    // if the system is turned off.
    // But the write operation is finite and usually capped at 100,000 cycles.
    // Please read: https://docs.arduino.cc/learn/programming/memory-guide
    // ============ LORA NETWORK SETTINGS ============
    #define I_WANT_TO_SEND_MY_LOCATION true  // "true" by default
    #define DATA_SENDING_INTERVAL 30000  // milliseconds

    #define MESH_MODE true  // "true" by default
    #define TTL 3  // Data packet lifetime (for transfer between devices)
    // ============ OTHER SETTINGS ==========
    #define USE_BLE true  // use BLE output
    #define BLE_UPDATE_INTERVAL 50000  // milliseconds
    // ============ SRAM STORAGE ==============
    // Maximum number of track points (struct DATA) in memory
    // Change and check free memory in "Output" after pressing "Verify".
    #define SRAM_STORAGE_SIZE 15    // DATA array size
    // not used if USE_EEPROM_MEMORY true, may be zero in this case
    // NOTE: Don't use all free memory! It's may broke BLE output. 
    // You should hold free memory for return String from "sendToPhone"
    // ============ EEPROM STORAGE ==============
    // EEPROM (non-volatile) memory
    // not used if define USE_EEPROM_MEMORY false
    #define EEPROM_BEGIN_ADDRESS 0  //bytes
    // reserve for storing settings
    // not used if USE_EEPROM_MEMORY false
    // ================ TESTS ==================
    #define TEST_LORA_DATA false // test mode (virtual tracker)
    #define OTHER_NAME "Summer" // virtual tracker's name
    // =========================================
    // ========== END OF SETTINGS ==============
    // =========================================

#endif