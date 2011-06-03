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

#include <QtDBus/QtDBus>
#include <QtCore/QObject>

#include "ofonophonebook.h"
#include "ofonointerface.h"

#define IMPORT_TIMEOUT 300000

OfonoPhonebook::OfonoPhonebook(OfonoModem::SelectionSetting modemSetting, const QString &modemPath, QObject *parent)
    : OfonoModemInterface(modemSetting, modemPath, "org.ofono.Phonebook", OfonoGetAllOnFirstRequest, parent)
{

}

OfonoPhonebook::~OfonoPhonebook()
{
}

void OfonoPhonebook::import()
{
    QDBusMessage request;

    request = QDBusMessage::createMethodCall("org.ofono",
					     path(), m_if->ifname(),
					     "Import");
    request.setArguments(QList<QVariant>());

    QDBusConnection::systemBus().callWithCallback(request, this,
					SLOT(importResp(QString)),
					SLOT(importErr(const QDBusError&)),
					IMPORT_TIMEOUT);
}

void OfonoPhonebook::importResp(const QString &entries)
{
    emit importComplete(true, entries);
}

void OfonoPhonebook::importErr(const QDBusError& error)
{
    m_if->setError(error.name(), error.message());
    emit importComplete(false, QString());
}

