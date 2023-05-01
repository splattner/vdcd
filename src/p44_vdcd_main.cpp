//  SPDX-License-Identifier: GPL-3.0-or-later
//
//  Copyright (c) 2013-2023 plan44.ch / Lukas Zeller, Zurich, Switzerland
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

#include "application.hpp"

#include "p44_vdcd_host.hpp"

// APIs to be used
#include "jsonvdcapi.hpp"
#include "pbufvdcapi.hpp"

// vDCs to be used
#if ENABLE_DALI
#include "dalivdc.hpp"
#endif
#if ENABLE_ENOCEAN
#include "enoceanvdc.hpp"
#endif
#if ENABLE_ELDAT
#include "eldatvdc.hpp"
#endif
#if ENABLE_ELDAT
#include "zfvdc.hpp"
#endif
#if ENABLE_HUE
#include "huevdc.hpp"
#endif
#if ENABLE_STATIC
#include "staticvdc.hpp"
#endif
#if ENABLE_EXTERNAL
#include "externalvdc.hpp"
#endif
#if ENABLE_SCRIPTED
#include "scriptedvdc.hpp"
#endif
#if ENABLE_EVALUATORS
#include "evaluatorvdc.hpp"
#endif
#if ENABLE_JSONBRIDGEAPI
#include "bridgevdc.hpp"
#endif
#if ENABLE_P44FEATURES
#include "featureapi.hpp"
#endif

#if ENABLE_OLA
#include "olavdc.hpp"
#endif
#if ENABLE_LEDCHAIN
#include "ledchainvdc.hpp"
#endif
#if ENABLE_P44FEATURES
#include "featureapi.hpp"
#endif

#if !DISABLE_DISCOVERY
#include "discovery.hpp"
#endif

#include "digitalio.hpp"

#define DEFAULT_USE_PROTOBUF_API 1 // 0: no, 1: yes

#define DEFAULT_DALIPORT 2101
#define DEFAULT_ENOCEANPORT 2102
#define DEFAULT_ELDATPORT 2103
#define DEFAULT_ZFPORT 2104
#define DEFAULT_JSON_VDSMSERVICE "8440"
#define DEFAULT_PBUF_VDSMSERVICE "8340"
#define DEFAULT_DBDIR "/tmp"

#define DEFAULT_LOGLEVEL LOG_NOTICE

using namespace p44;

/// Main program for plan44.ch P44-DSB-DEH in form of the "vdcd" daemon)
class P44Vdcd : public CmdLineApp
{
  typedef CmdLineApp inherited;

  // status
  typedef enum {
    status_ok,  // ok, all normal
    status_busy,  // busy, but normal
    status_interaction, // expecting user interaction
    status_error, // error
    status_fatalerror  // fatal error that needs user interaction
  } AppStatus;

  typedef enum {
    tempstatus_none,  // no temp activity status
    tempstatus_activityflash,  // activity LED flashing (yellow flash)
    tempstatus_identification,  // identification LED flashing (red light for a moment)
    tempstatus_buttonpressed, // button is pressed (steady yellow)
    tempstatus_buttonpressedlong, // button is pressed longer (steady red)
    tempstatus_factoryresetwait, // detecting possible factory reset (blinking red)
    tempstatus_success,  // success/learn-in indication (green blinking)
    tempstatus_failure,  // failure/learn-out indication (red blinking)
  } TempStatus;

  #if ENABLE_STATIC
  // command line defined devices
  DeviceConfigMap mStaticDeviceConfigs;
  #endif
  #if ENABLE_LEDCHAIN
  LEDChainArrangementPtr mLedChainArrangement;
  #endif

  // App status
  bool mFactoryResetWait;
  AppStatus mAppStatus;
  TempStatus mCurrentTempStatus;
  MLTicket mTempStatusTicket;

  #if SELFTESTING_ENABLED
  bool mSelfTesting;
  #endif

  // the device container
  // Note: must be a intrusive ptr, as it is referenced by intrusive ptrs later. Statically defining it leads to crashes.
  P44VdcHostPtr mP44VdcHost;

  // indicators and button
  IndicatorOutputPtr mRedLED;
  IndicatorOutputPtr mGreenLED;
  ButtonInputPtr mButton;

  // learning
  MLTicket mLearningTimerTicket;
  MLTicket mShutDownTicket;

public:

  P44Vdcd() :
    #if SELFTESTING_ENABLED
    mSelfTesting(false),
    #endif
    mAppStatus(status_busy),
    mCurrentTempStatus(tempstatus_none),
    mFactoryResetWait(false)
  {
  }

  void setAppStatus(AppStatus aStatus)
  {
    mAppStatus = aStatus;
    // update LEDs
    showAppStatus();
  }

  void indicateTempStatus(TempStatus aStatus)
  {
    if (aStatus>=mCurrentTempStatus) {
      // higher priority than current temp status, apply
      mCurrentTempStatus = aStatus; // overrides app status updates for now
      mTempStatusTicket.cancel();
      // initiate
      MLMicroSeconds timer = Never;
      switch (aStatus) {
        case tempstatus_activityflash:
          // short yellow LED flash
          if (mAppStatus==status_ok) {
            // activity flashes only during normal operation
            timer = 50*MilliSecond;
            mRedLED->steadyOn();
            mGreenLED->steadyOn();
          }
          else {
            mCurrentTempStatus = tempstatus_none;
          }
          break;
        case tempstatus_identification:
          // 4 red/yellow blinks
          timer = 6*Second;
          mRedLED->steadyOn();
          mGreenLED->blinkFor(timer, 1.5*Second, 50);
          break;
        case tempstatus_buttonpressed:
          // just yellow
          mRedLED->steadyOn();
          mGreenLED->steadyOn();
          break;
        case tempstatus_buttonpressedlong:
          // just red
          mRedLED->steadyOn();
          mGreenLED->steadyOff();
          break;
        case tempstatus_factoryresetwait:
          // fast red blinking
          mGreenLED->steadyOff();
          mRedLED->blinkFor(p44::Infinite, 200*MilliSecond, 20);
          break;
        case tempstatus_success:
          timer = 1600*MilliSecond;
          mRedLED->steadyOff();
          mGreenLED->blinkFor(timer, 400*MilliSecond, 30);
          break;
        case tempstatus_failure:
          timer = 1600*MilliSecond;
          mGreenLED->steadyOff();
          mRedLED->blinkFor(timer, 400*MilliSecond, 30);
          break;
        default:
          break;
      }
      if (timer!=Never) {
        mTempStatusTicket.executeOnce(boost::bind(&P44Vdcd::endTempStatus, this), timer);
      }
    }
  }


