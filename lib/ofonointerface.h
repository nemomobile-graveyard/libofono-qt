/**
 * This file is part of CellularQt
 *
 * Copyright (C) 2010 Nokia Corporation. All rights reserved.
 */

#ifndef OFONOINTERFACE_H
#define OFONOINTERFACE_H

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

#include <QtCore/QObject>
#include <QVariant>
#include <QDBusVariant>
#include <QDBusError>
#include "libofono-qt_global.h"

class OFONO_QT_EXPORT OfonoInterface : public QObject
{
    Q_OBJECT
public:
    enum GetPropertySetting {
    	GetAllOnStartup,
    	GetAllOnFirstRequest
    };

    OfonoInterface(const QString& path, const QString& ifname, GetPropertySetting setting, QObject *parent=0);
    ~OfonoInterface();

    QVariantMap properties() const;
    void requestProperty(const QString& name);
    void setProperty(const QString& name, const QVariant& property);
    void resetProperties();
    
    QString path() const {return m_path;}
    
    QString ifname() const {return m_ifname;}
    QString errorName() const {return m_errorName;}
    QString errorMessage() const {return m_errorMessage;}

signals:
    void propertyChanged(const QString& name, const QVariant& property);
    void requestPropertyComplete(bool success, const QString& name, const QVariant& property);
    void setPropertyFailed(const QString& name);

private slots:
    void onPropertyChanged(QString property, QDBusVariant value);
    void getPropertiesAsyncResp(QVariantMap properties);
    void getPropertiesAsyncErr(const QDBusError&);
    void setPropertyResp();
    void setPropertyErr(const QDBusError& error);
protected slots:
    void setPath(const QString& path);
private:
    QVariantMap getAllPropertiesSync();
    
protected:
   QString m_errorName;
   QString m_errorMessage;
    
private:
   QString m_path;
   QString m_ifname;
   QVariantMap m_properties;
   QString m_pendingProperty;
   GetPropertySetting m_getpropsetting;
};

#endif
