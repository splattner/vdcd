//
//  Copyright (c) 2013-2014 plan44.ch / Lukas Zeller, Zurich, Switzerland
//
//  Author: Lukas Zeller <luz@plan44.ch>
//
//  This file is part of vdcd.
//
//  vdcd is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  vdcd is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with vdcd. If not, see <http://www.gnu.org/licenses/>.
//

#include "dalidevicecontainer.hpp"

#include "dalidevice.hpp"

using namespace p44;


DaliDeviceContainer::DaliDeviceContainer(int aInstanceNumber, DeviceContainer *aDeviceContainerP) :
  DeviceClassContainer(aInstanceNumber, aDeviceContainerP),
	daliComm(SyncIOMainLoop::currentMainLoop())
{
}



// device class name
const char *DaliDeviceContainer::deviceClassIdentifier() const
{
  return "DALI_Bus_Container";
}



#ifdef DEBUG
//  #warning "%%% limiting number of devices"
//  #define MAX_DEVICES_COLLECTED 1
#endif

class DaliDeviceCollector
{
  DaliComm *daliCommP;
  CompletedCB callback;
  DaliComm::ShortAddressListPtr deviceShortAddresses;
  DaliComm::ShortAddressList::iterator nextDev;
  DaliDeviceContainer *daliDeviceContainerP;
  bool incremental;
  #if MAX_DEVICES_COLLECTED
  int collectedDevices;
  #endif
public:
  static void collectDevices(DaliDeviceContainer *aDaliDeviceContainerP, DaliComm *aDaliCommP, CompletedCB aCallback, bool aIncremental, bool aForceFullScan)
  {
    // create new instance, deletes itself when finished
    new DaliDeviceCollector(aDaliDeviceContainerP, aDaliCommP, aCallback, aIncremental, aForceFullScan);
  };
private:
  DaliDeviceCollector(DaliDeviceContainer *aDaliDeviceContainerP, DaliComm *aDaliCommP, CompletedCB aCallback, bool aIncremental, bool aForceFullScan) :
    daliCommP(aDaliCommP),
    callback(aCallback),
    incremental(aIncremental),
    daliDeviceContainerP(aDaliDeviceContainerP)
  {
    #if MAX_DEVICES_COLLECTED
    collectedDevices = 0;
    #endif
    daliCommP->daliFullBusScan(boost::bind(&DaliDeviceCollector::deviceListReceived, this, _2, _3), !aForceFullScan); // allow quick scan when not forced
  }

  void deviceListReceived(DaliComm::ShortAddressListPtr aDeviceListPtr, ErrorPtr aError)
  {
    // save list of short addresses
    deviceShortAddresses = aDeviceListPtr;
    // check if any devices
    if (aError || deviceShortAddresses->size()==0)
      return completed(aError); // no devices to query, completed
    // start collecting device info now
    nextDev = deviceShortAddresses->begin();
    queryNextDev(ErrorPtr());
  }

  void queryNextDev(ErrorPtr aError)
  {
    if (!aError && nextDev!=deviceShortAddresses->end())
      daliCommP->daliReadDeviceInfo(boost::bind(&DaliDeviceCollector::deviceInfoReceived, this, _2, _3), *nextDev);
    else
      return completed(aError);
  }

  void deviceInfoReceived(DaliComm::DaliDeviceInfoPtr aDaliDeviceInfoPtr, ErrorPtr aError)
  {
    bool missingData = aError && aError->isError(DaliCommError::domain(), DaliCommErrorMissingData);
    bool badData = aError && aError->isError(DaliCommError::domain(), DaliCommErrorBadChecksum);
    if (!aError || missingData || badData) {
      if (missingData) { LOG(LOG_INFO,"Device at shortAddress %d does not have device info\n",aDaliDeviceInfoPtr->shortAddress); }
      if (badData) { LOG(LOG_INFO,"Device at shortAddress %d does not have invalid device info (checksum error)\n",aDaliDeviceInfoPtr->shortAddress); }
      #if MAX_DEVICES_COLLECTED
      if (collectedDevices<MAX_DEVICES_COLLECTED) {
        collectedDevices++;
      #else
      {
      #endif
        // - create device
        DaliDevicePtr daliDevice(new DaliDevice(daliDeviceContainerP));
        // - give it device info (such that it can calculate its dSUID)
        //   Note: device info might be empty except for short address
        daliDevice->setDeviceInfo(*aDaliDeviceInfoPtr);
        // - make it 
        // - add it to our collection (if not already there)
        daliDeviceContainerP->addDevice(daliDevice);
      }
    }
    else {
      LOG(LOG_ERR,"Error reading device info: %s\n",aError->description().c_str());
      return completed(aError);
    }
    // check next
    ++nextDev;
    queryNextDev(ErrorPtr());
  }

  void completed(ErrorPtr aError)
  {
    // completed
    callback(aError);
    // done, delete myself
    delete this;
  }

};



/// collect devices from this device class
/// @param aCompletedCB will be called when device scan for this device class has been completed
void DaliDeviceContainer::collectDevices(CompletedCB aCompletedCB, bool aIncremental, bool aExhaustive)
{
  if (!aIncremental) {
    removeDevices(false);
  }
  DaliDeviceCollector::collectDevices(this, &daliComm, aCompletedCB, aIncremental, aExhaustive);
}