  void endTempStatus()
  {
    mTempStatusTicket.cancel();
    mCurrentTempStatus = tempstatus_none;
    showAppStatus();
  }


  // show global status on LEDs
  void showAppStatus()
  {
    if (mCurrentTempStatus==tempstatus_none) {
      switch (mAppStatus) {
        case status_ok:
          mRedLED->steadyOff();
          mGreenLED->steadyOn();
          break;
        case status_busy:
          mGreenLED->steadyOn();
          mRedLED->steadyOn();
          break;
        case status_interaction:
          mGreenLED->blinkFor(p44::Infinite, 400*MilliSecond, 80);
          mRedLED->blinkFor(p44::Infinite, 400*MilliSecond, 80);
          break;
        case status_error:
          LOG(LOG_ERR, "****** Error - operation may be limited or entirely prevented - check logs!");
          mGreenLED->steadyOff();
          mRedLED->steadyOn();
          break;
        case status_fatalerror:
          LOG(LOG_ALERT, "****** Fatal error - operation cannot continue - try restarting!");
          mGreenLED->steadyOff();
          mRedLED->blinkFor(p44::Infinite, 800*MilliSecond, 50);;
          break;
      }
    }
  }

  void eventMonitor(VdchostEvent aEvent)
  {
    switch (aEvent) {
      case vdchost_activitysignal:
        indicateTempStatus(tempstatus_activityflash);
        break;
      case vdchost_identify:
        indicateTempStatus(tempstatus_identification);
        break;
      case vdchost_descriptionchanged:
        #if !DISABLE_DISCOVERY
        ServiceAnnouncer::sharedServiceAnnouncer().refreshAdvertisingDevice();
        #endif
        break;
      default:
        break;
    }
  }

  virtual void signalOccurred(int aSignal, siginfo_t *aSiginfo)
  {
    if (aSignal==SIGUSR1) {
      if (mP44VdcHost) mP44VdcHost->postEvent(vdchost_logstats);
    }
    inherited::signalOccurred(aSignal, aSiginfo);
  }



  virtual bool processOption(const CmdLineOptionDescriptor &aOptionDescriptor, const char *aOptionValue)
  {
    #if ENABLE_STATIC
    if (strcmp(aOptionDescriptor.longOptionName,"digitalio")==0) {
      mStaticDeviceConfigs.insert(make_pair("digitalio", aOptionValue));
    }
    if (strcmp(aOptionDescriptor.longOptionName,"analogio")==0) {
      mStaticDeviceConfigs.insert(make_pair("analogio", aOptionValue));
    }
    else if (strcmp(aOptionDescriptor.longOptionName,"consoleio")==0) {
      mStaticDeviceConfigs.insert(make_pair("console", aOptionValue));
    }
    else
    #endif
    #if ENABLE_LEDCHAIN
    if (strcmp(aOptionDescriptor.longOptionName,"ledchain")==0) {
      LEDChainArrangement::addLEDChain(mLedChainArrangement, aOptionValue);
    }
    else
    #endif
    {
      return inherited::processOption(aOptionDescriptor, aOptionValue);
    }
    return true;
  }


