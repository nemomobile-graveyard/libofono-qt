/*
 * This file is part of ofono-qt
 *
 * Copyright (C) 2012 Jolla Ltd.
 *
 * Contact: Lorn Potter <lorn.potter@jollamobile.com>
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


#ifndef OFONOCELLBROADCAST_H
#define OFONOCELLBROADCAST_H

#include <QtCore/QObject>
#include <QVariant>
#include <QStringList>
#include <QDBusError>
#include "ofonomodeminterface.h"
#include "libofono-qt_global.h"


//! This class is used to access oFono cell broadcast API
/*!
 * The API is documented in
 * http://git.kernel.org/?p=network/ofono/ofono.git;a=blob_plain;f=doc/cell-broadcast-api.txt
 */
class OFONO_QT_EXPORT OfonoCellBroadcast : public OfonoModemInterface
{
    Q_OBJECT

    Q_PROPERTY(QString path READ path)
    Q_PROPERTY(QString errorName READ errorName)
    Q_PROPERTY(QString errorMessage READ errorMessage)

    Q_PROPERTY(bool powered READ powered WRITE setPowered NOTIFY powerChanged)
    Q_PROPERTY(QString topics READ topics NOTIFY topicsChanged)

public:
    explicit OfonoCellBroadcast(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent = 0);
    ~OfonoCellBroadcast();

    QString path() const;

    bool powered() const;
    void setPowered(bool b);
    QString topics() const;
    void setTopics(const QString &) const;

    QString errorName() const;
    QString errorMessage() const;
signals:
    void powerChanged(bool);
    void topicsChanged(const QString &);
    void incomingBroadcast(const QString &, quint16);
    void emergencyBroadcast(const QString &, const QVariantMap &);

public slots:

private slots:
    void propertyChanged(const QString& property, const QVariant& value);
    void inBroadcast(const QString &message, quint16 channel);
    void emBroadcast(const QString &message, const QVariantMap &properties);

};

#endif // OFONOCELLBROADCAST_H
