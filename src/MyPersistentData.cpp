#include "Particle.h"
#include "MB85RC256V-FRAM-RK.h"
#include "StorageHelperRK.h"
#include "MyPersistentData.h"
#include "gateway_configuration.h"


MB85RC64 fram(Wire, 0);   

// Common Functions
/**
 * @brief Resets all counts to start a new day.
 *
 * @details Once run, it will reset all daily-specific counts and trigger an update in FRAM.
 */
void resetEverything() {                                              // The device is waking up in a new day or is a new install
  Log.info("A new day - resetting everything");
  current.set_dailyCount(0);                                            // Reset the counts in FRAM as well
  current.set_hourlyCount(0);
  current.set_lastCountTime(Time.now());
  sysStatus.set_resetCount(0);                                          // Reset the reset count as well
  sysStatus.set_messageCount(0);
}

/**
 * @brief This function is called in setup if the version of the FRAM storage map has been changed
 * 
 */
void loadSystemDefaults() {                         // This code is only executed with a new device or a new storage object structure
  if (Particle.connected()) {
    Particle.publish("Mode","Loading System Defaults", PRIVATE);
  }
  Log.info("Loading system defaults");              // Letting us know that defaults are being loaded

  sysStatus.set_nodeNumber(0);                     // Default for a Gateway
  sysStatus.set_structuresVersion(1);
  sysStatus.set_magicNumber(27617);
  sysStatus.set_firmwareRelease(1);
  sysStatus.set_resetCount(0);
  sysStatus.set_messageCount(0);
  sysStatus.set_lastHookResponse(0);
  sysStatus.set_frequencyMinutes(60);
  sysStatus.set_alertCodeGateway(0);
  sysStatus.set_alertTimestampGateway(0);
  sysStatus.set_openTime(6);
  sysStatus.set_closeTime(22);
  sysStatus.set_verizonSIM(false);

  setGatewayConfiguration();                             // Here we will fix the settings specific to the gateway
}


void resetNodeIDs() {
    // String blank = "{\"nodes\":[{}]}";
    // String blank = "{\"nodes\":[{\"node\":1,\"dID\":\"aaaaaaaaaaaaaaaaaaaaa1\",\"last\":1667835489,\"type\":1}]}";
    String blank = "{\"nodes\":[]}";
    nodeID.set_nodeIDJson(blank);

    Log.info("Resettig NodeID config to: %s", blank.c_str());
}



// *******************  SysStatus Storage Object **********************
//
// ********************************************************************

sysStatusData *sysStatusData::_instance;

// [static]
sysStatusData &sysStatusData::instance() {
    if (!_instance) {
        _instance = new sysStatusData();
    }
    return *_instance;
}

sysStatusData::sysStatusData() : StorageHelperRK::PersistentDataFRAM(::fram, 0, &sysData.sysHeader, sizeof(SysData), SYS_DATA_MAGIC, SYS_DATA_VERSION) {

};

sysStatusData::~sysStatusData() {
}

void sysStatusData::setup() {
    fram.begin();
    sysStatus.load();
    setGatewayConfiguration();                             // Here we will fix the settings specific to the node
}

void sysStatusData::loop() {
    sysStatus.flush(false);
}

uint8_t sysStatusData::get_nodeNumber() const {
    return getValue<uint8_t>(offsetof(SysData, nodeNumber));
}

void sysStatusData::set_nodeNumber(uint8_t value) {
    setValue<uint8_t>(offsetof(SysData, nodeNumber), value);
}

uint8_t sysStatusData::get_structuresVersion() const {
    return getValue<uint8_t>(offsetof(SysData, structuresVersion));
}

void sysStatusData::set_structuresVersion(uint8_t value) {
    setValue<uint8_t>(offsetof(SysData, structuresVersion), value);
}

uint16_t sysStatusData::get_magicNumber() const {
    return getValue<uint16_t>(offsetof(SysData, magicNumber));
}

void sysStatusData::set_magicNumber(uint16_t value) {
    setValue<uint16_t>(offsetof(SysData, magicNumber), value);
}

uint8_t sysStatusData::get_firmwareRelease() const {
    return getValue<uint8_t>(offsetof(SysData, firmwareRelease));
}

void sysStatusData::set_firmwareRelease(uint8_t value) {
    setValue<uint8_t>(offsetof(SysData, firmwareRelease), value);
}

uint8_t sysStatusData::get_resetCount() const {
    return getValue<uint8_t>(offsetof(SysData, resetCount));
}

void sysStatusData::set_resetCount(uint8_t value) {
    setValue<uint8_t>(offsetof(SysData, resetCount), value);
}

uint8_t sysStatusData::get_messageCount() const {
    return getValue<uint8_t>(offsetof(SysData, messageCount));
}

void sysStatusData::set_messageCount(uint8_t value) {
    setValue<uint8_t>(offsetof(SysData, messageCount), value);
}

