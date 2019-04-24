#pragma once

#include <stdint.h>
#include <Arduino.h>

#include "CentralObject.hpp"
#include "SharedEnums.hpp"
#include "SharedStructDefinitions.hpp"

#include <bluefruit.h>

BLEService        BikeService = BLEService(0x591afb07ae374a0fb295180550e26178);
BLECharacteristic CadenceCharacteristic     = BLECharacteristic(0x20ceb34c0349427b9e9ae2e63f728d76);
BLECharacteristic GearCharacteristic        = BLECharacteristic(0x118ae3fd894d4bde9e5ad9fae68a10b5);
BLECharacteristic DesiredCharacteristic     = BLECharacteristic(0x477343f037d8448885645f8b79724180);

BLECharacteristic gear1Down  = BLECharacteristic(0x8537045a364d461e9109e6031f5ab075);
BLECharacteristic gear2Down  = BLECharacteristic(0xdf5dd0937ea045028c62115f4ef824c1);
BLECharacteristic gear3Down  = BLECharacteristic(0x6c011de3e781463dad42f4ca06da82e9);
BLECharacteristic gear4Down  = BLECharacteristic(0x7e96b8d293994e798ceaf0f8a8a7ca9c);
BLECharacteristic gear5Down  = BLECharacteristic(0x0988710aae9045298a9be6dc6f008c86);

BLECharacteristic gear1Up    = BLECharacteristic(0x662be3945d594cad9f9862512c6811e9);
BLECharacteristic gear2Up    = BLECharacteristic(0x105e5c9a33d748fd9171aabd3ac6b66d);
BLECharacteristic gear3Up    = BLECharacteristic(0xb60dbf0b8ba84e978a088303de7ed9c9);
BLECharacteristic gear4Up    = BLECharacteristic(0xbccf463933aa4586a73c199e94513c75);
BLECharacteristic gear5Up    = BLECharacteristic(0x07228de4b0634a32a65a007832e44591);

BLECharacteristic linActPos    = BLECharacteristic(0x6606ad0291244e5d82dbe51bf9de85af);
BLECharacteristic calibrateNow = BLECharacteristic(0xc05704b888b4427fb43ecc769dada993);

BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEBas blebas;    // BAS (Battery Service) helper class instance

void connect_callback(uint16_t conn_handle);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);

class BluetoothOutput {
  private:
    CentralObject* centralObject;    

  uint8_t  bps = 0;

  
  public:
    void setupClass();
    void startAdv();
    void classMain();
    
    ShiftPointArr getShiftPoints() {
      ShiftPointArr shiftPointArr;

      shiftPointArr.gearArrDown[0] = gear1Down.read8();
      shiftPointArr.gearArrDown[1] = gear2Down.read8();
      shiftPointArr.gearArrDown[2] = gear3Down.read8();
      shiftPointArr.gearArrDown[3] = gear4Down.read8();
      shiftPointArr.gearArrDown[4] = gear5Down.read8();
      
      shiftPointArr.gearArrUp[0] = gear1Up.read8();
      shiftPointArr.gearArrUp[1] = gear2Up.read8();
      shiftPointArr.gearArrUp[2] = gear3Up.read8();
      shiftPointArr.gearArrUp[3] = gear4Up.read8();
      shiftPointArr.gearArrUp[4] = gear5Up.read8();
      
      return shiftPointArr;
    }

    void  setupService();
        
    BluetoothOutput (CentralObject* _CentralObject) {
      centralObject = _CentralObject;
    }
};

void BluetoothOutput::setupClass() {
  // Initialise the Bluefruit module
  Serial.println("Initialise the Bluefruit nRF52 module");
  Bluefruit.begin();

  // Set the advertised device name (keep it short!)
  Serial.println("Setting Device Name to 'Feather52 HRM'");
  Bluefruit.setName("MyBike");

  // Set the connect/disconnect callback handlers
  Bluefruit.setConnectCallback(connect_callback);
  Bluefruit.setDisconnectCallback(disconnect_callback);

  // Configure and Start the Device Information Service
  Serial.println("Configuring the Device Information Service");
  bledis.setManufacturer("Team Shift Into Gear");
  bledis.setModel("Automatic Transmission");
  bledis.begin();

  // Start the BLE Battery Service and set it to 100%
  Serial.println("Configuring the Battery Service");
  blebas.begin();
  blebas.write(100);

  // Setup the Heart Rate Monitor service using
  // BLEService and BLECharacteristic classes
  Serial.println("Configuring the Bike Service");
  setupService();

  // Setup the advertising packet(s)
  Serial.println("Setting up the advertising payload(s)");
  startAdv();
}

