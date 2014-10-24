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

#ifndef __vdcd__enocean4bs__
#define __vdcd__enocean4bs__

#include "vdcd_common.hpp"

#include "enoceandevice.hpp"


using namespace std;

namespace p44 {


  /// single EnOcean device channel
  class Enocean4bsHandler : public EnoceanChannelHandler
  {
    typedef EnoceanChannelHandler inherited;

  protected:

    /// protected constructor
    /// @note create new channels using factory static methods of specialized subclasses
    Enocean4bsHandler(EnoceanDevice &aDevice) : inherited(aDevice) {};

  public:

    /// factory: (re-)create logical device from address|channel|profile|manufacturer tuple
    /// @param aClassContainerP the class container
    /// @param aSubDeviceIndex subdevice number to create (multiple logical EnoceanDevices might exists for the same EnoceanAddress)
    /// @param aEEProfile RORG/FUNC/TYPE EEP profile number
    /// @param aEEManufacturer manufacturer number (or manufacturer_unknown)
    /// @param aSendTeachInResponse enable sending teach-in response for this device
    /// @return returns NULL if no device can be created for the given aSubDeviceIndex, new device otherwise
    static EnoceanDevicePtr newDevice(
      EnoceanDeviceContainer *aClassContainerP,
      EnoceanAddress aAddress,
      EnoceanSubDevice aSubDeviceIndex,
      EnoceanProfile aEEProfile, EnoceanManufacturer aEEManufacturer,
      bool aSendTeachInResponse
    );

    /// prepare aOutgoingPacket for sending 4BS data.
    /// creates new packet if none passed in, and returns already collected data
    /// @param aOutgoingPacket existing packet will be used, if NULL, new packet will be created
    /// @param a4BSdata will be set to already collected 4BS data (from already consulted channels or device global bits like LRN)
    void prepare4BSpacket(Esp3PacketPtr &aOutgoingPacket, uint32_t &a4BSdata);

  };
  typedef boost::intrusive_ptr<Enocean4bsHandler> Enocean4bsHandlerPtr;



  class Enocean4BSDevice : public EnoceanDevice
  {
    typedef EnoceanDevice inherited;

  public:

    /// constructor
    Enocean4BSDevice(EnoceanDeviceContainer *aClassContainerP);

    /// device type identifier
		/// @return constant identifier for this type of device (one container might contain more than one type)
    virtual const char *deviceTypeIdentifier() { return "enocean_4bs"; };

    /// device specific teach in response
    /// @note will be called from newDevice() when created device needs a teach-in response
    virtual void sendTeachInResponse();

  };


  #pragma mark - handler implementations


  /// generic, table driven sensor channel handler
  struct Enocean4BSSensorDescriptor;
  class Enocean4bsSensorHandler : public Enocean4bsHandler
  {
    typedef Enocean4bsHandler inherited;

    /// private constructor, friend class' Enocean4bsHandler::newDevice is the place to call it from
    Enocean4bsSensorHandler(EnoceanDevice &aDevice);

  public:

    /// the sensor channel descriptor
    const Enocean4BSSensorDescriptor *sensorChannelDescriptorP;

    /// factory: (re-)create logical device from address|channel|profile|manufacturer tuple
    /// @param aClassContainerP the class container
    /// @param aSubDeviceIndex subdevice number to create (multiple logical EnoceanDevices might exists for the same EnoceanAddress)
    /// @param aEEProfile RORG/FUNC/TYPE EEP profile number
    /// @param aEEManufacturer manufacturer number (or manufacturer_unknown)
    /// @param aSendTeachInResponse enable sending teach-in response for this device
    /// @return returns NULL if no device can be created for the given aSubDeviceIndex, new device otherwise
    static EnoceanDevicePtr newDevice(
      EnoceanDeviceContainer *aClassContainerP,
      EnoceanAddress aAddress,
      EnoceanSubDevice aSubDeviceIndex,
      EnoceanProfile aEEProfile, EnoceanManufacturer aEEManufacturer,
      bool aNeedsTeachInResponse
    );


    /// factory: add sensor channel to device by descriptor
    /// @param aDevice the device to add the channel to
    /// @param aSensorDescriptor a sensor descriptor
    /// @param aSetDeviceDescription if set, this sensor channel is the "main" channel and will set description on the device itself
    static void addSensorChannel(
      EnoceanDevicePtr aDevice,
      const Enocean4BSSensorDescriptor &aSensorDescriptor,
      bool aSetDeviceDescription
    );


    /// handle radio packet related to this channel
    /// @param aEsp3PacketPtr the radio packet to analyze and extract channel related information
    virtual void handleRadioPacket(Esp3PacketPtr aEsp3PacketPtr);

    /// check if channel is alive = has received life sign within timeout window
    virtual bool isAlive();

    /// short (text without LFs!) description of object, mainly for referencing it in log messages
    /// @return textual description of object
    virtual string shortDesc();

  };
  typedef boost::intrusive_ptr<Enocean4bsSensorHandler> Enocean4bsSensorHandlerPtr;



  /// heating valve handler
  class EnoceanA52001Handler : public Enocean4bsHandler
  {
    typedef Enocean4bsHandler inherited;
    friend class Enocean4bsHandler;

    enum {
      service_idle,
      service_openvalve,
      service_closevalve,
      service_finish
    } serviceState;

    /// private constructor, friend class' Enocean4bsHandler::newDevice is the place to call it from
    EnoceanA52001Handler(EnoceanDevice &aDevice);

  public:

    /// factory: (re-)create logical device from address|channel|profile|manufacturer tuple
    /// @param aClassContainerP the class container
    /// @param aSubDeviceIndex subdevice number to create (multiple logical EnoceanDevices might exists for the same EnoceanAddress)
    /// @param aEEProfile RORG/FUNC/TYPE EEP profile number
    /// @param aEEManufacturer manufacturer number (or manufacturer_unknown)
    /// @param aSendTeachInResponse enable sending teach-in response for this device
    /// @return returns NULL if no device can be created for the given aSubDeviceIndex, new device otherwise
    static EnoceanDevicePtr newDevice(
      EnoceanDeviceContainer *aClassContainerP,
      EnoceanAddress aAddress,
      EnoceanSubDevice aSubDeviceIndex,
      EnoceanProfile aEEProfile, EnoceanManufacturer aEEManufacturer,
      bool aSendTeachInResponse
    );

    /// handle radio packet related to this channel
    /// @param aEsp3PacketPtr the radio packet to analyze and extract channel related information
    virtual void handleRadioPacket(Esp3PacketPtr aEsp3PacketPtr);

    /// collect data for outgoing message from this channel
    /// @param aEsp3PacketPtr must be set to a suitable packet if it is empty, or packet data must be augmented with
    ///   channel's data when packet already exists
    virtual void collectOutgoingMessageData(Esp3PacketPtr &aEsp3PacketPtr);

    /// short (text without LFs!) description of object, mainly for referencing it in log messages
    /// @return textual description of object
    virtual string shortDesc();
  };




} // namespace p44

#endif /* defined(__vdcd__enocean4bs__) */
