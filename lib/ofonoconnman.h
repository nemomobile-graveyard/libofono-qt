/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef OFONOCONNMAN_H
#define OFONOCONNMAN_H

#include <QtCore/QObject>
#include <QStringList>
#include <QDBusError>
#include <QDBusObjectPath>
#include "ofonomodeminterface.h"
#include "libofono-qt_global.h"

struct OfonoConnmanStruct {
    QDBusObjectPath path;
    QVariantMap properties;
};
typedef QList<OfonoConnmanStruct> OfonoConnmanList;
Q_DECLARE_METATYPE(OfonoConnmanStruct)
Q_DECLARE_METATYPE(OfonoConnmanList)

//! This class is used to access oFono connman API
/*!
 * The API is documented in
 * http://git.kernel.org/?p=network/ofono/ofono.git;a=blob_plain;f=doc/connman-api.txt
 */
class OFONO_QT_EXPORT OfonoConnMan : public OfonoModemInterface
{
    Q_OBJECT

    Q_PROPERTY(bool attached READ attached NOTIFY attachedChanged)
    Q_PROPERTY(QString bearer READ bearer NOTIFY bearerChanged)
    Q_PROPERTY(bool suspended READ suspended NOTIFY suspendedChanged)
    Q_PROPERTY(bool roamingAllowed READ roamingAllowed WRITE setRoamingAllowed NOTIFY roamingAllowedChanged)
    Q_PROPERTY(bool powered READ powered WRITE setPowered NOTIFY poweredChanged)

public:
    OfonoConnMan(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent=0);
    ~OfonoConnMan();

    /* Properties */
    bool attached() const;
    QString bearer() const;
    bool suspended() const;
    bool roamingAllowed() const;
    bool powered() const;

    void setPowered(const bool);
    void setRoamingAllowed(const bool);
    void deactivateAll();
    QStringList getContexts();
    void addContext(const QString& type);
    void removeContext(const QString& path);

signals:
    void attachedChanged(const bool value);
    void bearerChanged(const QString &bearer);
    void suspendedChanged(const bool suspnd);
    void roamingAllowedChanged(const bool roaming);
    void poweredChanged(const bool powrd);
    void contextAdded(const QString& path);
    void contextRemoved(const QString& path);

    void deactivateAllComplete(bool success);
    void addContextComplete(bool success);
    void removeContextComplete(bool success);

    void setPoweredFailed();
    void setRoamingAllowedFailed();

private slots:
    void propertyChanged(const QString& property, const QVariant& value);
    void contextAddedChanged(const QDBusObjectPath &path, const QVariantMap &properties);
    void contextRemovedChanged(const QDBusObjectPath &path);
    void deactivateAllResp();
    void deactivateAllErr(const QDBusError& error);
    void addContextResp();
    void addContextErr(const QDBusError& error);
    void removeContextResp();
    void removeContextErr(const QDBusError& error);
    void setPropertyFailed(const QString& property);
private:
    QStringList m_contextlist;
};

#endif  /* !OFONOCONNMAN_H */
