#ifndef AGLoRa_LORA
    #define AGLoRa_LORA

    #include "AGLoRa_Indication.h"
    #include "AGLoRa_Config.h"

    
    #include "LoRa_E220.h"        // install from Arduino IDE
    // Docs: https://github.com/xreef/EByte_LoRa_E220_Series_Library


    //#include "AGLoRa.h"
    //#include "AGLoRa_Classes.h"

    /*
    This is the structure of the LoRa data package.
    If you want to send additional data between AGLoRa trackers
    you should add it to this section.
    Note that all software version must be the same for all trackers.
    */
    struct DATA {
        char name[NAME_LENGTH];  // name

        float lat;  // coordinates
        float lon;

        unsigned char year;  // the Year minus 2000
        unsigned char month;
        unsigned char day;

        unsigned char hour;
        unsigned char minute;
        unsigned char second;

        unsigned char sat;
        unsigned char hdop;

        // Add more data fields here if you need
        // ...
        unsigned char battery;
        // unsigned char speed;
        // unsigned char sensor1;
        // unsigned char sensor2;
        // ...
    };


    struct LORADATA
    {
        DATA data;
        unsigned char ttl;   // time to live (for mesh network)
    };


    class LORA
    {
        //SoftwareSerial loraPort;
        //#define loraPort Serial5
        //LoRa_E220 e220ttl;
    public:
        LORA(uint8_t pinRx, uint8_t pinTx, long speed, uint8_t aux, uint8_t m0, uint8_t m1, INDICATION * indication);
        void setup();
        void send(LORADATA *loraDataPackage);
        bool hasNewData(LORADATA *loraDataPackage);


    private:
        bool _debugMode;
        uint8_t _ledPin;
        INDICATION * _indication;
        ResponseStructContainer rsc;
    };




#endif