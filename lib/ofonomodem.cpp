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

#include "ofonomodem.h"
#include "ofonointerface.h"
#include "ofonomodemmanager.h"

OfonoModem::OfonoModem(SelectionSetting setting, const QString &modemPath, QObject *parent)
    : OfonoInterface("/", "org.ofono.Modem", OfonoInterface::GetAllOnStartup, parent), m_selectionSetting(setting)
{
    QString finalModemPath = modemPath;
    
    m_mm = new OfonoModemManager(this);
    connect(m_mm, SIGNAL(modemAdded(QString)), this, SLOT(modemAdded(QString)));
    connect(m_mm, SIGNAL(modemRemoved(QString)), this, SLOT(modemRemoved(QString)));

    if (setting == AutomaticSelect)
        finalModemPath = m_mm->modems().value(0);
    
    if (finalModemPath.isEmpty()) {
        finalModemPath = "/";
    } 
    connect(this, SIGNAL(propertyChanged(const QString&, const QVariant&)), 
            this, SLOT(propertyChanged(const QString&, const QVariant&)));
    connect(this, SIGNAL(setPropertyFailed(const QString&)), 
            this, SLOT(setPropertyFailed(const QString&)));
    setPath(finalModemPath);
    m_isValid = m_mm->modems().contains(finalModemPath);
}

OfonoModem::~OfonoModem()
{
}

void OfonoModem::propertyChanged(const QString& property, const QVariant& value)
{
    if (property == "Online")
        emit onlineChanged(value.value<bool>());
    else if (property == "Powered")
        emit poweredChanged(value.value<bool>());
    else if (property == "Emergency")
        emit emergencyChanged(value.value<bool>());
    else if (property == "Name")
        emit nameChanged(value.value<QString>());
    else if (property == "Manufacturer")
        emit manufacturerChanged(value.value<QString>());
    else if (property == "Model")
        emit modelChanged(value.value<QString>());
    else if (property == "Revision")
        emit revisionChanged(value.value<QString>());
    else if (property == "Serial")
        emit serialChanged(value.value<QString>());
    else if (property == "Features")
        emit featuresChanged(value.value<QStringList>());
    else if (property == "Interfaces")
        emit interfacesChanged(value.value<QStringList>());
}

void OfonoModem::setPropertyFailed(const QString& property)
{
    if (property == "Online")
        emit setOnlineFailed();
    else if (property == "Powered")
        emit setPoweredFailed();
}

void OfonoModem::modemAdded(const QString& /*modem*/)
{
    modemsChanged();
}

void OfonoModem::modemRemoved(const QString& /*modem*/)
{
    modemsChanged();
}

void OfonoModem::modemsChanged()
{
    // validity has changed
    if (isValid() != m_mm->modems().contains(modemPath())) {
        m_isValid = m_mm->modems().contains(modemPath());
        emit validityChanged(isValid());
    }
    if (!m_mm->modems().contains(modemPath())) {
        if (m_selectionSetting == AutomaticSelect) {
            QString modemPath = m_mm->modems().value(0);
            if (modemPath.isEmpty()) {
                modemPath = "/";
            }
            setPath(modemPath);
            emit modemPathChanged(modemPath);
        }
    }
    // validity has changed
    if (isValid() != m_mm->modems().contains(modemPath())) {
        m_isValid = m_mm->modems().contains(modemPath());
        emit validityChanged(isValid());
    }
}


bool OfonoModem::isValid() const
{
    return m_isValid;
}

QString OfonoModem::modemPath() const
{
    return path();
}

bool OfonoModem::powered() const
{
    return properties()["Powered"].value<bool>();
}

void OfonoModem::setPowered(bool powered)
{
    setProperty("Powered", qVariantFromValue(powered));
}

bool OfonoModem::online() const
{
    return properties()["Online"].value<bool>();
}

void OfonoModem::setOnline(bool online)
{
    setProperty("Online", qVariantFromValue(online));
}

bool OfonoModem::emergency() const
{
    return properties()["Emergency"].value<bool>();
}

QString OfonoModem::name() const
{
    return properties()["Name"].value<QString>();
}

QString OfonoModem::manufacturer() const
{
    return properties()["Manufacturer"].value<QString>();
}

QString OfonoModem::model() const
{
    return properties()["Model"].value<QString>();
}

QString OfonoModem::revision() const
{
    return properties()["Revision"].value<QString>();
}

QString OfonoModem::serial() const
{
    return properties()["Serial"].value<QString>();
}

QStringList OfonoModem::features() const
{
    return properties()["Features"].value<QStringList>();
}

QStringList OfonoModem::interfaces() const
{
    return properties()["Interfaces"].value<QStringList>();
}


