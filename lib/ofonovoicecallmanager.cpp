/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2010-2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ofonovoicecallmanager.h"
#include "ofonointerface.h"

#define DIAL_TIMEOUT 30000
#define TONE_TIMEOUT 10000
#define TRANSFER_TIMEOUT 20000
#define SWAP_TIMEOUT 20000
#define HANGUP_TIMEOUT 30000
#define HOLD_TIMEOUT 30000

QDBusArgument &operator<<(QDBusArgument &argument, const OfonoVoiceCallManagerStruct &call)
{
    argument.beginStructure();
    argument << call.path << call.properties;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, OfonoVoiceCallManagerStruct &call)
{
    argument.beginStructure();
    argument >> call.path >> call.properties;
    argument.endStructure();
    return argument;
}

OfonoVoiceCallManager::OfonoVoiceCallManager(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent)
    : OfonoModemInterface(modemSetting, modemPath, "org.ofono.VoiceCallManager", OfonoGetAllOnStartup, parent)
{
    QDBusReply<OfonoVoiceCallManagerList> reply;
    OfonoVoiceCallManagerList calls;

    QDBusMessage request;

    qDBusRegisterMetaType<OfonoVoiceCallManagerStruct>();
    qDBusRegisterMetaType<OfonoVoiceCallManagerStruct>();

    request = QDBusMessage::createMethodCall("org.ofono",
                                             path(), m_if->ifname(),
                                             "GetCalls");
    reply = QDBusConnection::systemBus().call(request);

    calls = reply;
    foreach(OfonoVoiceCallManagerStruct call, calls) {
        m_calllist<< call.path.path();
    }

    connect(m_if, SIGNAL(propertyChanged(const QString&, const QVariant&)), 
            this, SLOT(propertyChanged(const QString&, const QVariant&)));

    QDBusConnection::systemBus().connect("org.ofono",path(),m_if->ifname(),
                                         "CallAdded", this,
                                         SLOT(callAddedChanged(const QDBusObjectPath&, const QVariantMap&)));

    QDBusConnection::systemBus().connect("org.ofono",path(),m_if->ifname(),
                                         "CallRemoved", this,
                                         SLOT(callRemovedChanged(const QDBusObjectPath&)));
}

OfonoVoiceCallManager::~OfonoVoiceCallManager()
{
}

void OfonoVoiceCallManager::dial(const QString &number, const QString &callerid_hide)
{
    QDBusMessage request;
    request = QDBusMessage::createMethodCall("org.ofono",
                                             path(), m_if->ifname(),
                                             "Dial");
    QList<QVariant>arg;
    arg.append(QVariant(number));
    arg.append(QVariant(callerid_hide));
    request.setArguments(arg);
    QDBusConnection::systemBus().callWithCallback(request, this,
                                        SLOT(dialResp()),
                                        SLOT(dialErr(const QDBusError&)),
                                        DIAL_TIMEOUT);
}

void OfonoVoiceCallManager::hangupAll()
{
    QDBusMessage request;
    request = QDBusMessage::createMethodCall("org.ofono",
                                             path(), m_if->ifname(),
                                             "HangupAll");

    QDBusConnection::systemBus().callWithCallback(request, this,
                                        SLOT(hangupAllResp()),
                                        SLOT(hangupAllErr(const QDBusError&)),
                                        HANGUP_TIMEOUT);
}

void OfonoVoiceCallManager::sendTones(const QString &tonestring)
{
    QDBusMessage request;
    request = QDBusMessage::createMethodCall("org.ofono",
                                             path(), m_if->ifname(),
                                             "SendTones");
    QList<QVariant>arg;
    arg.append(QVariant(tonestring));
    request.setArguments(arg);

    QDBusConnection::systemBus().callWithCallback(request, this,
                                        SLOT(sendTonesResp()),
                                        SLOT(sendTonesErr(const QDBusError&)),
                                        (TONE_TIMEOUT*tonestring.length()));
}

void OfonoVoiceCallManager::transfer()
{
    QDBusMessage request;
    request = QDBusMessage::createMethodCall("org.ofono",
                                             path(), m_if->ifname(),
                                             "Transfer");

    QDBusConnection::systemBus().callWithCallback(request, this,
                                        SLOT(transferResp()),
                                        SLOT(transferErr(const QDBusError&)),
                                        TRANSFER_TIMEOUT);
}

