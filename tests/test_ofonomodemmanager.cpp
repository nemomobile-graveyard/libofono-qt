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

#include <QtTest/QtTest>
#include <QtCore/QObject>

#include <ofonomodemmanager.h>

#include <QtDebug>


class TestOfonoModemManager : public QObject
{
    Q_OBJECT

private slots:
    void modemAdded(QString modem)
    {
    qDebug() << "Modem added" << modem;
    }

    void modemRemoved(QString modem)
    {
    qDebug() << "Modem removed" << modem;
    }

    void initTestCase()
    {
	mm = new OfonoModemManager(this);
	connect(mm, SIGNAL(modemAdded(QString)), this, SLOT(modemAdded(QString)));
	connect(mm, SIGNAL(modemRemoved(QString)), this, SLOT(modemRemoved(QString)));
    }

    void testOfonoModemManager()
    {
    qDebug() << mm->modems();
    QTest::qWait(30000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoModemManager *mm;
};

QTEST_MAIN(TestOfonoModemManager)
#include "test_ofonomodemmanager.moc"
