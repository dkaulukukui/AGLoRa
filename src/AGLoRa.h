#ifndef AGLoRa

    #define AGLoRa

    #include "AGLoRa_BLE.h"
    #include "AGLoRa_SRAM.h"
    #include "AGLoRa_GPS.h"

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