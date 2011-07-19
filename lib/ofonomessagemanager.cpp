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

#include "ofonomessagemanager.h"
#include "ofonointerface.h"

QDBusArgument &operator<<(QDBusArgument &argument, const OfonoMessageManagerStruct &message)
{
    argument.beginStructure();
    argument << message.path << message.properties;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, OfonoMessageManagerStruct &message)
{
    argument.beginStructure();
    argument >> message.path >> message.properties;
    argument.endStructure();
    return argument;
}

OfonoMessageManager::OfonoMessageManager(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent)
    : OfonoModemInterface(modemSetting, modemPath, "org.ofono.MessageManager", OfonoGetAllOnFirstRequest, parent)
{
    qDBusRegisterMetaType<OfonoMessageManagerStruct>();
    qDBusRegisterMetaType<OfonoMessageManagerList>();

    m_messagelist = getMessageList();

    connect(m_if, SIGNAL(propertyChanged(const QString&, const QVariant&)), 
            this, SLOT(propertyChanged(const QString&, const QVariant&)));
    connect(m_if, SIGNAL(setPropertyFailed(const QString&)), 
            this, SLOT(setPropertyFailed(const QString&)));
    connect(m_if, SIGNAL(requestPropertyComplete(bool, const QString&, const QVariant&)),
    	    this, SLOT(requestPropertyComplete(bool, const QString&, const QVariant&)));
    connect(this, SIGNAL(validityChanged(bool)),
            this, SLOT(validityChanged(bool)));
    connect(modem(), SIGNAL(pathChanged(QString)), this, SLOT(pathChanged(const QString&)));

    connectDbusSignals(path());
}

OfonoMessageManager::~OfonoMessageManager()
{
}

void OfonoMessageManager::validityChanged(bool /*validity*/)
{
    m_messagelist = getMessageList();
}

void OfonoMessageManager::pathChanged(const QString& path)
{
    connectDbusSignals(path);
}

QStringList OfonoMessageManager::getMessageList()
{
    QDBusReply<OfonoMessageManagerList> reply;
    OfonoMessageManagerList messages;
    QDBusMessage request;
    QStringList messageList;

    request = QDBusMessage::createMethodCall("org.ofono",
                                             path(), m_if->ifname(),
                                             "GetMessages");
    reply = QDBusConnection::systemBus().call(request);

    messages = reply;
    foreach(OfonoMessageManagerStruct message, messages) {
        messageList << message.path.path();
    }
    return messageList;
}

void OfonoMessageManager::connectDbusSignals(const QString& path)
{
    QDBusConnection::systemBus().disconnect("org.ofono", QString(), m_if->ifname(),
                                         "MessageAdded",
                                         this,
                                         SLOT(onMessageAdded(const QDBusObjectPath&, const QVariantMap&)));
    QDBusConnection::systemBus().disconnect("org.ofono", QString(), m_if->ifname(),
                                         "MessageRemoved",
                                         this,
                                         SLOT(onMessageRemoved(const QDBusObjectPath&)));
    QDBusConnection::systemBus().disconnect("org.ofono", QString(), m_if->ifname(),
                                         "IncomingMessage",
                                         this,
                                         SIGNAL(incomingMessage(QString, QVariantMap)));
    QDBusConnection::systemBus().disconnect("org.ofono", QString(), m_if->ifname(),
                                         "ImmediateMessage",
                                         this,
                                         SIGNAL(immediateMessage(QString, QVariantMap)));

    QDBusConnection::systemBus().connect("org.ofono", path, m_if->ifname(),
                                         "MessageAdded",
                                         this,
                                         SLOT(onMessageAdded(const QDBusObjectPath&, const QVariantMap&)));
    QDBusConnection::systemBus().connect("org.ofono", path, m_if->ifname(),
                                         "MessageRemoved",
                                         this,
                                         SLOT(onMessageRemoved(const QDBusObjectPath&)));
    QDBusConnection::systemBus().connect("org.ofono", path, m_if->ifname(),
                                         "IncomingMessage",
                                         this,
                                         SIGNAL(incomingMessage(QString, QVariantMap)));
    QDBusConnection::systemBus().connect("org.ofono", path, m_if->ifname(),
                                         "ImmediateMessage",
                                         this,
                                         SIGNAL(immediateMessage(QString, QVariantMap)));
}