void OfonoVoiceCallManager::swapCalls()
{
    QDBusMessage request;
    request = QDBusMessage::createMethodCall("org.ofono",
                                             path(), m_if->ifname(),
                                             "SwapCalls");

    QDBusConnection::systemBus().callWithCallback(request, this,
                                        SLOT(swapCallsResp()),
                                        SLOT(swapCallsErr(const QDBusError&)),
                                        SWAP_TIMEOUT);
}

void OfonoVoiceCallManager::releaseAndAnswer()
{
    QDBusMessage request;
    request = QDBusMessage::createMethodCall("org.ofono",
                                             path(), m_if->ifname(),
                                             "ReleaseAndAnswer");

    QDBusConnection::systemBus().callWithCallback(request, this,
                                        SLOT(releaseAndAnswerResp()),
                                        SLOT(releaseAndAnswerErr(const QDBusError&)),
                                        HANGUP_TIMEOUT);
}

void OfonoVoiceCallManager::holdAndAnswer()
{
    QDBusMessage request;
    request = QDBusMessage::createMethodCall("org.ofono",
                                             path(), m_if->ifname(),
                                             "HoldAndAnswer");

    QDBusConnection::systemBus().callWithCallback(request, this,
                                        SLOT(holdAndAnswerResp()),
                                        SLOT(holdAndAnswerErr(const QDBusError&)),
                                        HOLD_TIMEOUT);
}

void OfonoVoiceCallManager::holdAndAnswerResp()
{
    emit holdAndAnswerComplete(TRUE);
}

void OfonoVoiceCallManager::holdAndAnswerErr(const QDBusError &error)
{
    qDebug() << "request failed" << error;
    m_if->setError(error.name(), error.message());
    emit holdAndAnswerComplete(FALSE);
}

void OfonoVoiceCallManager::releaseAndAnswerResp()
{
    emit releaseAndAnswerComplete(TRUE);
}

void OfonoVoiceCallManager::releaseAndAnswerErr(const QDBusError &error)
{
    qDebug() << "request failed" << error;
    m_if->setError(error.name(), error.message());
    emit releaseAndAnswerComplete(FALSE);
}

void OfonoVoiceCallManager::swapCallsResp()
{
    emit swapCallsComplete(TRUE);
}

void OfonoVoiceCallManager::swapCallsErr(const QDBusError &error)
{
    qDebug() << "request failed" << error;
    m_if->setError(error.name(), error.message());
    emit swapCallsComplete(FALSE);
}

void OfonoVoiceCallManager::dialResp()
{
    emit dialComplete(TRUE);
}

void OfonoVoiceCallManager::dialErr(const QDBusError &error)
{
    qDebug() << "request failed" << error;
    m_if->setError(error.name(), error.message());
    emit dialComplete(FALSE);
}

void OfonoVoiceCallManager::hangupAllResp()
{
    emit hangupAllComplete(TRUE);
}

void OfonoVoiceCallManager::hangupAllErr(const QDBusError &error)
{
    qDebug() << "request failed" << error;
    m_if->setError(error.name(), error.message());
    emit hangupAllComplete(FALSE);
}
void OfonoVoiceCallManager::sendTonesResp()
{
    emit sendTonesComplete(TRUE);
}

void OfonoVoiceCallManager::sendTonesErr(const QDBusError &error)
{
    qDebug() << "request failed" << error;
    m_if->setError(error.name(), error.message());
    emit sendTonesComplete(FALSE);
}

void OfonoVoiceCallManager::transferResp()
{
    emit transferComplete(TRUE);
}

void OfonoVoiceCallManager::transferErr(const QDBusError &error)
{
    qDebug() << "request failed" << error;
    m_if->setError(error.name(), error.message());
    emit transferComplete(FALSE);
}

QStringList OfonoVoiceCallManager::emergencyNumbers() const
{
    return m_if->properties()["EmergencyNumbers"].value<QStringList>();
}

void OfonoVoiceCallManager::propertyChanged(const QString &property, const QVariant &value)
{
    if (property == "EmergencyNumbers") {	
        emit emergencyNumbersChanged(value.value<QStringList>());
    }
}

QStringList OfonoVoiceCallManager::getCalls() const
{
    return m_calllist;
}

void OfonoVoiceCallManager::callAddedChanged(const QDBusObjectPath &path, const QVariantMap& /*values*/)
{
    m_calllist << path.path();
    emit callAdded(path.path());
}

void OfonoVoiceCallManager::callRemovedChanged(const QDBusObjectPath &path)
{
    m_calllist.removeAll(path.path());
    emit callRemoved(path.path());
}
