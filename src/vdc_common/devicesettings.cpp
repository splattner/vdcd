//
//  Copyright (c) 2013 plan44.ch / Lukas Zeller, Zurich, Switzerland
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

#include "devicesettings.hpp"

#include "device.hpp"

using namespace p44;


DeviceSettings::DeviceSettings(Device &aDevice) :
  inherited(aDevice.getDeviceContainer().getDsParamStore()),
  device(aDevice),
  deviceFlags(0),
  zoneID(0),
  extraGroups(1<<group_variable) // all devices are also in group 0 by default 
{
}


// SQLIte3 table name to store these parameters to
const char *DeviceSettings::tableName()
{
  return "DeviceSettings";
}


// data field definitions

static const size_t numFields = 4;

size_t DeviceSettings::numFieldDefs()
{
  return inherited::numFieldDefs()+numFields;
}


const FieldDefinition *DeviceSettings::getFieldDef(size_t aIndex)
{
  static const FieldDefinition dataDefs[numFields] = {
    { "deviceFlags", SQLITE_INTEGER },
    { "deviceName", SQLITE_TEXT },
    { "zoneID", SQLITE_INTEGER },
    { "extraGroups", SQLITE_INTEGER }
  };
  if (aIndex<inherited::numFieldDefs())
    return inherited::getFieldDef(aIndex);
  aIndex -= inherited::numFieldDefs();
  if (aIndex<numFields)
    return &dataDefs[aIndex];
  return NULL;
}


/// load values from passed row
void DeviceSettings::loadFromRow(sqlite3pp::query::iterator &aRow, int &aIndex)
{
  inherited::loadFromRow(aRow, aIndex);
  // get the field value
  deviceFlags = aRow->get<int>(aIndex++);
  device.setName(nonNullCStr(aRow->get<const char *>(aIndex++)));
  zoneID = aRow->get<int>(aIndex++);
  extraGroups = aRow->get<long long>(aIndex++);
}


// bind values to passed statement
void DeviceSettings::bindToStatement(sqlite3pp::statement &aStatement, int &aIndex, const char *aParentIdentifier)
{
  inherited::bindToStatement(aStatement, aIndex, aParentIdentifier);
  // bind the fields
  aStatement.bind(aIndex++, deviceFlags);
  aStatement.bind(aIndex++, device.getName().c_str());
  aStatement.bind(aIndex++, zoneID);
  aStatement.bind(aIndex++, (long long)extraGroups);
}
