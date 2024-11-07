// #include "AGLoRa.h"

// #include "EEPROM.h"

// struct EEPROMDATA {
//      unsigned char counter;
//      DATA data;
//      unsigned char crc;
//   };



// class EEPROMAglora : public IMemory
// {
// public:
//     EEPROMAglora();
//     void setup();
//     bool checkUnique(DATA *newPoint);
//     unsigned int save(DATA *newData);
//     DATA *load(unsigned int index);
//     void clearAllPositions();
//     bool checkCRC();
//     bool checkCRC(unsigned int index);
//     unsigned int getSize();
//     unsigned int getIndex();
//     bool getStorageOverwrite();

// private:

//     EEPROMDATA eepromdata;
//     unsigned int EEPROMStorageIndex = 0;  // index in memory (address = EEPROMStorageIndex * EEPROMDataSize)  
//     unsigned int incrementCounter = 0;             // min 0, max 254 (because default EEPROM is 255)

//     unsigned int EEPROMStorageSize;
//     byte dataSize;
//     bool storageOverwrite = false;

//     const unsigned char EEPROMDataSize = sizeof(EEPROMDATA);
// };