  virtual int main(int argc, char **argv)
  {
    const char *usageText =
      "Usage: ${toolname} [options]\n";
    const CmdLineOptionDescriptor options[] = {
      { 0  , "dsuid",            true,  "dSUID;set dSUID for this vDC host (usually UUIDv1 generated on the host)" },
      { 0  , "instance",         true,  "instancenumber;set instance number (default 0, use 1,2,... for multiple vdchosts on same host/mac)" },
      { 0  , "ifnameformac",     true,  "network if;set network interface to get MAC address from" },
      { 0  , "ifnameforconn",    true,  "network if;set network interface to get IP from and check for connectivity" },
      { 0  , "sgtin",            true,  "part,gcp,itemref,serial;set dSUID for this vDC as SGTIN" },
      { 0  , "productname",      true,  "name;set product name for this vdc host and its vdcs" },
      { 0  , "productversion",   true,  "version;set version string for this vdc host and its vdcs" },
      { 0  , "nextversionfile",  true,  "path;file containing next installable product version (if any)" },
      { 0  , "deviceid",         true,  "device id;a string that may identify the device to the end user, e.g. a serial number" },
      { 0  , "description",      true,  "description(template);used in service announcement, can contain %V,%M,%N,%S to insert vendor/model/name/serial. "
                                        "When not set or empty, defaults to built-in standard description." },
      { 0  , "vdcdescription",   true,  "vdcmodelname(template);can contain %V,%M,%m,%S to insert vendor/productname/modelsuffix/serial. "
                                        "When not set or empty, defaults to built-in standard description." },
      #if ENABLE_DALI
      { 'a', "dali",             true,  "bridge;DALI bridge serial port device or proxy host[:port]" },
      { 0  , "daliportidle",     true,  "seconds;DALI serial port will be closed after this timeout and re-opened on demand only" },
      { 0  , "dalitxadj",        true,  "adjustment;DALI signal adjustment for sending" },
      { 0  , "dalirxadj",        true,  "adjustment;DALI signal adjustment for receiving" },
      #endif
      #if ENABLE_ENOCEAN
      { 'b', "enocean",          true,  "bridge;EnOcean modem serial port device or proxy host[:port]" },
      { 0,   "enoceanreset",     true,  "pinspec;set I/O pin connected to EnOcean module reset" },
      #endif
      #if ENABLE_HUE
      { 0,   "huelights",        false, "enable support for hue LED lamps (via hue bridge)" },
      { 0,   "hueapiurl",        true,  NULL, /* dummy, but kept to prevent breaking startup in installations that use this option */ },
      #endif
      #if ENABLE_OLA
      { 0,   "ola",              false, "enable support for OLA (Open Lighting Architecture) server" },
      #endif
      #if ENABLE_LEDCHAIN
      CMDLINE_LEDCHAIN_OPTIONS,
      { 0,   "noledchaindevices",false, "no chain light devices, --ledchain is reserved for programmatic use only" },
      #endif
      #if ENABLE_EVALUATORS
      { 0,   "evaluators",       false, "enable sensor value evaluator devices" },
      #endif
      #if ENABLE_ELDAT
      { 0,   "eldat",            true,  "interface;ELDAT interface serial port device or proxy host[:port]" },
      #endif
      #if ENABLE_ZF
      { 0,   "zf",               true,  "interface;ZF interface serial port device or proxy host[:port]" },
      #endif
      #if ENABLE_EXTERNAL
      { 0,   "externaldevices",  true, "port/socketpath;enable support for external devices connecting via specified port or local socket path" },
      { 0,   "externalnonlocal", false, "allow external device connections from non-local clients" },
      #endif
      #if ENABLE_SCRIPTED
      { 0,   "scripteddevices",  false, "enable support for devices implemented in p44script" },
      #endif
      #if ENABLE_STATIC
      { 0,   "staticdevices",    false, "enable support for statically defined devices" },
      { 'g', "digitalio",        true,  "iospec:(button|light|relay);add static digital input or output device\n"
                                        "iospec is of form [+][/][bus.[device[-opts].]]pin\n"
                                        "prefix with / for inverted polarity (default is noninverted)\n"
                                        "prefix with + to enable pullup (for inputs, if pin supports it)"
                                        "\n- gpio.gpionumber : generic Linux GPIO"
      #if !DISABLE_I2C
                                        "\n- i2cN.DEVICE[-OPT]@i2caddr.pinNumber : numbered pin of device at i2caddr on i2c bus N "
                                        "(supported for DEVICE : TCA9555, PCF8574, MCP23017)"
      #endif
      #if !DISABLE_SPI
                                        "\n- spiXY.DEVICE[-OPT]@spiaddr.pinNumber : numbered pin of device at spiaddr on spidevX.Y"
                                        "(supported for DEVICE : MCP23S17)"
      #endif
                                        },
      { 0  , "analogio",         true,  "iospec:(dimmer|rgbdimmer|valve);add static analog input or output device\n"
                                        "iospec is of form [bus.[device.]]pin:"
      #if !DISABLE_I2C
                                        "\n- i2cN.DEVICE[-OPT]@i2caddr.pinNumber : numbered pin of device at i2caddr on i2c bus N "
                                        "(supported for DEVICE : PCA9685)"
      #endif
                                        },
      { 'k', "consoleio",        true,  "name[:(dimmer|colordimmer|button|valve)];add static debug device which reads and writes console "
                                        "(for inputs: first char of name=action key)" },
      #endif // ENABLE_STATIC
      { 0  , "protobufapi",      true,  NULL /* enabled;1=use Protobuf API, 0=use JSON RPC 2.0 API */ },
      { 0  , "saveoutputs",      false, "save/restore output (channel) states by default" },
      #if ENABLE_LOCALCONTROLLER
      { 0  , "localcontroller",  false,"enable local controller (offline) features" },
      #endif
      #if !DISABLE_DISCOVERY
      { 0  , "noauto",           false, "prevent auto-connection to this vdc host" },
      { 0  , "noigmphelp",       false, NULL /* FIXME: kept as dummy to avoid breaking manually configured installations */ },
      { 0  , "nodiscovery",      false, "completely disable discovery (no publishing of services)" },
      { 0  , "hostname",         true,  "hostname;host name to use to publish this vdc host" },
      { 0  , "sshport",          true,  "portno;publish ssh access at given port" },
      #endif
      { 0  , "webuiport",        true,  "portno;publish a Web-UI service at given port" },
      { 0  , "webuipath",        true,  "path;file path for webui (must start with /, defaults to none)" },
      { 0  , "novdcapi",         false, "disable vDC API (and advertisement of it)" },
      { 'C', "vdsmport",         true,  "port;port number/service name for vdSM to connect to (default pbuf:" DEFAULT_PBUF_VDSMSERVICE ", JSON:" DEFAULT_JSON_VDSMSERVICE ")" },
      { 'i', "vdsmnonlocal",     false, "allow vdSM connections from non-local clients" },
      { 0  , "maxapiversion",    true,  "apiversion;set max API version to support, 0=support all implemented ones" },
      { 0  , "allowcloud",       false, "allow use of non-explicitly configured/expected cloud services such as N-UPnP" },
      { 'w', "startupdelay",     true,  "seconds;delay startup" },
      { 's', "sqlitedir",        true,  "dirpath;set SQLite DB directory (default = " DEFAULT_DBDIR ")" },
      { 0  , "icondir",          true,  "icon directory;specifiy path to directory containing device icons" },
      { 0  , "configdir",        true,  "dirpath;set directory for config files (defaults to sqlitedir)" },
      #if P44SCRIPT_FULL_SUPPORT
      { 0  , "initscript",       true,  "filepath;script to run after all devices collected and initialized (path relative to resource path)" },
      { 0  , "setupscript",      true,  "filepath;setup script run once and deleted when it returns true (path relative to resource path)" },
      #endif
      #if ENABLE_JSONCFGAPI
      { 'W', "cfgapiport",       true,  "port;server port number for web configuration JSON API (default=none)" },
      { 0  , "cfgapinonlocal",   false, "allow web configuration JSON API from non-local clients" },
      #endif
      #if ENABLE_JSONBRIDGEAPI
      { 0  , "bridgeapiport",    true,  "port;server port number for bridge API (default=none)" },
      { 0  , "bridgeapinonlocal",false, "allow bridge JSON API from non-local clients" },
      #endif
      #if ENABLE_UBUS
      { 0  , "ubusapi",          false, "enable ubus API" },
      #endif
      #if ENABLE_P44FEATURES
      P44FEATURE_CMDLINE_OPTIONS,
      #endif
      { 0  , "greenled",         true,  "pinspec;set I/O pin connected to green part of status LED" },
      { 0  , "redled",           true,  "pinspec;set I/O pin connected to red part of status LED" },
      { 0  , "button",           true,  "pinspec;set I/O pin connected to learn button" },
      #if SELFTESTING_ENABLED
      { 0,   "selftest",         false, "run in self test mode" },
      #endif
      DAEMON_APPLICATION_LOGOPTIONS,
      { 0  , "mainloopstats",    true,  "interval;0=no stats, 1..N interval (5Sec steps)" },
      CMDLINE_APPLICATION_STDOPTIONS,
      CMDLINE_APPLICATION_PATHOPTIONS,
      { 0, NULL } // list terminator
    };

    // parse the command line, exits when syntax errors occur
    setCommandDescriptors(usageText, options);
    if (parseCommandLine(argc, argv)) {

      if ((numOptions()<1) || (numArguments()>0)) {
        // show usage
        showUsage();
        terminateApp(EXIT_SUCCESS);
      }
      else {

        // create the root object
        bool withLocalController = getOption("localcontroller");
        mP44VdcHost = P44VdcHostPtr(new P44VdcHost(withLocalController, getOption("saveoutputs")));

        #if SELFTESTING_ENABLED
        // test or operation
        mSelfTesting = getOption("selftest");
        int errlevel = mSelfTesting ? LOG_EMERG: LOG_ERR; // testing by default only reports to stdout
        #else
        int errlevel = LOG_ERR;
        #endif

        // daemon log options
        processStandardLogOptions(true, errlevel);

        // use of non-explicitly configured cloud services (e.g. N-UPnP)
        mP44VdcHost->setAllowCloud(getOption("allowcloud"));

        // startup delay?
        int startupDelay = 0; // no delay
        getIntOption("startupdelay", startupDelay);

        // web ui
        int webUiPort = 0;
        getIntOption("webuiport", webUiPort);
        mP44VdcHost->webUiPort = webUiPort;
        getStringOption("webuipath", mP44VdcHost->webUiPath);

        // max API version
        int maxApiVersion = 0; // no limit
        if (getIntOption("maxapiversion", maxApiVersion)) {
          mP44VdcHost->setMaxApiVersion(maxApiVersion);
        }


        // before starting anything, delay
        if (startupDelay>0) {
          LOG(LOG_NOTICE, "Delaying startup by %d seconds (-w command line option)", startupDelay);
          sleep(startupDelay);
        }

        // Connect LEDs and button
        const char *pinName;
        pinName = "missing";
        getStringOption("greenled", pinName);
        mGreenLED = IndicatorOutputPtr(new IndicatorOutput(pinName, false));
        pinName = "missing";
        getStringOption("redled", pinName);
        mRedLED = IndicatorOutputPtr(new IndicatorOutput(pinName, false));
        pinName = "missing";
        getStringOption("button", pinName);
        mButton = ButtonInputPtr(new ButtonInput(pinName));

        // now show status for the first time
        showAppStatus();

        // Check for factory reset as very first action, to avoid that corrupt data might already crash the daemon
        // before we can do the factory reset
        // Note: we do this even for BUTTON_NOT_AVAILABLE_AT_START mode, because it gives the opportunity
        //   to prevent crashing the daemon with a little bit of timing (wait until uboot done, then press)
        if (mButton->isSet()) {
          LOG(LOG_WARNING, "Button held at startup -> enter factory reset wait mode");
          // started with button pressed - go into factory reset wait mode
          mFactoryResetWait = true;
          indicateTempStatus(tempstatus_factoryresetwait);
        }
        else {
          // Init the device container root object
          // - set DB dir
          const char *dbdir = DEFAULT_DBDIR;
          getStringOption("sqlitedir", dbdir);
          mP44VdcHost->setPersistentDataDir(dbdir);

          // - set conf dir
          const char *confdir = dbdir;
          getStringOption("configdir", confdir);
          mP44VdcHost->setConfigDir(confdir);

          // - set icon directory
          const char *icondir = NULL;
          getStringOption("icondir", icondir);
          mP44VdcHost->setIconDir(icondir);
          string s;

          // - set dSUID mode
          DsUidPtr externalDsUid;
          if (getStringOption("dsuid", s)) {
            externalDsUid = DsUidPtr(new DsUid(s));
          }
          else if (getStringOption("sgtin", s)) {
            int part;
            uint64_t gcp;
            uint32_t itemref;
            uint64_t serial;
            sscanf(s.c_str(), "%d,%llu,%u,%llu", &part, &gcp, &itemref, &serial);
            externalDsUid = DsUidPtr(new DsUid(s));
            externalDsUid->setGTIN(gcp, itemref, part);
            externalDsUid->setSerial(serial);
          }
          int instance = 0;
          string macif;
          getStringOption("ifnameformac", macif);
          getIntOption("instance", instance);
          mP44VdcHost->setIdMode(externalDsUid, macif, instance);

          // - network interface
          if (getStringOption("ifnameforconn", s)) {
            mP44VdcHost->setNetworkIf(s);
          }

          // - set product name and version
          if (getStringOption("productname", s)) {
            mP44VdcHost->setProductName(s);
          }
          // - set product version
          if (getStringOption("productversion", s)) {
            mP44VdcHost->setProductVersion(s);
          }
          // - set product device id (e.g. serial)
          if (getStringOption("deviceid", s)) {
            mP44VdcHost->setDeviceHardwareId(s);
          }
          // - set description (template)
          if (getStringOption("description", s)) {
            mP44VdcHost->setDescriptionTemplate(s);
          }
          // - set vdc modelName (template)
          if (getStringOption("vdcdescription", s)) {
            mP44VdcHost->setVdcModelNameTemplate(s);
          }

          // - set custom mainloop statistics output interval
          int mainloopStatsInterval;
          if (getIntOption("mainloopstats", mainloopStatsInterval)){
            mP44VdcHost->setMainloopStatsInterval(mainloopStatsInterval);
          }

          // - set API (if not disabled)
          if (!getOption("novdcapi")) {
            int protobufapi = DEFAULT_USE_PROTOBUF_API;
            getIntOption("protobufapi", protobufapi);
            const char *vdcapiservice;
            if (protobufapi) {
              mP44VdcHost->mVdcApiServer = VdcApiServerPtr(new VdcPbufApiServer());
              vdcapiservice = (char *) DEFAULT_PBUF_VDSMSERVICE;
            }
            else {
              mP44VdcHost->mVdcApiServer = VdcApiServerPtr(new VdcJsonApiServer());
              vdcapiservice = (char *) DEFAULT_JSON_VDSMSERVICE;
            }
            // set up server for vdSM to connect to
            getStringOption("vdsmport", vdcapiservice);
            mP44VdcHost->mVdcApiServer->setConnectionParams(NULL, vdcapiservice, SOCK_STREAM, AF_INET);
            mP44VdcHost->mVdcApiServer->setAllowNonlocalConnections(getOption("vdsmnonlocal"));
          }

          // Prepare Web configuration JSON API server
          #if ENABLE_JSONCFGAPI
          const char *configApiPort = getOption("cfgapiport");
          if (configApiPort) {
            mP44VdcHost->enableConfigApi(configApiPort, getOption("cfgapinonlocal")!=NULL);
          }
          #endif

          #if ENABLE_JSONBRIDGEAPI
          const char *bridgeApiPort = getOption("bridgeapiport");
          if (bridgeApiPort) {
            mP44VdcHost->enableBridgeApi(bridgeApiPort, getOption("bridgeapinonlocal")!=NULL);
          }
          #endif

          #if ENABLE_UBUS
          // Prepare ubus API
          if (getOption("ubusapi")) {
            mP44VdcHost->enableUbusApi();
          }
          #endif

          #if ENABLE_P44FEATURES
          // - instantiate (hardware) features we might need already for scripted devices
          #if ENABLE_LEDCHAIN
          FeatureApi::addFeaturesFromCommandLine(mLedChainArrangement);
          #else
          FeatureApi::addFeaturesFromCommandLine();
          #endif
          #endif

          // Create class containers

          // - first, prepare (make sure dSUID is available)
          mP44VdcHost->prepareForVdcs(false);

          #if ENABLE_DALI
          // - Add DALI devices class if DALI bridge serialport/host is specified
          const char *daliname = getOption("dali");
          if (daliname) {
            int sec = 0;
            getIntOption("daliportidle", sec);
            DaliVdcPtr daliVdc = DaliVdcPtr(new DaliVdc(1, mP44VdcHost.get(), 1)); // Tag 1 = DALI
            daliVdc->mDaliComm.setConnectionSpecification(daliname, DEFAULT_DALIPORT, sec*Second);
            int adj;
            if (getIntOption("dalitxadj", adj)) daliVdc->mDaliComm.setDaliSendAdj(adj);
            if (getIntOption("dalirxadj", adj)) daliVdc->mDaliComm.setDaliSampleAdj(adj);
            daliVdc->addVdcToVdcHost();
          }
          #endif

          #if ENABLE_ENOCEAN
          // - Add EnOcean devices class if modem serialport/host is specified
          const char *enoceanname = getOption("enocean");
          const char *enoceanresetpin = getOption("enoceanreset");
          if (enoceanname) {
            EnoceanVdcPtr enoceanVdc = EnoceanVdcPtr(new EnoceanVdc(1, mP44VdcHost.get(), 2)); // Tag 2 = EnOcean
            enoceanVdc->enoceanComm.setConnectionSpecification(enoceanname, DEFAULT_ENOCEANPORT, enoceanresetpin);
            // add
            enoceanVdc->addVdcToVdcHost();
          }
          #endif

          #if ENABLE_ELDAT
          // - Add Eldat devices class if modem serialport/host is specified
          const char *eldatname = getOption("eldat");
          if (eldatname) {
            EldatVdcPtr eldatVdc = EldatVdcPtr(new EldatVdc(1, mP44VdcHost.get(), 9)); // Tag 9 = ELDAT
            eldatVdc->eldatComm.setConnectionSpecification(eldatname, DEFAULT_ELDATPORT);
            // add
            eldatVdc->addVdcToVdcHost();
          }
          #endif

          #if ENABLE_ZF
          // - Add ZF devices class if modem serialport/host is specified
          const char *zfname = getOption("zf");
          if (zfname) {
            ZfVdcPtr zfVdc = ZfVdcPtr(new ZfVdc(1, mP44VdcHost.get(), 10)); // Tag 10 = ZF
            zfVdc->zfComm.setConnectionSpecification(zfname, DEFAULT_ZFPORT);
            // add
            zfVdc->addVdcToVdcHost();
          }
          #endif

          #if ENABLE_HUE
          // - Add hue support
          if (getOption("huelights")) {
            HueVdcPtr hueVdc = HueVdcPtr(new HueVdc(1, mP44VdcHost.get(), 3)); // Tag 3 = hue
            hueVdc->addVdcToVdcHost();
          }
          #endif

          #if ENABLE_OLA
          // - Add OLA support
          if (getOption("ola")) {
            OlaVdcPtr olaVdc = OlaVdcPtr(new OlaVdc(1, mP44VdcHost.get(), 5)); // Tag 5 = ola
            olaVdc->addVdcToVdcHost();
          }
          #endif

          #if ENABLE_LEDCHAIN
          // - Add Led chain light device support
          if (mLedChainArrangement && !getOption("noledchaindevices")) {
            LedChainVdcPtr ledChainVdc = LedChainVdcPtr(new LedChainVdc(1, mLedChainArrangement, mP44VdcHost.get(), 6)); // Tag 6 = led chain
            // led chain arrangement options
            mLedChainArrangement->processCmdlineOptions(); // as advertised in CMDLINE_LEDCHAIN_OPTIONS
            ledChainVdc->addVdcToVdcHost();
          }
          #endif

          #if ENABLE_STATIC
          // - Add static devices if we explictly want it or have collected any config from the command line
          if (getOption("staticdevices") || mStaticDeviceConfigs.size()>0) {
            StaticVdcPtr staticVdc = StaticVdcPtr(new StaticVdc(1, mStaticDeviceConfigs, mP44VdcHost.get(), 4)); // Tag 4 = static
            staticVdc->addVdcToVdcHost();
            mStaticDeviceConfigs.clear(); // no longer needed, free memory
          }
          #endif

          #if ENABLE_EVALUATORS
          // - Add evaluator devices
          if (getOption("evaluators")) {
            EvaluatorVdcPtr evaluatorVdc = EvaluatorVdcPtr(new EvaluatorVdc(1, mP44VdcHost.get(), 8)); // Tag 8 = evaluators
            evaluatorVdc->addVdcToVdcHost();
          }
          #endif

          #if ENABLE_EXTERNAL
          // - Add support for external devices connecting via socket
          const char *extdevname = getOption("externaldevices");
          if (extdevname) {
            ExternalVdcPtr externalVdc = ExternalVdcPtr(new ExternalVdc(1, extdevname, getOption("externalnonlocal"), mP44VdcHost.get(), 7)); // Tag 7 = external
            externalVdc->addVdcToVdcHost();
          }
          #endif

          #if ENABLE_SCRIPTED
          // - Add support for scripted devices (p44script implementations of "external" devices)
          if (getOption("scripteddevices")) {
            ScriptedVdcPtr scriptedVdc = ScriptedVdcPtr(new ScriptedVdc(1, mP44VdcHost.get(), 11)); // Tag 11 = scripted
            scriptedVdc->addVdcToVdcHost();
          }
          #endif

          #if ENABLE_JSONBRIDGEAPI
          if (
            mP44VdcHost->getBridgeApi()
            #if ENABLE_LOCALCONTROLLER
            && !withLocalController
            #endif
          ) {
            // the bridge vdc gets added when the bridge API is enabled and we don't have the localcontroller
            BridgeVdcPtr bridgeVdc = BridgeVdcPtr(new BridgeVdc(1, mP44VdcHost.get(), 12)); // Tag 12 = bridge devices
            bridgeVdc->addVdcToVdcHost();
          }
          #endif

          // install event monitor
          mP44VdcHost->setEventMonitor(boost::bind(&P44Vdcd::eventMonitor, this, _1));
        }
      } // command line ok
    } // option processing did not terminate app
    // app now ready to run (or cleanup when already terminated)
    return run();
  }


