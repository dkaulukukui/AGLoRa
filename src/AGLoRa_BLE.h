#ifndef AGLoRa_BLE
    #define AGLoRa_BLE

    #include "AGLoRa_Config.h"

    class BLE_HM10
    {
    public:
        BLE_HM10();
        void setup();
        String read();
        void send(String * package);

    private:
        const byte MTU = 22;
        void sendCommand(const String command);
    };


#endif