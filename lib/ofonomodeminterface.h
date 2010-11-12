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

#ifndef OFONOMODEMINTERFACE_H
#define OFONOMODEMINTERFACE_H

#include <QtCore/QObject>
#include <QVariant>
#include <QDBusVariant>
#include <QStringList>
#include "ofonointerface.h"
#include "ofonomodem.h"
#include "libofono-qt_global.h"

//! This class implements a generic modem interface object
/*!
 * It adds validity checking and modem binding to OfonoInterface class.
 * It should not be instantiated directly; instead you should instantiate
 * interface-specific subclasses.
 */
class OFONO_QT_EXPORT OfonoModemInterface : public OfonoInterface
{
    Q_OBJECT
public:

    //! Construct a modem interface object
    /*!
     * \param modemSetting modem selection setting
     * \param modemPath path to the modem (may not be significant, depending on modemSetting)
     * \param ifname d-bus interface name
     * \param propertySetting oFono d-bus properties setting
     */
    OfonoModemInterface(OfonoModem::SelectionSetting modemSetting, const QString& modemPath, const QString& ifname, GetPropertySetting propertySetting, QObject *parent=0);
    ~OfonoModemInterface();

    //! Check that the modem interface object is valid
    /*!
     * This means that a modem d-bus object
     * exists and has the d-bus interface specified in the contstructor.
     */
    bool isValid() const;
    
    //! Get the modem object that this interface belongs to
    OfonoModem *modem() const;

signals:
    //! Interface validity has changed
    /*!
     * This may mean that modem has become unavailable
     * (or available again) or that the modem interface has become unavailable
     * (or available again)
     */
    void validityChanged(bool validity);

private:
    bool checkValidity();
    void updateValidity();

private slots:
    void modemValidityChanged(bool validity);
    void interfacesChanged(const QStringList &interfaces);

private:
    OfonoModem *m_m;
    bool m_isValid;
};
#endif
