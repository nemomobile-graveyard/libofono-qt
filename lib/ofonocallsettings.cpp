/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2010-2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ofonocallsettings.h"
#include "ofonointerface.h"

OfonoCallSettings::OfonoCallSettings(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent)
    : OfonoModemInterface(modemSetting, modemPath, "org.ofono.CallSettings", OfonoGetAllOnFirstRequest, parent)
{
    connect(m_if, SIGNAL(propertyChanged(const QString&, const QVariant&)), 
            this, SLOT(propertyChanged(const QString&, const QVariant&)));
    connect(m_if, SIGNAL(setPropertyFailed(const QString&)), 
            this, SLOT(setPropertyFailed(const QString&)));
    connect(m_if, SIGNAL(requestPropertyComplete(bool, const QString&, const QVariant&)),
    	    this, SLOT(requestPropertyComplete(bool, const QString&, const QVariant&)));
}

OfonoCallSettings::~OfonoCallSettings()
{
}

void OfonoCallSettings::requestCallingLinePresentation()
{
    m_if->requestProperty("CallingLinePresentation");
}

void OfonoCallSettings::requestCalledLinePresentation()
{
    m_if->requestProperty("CalledLinePresentation");
}

void OfonoCallSettings::requestConnectedLinePresentation()
{
    m_if->requestProperty("ConnectedLinePresentation");
}

void OfonoCallSettings::requestConnectedLineRestriction()
{
    m_if->requestProperty("ConnectedLineRestriction");
}

void OfonoCallSettings::requestCallingLineRestriction()
{
    m_if->requestProperty("CallingLineRestriction");
}

void OfonoCallSettings::requestHideCallerId()
{
    m_if->requestProperty("HideCallerId");
}

void OfonoCallSettings::requestVoiceCallWaiting()
{
    m_if->requestProperty("VoiceCallWaiting");
}

void OfonoCallSettings::setHideCallerId(const QString &preference)
{
    return m_if->setProperty("HideCallerId", qVariantFromValue(preference));
}

void OfonoCallSettings::setVoiceCallWaiting(const QString &preference)
{
    return m_if->setProperty("VoiceCallWaiting", qVariantFromValue(preference));
}

void OfonoCallSettings::requestPropertyComplete(bool success, const QString& property, const QVariant& value)
{
    if (property == "CallingLinePresentation") {	
        success ? emit callingLinePresentationComplete(true, value.value<QString>()) : emit callingLinePresentationComplete(false, value.value<QString>());
    } else if (property == "CalledLinePresentation") {	
        success ? emit calledLinePresentationComplete(true, value.value<QString>()) : emit calledLinePresentationComplete(false, value.value<QString>());
    } else if (property == "ConnectedLinePresentation") {
        success ? emit connectedLinePresentationComplete(true, value.value<QString>()) : emit connectedLinePresentationComplete(false, value.value<QString>());
    } else if (property == "ConnectedLineRestriction") {
        success ? emit connectedLineRestrictionComplete(true, value.value<QString>()) : emit connectedLineRestrictionComplete(false, value.value<QString>());
    } else if (property == "CallingLineRestriction") {	
        success ? emit callingLineRestrictionComplete(true, value.value<QString>()) : emit callingLineRestrictionComplete(false, value.value<QString>());
    } else if (property == "HideCallerId") {	
        success ? emit hideCallerIdComplete(true, value.value<QString>()) : emit hideCallerIdComplete(false, value.value<QString>());
    } else if (property == "VoiceCallWaiting") {	
        success ? emit voiceCallWaitingComplete(true, value.value<QString>()) : emit voiceCallWaitingComplete(false, value.value<QString>());
    }
}

void OfonoCallSettings::propertyChanged(const QString& property, const QVariant& value)
{
    if (property == "CallingLinePresentation") {	
        emit callingLinePresentationChanged(value.value<QString>());
    } else if (property == "CalledLinePresentation") {	
        emit calledLinePresentationChanged(value.value<QString>());
    } else if (property == "ConnectedLinePresentation") {
        emit connectedLinePresentationChanged(value.value<QString>());
    } else if (property == "ConnectedLineRestriction") {
        emit connectedLineRestrictionChanged(value.value<QString>());
    } else if (property == "CallingLineRestriction") {	
        emit callingLineRestrictionChanged(value.value<QString>());
    } else if (property == "HideCallerId") {	
        emit hideCallerIdChanged(value.value<QString>());
    } else if (property == "VoiceCallWaiting") {	
        emit voiceCallWaitingChanged(value.value<QString>());
    }
}

void OfonoCallSettings::setPropertyFailed(const QString& property)
{
    if (property == "HideCallerId") {	
        emit setHideCallerIdFailed();
    } else if (property == "VoiceCallWaiting") {	
        emit setVoiceCallWaitingFailed();
    }
}
