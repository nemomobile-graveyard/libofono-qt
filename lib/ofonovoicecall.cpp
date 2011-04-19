/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ofonointerface.h"
#include "ofonovoicecall.h"

#define VOICECALL_TIMEOUT 30000

OfonoVoiceCall::OfonoVoiceCall(const QString& callId, QObject *parent)
    : QObject(parent)
{
    m_if = new OfonoInterface(callId, "org.ofono.VoiceCall", OfonoGetAllOnStartup, this);

    connect(m_if, SIGNAL(propertyChanged(const QString&, const QVariant&)),
            this, SLOT(propertyChanged(const QString&, const QVariant&)));

    QDBusConnection::systemBus().connect("org.ofono",path(),m_if->ifname(),
                                         "DisconnectReason", this,
                                         SIGNAL(disconnectReason(const QString&)));

}

OfonoVoiceCall::OfonoVoiceCall(const OfonoVoiceCall& call)
    : QObject(call.parent())
{
    m_if = new OfonoInterface(call.path(), "org.ofono.VoiceCall", OfonoGetAllOnStartup, this);

    connect(m_if, SIGNAL(propertyChanged(const QString&, const QVariant&)),
            this, SLOT(propertyChanged(const QString&, const QVariant&)));

    QDBusConnection::systemBus().connect("org.ofono",path(),m_if->ifname(),
                                         "DisconnectReason", this,
                                         SIGNAL(disconnectReason(const QString&)));
}

bool OfonoVoiceCall::operator==(const OfonoVoiceCall &call)
{
    return path() == call.path();
}

OfonoVoiceCall::~OfonoVoiceCall()
{
}

void OfonoVoiceCall::answer()
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
                                             path(), m_if->ifname(),
                                             "Answer");

    QDBusConnection::systemBus().callWithCallback(request, this,
                                        SLOT(answerResp()),
                                        SLOT(answerErr(const QDBusError&)),
                                        VOICECALL_TIMEOUT);
}

void OfonoVoiceCall::hangup()
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
                                             path(), m_if->ifname(),
                                             "Hangup");

    QDBusConnection::systemBus().callWithCallback(request, this,
                                        SLOT(hangupResp()),
                                        SLOT(hangupErr(const QDBusError&)),
                                        VOICECALL_TIMEOUT);
}

void OfonoVoiceCall::deflect(const QString &number)
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
                                             path(), m_if->ifname(),
                                             "Deflect");
    QList<QVariant>arg;
    arg.append(QVariant(number));
    request.setArguments(arg);

    QDBusConnection::systemBus().callWithCallback(request, this,
                                        SLOT(deflectResp()),
                                        SLOT(deflectErr(const QDBusError&)),
                                        VOICECALL_TIMEOUT);
}

void OfonoVoiceCall::answerResp()
{
    emit answerComplete(TRUE);
}

void OfonoVoiceCall::answerErr(const QDBusError &error)
{
    m_if->setError(error.name(), error.message());
    emit answerComplete(FALSE);
}

void OfonoVoiceCall::hangupResp()
{
    emit hangupComplete(TRUE);
}

void OfonoVoiceCall::hangupErr(const QDBusError &error)
{
    m_if->setError(error.name(), error.message());
    emit hangupComplete(FALSE);
}

void OfonoVoiceCall::deflectResp()
{
    emit deflectComplete(TRUE);
}

void OfonoVoiceCall::deflectErr(const QDBusError &error)
{
    m_if->setError(error.name(), error.message());
    emit deflectComplete(FALSE);
}

QString OfonoVoiceCall::incomingLine() const
{
    return m_if->properties()["IncomingLine"].value<QString>();
}

QString OfonoVoiceCall::lineIdentification() const
{
    return m_if->properties()["LineIdentification"].value<QString>();
}

QString OfonoVoiceCall::name() const
{
    return m_if->properties()["Name"].value<QString>();
}

QString OfonoVoiceCall::state() const
{
    return m_if->properties()["State"].value<QString>();
}

QString OfonoVoiceCall::startTime() const
{
    return m_if->properties()["StartTime"].value<QString>();
}

QString OfonoVoiceCall::information() const
{
    return m_if->properties()["Information"].value<QString>();
}

bool OfonoVoiceCall::multiparty() const
{
    return m_if->properties()["Multiparty"].value<bool>();
}

bool OfonoVoiceCall::emergency() const
{
    return m_if->properties()["Emergency"].value<bool>();
}

quint8 OfonoVoiceCall::icon() const
{
    return m_if->properties()["Icon"].value<quint8>();
}

void OfonoVoiceCall::propertyChanged(const QString &property, const QVariant &value)
{
    if (property == "LineIdentification") {
        emit lineIdentificationChanged(value.value<QString>());
    } else if (property == "Name") {
        emit nameChanged(value.value<QString>());
    } else if (property == "State") {
        emit stateChanged(value.value<QString>());
    } else if (property == "Information") {
        emit informationChanged(value.value<QString>());
    } else if (property == "IncomingLine") {
        emit incomingLineChanged(value.value<QString>());
    } else if (property == "Multiparty") {
        emit multipartyChanged(value.value<bool>());
    } else if (property == "Emergency") {
        emit emergencyChanged(value.value<bool>());
    } else if (property == "StartTime") {
        emit startTimeChanged(value.value<QString>());
    } else if (property == "Icon") {
            emit iconChanged(value.value<quint8>());
    }
}

QString OfonoVoiceCall::path() const
{
    return m_if->path();
}

QString OfonoVoiceCall::errorName() const
{
    return m_if->errorName();
}

QString OfonoVoiceCall::errorMessage() const
{
    return m_if->errorMessage();
}
