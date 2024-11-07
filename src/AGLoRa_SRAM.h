#ifndef AGLoRa_SRAM
    #define AGLoRa_SRAM

    #include "AGLoRa_LORA.h"
    #include "AGLoRa_Config.h"


    class IMemory {         // interface
        public:
            virtual void setup() = 0;

            virtual void clearAllPositions() = 0;
            virtual bool checkUnique(DATA *newPoint) = 0;
            virtual unsigned int save(DATA *newData) = 0;
            virtual DATA * load(unsigned int index) = 0;

            virtual bool checkCRC() = 0;    // all memory
            virtual bool checkCRC(unsigned int index) = 0;

            virtual unsigned int getSize() = 0;
            virtual unsigned int getIndex() = 0;
            virtual bool getStorageOverwrite() = 0;
    };


    struct SRAMDATA
    {
        DATA data;
        unsigned char crc;
    };


    class SRAM: public IMemory
    {
    public:
        SRAM();
        void setup();
        bool checkUnique(DATA *newPoint);
        unsigned int save(DATA *newData); //function returns the index
        DATA * load(unsigned int index);
        void clearAllPositions();
        bool checkCRC();
        bool checkCRC(unsigned int index);
        unsigned int getSize();
        unsigned int getIndex();
        bool getStorageOverwrite();

    private:
        SRAMDATA storage[SRAM_STORAGE_SIZE];
        unsigned int storageIndex = 0;
        bool storageOverwrite = false;
        byte dataSize;
        bool checkCRC(SRAMDATA *point);
    };


    
    unsigned char calculateCRC(unsigned char *buffer, unsigned char size);


#endif