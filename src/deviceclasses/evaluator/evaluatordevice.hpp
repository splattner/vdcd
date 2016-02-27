//
//  Copyright (c) 2016 plan44.ch / Lukas Zeller, Zurich, Switzerland
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

#ifndef __vdcd__evaluatordevice__
#define __vdcd__evaluatordevice__

#include "device.hpp"

#if ENABLE_EVALUATORS

using namespace std;

namespace p44 {


  class EvaluatorDeviceContainer;

  class EvaluatorDevice : public Device
  {
    typedef Device inherited;
    friend class EvaluatorDeviceContainer;

    long long evaluatorDeviceRowID; ///< the ROWID this device was created from (0=none)
    
    typedef enum {
      evaluator_unknown,
      evaluator_rocker,
      evaluator_input,
    } EvaluatorType;

    EvaluatorType evaluatorType;
    string evaluatorID;

    /// active value sources
    typedef map<string, ValueSource *> ValueSourcesMap;
    ValueSourcesMap valueMap;
    long valueParseTicket;

    Tristate currentState;

    /// configuration strings
    string valueDefs;
    string onCondition;
    string offCondition;

  public:

    EvaluatorDevice(EvaluatorDeviceContainer *aClassContainerP, const string &aEvaluatorID, const string &aEvaluatorConfig);
    virtual ~EvaluatorDevice();
    
    /// device type identifier
		/// @return constant identifier for this type of device (one container might contain more than one type)
    virtual const char *deviceTypeIdentifier() { return "evaluator"; };

    EvaluatorDeviceContainer &getEvaluatorDeviceContainer();

    /// check if device can be disconnected by software (i.e. Web-UI)
    /// @return true if device might be disconnectable (deletable) by the user via software (i.e. web UI)
    /// @note devices returning true here might still refuse disconnection on a case by case basis when
    ///   operational state does not allow disconnection.
    /// @note devices returning false here might still be disconnectable using disconnect() triggered
    ///   by vDC API "remove" method.
    virtual bool isSoftwareDisconnectable() { return true; };

    /// disconnect device. For static device, this means removing the config from the container's DB. Note that command line
    /// static devices cannot be disconnected.
    /// @param aForgetParams if set, not only the connection to the device is removed, but also all parameters related to it
    ///   such that in case the same device is re-connected later, it will not use previous configuration settings, but defaults.
    /// @param aDisconnectResultHandler will be called to report true if device could be disconnected,
    ///   false in case it is certain that the device is still connected to this and only this vDC
    virtual void disconnect(bool aForgetParams, DisconnectCB aDisconnectResultHandler);

    /// description of object, mainly for debug and logging
    /// @return textual description of object
    virtual string description();


    /// @name identification of the addressable entity
    /// @{

    /// @return human readable model name/short description
    virtual string modelName();

    /// Get icon data or name
    /// @param aIcon string to put result into (when method returns true)
    /// - if aWithData is set, binary PNG icon data for given resolution prefix is returned
    /// - if aWithData is not set, only the icon name (without file extension) is returned
    /// @param aWithData if set, PNG data is returned, otherwise only name
    /// @return true if there is an icon, false if not
    virtual bool getDeviceIcon(string &aIcon, bool aWithData, const char *aResolutionPrefix);

    /// @}

    /// initializes the physical device for being used
    /// @param aFactoryReset if set, the device will be inititalized as thoroughly as possible (factory reset, default settings etc.)
    /// @note this is called before interaction with dS system starts
    /// @note implementation should call inherited when complete, so superclasses could chain further activity
    virtual void initializeDevice(StatusCB aCompletedCB, bool aFactoryReset);


  protected:

    void deriveDsUid();

  private:

    void forgetValueDefs();
    void parseValueDefs();

    void dependentValueNotification(ValueSource &aValueSource, ValueListenerEvent aEvent);

    /// expression evaluation
    Tristate evaluateBoolean(string aExpression);
    ErrorPtr evaluateExpression(const char * &aText, double &aValue, int aPrecedence);
    ErrorPtr evaluateTerm(const char * &aText, double &aValue);

  };
  typedef boost::intrusive_ptr<EvaluatorDevice> EvaluatorDevicePtr;

} // namespace p44

#endif // ENABLE_EVALUATORS
#endif // __vdcd__evaluatordevice__