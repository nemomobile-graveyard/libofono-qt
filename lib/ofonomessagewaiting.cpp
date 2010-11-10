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

#include "ofonomessagewaiting.h"


OfonoMessageWaiting::OfonoMessageWaiting(QString modemId, QObject *parent)
    : OfonoModemInterface(modemId, "org.ofono.MessageWaiting", OfonoInterface::GetAllOnStartup, parent)
{
    connect(this, SIGNAL(propertyChanged(const QString&, const QVariant&)), 
            this, SLOT(propertyChanged(const QString&, const QVariant&)));
    connect(this, SIGNAL(setPropertyFailed(const QString&)), 
            this, SLOT(setPropertyFailed(const QString&)));

}

OfonoMessageWaiting::~OfonoMessageWaiting()
{
}

bool OfonoMessageWaiting::voicemailWaiting()
{
    return properties()["VoicemailWaiting"].value<bool>();
}

int OfonoMessageWaiting::voicemailMessageCount()
{
    return properties()["VoicemailMessageCount"].value<int>();
}

QString OfonoMessageWaiting::voicemailMailboxNumber()
{
    return properties()["VoicemailMailboxNumber"].value<QString>();
}

void OfonoMessageWaiting::setVoicemailMailboxNumber(QString mailboxnumber)
{
    setProperty("VoicemailMailboxNumber", qVariantFromValue(mailboxnumber));
}

void OfonoMessageWaiting::setPropertyFailed(const QString& property)
{
    if (property == "VoicemailMailboxNumber")
        emit setVoicemailMailboxNumberFailed();
}

void OfonoMessageWaiting::propertyChanged(const QString& property, const QVariant& value)
{
    if (property == "VoicemailWaiting")
        emit voicemailWaitingChanged(value.value<bool>());
    else if (property == "VoicemailMessageCount")
        emit voicemailMessageCountChanged(value.value<int>());
    else if (property == "VoicemailMailboxNumber")
        emit voicemailMailboxNumberChanged(value.value<QString>());
}


