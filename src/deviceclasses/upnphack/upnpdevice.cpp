//
//  upnpdevice.cpp
//  vdcd
//

#include "upnpdevice.hpp"

#include "fnv.hpp"

#include "buttonbehaviour.hpp"
#include "lightbehaviour.hpp"


using namespace p44;


UpnpDevice::UpnpDevice(UpnpDeviceContainer *aClassContainerP, string aLocation, string aUuid) :
  Device((DeviceClassContainer *)aClassContainerP),
  descriptionURL(aLocation),
  upnpDeviceUUID(aUuid),
  presenceTicket(0)
{
  // a demo device is a light which shows its dimming value as a string of 0..50 hashes on the console
  // - is a audio device
  primaryGroup = group_cyan_audio;
  // - derive a dSUID
	deriveDsUid();
}



void UpnpDevice::checkPresence(PresenceCB aPresenceResultHandler)
{
  SsdpSearchPtr srch = SsdpSearchPtr(new SsdpSearch(SyncIOMainLoop::currentMainLoop()));
  presenceTicket = MainLoop::currentMainLoop().executeOnce(boost::bind(&UpnpDevice::timeoutHandler, this, aPresenceResultHandler, srch), 3*Second);
  srch->startSearch(boost::bind(&UpnpDevice::presenceHandler, this, aPresenceResultHandler, _1, _2), upnpDeviceUUID.c_str(), true);
}



void UpnpDevice::presenceHandler(PresenceCB aPresenceResultHandler, SsdpSearch *aSsdpSearchP, ErrorPtr aError)
{
  printf("Ping response notify\n%s\n", aSsdpSearchP->response.c_str());
  aPresenceResultHandler(true);
  aSsdpSearchP->stopSearch();
  MainLoop::currentMainLoop().cancelExecutionTicket(presenceTicket);
}


void UpnpDevice::timeoutHandler(PresenceCB aPresenceResultHandler, SsdpSearchPtr aSrch)
{
  aSrch->stopSearch();
  aPresenceResultHandler(false);
  presenceTicket = 0;
}



void UpnpDevice::deriveDsUid()
{
  Fnv64 hash;

  if (getDeviceContainer().usingDsUids()) {
    // UPnP devices do have a uuid, use this as dSUID
    dSUID.setAsString(upnpDeviceUUID);
  }
  else {
    // for classic dsid, use a hash of the uuid + device class container identifier
    // - use class container's ID
    string s = classContainerP->deviceClassContainerInstanceIdentifier();
    hash.addBytes(s.size(), (uint8_t *)s.c_str());
    // - add-in the UPnP UUID
    hash.addCStr(upnpDeviceUUID.c_str());
    dSUID.setObjectClass(DSID_OBJECTCLASS_DSDEVICE);
    dSUID.setDsSerialNo(hash.getHash28()<<4); // leave lower 4 bits for input number
  }
}



#pragma mark - property access

ErrorPtr UpnpDevice::getUserPropertyMapping(int aUserPropertyIndex, string &aName, int &aIndex)
{
  if (aUserPropertyIndex==0) {
    // for UPnP devices, this maps to "descriptionURL"
    aName = "descriptionURL";
    aIndex = 0; // not an array
  }
  // unknown, let superclass handle it
  return inherited::getUserPropertyMapping(aUserPropertyIndex, aName, aIndex);
}




enum {
  descriptionURL_key,
  numProperties
};

static char upnpDevice_key;


int UpnpDevice::numProps(int aDomain)
{
  return inherited::numProps(aDomain)+numProperties;
}


const PropertyDescriptor *UpnpDevice::getPropertyDescriptor(int aPropIndex, int aDomain)
{
  static const PropertyDescriptor properties[numProperties] = {
    { "descriptionURL", ptype_string, false, descriptionURL_key, &upnpDevice_key }, // custom UPnP property revealing the description URL
  };
  return &properties[aPropIndex];
}


// access to all fields
bool UpnpDevice::accessField(bool aForWrite, JsonObjectPtr &aPropValue, const PropertyDescriptor &aPropertyDescriptor, int aIndex)
{
  if (aPropertyDescriptor.objectKey==&upnpDevice_key) {
    if (!aForWrite) {
      // read properties
      switch (aPropertyDescriptor.accessKey) {
          // Description properties
        case descriptionURL_key:
          aPropValue = JsonObject::newString(descriptionURL); return true;
          return true;
      }
    }
    else {
      // write properties
    }
  }
  // not my field, let base class handle it
  return inherited::accessField(aForWrite, aPropValue, aPropertyDescriptor, aIndex);
}



#pragma mark - description/shortDesc


string UpnpDevice::hardwareGUID()
{
  return "uuid:" + upnpDeviceUUID;
}


string UpnpDevice::modelName()
{
  return "Demo UPnP";
}


string UpnpDevice::description()
{
  string s = inherited::description();
  string_format_append(s, "- UPnP device with description URL: %s and UUID: %s\n", descriptionURL.c_str(), upnpDeviceUUID.c_str());
  return s;
}

