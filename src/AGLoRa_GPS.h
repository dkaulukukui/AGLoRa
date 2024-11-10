#ifndef AGLoRa_GPS
    #define AGLoRa_GPS

    //#include "AGLoRa.h"
    //#include "AGLoRa_LORA.h"
    #include "AGLoRa_Indication.h"
    #include "AGLoRa_Config.h"

    
    #include <TinyGPSPlus.h>      // install from Arduino IDE
    // Docs: http://arduiniana.org/libraries/tinygpsplus/


    struct GPSDATA {
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
    };


    class GPS
    {
        //SoftwareSerial gpsPort;
        //#define gpsPort Serial1
        TinyGPSPlus gpsModule;
    public:
        GPS(uint8_t pinRx, uint8_t pinTx, long speed, INDICATION * indication);
        void setup();
        void updateLocation(GPSDATA *dataPackage);


    private:
        bool _debugMode;
        INDICATION * _indication;
        void printGPSInfo();
        void printReadingIndication(unsigned long start, unsigned int delay);
        char _readingIndicator = 0;
    };


#endif