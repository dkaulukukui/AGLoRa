#include "AGLoRa.h"

const String bleProtocolPrefix = "AGLoRa-";
const String bleProtocolTypePoint = "point";
const String bleProtocolTypeMemory = "memory";
const String bleProtocolVersion = "&ver=2.1";
const String bleProtocolParamCRC = "&crc=";
const String bleProtocolOK = "ok";
const String bleProtocolError = "error";

const String bleProtocolParamMemorySize = "&memsize=";
const String bleProtocolParamMemoryOverwrite = "&overwrite=";
const String bleProtocolParamMemoryIndex = "&index=";

const String bleProtocolDivider = "\r\n";

AGLORA::AGLORA(IMemory *memory, BLE_HM10 *ble)
{
  _ble = ble;
  _memory = memory;
}

void AGLORA::hello()
{
#if DEBUG_MODE && DEBUG_AGLORA
  Serial.println(F("[power on]"));

  Serial.print(F("Waiting | "));
  for (int i = 0; i < 50; i++)
  {
    Serial.print(F("#"));
    delay(50);
  }
  Serial.println();
  Serial.println(F("AGLORA tracker started..."));
#endif
}

/// @brief
/// 1. clear
/// 2. set name
/// 3. set default ttl
/// @param loraDataPacket
void AGLORA::clearDataPacket(DATA *trackerData)
{
  memset(trackerData, 0, sizeof(&trackerData));
  strcpy(trackerData->name, NAME);
#if DEBUG_MODE && DEBUG_AGLORA
  Serial.println(F("🟢[AGLoRa: time to send your location📍, new loraDataPacket prepared 📦]"));
#endif
}

void AGLORA::printPackage(LORADATA *loraDataPacket)
{
  // DEBUG_MODE
#if DEBUG_MODE && DEBUG_AGLORA // dump out what was just received
  Serial.println(F("🟢[AGLoRa: loraDataPacket now contains ↴]"));
  Serial.print(F("     Name: "));
  Serial.print(loraDataPacket->data.name);
  Serial.print(F(", lat: "));
  Serial.print(loraDataPacket->data.lat, 6);
  Serial.print(F(", lon: "));
  Serial.print(loraDataPacket->data.lon, 6);
  Serial.print(F(", sat: "));
  Serial.print(loraDataPacket->data.sat);
  Serial.print(F(", hdop: "));
  Serial.print(loraDataPacket->data.hdop);

  Serial.print(F(", date: "));
  Serial.print(loraDataPacket->data.year);
  Serial.print(F("/"));
  if (loraDataPacket->data.month < 10)
    Serial.print(F("0"));
  Serial.print(loraDataPacket->data.month);
  Serial.print(F("/"));
  if (loraDataPacket->data.day < 10)
    Serial.print(F("0"));
  Serial.print(loraDataPacket->data.day);

  Serial.print(F(", time: "));
  Serial.print(loraDataPacket->data.hour);
  Serial.print(F(":"));
  if (loraDataPacket->data.minute < 10)
    Serial.print(F("0"));
  Serial.print(loraDataPacket->data.minute);
  Serial.print(F(":"));
  if (loraDataPacket->data.second < 10)
    Serial.print(F("0"));
  Serial.print(loraDataPacket->data.second);
  Serial.print(F(" (UTC)"));

  Serial.print(F(" TTL="));
  Serial.print(loraDataPacket->ttl);

  Serial.println();
#endif
}

void AGLORA::getRequest(String request)
{
  if (request.length() == 0)
  {
    return;
  }

#if DEBUG_MODE && DEBUG_AGLORA
  Serial.println();
  Serial.print(F("🟢[AGLoRa: 📭 BLE request received <<"));
  Serial.print(request);
  Serial.println(F(">> received]"));
  Serial.println();
#endif

  if (request.startsWith(F("crc")))
  {
    checkMemoryToBLE();
    return;
  }

  if (request.startsWith(F("clear")))
  {
    _memory->clearAllPositions();
    checkMemoryToBLE();
    return;
  }

  if (request.startsWith(F("all")))
  {
    sendAllPackagesToBLE();
    return;
  }

  if (request.startsWith(F("id")))
  {
    request.remove(0, 2);
    unsigned int index = request.toInt();
    sendPackageToBLEFromStorage(index);

    return;
  }
}

void AGLORA::updateSensors(DATA *loraDataPacket)
{

    loraDataPacket->battery = 100; // just for example

    //Get GPS data and update 
    GPSDATA newGPSDATA;
    _gpsmodule->updateLocation(&newGPSDATA);

    loraDataPacket->lat = newGPSDATA.lat;
    loraDataPacket->lon = newGPSDATA.lon;
    loraDataPacket->year = newGPSDATA.year;
    loraDataPacket->day = newGPSDATA.day;
    loraDataPacket->hour = newGPSDATA.hour;
    loraDataPacket->minute = newGPSDATA.minute;
    loraDataPacket->second = newGPSDATA.second;

#if DEBUG_MODE && DEBUG_AGLORA
    Serial.print(F("🟢[AGLoRa sensors: "));
    Serial.print(F("🔋 - "));
    Serial.print(loraDataPacket->battery);
    Serial.println(F("]"));
#endif
}

