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

#include <ofononetworkregistration.h>

#include <QtDebug>


class TestOfonoNetworkRegistration : public QObject
{
    Q_OBJECT

private slots:

    void modeChanged(QString mode)
    {
	qDebug() << "modeChanged" << mode;
    }

    void statusChanged(QString status)
    {
	qDebug() << "statusChanged" << status;
    }

    void locationAreaCodeChanged(uint locationAreaCode)
    {
	qDebug() << "locationAreaCodeChanged" << locationAreaCode;
    }

    void cellIdChanged(uint cellId)
    {
	qDebug() << "cellIdChanged" << cellId;
    }

    void mccChanged(QString mcc)
    {
	qDebug() << "mccChanged" << mcc;
    }

    void mncChanged(QString mnc)
    {
	qDebug() << "mncChanged" << mnc;
    }

    void technologyChanged(QString technology)
    {
	qDebug() << "technologyChanged" << technology;
    }

    void nameChanged(QString name)
    {
	qDebug() << "nameChanged" << name;
    }

    void strengthChanged(uint strength)
    {
	qDebug() << "strengthChanged" << strength;
    }

    void baseStationChanged(QString baseStation)
    {
	qDebug() << "baseStationChanged" << baseStation;
    }

    void registerComplete(bool success)
    {
	qDebug() << "registerComplete:" << success;
    }

    void deregisterComplete(bool success)
    {
	qDebug() << "deregisterComplete:" << success;
    }

    void getOperatorsComplete(bool success, QStringList oplist)
    {
	qDebug() << "getOperatorsComplete:" << success << oplist;
    }

    void scanComplete(bool success, QStringList oplist)
    {
	qDebug() << "scanComplete:" << success << oplist;
    }

    void initTestCase()
    {
	m = new OfonoNetworkRegistration(OfonoModem::AutomaticSelect, QString(), this);

	connect(m, SIGNAL(registerComplete(bool)), this, 
		SLOT(registerComplete(bool)));
	connect(m, SIGNAL(deregisterComplete(bool)), this, 
		SLOT(deregisterComplete(bool)));
    connect(m, SIGNAL(getOperatorsComplete(bool, QStringList)), 
            this, SLOT(getOperatorsComplete(bool, QStringList)));
    connect(m, SIGNAL(scanComplete(bool, QStringList)), 
            this, SLOT(scanComplete(bool, QStringList)));

	connect(m, SIGNAL(modeChanged(QString)), 
		this, SLOT(modeChanged(QString)));
	connect(m, SIGNAL(statusChanged(QString)), 
		this, SLOT(statusChanged(QString)));
	connect(m, SIGNAL(locationAreaCodeChanged(uint)), 
		this, SLOT(locationAreaCodeChanged(uint)));
	connect(m, SIGNAL(cellIdChanged(uint)), 
		this, SLOT(cellIdChanged(uint)));
	connect(m, SIGNAL(mccChanged(QString)), 
		this, SLOT(mccChanged(QString)));
	connect(m, SIGNAL(mncChanged(QString)), 
		this, SLOT(mncChanged(QString)));
	connect(m, SIGNAL(technologyChanged(QString)), 
		this, SLOT(technologyChanged(QString)));
	connect(m, SIGNAL(nameChanged(QString)), 
		this, SLOT(nameChanged(QString)));
	connect(m, SIGNAL(strengthChanged(uint)), 
		this, SLOT(strengthChanged(uint)));
	connect(m, SIGNAL(baseStationChanged(QString)), 
		this, SLOT(baseStationChanged(QString)));

    }

    void testOfonoNetworkRegistration()
    {
	qDebug() << "mode():" << m->mode();
	qDebug() << "status():" << m->status();
	qDebug() << "lac():" << m->locationAreaCode();
	qDebug() << "cellid():" << m->cellId();
	qDebug() << "mobileCountryCode():" << m->mcc();
	qDebug() << "mobileNetworkCode():" << m->mnc();
	qDebug() << "technology():" << m->technology();
	qDebug() << "name():" << m->name();
	qDebug() << "strength():" << m->strength();
	qDebug() << "baseStation():" << m->baseStation();
	
	m->getOperators();
	QTest::qWait(5000);

	m->scan();
	QTest::qWait(10000);
	
	m->deregister();
	QTest::qWait(10000);
	m->registerOp();

	QTest::qWait(120000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoNetworkRegistration *m;
};

QTEST_MAIN(TestOfonoNetworkRegistration)
#include "test_ofononetworkregistration.moc"