  #define LEARN_TIMEOUT (20*Second)


  void deviceLearnHandler(bool aLearnIn, ErrorPtr aError)
  {
    // back to normal...
    stopLearning(false);
    // ...but as we acknowledge the learning with the LEDs, schedule a update for afterwards
    mShutDownTicket.executeOnce(boost::bind(&P44Vdcd::showAppStatus, this), 2*Second);
    // acknowledge the learning (if any, can also be timeout or manual abort)
    if (Error::isOK(aError)) {
      if (aLearnIn) {
        // show device learned
        indicateTempStatus(tempstatus_success);
      }
      else {
        // show device unlearned
        indicateTempStatus(tempstatus_failure);
      }
    }
    else {
      LOG(LOG_ERR, "Learning error: %s", aError->text());
    }
  }


  void stopLearning(bool aFromTimeout)
  {
    mP44VdcHost->stopLearning();
    mLearningTimerTicket.cancel();
    setAppStatus(status_ok);
    if (aFromTimeout) {
      // letting learn run into timeout will re-collect all devices incrementally
      collectDevices(rescanmode_incremental);
    }
  }



  #define UPGRADE_CHECK_HOLD_TIME (5*Second)
  #define FACTORY_RESET_MODE_TIME (20*Second)
  #if BUTTON_NOT_AVAILABLE_AT_START
  #define FACTORY_RESET_HOLD_TIME (FACTORY_RESET_MODE_TIME+20*Second) // 20 seconds to enter factory reset mode, 20 more to actually trigger it
  #else
  #define FACTORY_RESET_HOLD_TIME (20*Second) // pressed-at start enters factory reset mode, only 20 secs until trigger
  #endif
  #define FACTORY_RESET_MAX_HOLD_TIME (FACTORY_RESET_HOLD_TIME+10*Second)