void BluetoothOutput::startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include HRM Service UUID
  Bluefruit.Advertising.addService(BikeService);

  // Include Name
  Bluefruit.Advertising.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}
void connect_callback(uint16_t conn_handle)
{
  char central_name[32] = { 0 };
  Bluefruit.Gap.getPeerName(conn_handle, central_name, sizeof(central_name));

  Serial.print("Connected to ");
  Serial.println(central_name);
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle connection where this event happens
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 * https://github.com/adafruit/Adafruit_nRF52_Arduino/blob/master/cores/nRF5/nordic/softdevice/s140_nrf52_6.1.1_API/include/ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.println("Disconnected");
  Serial.println("Advertising!");
}

void BluetoothOutput::setupService(void) {
  BikeService.begin();

  CadenceCharacteristic.setProperties(CHR_PROPS_READ);
  CadenceCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  CadenceCharacteristic.setFixedLen(1);
  CadenceCharacteristic.begin();
  CadenceCharacteristic.write8(3);  
  
  GearCharacteristic.setProperties(CHR_PROPS_READ);
  GearCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  GearCharacteristic.setFixedLen(1);
  GearCharacteristic.begin();
  GearCharacteristic.write8(5);
  
  DesiredCharacteristic.setProperties(CHR_PROPS_WRITE | CHR_PROPS_READ);
  DesiredCharacteristic.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  DesiredCharacteristic.setFixedLen(1);
  DesiredCharacteristic.begin();  
  DesiredCharacteristic.write8(7);

  gear1Down.setProperties(CHR_PROPS_WRITE | CHR_PROPS_READ);
  gear1Down.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  gear1Down.setFixedLen(1);
  gear1Down.begin();  
  gear1Down.write8(7);

  gear2Down.setProperties(CHR_PROPS_WRITE | CHR_PROPS_READ);
  gear2Down.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  gear2Down.setFixedLen(1);
  gear2Down.begin();  
  gear2Down.write8(7);

  gear3Down.setProperties(CHR_PROPS_WRITE | CHR_PROPS_READ);
  gear3Down.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  gear3Down.setFixedLen(1);
  gear3Down.begin();  
  gear3Down.write8(7);

  gear4Down.setProperties(CHR_PROPS_WRITE | CHR_PROPS_READ);
  gear4Down.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  gear4Down.setFixedLen(1);
  gear4Down.begin();  
  gear4Down.write8(7);

  gear5Down.setProperties(CHR_PROPS_WRITE | CHR_PROPS_READ);
  gear5Down.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  gear5Down.setFixedLen(1);
  gear5Down.begin();  
  gear5Down.write8(7);

  gear1Up.setProperties(CHR_PROPS_WRITE | CHR_PROPS_READ);
  gear1Up.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  gear1Up.setFixedLen(1);
  gear1Up.begin();  
  gear1Up.write8(7);
         
  gear2Up.setProperties(CHR_PROPS_WRITE | CHR_PROPS_READ);
  gear2Up.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  gear2Up.setFixedLen(1);
  gear2Up.begin();  
  gear2Up.write8(7);
         
  gear3Up.setProperties(CHR_PROPS_WRITE | CHR_PROPS_READ);
  gear3Up.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  gear3Up.setFixedLen(1);
  gear3Up.begin();  
  gear3Up.write8(7);
         
  gear4Up.setProperties(CHR_PROPS_WRITE | CHR_PROPS_READ);
  gear4Up.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  gear4Up.setFixedLen(1);
  gear4Up.begin();  
  gear4Up.write8(7);
         
  gear5Up.setProperties(CHR_PROPS_WRITE | CHR_PROPS_READ);
  gear5Up.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  gear5Up.setFixedLen(1);
  gear5Up.begin();  
  gear5Up.write8(7);

  linActPos.setProperties(CHR_PROPS_WRITE | CHR_PROPS_READ);
  linActPos.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  linActPos.setFixedLen(1);
  linActPos.begin();  
  linActPos.write8(7);
  
  calibrateNow.setProperties(CHR_PROPS_WRITE | CHR_PROPS_READ);
  calibrateNow.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  calibrateNow.setFixedLen(1);
  calibrateNow.begin();  
  calibrateNow.write8(7);
}





void BluetoothOutput::classMain() {
  if ( Bluefruit.connected() ) {
    uint8_t desiredCadence = DesiredCharacteristic.read8();

    centralObject->setTargetCadence((double)desiredCadence);
    CadenceCharacteristic.write8((uint8_t)centralObject->getCadence());
    GearCharacteristic.write8((uint8_t)centralObject->getGear());

    if (calibrateNow.read8() == 0x01) {
      centralObject->updateShiftPoints(getShiftPoints());
      calibrateNow.write8(0x00);
    }
  }
  
  delay(2000);
}