void AGLORA::fakeUpdateSensors(DATA *loraDataPacket)
{

    loraDataPacket->battery = 69; // just for example

    //Get GPS data and update 

    loraDataPacket->lat = 21.2825; //float
    loraDataPacket->lon = -157.7866; //float
    loraDataPacket->year = 24; //unsigned char
    loraDataPacket->month = 4;
    loraDataPacket->day = 10; //char
    loraDataPacket->hour = 4;  //char
    loraDataPacket->minute = 20;  //char
    loraDataPacket->second = 0;  //char


}


void AGLORA::checkMemoryToBLE()
{
  String response = bleProtocolPrefix +
                    bleProtocolTypeMemory +
                    bleProtocolVersion;
  response += bleProtocolParamCRC;
  response += _memory->checkCRC() ? bleProtocolOK : bleProtocolError;
  response += bleProtocolParamMemorySize + _memory->getSize();
  response += bleProtocolParamMemoryIndex + _memory->getIndex();
  response += bleProtocolParamMemoryOverwrite + _memory->getStorageOverwrite();
  response += bleProtocolDivider;
  _ble->send(&response);
}

void AGLORA::sendPackageToBLE(DATA *trackerData, int index)
{
  String response = bleProtocolPrefix +
                    bleProtocolTypePoint +
                    bleProtocolVersion;

  response += sendToPhone(trackerData);
  response += bleProtocolParamMemoryIndex + index;
  response += bleProtocolParamCRC;
  response += _memory->checkCRC(index) ? bleProtocolOK : bleProtocolError;
  response += bleProtocolDivider;

#if DEBUG_MODE && DEBUG_AGLORA
  Serial.print(F("🟢AGLoRa: send point 📦 to BLE → "));
  Serial.print(response);
#endif

  _ble->send(&response);
}

void AGLORA::sendAllPackagesToBLE()
{
  unsigned int maxIndex = _memory->getStorageOverwrite() ? _memory->getSize() : _memory->getIndex();
  for (unsigned int i = 0; i < maxIndex; ++i)
  {
#if DEBUG_MODE && DEBUG_AGLORA
    Serial.print(F("🟢[AGLoRa: loading "));
    Serial.print(i + 1);
    Serial.print(F("/"));
    Serial.print(maxIndex);
    Serial.print(F(" 📦 from memory ]"));
#endif

    sendPackageToBLE(_memory->load(i), i);
  }

#if DEBUG_MODE && DEBUG_AGLORA
  Serial.println();
#endif
}

/*
This is a function that sends data to the app.
Data packets are sent using OsmAnd-like protocol:
id=name&lat={0}&lon={1}&timestamp={2}&speed={3}&altitude={4}
*/
String AGLORA::sendToPhone(DATA *package) {

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

void AGLORA::sendPackageToBLEFromStorage(unsigned int index)
{
#if DEBUG_MODE && DEBUG_AGLORA
  Serial.print(F("🟢[AGLoRa: loading 📦  from index "));
  Serial.print(index);
  Serial.print(F("]"));
#endif

  if ((_memory->getStorageOverwrite() == false) && (_memory->getIndex() == 0))
  {
#if DEBUG_MODE && DEBUG_AGLORA
    Serial.println(F("- error 🚨 empty memory 🚨"));
#endif

    String response = bleProtocolPrefix +
                      bleProtocolTypeMemory +
                      bleProtocolVersion;
    response += bleProtocolParamMemorySize + _memory->getSize();
    response += bleProtocolParamMemoryIndex + _memory->getIndex();
    response += bleProtocolParamMemoryOverwrite + _memory->getStorageOverwrite();
    response += bleProtocolDivider;
    _ble->send(&response);

    return;
  }

  unsigned int maxIndex = _memory->getStorageOverwrite() ? _memory->getSize() : _memory->getIndex();
  if (index > maxIndex - 1)
  {
#if DEBUG_MODE && DEBUG_AGLORA
    Serial.println(F("- error 🚨 index out of range 🚨"));
#endif
    return;
    // TODO: send error
  }

  sendPackageToBLE(_memory->load(index), index);
}


bool TESTS::hasNewDataEveryXSec(LORADATA *loraDataPacket, GPS *gps, byte interval)
{
        int _intervarSec = interval * 1000;
        if (_timeOfLastSendedPacket + _intervarSec < millis())
        {
#if DEBUG_MODE
                Serial.println();
                Serial.println(F("💛💛💛 [TEST: virtual tracker transmitted the data] 💛💛💛"));
#endif

                //gps->updateLocation(&loraDataPacket->data); // we need an actual time
                strcpy(loraDataPacket->data.name, OTHER_NAME);
                loraDataPacket->data.lat = 45.455631;
                loraDataPacket->data.lon = 54.084960;
                loraDataPacket->ttl = TTL;

#if DEBUG_MODE
                Serial.println();
#endif

                _timeOfLastSendedPacket = millis();
                return true;
        }

        return false;
}