  virtual bool buttonHandler(bool aState, bool aHasChanged, MLMicroSeconds aTimeSincePreviousChange)
  {
    LOG(LOG_NOTICE, "Device button event: state=%d, hasChanged=%d, timeSincePreviousChange=%.1f", aState, aHasChanged, (double)aTimeSincePreviousChange/Second);
    // different handling if we are waiting for factory reset
    if (mFactoryResetWait) {
      return factoryResetButtonHandler(aState, aHasChanged, aTimeSincePreviousChange);
    }
    // LED yellow as long as button pressed
    if (aHasChanged) {
      if (aState) indicateTempStatus(tempstatus_buttonpressed);
      else endTempStatus();
    }
    if (aState==true && !aHasChanged) {
      // keypress reported again
      // - first check for very long keypress
      if (aTimeSincePreviousChange>=FACTORY_RESET_MODE_TIME) {
        // very long button press
        #if BUTTON_NOT_AVAILABLE_AT_START
        // - E2, DEH2: button is not available at system startup (has uboot functionality) -> use very long press for factory reset.
        //   Button is not exposed (ball pen hole) so is highly unlikely to get stuck accidentally.
        LOG(LOG_WARNING, "Button held for >%.1f seconds -> enter factory reset wait mode", (double)FACTORY_RESET_MODE_TIME/Second);
        mFactoryResetWait = true;
        indicateTempStatus(tempstatus_factoryresetwait);
        return true;
        #else
        // - E, DEHv3..5: button is exposed and might be stuck accidentally -> very long press just exits vdcd.
        //   needs long press present from startup for factory reset
        setAppStatus(status_error);
        LOG(LOG_WARNING, "Button held for >%.1f seconds -> clean exit(%d) in 2 seconds", (double)FACTORY_RESET_MODE_TIME/Second, P44_EXIT_LOCALMODE);
        mButton->setButtonHandler(NoOP, true); // disconnect button
        mP44VdcHost->setEventMonitor(NoOP); // no activity monitoring any more
        // for now exit(2) is switching off daemon, so we switch off the LEDs as well
        mRedLED->steadyOff();
        mGreenLED->steadyOff();
        // give mainloop some time to close down API connections
        mShutDownTicket.executeOnce(boost::bind(&P44Vdcd::terminateApp, this, P44_EXIT_LOCALMODE), 2*Second);
        return true;
        #endif
      }
      else if (aTimeSincePreviousChange>=UPGRADE_CHECK_HOLD_TIME) {
        // visually acknowledge long keypress by turning LED red
        indicateTempStatus(tempstatus_buttonpressedlong);
        LOG(LOG_WARNING, "Button held for >%.1f seconds -> upgrade check if released now", (double)UPGRADE_CHECK_HOLD_TIME/Second);
      }
    }
    if (aState==false) {
      // keypress release
      if (aTimeSincePreviousChange>=5*Second) {
        // long press (labelled "Software Update" on the case)
        setAppStatus(status_busy);
        LOG(LOG_WARNING, "Long button press detected -> upgrade to latest firmware requested -> clean exit(%d) in 500 mS", P44_EXIT_FIRMWAREUPDATE);
        mButton->setButtonHandler(NoOP, true); // disconnect button
        mP44VdcHost->setEventMonitor(NoOP); // no activity monitoring any more
        // give mainloop some time to close down API connections
        mShutDownTicket.executeOnce(boost::bind(&P44Vdcd::terminateApp, this, P44_EXIT_FIRMWAREUPDATE), 500*MilliSecond);
      }
      else {
        // short press: start/stop learning
        if (!mLearningTimerTicket) {
          // start
          setAppStatus(status_interaction);
          mLearningTimerTicket.executeOnce(boost::bind(&P44Vdcd::stopLearning, this, true), LEARN_TIMEOUT);
          mP44VdcHost->startLearning(boost::bind(&P44Vdcd::deviceLearnHandler, this, _1, _2));
        }
        else {
          // stop
          stopLearning(false);
        }
      }
    }
    return true;
  }


