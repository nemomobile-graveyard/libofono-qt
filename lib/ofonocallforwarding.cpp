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

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "ofonocallforwarding.h"
#include "ofonointerface.h"


OfonoCallForwarding::OfonoCallForwarding(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent)
    : OfonoModemInterface(modemSetting, modemPath, "org.ofono.CallForwarding", OfonoGetAllOnFirstRequest, parent)
{
    connect(m_if, SIGNAL(propertyChanged(const QString&, const QVariant&)), 
            this, SLOT(propertyChanged(const QString&, const QVariant&)));
    connect(m_if, SIGNAL(setPropertyFailed(const QString&)), 
            this, SLOT(setPropertyFailed(const QString&)));
    connect(m_if, SIGNAL(requestPropertyComplete(bool, const QString&, const QVariant&)),
    	    this, SLOT(requestPropertyComplete(bool, const QString&, const QVariant&)));
}

OfonoCallForwarding::~OfonoCallForwarding()
{
}


void OfonoCallForwarding::disableAll(const QString &type)
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
					     "DisableAll");
    request << type;

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(disableAllResp()),
					SLOT(disableAllErr(const QDBusError&)));
}


void OfonoCallForwarding::requestVoiceUnconditional()
{
    m_if->requestProperty("VoiceUnconditional");
}

void OfonoCallForwarding::setVoiceUnconditional(const QString &property)
{
    m_if->setProperty("VoiceUnconditional", qVariantFromValue(property));
}

void OfonoCallForwarding::requestVoiceBusy()
{
    m_if->requestProperty("VoiceBusy");
}

void OfonoCallForwarding::setVoiceBusy(const QString &property)
{
    return m_if->setProperty("VoiceBusy", qVariantFromValue(property));
}

void OfonoCallForwarding::requestVoiceNoReply()
{
    m_if->requestProperty("VoiceNoReply");
}

void OfonoCallForwarding::setVoiceNoReply(const QString &property)
{
    return m_if->setProperty("VoiceNoReply", qVariantFromValue(property));
}

void OfonoCallForwarding::requestVoiceNoReplyTimeout()
{
    m_if->requestProperty("VoiceNoReplyTimeout");
}

void OfonoCallForwarding::setVoiceNoReplyTimeout(ushort timeout)
{
    return m_if->setProperty("VoiceNoReplyTimeout", qVariantFromValue(timeout));
}

void OfonoCallForwarding::requestVoiceNotReachable()
{
    m_if->requestProperty("VoiceNotReachable");
}

void OfonoCallForwarding::setVoiceNotReachable(const QString &property)
{
    return m_if->setProperty("VoiceNotReachable", qVariantFromValue(property));
}


void OfonoCallForwarding::propertyChanged(const QString& property, const QVariant& value)
{
    if (property == "VoiceUnconditional") {	
        emit voiceUnconditionalChanged(value.value<QString>());
    } else  if (property == "VoiceBusy") {	
        emit voiceBusyChanged(value.value<QString>());
    } else  if (property == "VoiceNoReply") {	
        emit voiceNoReplyChanged(value.value<QString>());
    } else  if (property == "VoiceNoReplyTimeout") {	
        emit voiceNoReplyTimeoutChanged(value.value<ushort>());
    } else  if (property == "VoiceNotReachable") {	
        emit voiceNotReachableChanged(value.value<QString>());
    }
}

void OfonoCallForwarding::setPropertyFailed(const QString& property)
{
    if (property == "VoiceUnconditional") {	
        emit setVoiceUnconditionalFailed();
    } else  if (property == "VoiceBusy") {	
        emit setVoiceBusyFailed();
    } else  if (property == "VoiceNoReply") {	
        emit setVoiceNoReplyFailed();
    } else  if (property == "VoiceNoReplyTimeout") {	
        emit setVoiceNoReplyTimeoutFailed();
    } else  if (property == "VoiceNotReachable") {	
        emit setVoiceNotReachableFailed();
    }
}

void OfonoCallForwarding::requestPropertyComplete(bool success, const QString& property, const QVariant& value)
{
    if (property == "VoiceUnconditional") {	
        success ? emit voiceUnconditionalComplete(true, value.value<QString>()) : emit voiceUnconditionalComplete(false, QString());
    } else if (property == "VoiceBusy") {	
        success ? emit voiceBusyComplete(true, value.value<QString>()) : emit voiceBusyComplete(false, QString());
    } else if (property == "VoiceNoReply") {	
        success ? emit voiceNoReplyComplete(true, value.value<QString>()) : emit voiceNoReplyComplete(false, QString());
    } else if (property == "VoiceNoReplyTimeout") {
        success ? emit voiceNoReplyTimeoutComplete(true, value.value<ushort>()) : emit voiceNoReplyTimeoutComplete(false, 0);
    } else if (property == "VoiceNotReachable") {
        success ? emit voiceNotReachableComplete(true, value.value<QString>()) : emit voiceNotReachableComplete(false, QString());
    }
}

void OfonoCallForwarding::disableAllResp()
{
    emit disableAllComplete(TRUE);
}

void OfonoCallForwarding::disableAllErr(QDBusError error)
{
    qDebug() << "DisableAll failed" << error;
    m_if->setError(error.name(), error.message());
    emit disableAllComplete(FALSE);
}

