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

#include <ofonosimmanager.h>

#include <QtDebug>


class TestOfonoSimManager : public QObject
{
    Q_OBJECT

private slots:

    void validityChanged(bool validity)
    {
	qDebug() << "ValidityChanged" << validity;
    }

    void presenceChanged(bool ispresent)
    {
	qDebug() << "presenceChanged" << ispresent;
    }
    void subscriberIdentityChanged(QString imsi)
    {
	qDebug() << "subscriberIdentityChanged" << imsi;
    }
    void mobileCountryCodeChanged(QString mcc)
    {
	qDebug() << "mobileCountryCodeChanged" << mcc;
    }
    void mobileNetworkCodeChanged(QString mnc)
    {
	qDebug() << "mobileNetworkCodeChanged" << mnc;
    }
    void subscriberNumbersChanged(QStringList msisdns)
    {
	qDebug() << "subscriberNumbersChanged" << msisdns;
    }
    void serviceNumbersChanged(QMap<QString, QString> sdns)
    {
	qDebug() << "serviceNumbersChanged" << sdns;
    }
    void pinRequiredChanged(QString pintype)
    {
	qDebug() << "pinRequiredChanged" << pintype;
    }
    void lockedPinsChanged(QStringList pins)
    {
	qDebug() << "lockedPinsChanged" << pins;
    }
    void cardIdentifierChanged(QString iccid)
    {
	qDebug() << "cardIdentifierChanged" << iccid;
    }
    void preferredLanguagesChanged(QStringList languages)
    {
	qDebug() << "preferredLanguagesChanged" << languages;
    }

    void enterPinComplete(bool success)
    {
	qDebug() << "enterPinComplete:" << success;
	if (!success)
	    qDebug() << "Error name/message" << m->errorName() << m->errorMessage();
    }
    void resetPinComplete(bool success)
    {
	qDebug() << "resetPinComplete:" << success;
	if (!success)
	    qDebug() << "Error name/message" << m->errorName() << m->errorMessage();
    }

    void changePinComplete(bool success)
    {
	qDebug() << "changePinComplete:" << success;
    }

    void lockPinComplete(bool success)
    {
	qDebug() << "lockPinComplete:" << success;
    }

    void unlockPinComplete(bool success)
    {
	qDebug() << "unlockPinComplete:" << success;
    }

    void initTestCase()
    {
	m = new OfonoSimManager(QString(), this);
	connect(m, SIGNAL(validityChanged(bool)), this, 
		SLOT(validityChanged(bool)));
	connect(m, SIGNAL(presenceChanged(bool)), 
		this, 
		SLOT(presenceChanged(bool)));
	connect(m, SIGNAL(subscriberIdentityChanged(QString)), this, 
		SLOT(subscriberIdentityChanged(QString)));
	connect(m, SIGNAL(mobileCountryCodeChanged(QString)), this, 
		SLOT(mobileCountryCodeChanged(QString)));
	connect(m, SIGNAL(mobileNetworkCodeChanged(QString)), this, 
		SLOT(mobileNetworkCodeChanged(QString)));
	connect(m, SIGNAL(subscriberNumbersChanged(QStringList)), this, 
		SLOT(subscriberNumbersChanged(QStringList)));
	connect(m, SIGNAL(serviceNumbersChanged(QMap<QString, QString>)), this, 
		SLOT(serviceNumbersChanged(QMap<QString, QString>)));
	connect(m, SIGNAL(pinRequiredChanged(QString)), this, 
		SLOT(pinRequiredChanged(QString)));
	connect(m, SIGNAL(lockedPinsChanged(QStringList)), this, 
		SLOT(lockedPinsChanged(QStringList)));
	connect(m, SIGNAL(cardIdentifierChanged(QString)), this, 
		SLOT(cardIdentifierChanged(QString)));
	connect(m, SIGNAL(preferredLanguagesChanged(QStringList)), this, 
		SLOT(preferredLanguagesChanged(QStringList)));
    }

    void testOfonoSimManager()
    {
	qDebug() << "validity:" << m->isValid();
	qDebug() << "present():" << m->present();
	qDebug() << "subscriberIdentity():" << m->subscriberIdentity();
	qDebug() << "mobileCountryCode():" << m->mobileCountryCode();
	qDebug() << "mobileNetworkCode():" << m->mobileNetworkCode();
	qDebug() << "subscriberNumbers():" << m->subscriberNumbers();
	qDebug() << "serviceNumbers():" << m->serviceNumbers();
	qDebug() << "pinRequired():" << m->pinRequired();
	qDebug() << "lockedPins():" << m->lockedPins();
	qDebug() << "cardIdentifier():" << m->cardIdentifier();
	qDebug() << "preferredLanguages():" << m->preferredLanguages();

	/* If we are using phonesim */
	if (m->subscriberIdentity() == "246813579") {
	    connect(m, SIGNAL(lockPinComplete(bool)), 
		    this, SLOT(lockPinComplete(bool)));
	    connect(m, SIGNAL(unlockPinComplete(bool)), 
		    this, SLOT(unlockPinComplete(bool)));
	    connect(m, SIGNAL(changePinComplete(bool)), 
		    this, SLOT(changePinComplete(bool)));

	    if (m->pinRequired() == "pin") {
		connect(m, SIGNAL(enterPinComplete(bool)), 
			this, SLOT(enterPinComplete(bool)));
		m->requestEnterPin("pin", "2468");
		QTest::qWait(1000);
	    } else if (m->pinRequired() == "puk") {
		connect(m, SIGNAL(resetPinComplete(bool)), 
			this, SLOT(resetPinComplete(bool)));
		m->requestResetPin("puk", "13243546", "2468");	    
	    }

	    if (m->lockedPins().contains("pin")) {
		m->requestUnlockPin("pin", "2468");
		QTest::qWait(1000);
		m->requestLockPin("pin", "2468");
		QTest::qWait(1000);
		m->requestChangePin("pin", "2468", "1234");
		QTest::qWait(1000);
		m->requestChangePin("pin", "1234", "2468");
	    } else {
		m->requestLockPin("pin", "2468");
		QTest::qWait(1000);
		m->requestChangePin("pin", "2468", "1234");
		QTest::qWait(1000);
		m->requestChangePin("pin", "1234", "2468");
		QTest::qWait(1000);
		m->requestUnlockPin("pin", "2468");
	    }
	}
	QTest::qWait(120000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoSimManager *m;
};

QTEST_MAIN(TestOfonoSimManager)
#include "test_ofonosimmanager.moc"