  virtual bool factoryResetButtonHandler(bool aState, bool aHasChanged, MLMicroSeconds aTimeSincePreviousChange)
  {
    if (aHasChanged && aState==false) {
      // released
      if (aTimeSincePreviousChange>FACTORY_RESET_HOLD_TIME && aTimeSincePreviousChange<FACTORY_RESET_MAX_HOLD_TIME) {
        // released after being in waiting-for-reset state lomng enough -> FACTORY RESET
        LOG(LOG_WARNING, "Button pressed long enough for factory reset -> FACTORY RESET = clean exit(%d) in 2 seconds", P44_EXIT_FACTORYRESET);
        // indicate red "error/danger" state
        mRedLED->steadyOn();
        mGreenLED->steadyOff();
        // give mainloop some time to close down API connections
        mShutDownTicket.executeOnce(boost::bind(&P44Vdcd::terminateApp, this, P44_EXIT_FACTORYRESET), 2*Second);
        return true;
      }
      else {
        // held in waiting-for-reset state non long enough or too long -> just restart
        LOG(LOG_WARNING, "Button not held long enough or too long for factory reset -> normal restart = clean exit(0) in 0.5 seconds");
        // indicate yellow "busy" state
        mRedLED->steadyOn();
        mGreenLED->steadyOn();
        // give mainloop some time to close down API connections
        mShutDownTicket.executeOnce(boost::bind(&P44Vdcd::terminateApp, this, EXIT_SUCCESS), 500*MilliSecond);
        return true;
      }
    }
    // if button is stuck, turn nervously yellow to indicate: something needs to be done
    if (mFactoryResetWait && !aHasChanged && aState) {
      if (aTimeSincePreviousChange>FACTORY_RESET_MAX_HOLD_TIME) {
        LOG(LOG_WARNING, "Button pressed too long -> releasing now will do normal restart");
        // held too long -> fast yellow blinking
        mGreenLED->blinkFor(p44::Infinite, 200*MilliSecond, 60);
        mRedLED->blinkFor(p44::Infinite, 200*MilliSecond, 60);
        // when button is released, a normal restart will occur, otherwise we'll remain in this state
      }
      else if (aTimeSincePreviousChange>FACTORY_RESET_HOLD_TIME) {
        LOG(LOG_WARNING, "Button pressed long enough for factory reset -> releasing now will do factory reset");
        // if released now, factory reset will occur (but if held still longer, will enter "button stuck" mode)
        // - just indicate it
        mRedLED->steadyOn();
        mGreenLED->steadyOff();
      }
    }
    return true;
  }




