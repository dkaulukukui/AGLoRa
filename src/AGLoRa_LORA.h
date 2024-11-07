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

    /*
    This is a function that sends data to the app.
    Data packets are sent using OsmAnd-like protocol:
    id=name&lat={0}&lon={1}&timestamp={2}&speed={3}&altitude={4}
    */
    String sendToPhone(DATA *package) {

    String result;

    result += F("&name=");  //other tracker's name
    result += package->name;  //NAME_LENGTH bytes

    result += F("&lat=");       // cordinates
    result += String(package->lat, 6);  // latitude
    result += F("&lon=");       // record separator
    result += String(package->lon, 6);  // longitute

    //Date and time format: 2023-06-07T15:21:00Z
    result += F("&timestamp=");      // record separator
    result += package->year + 2000;  // year
    result += F("-");                // data separator
    if (package->month < 10) result += F("0");
    result += package->month;        // month
    result += F("-");                // data separator
    if (package->day < 10) result += F("0");
    result += package->day;          // day
    result += F("T");                // record separator
    if (package->hour < 10) result += F("0");
    result += package->hour;         // hour
    result += F(":");                // time separator
    if (package->minute < 10) result += F("0");
    result += package->minute;       // minute
    result += F(":");                // time separator
    if (package->second < 10) result += F("0");
    result += package->second;       // second
    result += F("Z");                // UTC

    // Sensors and additional data
    result += F("&sat="); 
    result += package->sat;  // satellites  1 byte

    result += F("&hdop="); 
    result += package->hdop;  // HDOP  1 byte

    // Add more data here if you need ...
    // result += "&speed=";       // data's name in app
    // result += package->speed;   // value

    result += "&batt=";
    result += package->battery;

    // result += "&alienSensor=";
    // result += package->sensor1;

    // result += "&C-137-level=";
    // result += package->sensor2;

    return result;
    }



    struct LORADATA
    {
        DATA data;
        unsigned char ttl;   // time to live (for mesh network)
    };


    class LORA
    {
        //SoftwareSerial loraPort;
        #define loraPort Serial5
        LoRa_E220 e220ttl;
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