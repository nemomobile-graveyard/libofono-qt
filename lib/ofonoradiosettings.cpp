/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Contact: Alexander Kanavin <alexander.kanavin@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include <QtDBus/QtDBus>
#include <QtCore/QObject>

#include "ofonoradiosettings.h"

OfonoRadioSettings::OfonoRadioSettings(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent)
    : OfonoModemInterface(modemSetting, modemPath, "org.ofono.RadioSettings", OfonoInterface::GetAllOnStartup, parent)
{
    connect(this, SIGNAL(propertyChanged(const QString&, const QVariant&)), 
            this, SLOT(propertyChanged(const QString&, const QVariant&)));
    connect(this, SIGNAL(setPropertyFailed(const QString&)), 
            this, SLOT(setPropertyFailed(const QString&)));}

OfonoRadioSettings::~OfonoRadioSettings()
{
}

QString OfonoRadioSettings::technologyPreference() const
{
    return properties()["TechnologyPreference"].value<QString>();
}

void OfonoRadioSettings::setTechnologyPreference(QString preference)
{
    setProperty("TechnologyPreference", qVariantFromValue(preference));
}

void OfonoRadioSettings::setPropertyFailed(const QString& property)
{
    if (property == "TechnologyPreference")
        emit setTechnologyPreferenceFailed();
}

void OfonoRadioSettings::propertyChanged(const QString& property, const QVariant& value)
{
    if (property == "TechnologyPreference") {	
        emit technologyPreferenceChanged(value.value<QString>());
    }
}