  virtual void initialize()
  {
    #if SELFTESTING_ENABLED
    if (mSelfTesting) {
      // self testing
      // - initialize the device container
      mP44VdcHost->initialize(boost::bind(&P44Vdcd::initialized, this, _1), false); // no factory reset
    }
    else
    #endif
    {
      // - connect button
      mButton->setButtonHandler(boost::bind(&P44Vdcd::buttonHandler, this, _1, _2, _3), true, 1*Second);
      // - if not already in factory reset wait, initialize normally
      if (!mFactoryResetWait) {
        // - initialize the device container
        mP44VdcHost->initialize(boost::bind(&P44Vdcd::initialized, this, _1), false); // no factory reset
      }
    }
  }



  virtual void initialized(ErrorPtr aError)
  {
    #if SELFTESTING_ENABLED
    if (mSelfTesting) {
      // self test mode
      if (Error::isOK(aError)) {
        // start self testing (which might do some collecting if needed for testing)
        mP44VdcHost->selfTest(boost::bind(&P44Vdcd::selfTestDone, this, _1), mButton, mRedLED, mGreenLED); // do the self test
      }
      else {
        // - init already unsuccessful, consider test failed, call test end routine directly
        selfTestDone(aError);
      }
    }
    else
    #endif // SELFTESTING_ENABLED
    if (Error::notOK(aError)) {
      // cannot initialize, this is a fatal error
      setAppStatus(status_fatalerror);
      // exit in 15 seconds
      LOG(LOG_ALERT, "****** Fatal error - vdc host initialisation failed: %s", aError->text());
      mShutDownTicket.executeOnce(boost::bind(&P44Vdcd::terminateAppWith, this, aError), 15*Second);
      return;
    }
    else {
      // Initialized ok and not testing
      #if !DISABLE_DISCOVERY
      // - initialize discovery
      initDiscovery();
      #endif
      // - start running normally
      mP44VdcHost->startRunning();
      // - collect devices
      collectDevices(rescanmode_normal);
    }
  }

