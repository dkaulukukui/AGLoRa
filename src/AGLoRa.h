#ifndef AGLoRa

    #define AGLoRa

    #include "AGLoRa_Config.h"
    #include "AGLoRa_BLE.h"  //BLE includes LORA.h which includes Indication.h
    #include "AGLoRa_SRAM.h"
    #include "AGLoRa_GPS.h"
    #include "AGLoRa_LORA.h" 

    class AGLORA
    {
    public:
        AGLORA(IMemory *memory, BLE_HM10 *ble);
        void hello();
        void checkMemoryToBLE();
        void clearDataPacket(DATA * trackerData);
        void updateSensors(DATA * trackerData);
        void printPackage(LORADATA * loraDataPacket);
        void getRequest(String request);
        void sendPackageToBLE(DATA * trackerData, int index);

    private:
        IMemory * _memory;
        BLE_HM10 * _ble;
        GPS * _gpsmodule;
        void sendAllPackagesToBLE();
        void sendPackageToBLEFromStorage(unsigned int index);

    };

    class TESTS
    {
    public:
        bool hasNewDataEveryXSec(LORADATA *loraDataPacket, GPS *gps, byte interval);

    private:
        unsigned long _timeOfLastSendedPacket = 0;

    };

#endif