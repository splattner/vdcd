//
//  Copyright (c) 2014-2017 plan44.ch / Lukas Zeller, Zurich, Switzerland
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

#ifndef __p44vdc__config__
#define __p44vdc__config__


#if !P44_BUILD_DIGI && !P44_BUILD_RPI
  // non official release platform
  #undef ALWAYS_DEBUG // override per-file ALWAYS_DEBUG
  #define ALWAYS_DEBUG 1 // if set, DBGLOG statements will always be included, even in non-debug builds (but can be silenced via loglevel)
#endif

// auto-disable some features depending on platform
// - No i2c on Mac or DigiESP, but always on RaspberryPi
#if (defined(__APPLE__) || P44_BUILD_DIGI) && !P44_BUILD_RPI && !P44_BUILD_OW
  #define DISABLE_I2C 1 // No i2c
#endif
#if defined(__APPLE__)
  #define DISABLE_DISCOVERY 0 // Avahi usually makes no sense on Mac (but compiles with Avahi core available)
  #define ENABLE_AUXVDSM 1
  #define USE_AVAHI_CORE 1 // in case we want avahi, use avahi core
  #define BUTTON_NOT_AVAILABLE_AT_START 1 // as in newer xx2 devices
  #define ENABLE_LOCALCONTROLLER 1
#endif

// general defaults
#define ENABLE_LOCAL_BEHAVIOUR 1 // enabled local (minimalistic) light/button operations when no vdsm is connected

// Default build settings for different targets
#if P44_BUILD_DIGI
  // P44-DSB-DEH
  #define ENABLE_DALI 1
  #define ENABLE_DALI_INPUTS 0 // disabled because of DALI bridge restrictions
  #define ENABLE_ENOCEAN 1
  #define ENABLE_HUE 1
  #define ENABLE_STATIC 0 // disabled because of footprint
  #define ENABLE_FCU_SUPPORT 0 // disable because of footprint
  #define ENABLE_EXTERNAL 1
  #define ENABLE_EXTERNAL_SINGLEDEVICE 0 // disabled because of footprint
  #define ENABLE_EVALUATORS 1
  #define ENABLE_AUXVDSM 1
  #define USE_AVAHI_CORE 1 // use direct avahi-code functions (good for small embedded targets, not recommended for desktops)
  #define SELFTESTING_ENABLED 1
#elif P44_BUILD_RPI
  // P44-DSB-E
  #define ENABLE_ENOCEAN 1
  #define ENABLE_HUE 1
  #define ENABLE_LEDCHAIN 1
  #define ENABLE_ELDAT 1
  #define ENABLE_ZF 1
  #define ENABLE_STATIC 1
  #define ENABLE_FCU_SUPPORT 1
  #define ENABLE_EXTERNAL 1
  #define ENABLE_EXTERNAL_SINGLEDEVICE 1
  #define ENABLE_EVALUATORS 1
  #define ENABLE_AUXVDSM 1
  #define USE_AVAHI_CORE 1 // use direct avahi-code functions (good for small embedded targets, not recommended for desktops)
  #define SELFTESTING_ENABLED 1
#elif P44_BUILD_OW
  // P44-DSB-xx2
  #define ENABLE_DALI 1
  #define ENABLE_DALI_INPUTS 1
  #define ENABLE_ENOCEAN 1
  #define ENABLE_HUE 1
  #define ENABLE_LEDCHAIN 1
  #define ENABLE_ELDAT 1
  #define ENABLE_ZF 1
  #define ENABLE_STATIC 1
  #define ENABLE_FCU_SUPPORT 1
  #define ENABLE_EXTERNAL 1
  #define ENABLE_EXTERNAL_SINGLEDEVICE 1
  #define ENABLE_EVALUATORS 1
  #define ENABLE_AUXVDSM 1
  #define USE_AVAHI_CORE 0 // use dbus version of avahi via libavahi-client so other daemons can use it as well
  #define BUTTON_NOT_AVAILABLE_AT_START 1 // button has uboot function at system startup, so use alternative factory reset
  #define SELFTESTING_ENABLED 1
#else
  // Default build options unless defined otherwise already
  #ifndef ENABLE_HUE
    #define ENABLE_HUE 1
  #endif
  #ifndef ENABLE_STATIC
    #define ENABLE_STATIC 1
  #endif
  #ifndef ENABLE_EXTERNAL
    #define ENABLE_EXTERNAL 1
  #endif
  #ifndef ENABLE_EVALUATORS
    #define ENABLE_EVALUATORS 1
  #endif
#endif






#endif /* defined(__p44vdc__config__) */
