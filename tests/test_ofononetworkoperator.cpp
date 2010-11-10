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

#include <ofononetworkoperator.h>

#include <QtDebug>


class TestOfonoNetworkOperator : public QObject
{
    Q_OBJECT

private slots:

    void statusChanged(QString status)
    {
	qDebug() << "statusChanged" << status;
    }

    void registerComplete(bool success)
    {
	qDebug() << "registerComplete:" << success;
    }

    void initTestCase()
    {
	m = new OfonoNetworkOperator("/phonesim/operator/23406", this);
	connect(m, SIGNAL(registerComplete(bool)), this, 
		SLOT(registerComplete(bool)));
	connect(m, SIGNAL(statusChanged(QString)), 
		this, 
		SLOT(statusChanged(QString)));
    }

    void testOfonoNetworkOperator()
    {
	qDebug() << "name():" << m->name();
	qDebug() << "status():" << m->status();
	qDebug() << "mobileCountryCode():" << m->mcc();
	qDebug() << "mobileNetworkCode():" << m->mnc();
	qDebug() << "technologies():" << m->technologies();
	qDebug() << "additionalInfo():" << m->additionalInfo();
	m->requestRegister();

	QTest::qWait(120000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoNetworkOperator *m;
};

QTEST_MAIN(TestOfonoNetworkOperator)
#include "test_ofononetworkoperator.moc"
