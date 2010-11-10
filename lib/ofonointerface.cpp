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
#include <QtCore/QDebug>

#include "ofonointerface.h"

#define GET_PROPERTIES_TIMEOUT 300000
#define SET_PROPERTY_TIMEOUT 300000

OfonoInterface::OfonoInterface(const QString& path, const QString& ifname, GetPropertySetting setting, QObject *parent)
    : QObject(parent) , m_path(path), m_ifname(ifname), m_getpropsetting(setting)
{
    QDBusConnection::systemBus().connect("org.ofono", path, ifname, 
					     "PropertyChanged",
					     this,
					     SLOT(onPropertyChanged(QString, QDBusVariant)));
    if (setting == GetAllOnStartup && path != "/")
        m_properties = getAllPropertiesSync();
}

OfonoInterface::~OfonoInterface()
{
}

void OfonoInterface::setPath(const QString& path)
{
    QDBusConnection::systemBus().disconnect("org.ofono", m_path, m_ifname, 
					     "PropertyChanged",
					     this,
					     SLOT(onPropertyChanged(QString, QDBusVariant)));
    m_path = path;
    QDBusConnection::systemBus().connect("org.ofono", m_path, m_ifname, 
					     "PropertyChanged",
					     this,
					     SLOT(onPropertyChanged(QString, QDBusVariant)));

    if (m_getpropsetting == GetAllOnStartup)
        m_properties = getAllPropertiesSync();
    else
    	resetProperties();
}

QVariantMap OfonoInterface::properties() const
{
    return m_properties;
}

void OfonoInterface::resetProperties()
{
    m_properties = QVariantMap();
}

QVariantMap OfonoInterface::getAllPropertiesSync()
{
    QDBusReply<QVariantMap> reply;
    QVariantMap map;
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
                                             m_path, m_ifname,
                                             "GetProperties");
    reply = QDBusConnection::systemBus().call(request);
    map = reply;
    foreach (QString property, map.keys()) {
        emit propertyChanged(property, map[property]);
    }
    return map;
}

void OfonoInterface::requestProperty(const QString& name)
{
    if (m_pendingProperty.length() > 0) {
        // FIXME: should indicate that a setProperty is already in progress
        m_errorName = QString();
        m_errorMessage = QString();
        emit requestPropertyComplete(false, name, QVariant());
        return;
    }
    
    if (m_properties.keys().contains(name)) {
        emit requestPropertyComplete(true, name, m_properties[name]);
        return;
    }
    
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     m_path, m_ifname,
					     "GetProperties");

    bool result = QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(getPropertiesAsyncResp(QVariantMap)),
					SLOT(getPropertiesAsyncErr(const QDBusError&)),
					GET_PROPERTIES_TIMEOUT);
    if (!result) {
        // FIXME: should indicate that sending a message failed
        m_errorName = QString();
        m_errorMessage = QString();
        emit requestPropertyComplete(false, name, QVariant());
    	return;
    }
    m_pendingProperty = name;
}

void OfonoInterface::getPropertiesAsyncResp(QVariantMap properties)
{
    QString prop = m_pendingProperty;
    m_properties = properties;
    m_pendingProperty = QString();
    emit requestPropertyComplete(true, prop, properties[prop]);
    foreach (QString property, properties.keys()) {
        emit propertyChanged(property, properties[property]);
    }
}

void OfonoInterface::getPropertiesAsyncErr(const QDBusError& error)
{
    QString prop = m_pendingProperty;
    m_errorName = error.name();
    m_errorMessage = error.message();
    m_pendingProperty = QString();
    emit requestPropertyComplete(false, prop, QVariant());
}

void OfonoInterface::onPropertyChanged(QString property, QDBusVariant value)
{
    m_properties[property] = value.variant();
    emit propertyChanged(property, value.variant());
}

void OfonoInterface::setProperty(const QString& name, const QVariant& property)
{
    if (m_pendingProperty.length() > 0) {
        // FIXME: should indicate that a setProperty is already in progress
        m_errorName = QString();
        m_errorMessage = QString();
        emit setPropertyFailed(name);
        return;
    }

    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     m_path, m_ifname,
					     "SetProperty");
    request.setArguments(QList<QVariant>() 
                        << QVariant(name) 
                        << QVariant::fromValue(QDBusVariant(property)));
    bool result = QDBusConnection::systemBus().callWithCallback(request, this,
    					SLOT(setPropertyResp()),
    					SLOT(setPropertyErr(const QDBusError&)),
    					SET_PROPERTY_TIMEOUT);
    if (!result) {
        // FIXME: should indicate that sending a message failed
        m_errorName = QString();
        m_errorMessage = QString();
    	emit setPropertyFailed(name);
    	return;
    }
    m_pendingProperty = name;
}

void OfonoInterface::setPropertyResp()
{
    m_pendingProperty = QString();
    // emit nothing; we will get a PropertyChanged signal
}

void OfonoInterface::setPropertyErr(const QDBusError& error)
{
    QString prop = m_pendingProperty;
    m_errorName = error.name();
    m_errorMessage = error.message();
    m_pendingProperty = QString();
    emit setPropertyFailed(prop);
}

