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

//! This class is used to access an oFono modem object and its properties
/*!
 * oFono modem properties are documented in
 * http://git.kernel.org/?p=network/ofono/ofono.git;a=blob_plain;f=doc/modem-api.txt
 */
class OFONO_QT_EXPORT OfonoModem : public OfonoInterface 
{

Q_OBJECT

public:

    //! How the modem object should select a modem
    enum SelectionSetting {
    	AutomaticSelect,	/*!< Select the first available modem automatically;
    				 * if that modem becomes unavailable, select the first available
    				 * modem again. */
    	ManualSelect 	/*!< Do not select a modem automatically,
    			 * use the modem path provided in the constructor, and do not
    			 * attempt to select another modem if the first one becomes 
    			 * unavailable. */
    };

    /*!
     * \param setting sets the modem selection policy for the object
     * \param modemPath if modem selection policy is ManualSelect, then this contains
     * the D-Bus path to the modem object. Otherwise, it is ignored.
     */
    OfonoModem(SelectionSetting setting, const QString& modemPath, QObject *parent=0);

    ~OfonoModem();

    //! Returns true if D-Bus modem object exists.
    bool isValid() const;
    
    //! Returns the D-Bus object path of the modem
    QString modemPath() const;

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
    //! Issued when a modem becomes unavailable or available again
    void validityChanged(bool validity);
    //! Issued when the object has switched to another modem
    void modemPathChanged(QString modemPath);
    
    void poweredChanged(bool powered);
    void setPoweredFailed();
    void onlineChanged(bool online);
    void setOnlineFailed();
    void emergencyChanged(bool emergency);

    void nameChanged(const QString &name);
    void manufacturerChanged(const QString &manufacturer);
    void modelChanged(const QString &model);
    void revisionChanged(const QString &revision);
    void serialChanged(const QString &serial);

    void featuresChanged(const QStringList &features);
    void interfacesChanged(const QStringList &interfaces);


private slots:
    void propertyChanged(const QString &property, const QVariant &value);
    void setPropertyFailed(const QString& property);
    void modemAdded(const QString &modem);
    void modemRemoved(const QString &modem);

private:
    void modemsChanged();

private:
    OfonoModemManager *m_mm;
    SelectionSetting m_selectionSetting;
    bool m_isValid;
};
#endif
