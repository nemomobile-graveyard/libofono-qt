/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef OFONORADIOSETTINGS_H
#define OFONORADIOSETTINGS_H

#include <QtCore/QObject>
#include "ofonomodeminterface.h"
#include "libofono-qt_global.h"

//! This class is used to access oFono radio settings API
/*!
 * The API is documented in
 * http://git.kernel.org/?p=network/ofono/ofono.git;a=blob_plain;f=doc/radio-settings-api.txt
 */
class OFONO_QT_EXPORT OfonoRadioSettings : public OfonoModemInterface
{
    Q_OBJECT

    Q_PROPERTY(QString technologyPreference READ technologyPreference WRITE setTechnologyPreference NOTIFY technologyPreferenceChanged)
    Q_PROPERTY(QString gsmBand READ gsmBand WRITE setGsmBand NOTIFY gsmBandChanged)
    Q_PROPERTY(QString umtsBand READ umtsBand WRITE setUmtsBand NOTIFY umtsBandChanged)
    Q_PROPERTY(bool fastDormancy READ fastDormancy WRITE setFastDormancy NOTIFY fastDormancyChanged)

public:
    OfonoRadioSettings(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent=0);
    ~OfonoRadioSettings();

    QString technologyPreference() const;
    QString gsmBand() const;
    QString umtsBand() const;
    bool fastDormancy() const;
public slots:
    void setTechnologyPreference(QString preference);
    void setGsmBand(QString gsmBand);
    void setUmtsBand(QString umtsBand);
    void setFastDormancy(bool fastDormancy);

signals:
    void technologyPreferenceChanged(const QString &preference);
    void setTechnologyPreferenceFailed();
    void gsmBandChanged(const QString &gsmBand);
    void setGsmBandFailed();
    void umtsBandChanged(const QString &umtsBand);
    void setUmtsBandFailed();
    void fastDormancyChanged(bool fastDormancy);
    void setFastDormancyFailed();
        
private slots:
    void propertyChanged(const QString& property, const QVariant& value);
    void setPropertyFailed(const QString& property);    
};


#endif  /* !OFONORADIOSETTINGS_H */