time_t sysStatusData::get_lastHookResponse() const {
    return getValue<time_t>(offsetof(SysData, lastHookResponse));
}

void sysStatusData::set_lastHookResponse(time_t value) {
    setValue<time_t>(offsetof(SysData, lastHookResponse), value);
}

time_t sysStatusData::get_lastConnection() const {
    return getValue<time_t>(offsetof(SysData, lastConnection));
}

void sysStatusData::set_lastConnection(time_t value) {
    setValue<time_t>(offsetof(SysData, lastConnection), value);
}

uint16_t sysStatusData::get_lastConnectionDuration() const {
    return getValue<uint16_t>(offsetof(SysData,lastConnectionDuration));
}

void sysStatusData::set_lastConnectionDuration(uint16_t value) {
    setValue<uint16_t>(offsetof(SysData,lastConnectionDuration), value);
}

uint16_t sysStatusData::get_frequencyMinutes() const {
    return getValue<uint16_t>(offsetof(SysData,frequencyMinutes));
}

void sysStatusData::set_frequencyMinutes(uint16_t value) {
    setValue<uint16_t>(offsetof(SysData, frequencyMinutes), value);
}

uint16_t sysStatusData::get_updatedFrequencyMinutes() const {
    return getValue<uint16_t>(offsetof(SysData,updatedFrequencyMinutes));
}

void sysStatusData::set_updatedFrequencyMinutes(uint16_t value) {
    setValue<uint16_t>(offsetof(SysData, updatedFrequencyMinutes), value);
}

uint8_t sysStatusData::get_alertCodeGateway() const {
    return getValue<uint8_t>(offsetof(SysData, alertCodeGateway));
}

void sysStatusData::set_alertCodeGateway(uint8_t value) {
    setValue<uint8_t>(offsetof(SysData, alertCodeGateway), value);
}

time_t sysStatusData::get_alertTimestampGateway() const {
    return getValue<time_t>(offsetof(SysData, alertTimestampGateway));
}

void sysStatusData::set_alertTimestampGateway(time_t value) {
    setValue<time_t>(offsetof(SysData, alertTimestampGateway), value);
}

uint8_t sysStatusData::get_openTime() const {
    return getValue<uint8_t>(offsetof(SysData, openTime));
}

void sysStatusData::set_openTime(uint8_t value) {
    setValue<uint8_t>(offsetof(SysData, openTime), value);
}

uint8_t sysStatusData::get_closeTime() const {
    return getValue<uint8_t>(offsetof(SysData, closeTime));
}

void sysStatusData::set_closeTime(uint8_t value) {
    setValue<uint8_t>(offsetof(SysData, closeTime), value);
}

bool sysStatusData::get_verizonSIM() const {
    return getValue<bool>(offsetof(SysData, verizonSIM));
}

void sysStatusData::set_verizonSIM(bool value) {
    setValue<bool>(offsetof(SysData, verizonSIM), value);
}

uint8_t sysStatusData::get_sensorType() const {
    return getValue<uint8_t>(offsetof(SysData, sensorType));
}

void sysStatusData::set_sensorType(uint8_t value) {
    setValue<uint8_t>(offsetof(SysData, sensorType), value);
}

uint16_t sysStatusData::get_RSSI() const {
    return getValue<uint16_t>(offsetof(SysData,RSSI));
}

void sysStatusData::set_RSSI(uint16_t value) {
    setValue<uint16_t>(offsetof(SysData, RSSI), value);
}

// *****************  Current Status Storage Object *******************
// Offset of 50 bytes - make room for SysStatus
// ********************************************************************

currentStatusData *currentStatusData::_instance;

// [static]
currentStatusData &currentStatusData::instance() {
    if (!_instance) {
        _instance = new currentStatusData();
    }
    return *_instance;
}

currentStatusData::currentStatusData() : StorageHelperRK::PersistentDataFRAM(::fram, 50, &currentData.currentHeader, sizeof(CurrentData), CURRENT_DATA_MAGIC, CURRENT_DATA_VERSION) {
};

currentStatusData::~currentStatusData() {
}

void currentStatusData::setup() {
    fram.begin();
    sysStatus.load();
}

void currentStatusData::loop() {
    sysStatus.flush(false);
}

uint8_t currentStatusData::get_nodeNumber() const {
    return getValue<uint8_t>(offsetof(CurrentData, nodeNumber));
}

void currentStatusData::set_nodeNumber(uint8_t value) {
    setValue<uint8_t>(offsetof(CurrentData, nodeNumber), value);
}

uint8_t currentStatusData::get_tempNodeNumber() const {
    return getValue<uint8_t>(offsetof(CurrentData, tempNodeNumber));
}

void currentStatusData::set_tempNodeNumber(uint8_t value) {
    setValue<uint8_t>(offsetof(CurrentData, tempNodeNumber), value);
}

uint8_t currentStatusData::get_internalTempC() const {
    return getValue<uint8_t>(offsetof(CurrentData, internalTempC));
}

