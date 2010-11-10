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
 

#ifndef OFONOMODEM_H
#define OFONOMODEM_H

#include <QtDBus/QtDBus>
#include <QtCore/QObject>
#include "ofonointerface.h"
#include "libofono-qt_global.h"

class OfonoModemManager;

class OFONO_QT_EXPORT OfonoModem : public OfonoInterface 
{

Q_OBJECT

public:

    OfonoModem(QString modemId, QObject *parent);

    ~OfonoModem();

    bool isValid() const;
    QString modemId() const;
    
    bool powered() const;
    void setPowered(bool powered);
    bool online() const;
    void setOnline(bool online);
    bool emergency() const;
    
    QString name() const;
    QString manufacturer() const;
    QString model() const;
    QString revision() const;
    QString serial() const;
    
    QStringList features() const;
    QStringList interfaces() const;

signals:
    void validityChanged(bool validity);
    void modemIdChanged(QString modemId);
    
    void poweredChanged(bool powered);
    void setPoweredFailed();
    void onlineChanged(bool online);
    void setOnlineFailed();
    void emergencyChanged(bool emergency);

    void nameChanged(QString name);
    void manufacturerChanged(QString manufacturer);
    void modelChanged(QString model);
    void revisionChanged(QString revision);
    void serialChanged(QString serial);

    void featuresChanged(QStringList features);
    void interfacesChanged(QStringList interfaces);


private slots:
    void propertyChanged(const QString& property, const QVariant& value);
    void setPropertyFailed(const QString& property);
    void modemAdded(QString modem);
    void modemRemoved(QString modem);

private:
    void modemsChanged();

private:
    OfonoModemManager *m_mm;
    bool m_autoChoose;
    bool m_isValid;
};
#endif
