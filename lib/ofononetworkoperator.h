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
 

#ifndef OFONONETWORKOPERATOR_H
#define OFONONETWORKOPERATOR_H

#include <QtCore/QObject>
#include <QVariant>
#include <QStringList>
#include <QDBusError>

#include "libofono-qt_global.h"

class OfonoInterface;

//! This class is used to access oFono network operator API
/*!
 * The API is documented in
 * http://git.kernel.org/?p=network/ofono/ofono.git;a=blob_plain;f=doc/network-api.txt
 */
class OFONO_QT_EXPORT OfonoNetworkOperator : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString path READ path)
    Q_PROPERTY(QString errorName READ errorName)
    Q_PROPERTY(QString errorMessage READ errorMessage)
    
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString mcc READ mcc NOTIFY mccChanged)
    Q_PROPERTY(QString mnc READ mnc NOTIFY mncChanged)
    Q_PROPERTY(QStringList technologies READ technologies NOTIFY technologiesChanged)
    Q_PROPERTY(QString additionalInfo READ additionalInfo NOTIFY additionalInfoChanged)
    
public:
    OfonoNetworkOperator(const QString& operatorId, QObject *parent=0);
    OfonoNetworkOperator(const OfonoNetworkOperator& op);
    ~OfonoNetworkOperator();

    OfonoNetworkOperator operator=(const OfonoNetworkOperator &op);
    bool operator==(const OfonoNetworkOperator &op);

    //! Returns the D-Bus object path of the network operator object
    QString path() const;
    
    //! Get the D-Bus error name of the last operation.
    /*!
     * Returns the D-Bus error name of the last operation (setting a property
     * or calling a method) if it has failed
     */
    QString errorName() const;

    //! Get the D-Bus error message of the last operation.
    /*!
     * Returns the D-Bus error message of the last operation (setting a property
     * or calling a method) if it has failed
     */
    QString errorMessage() const;

    void registerOp();
    
    QString name() const;
    QString status() const;
    QString mcc() const;
    QString mnc() const;
    QStringList technologies() const;
    QString additionalInfo() const;
    
signals:
    void registerComplete(bool success);
    
    void nameChanged(const QString &name);
    void statusChanged(const QString &status);
    void mccChanged(const QString &mcc);
    void mncChanged(const QString &mnc);
    void technologiesChanged(const QStringList &technologies);
    void additionalInfoChanged(const QString &additionalInfo);

private:

private slots:
    void propertyChanged(const QString& property, const QVariant& value);
    
    void registerResp();
    void registerErr(const QDBusError& error);


private:
    OfonoInterface *m_if;

};
#endif
