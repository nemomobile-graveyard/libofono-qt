/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ofonoconnman.h"
#include "ofonointerface.h"

#define DEACTIVATE_TIMEOUT 30000
#define ADD_TIMEOUT 30000
#define REMOVE_TIMEOUT 30000

QDBusArgument &operator<<(QDBusArgument &argument, const OfonoConnmanStruct &context)
{
    argument.beginStructure();
    argument << context.path << context.properties;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, OfonoConnmanStruct &context)
{
    argument.beginStructure();
    argument >> context.path >> context.properties;
    argument.endStructure();
    return argument;
}

OfonoConnMan::OfonoConnMan(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent)
    : OfonoModemInterface(modemSetting, modemPath, "org.ofono.ConnectionManager", OfonoGetAllOnStartup, parent)
{
    qDBusRegisterMetaType<OfonoConnmanStruct>();
    qDBusRegisterMetaType<OfonoConnmanList>();

    m_contextlist = getContextList();

    connect(m_if, SIGNAL(propertyChanged(const QString&, const QVariant&)),
            this, SLOT(propertyChanged(const QString&, const QVariant&)));
    connect(this, SIGNAL(validityChanged(bool)),
            this, SLOT(validityChanged(bool)));
    connect(modem(), SIGNAL(pathChanged(QString)), this, SLOT(pathChanged(const QString&)));

    connectDbusSignals(path());
}

OfonoConnMan::~OfonoConnMan()
{
}

void OfonoConnMan::validityChanged(bool /*validity*/)
{
    m_contextlist = getContextList();
}

void OfonoConnMan::pathChanged(const QString& path)
{
    connectDbusSignals(path);
}

QStringList OfonoConnMan::getContextList()
{
    QDBusReply<OfonoConnmanList> reply;
    OfonoConnmanList contexts;
    QStringList contextList;

    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
                                             path(), m_if->ifname(),
                                             "GetContexts");

    reply = QDBusConnection::systemBus().call(request);

    contexts = reply;
    foreach(OfonoConnmanStruct context, contexts) {
        contextList << context.path.path();
    }
    return contextList;
}

void OfonoConnMan::connectDbusSignals(const QString& path)
{
    QDBusConnection::systemBus().disconnect("org.ofono",QString(),m_if->ifname(),
                                         "ContextAdded", this,
                                         SLOT(contextAddedChanged(const QDBusObjectPath&, const QVariantMap&)));

    QDBusConnection::systemBus().disconnect("org.ofono",QString(),m_if->ifname(),
                                         "ContextRemoved", this,
                                         SLOT(contextRemovedChanged(const QDBusObjectPath&)));

    QDBusConnection::systemBus().connect("org.ofono",path,m_if->ifname(),
                                         "ContextAdded", this,
                                         SLOT(contextAddedChanged(const QDBusObjectPath&, const QVariantMap&)));

    QDBusConnection::systemBus().connect("org.ofono",path,m_if->ifname(),
                                         "ContextRemoved", this,
                                         SLOT(contextRemovedChanged(const QDBusObjectPath&)));

}

void OfonoConnMan::deactivateAll()
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
                                             "DeactivateAll");

    QDBusConnection::systemBus().callWithCallback(request, this,
                                        SLOT(deactivateAllResp()),
                                        SLOT(deactivateAllErr(const QDBusError&)),
                                        DEACTIVATE_TIMEOUT);
}

void OfonoConnMan::deactivateAllResp()
{
    emit deactivateAllComplete(TRUE);
}

void OfonoConnMan::deactivateAllErr(const QDBusError& error)
{
    m_if->setError(error.name(), error.message());
    emit deactivateAllComplete(FALSE);
}

void OfonoConnMan::addContext(const QString& type)
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
                                             "AddContext");

    QList<QVariant>arg;
    arg.append(QVariant(type));
    request.setArguments(arg);

    QDBusConnection::systemBus().callWithCallback(request, this,
                                        SLOT(addContextResp()),
                                        SLOT(addContextErr(const QDBusError&)),
                                        ADD_TIMEOUT);
}

void OfonoConnMan::addContextResp()
{
    emit addContextComplete(TRUE);
}

void OfonoConnMan::addContextErr(const QDBusError& error)
{
    m_if->setError(error.name(), error.message());
    emit addContextComplete(FALSE);
}

void OfonoConnMan::removeContext(const QString& contextpath)
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
                                             "RemoveContext");

    QList<QVariant> argumentList;
    QDBusObjectPath context (contextpath);
    argumentList << qVariantFromValue(context);
    request.setArguments(argumentList);
    QDBusConnection::systemBus().callWithCallback(request, this,
                                        SLOT(removeContextResp()),
                                        SLOT(removeContextErr(const QDBusError&)),
                                        REMOVE_TIMEOUT);
}

void OfonoConnMan::removeContextResp()
{
    emit removeContextComplete(TRUE);
}

void OfonoConnMan::removeContextErr(const QDBusError& error)
{
    m_if->setError(error.name(), error.message());
    emit removeContextComplete(FALSE);
}

bool OfonoConnMan::attached() const
{
    return m_if->properties()["Attached"].value<bool>();
}

QString OfonoConnMan::bearer() const
{
    return m_if->properties()["Bearer"].value<QString>();
}

bool OfonoConnMan::suspended() const
{
    return m_if->properties()["Suspended"].value<bool>();
}

bool OfonoConnMan::roamingAllowed() const
{
    return m_if->properties()["RoamingAllowed"].value<bool>();
}

bool OfonoConnMan::powered() const
{
    return m_if->properties()["Powered"].value<bool>();
}

void OfonoConnMan::propertyChanged(const QString& property, const QVariant& value)
{
    if (property == "Attached") {
        emit attachedChanged(value.value<bool>());
    } else if (property == "Bearer") {
        emit bearerChanged(value.value<QString>());
    } else if (property == "Suspended") {
        emit suspendedChanged(value.value<bool>());
    } else if (property == "RoamingAllowed") {
        emit roamingAllowedChanged(value.value<bool>());
    } else if (property == "Powered") {
        emit poweredChanged(value.value<bool>());
    }
}

QStringList OfonoConnMan::getContexts()
{
    return m_contextlist;
}

void OfonoConnMan::contextAddedChanged(const QDBusObjectPath &path, const QVariantMap& /*values*/)
{
    m_contextlist << path.path();
    emit contextAdded(path.path());
}

void OfonoConnMan::contextRemovedChanged(const QDBusObjectPath &path)
{
    m_contextlist.removeAll(path.path());
    emit contextRemoved(path.path());
}

void OfonoConnMan::setPowered(const bool value)
{
    m_if->setProperty("Powered",QVariant(value));
}

void OfonoConnMan::setRoamingAllowed(const bool value)
{
    m_if->setProperty("RoamingAllowed",QVariant(value));
}

void OfonoConnMan::setPropertyFailed(const QString &property)
{
    if (property == "Powered") {
        emit setPoweredFailed();
    } else if (property == "RoamingAllowed") {
        emit setRoamingAllowedFailed();
    }
}
