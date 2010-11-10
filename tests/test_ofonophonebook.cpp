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

#include <ofonophonebook.h>

#include <QtDebug>

class TestOfonoPhonebook : public QObject
{
    Q_OBJECT

private slots:
    void validityChanged(bool validity)
    {
    qDebug() << "ValidityChanged" << validity;
    }

    void importComplete(bool success, QString entries)
    {
    qDebug() << "importComplete" << success << entries;
    }

    void initTestCase()
    {
//	m = new OfonoPhonebook("/isimodem0", this);
	m = new OfonoPhonebook(QString(), this);
	connect(m, SIGNAL(validityChanged(bool)), this, SLOT(validityChanged(bool)));
    connect(m, SIGNAL(importComplete(bool, QString)),
	    this, SLOT(importComplete(bool, QString)));
    }

    void testOfonoPhonebook()
    {
	qDebug() << "validity" << m->isValid();
	m->requestImport();
    QTest::qWait(120000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoPhonebook *m;
};

QTEST_MAIN(TestOfonoPhonebook)
#include "test_ofonophonebook.moc"