void OfonoMessageManager::requestServiceCenterAddress()
{
    m_if->requestProperty("ServiceCenterAddress");
}

void OfonoMessageManager::setServiceCenterAddress(QString address)
{
    m_if->setProperty("ServiceCenterAddress", qVariantFromValue(address));
}

void OfonoMessageManager::requestUseDeliveryReports()
{
    m_if->requestProperty("UseDeliveryReports");
}

void OfonoMessageManager::setUseDeliveryReports(bool useDeliveryReports)
{
    m_if->setProperty("UseDeliveryReports", qVariantFromValue(useDeliveryReports));
}

void OfonoMessageManager::requestBearer()
{
    m_if->requestProperty("Bearer");
}

void OfonoMessageManager::setBearer(QString bearer)
{
    m_if->setProperty("Bearer", qVariantFromValue(bearer));
}

void OfonoMessageManager::requestAlphabet()
{
    m_if->requestProperty("Alphabet");
}

void OfonoMessageManager::setAlphabet(QString alphabet)
{
    m_if->setProperty("Alphabet", qVariantFromValue(alphabet));
}


void OfonoMessageManager::sendMessage(const QString &to, const QString &message)
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
                                             path(), m_if->ifname(),
                                             "SendMessage");

    request << to << message;

    QDBusConnection::systemBus().callWithCallback(request, this,
                                        SLOT(sendMessageResp(const QDBusObjectPath&)),
                                        SLOT(sendMessageErr(const QDBusError&)));
}

void OfonoMessageManager::requestPropertyComplete(bool success, const QString& property, const QVariant& value)
{
    if (property == "ServiceCenterAddress") {
        success ? emit serviceCenterAddressComplete(true, value.value<QString>()) : emit serviceCenterAddressComplete(false, QString());
    } else if (property == "UseDeliveryReports") {
        success ? emit useDeliveryReportsComplete(true, value.value<bool>()) : emit useDeliveryReportsComplete(false, bool());
    } else if (property == "Bearer") {
        success ? emit bearerComplete(true, value.value<QString>()) : emit bearerComplete(false, QString());
    } else if (property == "Alphabet") {
        success ? emit alphabetComplete(true, value.value<QString>()) : emit alphabetComplete(false, QString());
    }
}

void OfonoMessageManager::propertyChanged(const QString& property, const QVariant& value)
{
    if (property == "ServiceCenterAddress") {	
        emit serviceCenterAddressChanged(value.value<QString>());
    } else if (property == "UseDeliveryReports") {
        emit useDeliveryReportsChanged(value.value<bool>());
    } else if (property == "Bearer") {
        emit bearerChanged(value.value<QString>());
    } else if (property == "Alphabet") {
        emit alphabetChanged(value.value<QString>());
    }
}

void OfonoMessageManager::setPropertyFailed(const QString& property)
{
    if (property == "ServiceCenterAddress") {
        emit setServiceCenterAddressFailed();
    } else if (property == "UseDeliveryReports") {
        emit setUseDeliveryReportsFailed();
    } else if (property == "Bearer") {
        emit setBearerFailed();
    } else if (property == "Alphabet") {
        emit setAlphabetFailed();
    }
}

void OfonoMessageManager::sendMessageResp(const QDBusObjectPath& objectPath)
{
    emit sendMessageComplete(true, objectPath);
}

void OfonoMessageManager::sendMessageErr(QDBusError error)
{
    m_if->setError(error.name(), error.message());
    emit sendMessageComplete(false, QDBusObjectPath());
}

QStringList OfonoMessageManager::getMessages() const
{
    return m_messagelist;
}

void OfonoMessageManager::onMessageAdded(const QDBusObjectPath &path, const QVariantMap& /*properties*/)
{
    m_messagelist << path.path();
    emit messageAdded(path.path());
}

void OfonoMessageManager::onMessageRemoved(const QDBusObjectPath &path)
{
    m_messagelist.removeAll(path.path());
    emit messageRemoved(path.path());
}