  #if SELFTESTING_ENABLED
  void selfTestDone(ErrorPtr aError)
  {
    // test done, exit with success or failure
    terminateApp(Error::isOK(aError) ? EXIT_SUCCESS : EXIT_FAILURE);
  }
  #endif


  virtual void collectDevices(RescanMode aRescanMode)
  {
    // initiate device collection
    setAppStatus(status_busy);
    mP44VdcHost->collectDevices(boost::bind(&P44Vdcd::devicesCollected, this, _1), aRescanMode);
  }


  virtual void devicesCollected(ErrorPtr aError)
  {
    if (Error::isOK(aError)) {
      setAppStatus(status_ok);
    }
    else {
      setAppStatus(status_error);
    }
  }



  #if !DISABLE_DISCOVERY

  void initDiscovery()
  {
    // get discovery params
    // - host name
    string s;
    string hostname;
    if (!getStringOption("hostname", hostname)) {
      // none specified, create default
      hostname = string_format("plan44-vdcd-%s", mP44VdcHost->getDsUid().getString().c_str());
    }
    // start DS advertising if not disabled
    if (!getOption("nodiscovery")) {
      // started ok, set discovery params
      int sshPort = 0;
      getIntOption("sshport", sshPort);
      ServiceAnnouncer::sharedServiceAnnouncer().advertiseVdcHostDevice(
        hostname.c_str(),
        mP44VdcHost,
        getOption("noauto"),
        mP44VdcHost->webUiPort,
        mP44VdcHost->webUiPath,
        sshPort
      );
    }
  }

  #endif // !DISABLE_DISCOVERY


};



#ifndef IS_MULTICALL_BINARY_MODULE

int main(int argc, char **argv)
{
  // prevent all logging until command line determines level
  SETLOGLEVEL(LOG_EMERG);
  SETERRLEVEL(LOG_EMERG, false); // messages, if any, go to stderr
  // create app with current mainloop
  P44Vdcd* application = new(P44Vdcd);
  // pass control
  int status = application->main(argc, argv);
  // done
  delete application;
  return status;
}

#endif // !IS_MULTICALL_BINARY_MODULE
