/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Contact: Alexander Kanavin <alex.kanavin@gmail.com>
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
#include "ofonointerface.h"

OfonoRadioSettings::OfonoRadioSettings(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent)
    : OfonoModemInterface(modemSetting, modemPath, "org.ofono.RadioSettings", OfonoGetAllOnStartup, parent)
{
    connect(m_if, SIGNAL(propertyChanged(const QString&, const QVariant&)), 
            this, SLOT(propertyChanged(const QString&, const QVariant&)));
    connect(m_if, SIGNAL(setPropertyFailed(const QString&)), 
            this, SLOT(setPropertyFailed(const QString&)));}

OfonoRadioSettings::~OfonoRadioSettings()
{
}

QString OfonoRadioSettings::technologyPreference() const
{
    return m_if->properties()["TechnologyPreference"].value<QString>();
}

void OfonoRadioSettings::setTechnologyPreference(QString preference)
{
    m_if->setProperty("TechnologyPreference", qVariantFromValue(preference));
}

QString OfonoRadioSettings::gsmBand() const
{
    return m_if->properties()["GsmBand"].value<QString>();
}

void OfonoRadioSettings::setGsmBand(QString gsmBand)
{
    m_if->setProperty("GsmBand", qVariantFromValue(gsmBand));
}

QString OfonoRadioSettings::umtsBand() const
{
    return m_if->properties()["UmtsBand"].value<QString>();
}

void OfonoRadioSettings::setUmtsBand(QString umtsBand)
{
    m_if->setProperty("UmtsBand", qVariantFromValue(umtsBand));
}

bool OfonoRadioSettings::fastDormancy() const
{
    return m_if->properties()["FastDormancy"].value<bool>();
}

void OfonoRadioSettings::setFastDormancy(bool fastDormancy)
{
    m_if->setProperty("FastDormancy", qVariantFromValue(fastDormancy));
}

void OfonoRadioSettings::setPropertyFailed(const QString& property)
{
    if (property == "TechnologyPreference")
        emit setTechnologyPreferenceFailed();
    else if (property == "GsmBand")
        emit setGsmBandFailed();
    else if (property == "UmtsBand")
        emit setUmtsBandFailed();
    else if (property == "FastDormancy")
        emit setFastDormancyFailed();
}

void OfonoRadioSettings::propertyChanged(const QString& property, const QVariant& value)
{
    if (property == "TechnologyPreference") {	
        emit technologyPreferenceChanged(value.value<QString>());
    } else if (property == "GsmBand") {	
        emit gsmBandChanged(value.value<QString>());
    } else if (property == "UmtsBand") {	
        emit umtsBandChanged(value.value<QString>());
    } else if (property == "FastDormancy") {	
        emit fastDormancyChanged(value.value<bool>());
    }

}