void currentStatusData::set_internalTempC(uint8_t value) {
    setValue<uint8_t>(offsetof(CurrentData, internalTempC), value);
}

double currentStatusData::get_stateOfCharge() const {
    return getValue<double>(offsetof(CurrentData, stateOfCharge));
}

void currentStatusData::set_stateOfCharge(double value) {
    setValue<double>(offsetof(CurrentData, stateOfCharge), value);
}

uint8_t currentStatusData::get_batteryState() const {
    return getValue<uint8_t>(offsetof(CurrentData, batteryState));
}

void currentStatusData::set_batteryState(uint8_t value) {
    setValue<uint8_t>(offsetof(CurrentData, batteryState), value);
}

uint8_t currentStatusData::get_resetCount() const {
    return getValue<uint8_t>(offsetof(CurrentData, resetCount));
}

void currentStatusData::set_resetCount(uint8_t value) {
    setValue<uint8_t>(offsetof(CurrentData, resetCount), value);
}

uint16_t currentStatusData::get_RSSI() const {
    return getValue<uint16_t>(offsetof(CurrentData, RSSI));
}

void currentStatusData::set_RSSI(uint16_t value) {
    setValue<uint16_t>(offsetof(CurrentData, RSSI), value);
}

uint8_t currentStatusData::get_messageNumber() const {
    return getValue<uint8_t>(offsetof(CurrentData, messageNumber));
}

void currentStatusData::set_messageNumber(uint8_t value) {
    setValue<uint8_t>(offsetof(CurrentData, messageNumber), value);
}

time_t currentStatusData::get_lastCountTime() const {
    return getValue<time_t>(offsetof(CurrentData, lastCountTime));
}

void currentStatusData::set_lastCountTime(time_t value) {
    setValue<time_t>(offsetof(CurrentData, lastCountTime), value);
}

uint16_t currentStatusData::get_hourlyCount() const {
    return getValue<uint16_t>(offsetof(CurrentData, hourlyCount));
}

void currentStatusData::set_hourlyCount(uint16_t value) {
    setValue<uint16_t>(offsetof(CurrentData, hourlyCount), value);
}

uint16_t currentStatusData::get_dailyCount() const {
    return getValue<uint16_t>(offsetof(CurrentData, dailyCount));
}

void currentStatusData::set_dailyCount(uint16_t value) {
    setValue<uint16_t>(offsetof(CurrentData, dailyCount), value);
}

uint8_t currentStatusData::get_alertCodeNode() const {
    return getValue<uint8_t>(offsetof(CurrentData, alertCodeNode));
}

void currentStatusData::set_alertCodeNode(uint8_t value) {
    setValue<uint8_t>(offsetof(CurrentData, alertCodeNode), value);
}

time_t currentStatusData::get_alertTimestampNode() const {
    return getValue<time_t>(offsetof(CurrentData, alertTimestampNode));
}

void currentStatusData::set_alertTimestampNode(time_t value) {
    setValue<time_t>(offsetof(CurrentData, alertTimestampNode), value);
}

bool currentStatusData::get_openHours() const {
    return getValue<bool>(offsetof(CurrentData, openHours));
}

void currentStatusData::set_openHours(bool value) {
    setValue<bool>(offsetof(CurrentData, openHours), value);
}

uint8_t currentStatusData::get_sensorType() const {
    return getValue<uint8_t>(offsetof(CurrentData, sensorType));
}

void currentStatusData::set_sensorType(uint8_t value) {
    setValue<uint8_t>(offsetof(CurrentData, sensorType), value);
}

void currentStatusData::logData(const char *msg) {
    Log.info("Current Structure values - %d, %d, %4.2f", currentData.nodeNumber, currentData.internalTempC, currentData.stateOfCharge);
}


// *******************  nodeID Storage Object **********************
//
// ******************** Offset of 150         ************************

nodeIDData *nodeIDData::_instance;

// [static]
nodeIDData &nodeIDData::instance() {
    if (!_instance) {
        _instance = new nodeIDData();
    }
    return *_instance;
}

nodeIDData::nodeIDData() : StorageHelperRK::PersistentDataFRAM(::fram, 150, &nodeData.nodeHeader, sizeof(NodeData), NODEID_DATA_MAGIC, NODEID_DATA_VERSION) {

};

nodeIDData::~nodeIDData() {
}

void nodeIDData::setup() {
    fram.begin();
    nodeID.load();
}

void nodeIDData::loop() {
    nodeID.flush(false);
}

String nodeIDData::get_nodeIDJson() const {
	String result;
	getValueString(offsetof(NodeData, nodeIDJson), sizeof(NodeData::nodeIDJson), result);
	return result;
}

bool nodeIDData::set_nodeIDJson(const char *str) {
	return setValueString(offsetof(NodeData, nodeIDJson), sizeof(NodeData::nodeIDJson), str);
}
