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

#include <ofonomodem.h>

#include <QtDebug>

class TestOfonoModem : public QObject
{
    Q_OBJECT

private slots:
    void validityChanged(bool validity)
    {
    qDebug() << "ValidityChanged" << validity;
	qDebug() << "validity" << m->isValid() << "modemPath" << m->modemPath();
    }

    void modemPathChanged(QString modemPath)
    {
    qDebug() << "ModemPathChanged" << modemPath;
	qDebug() << "validity" << m->isValid() << "modemPath" << m->modemPath();
    }

    void poweredChanged(bool powered)
    {
    qDebug() << "PoweredChanged" << powered;
    }

    void onlineChanged(bool online)
    {
    qDebug() << "OnlineChanged" << online;
    }

    void nameChanged(QString name)
    {
    qDebug() << "nameChanged" << name;
    }
    
    void manufacturerChanged(QString manufacturer)
    {
    qDebug() << "manufacturerChanged" << manufacturer;
    }

    void modelChanged(QString model)
    {
    qDebug() << "modelChanged" << model;
    }

    void revisionChanged(QString revision)
    {
    qDebug() << "revisionChanged" << revision;
    }

    void serialChanged(QString serial)
    {
    qDebug() << "serialChanged" << serial;
    }

    void featuresChanged(QStringList features)
    {
    qDebug() << "FeaturesChanged" << features;
    }

    void interfacesChanged(QStringList interfaces)
    {
    qDebug() << "InterfacesChanged" << interfaces;
    }

    void initTestCase()
    {
	m = new OfonoModem(OfonoModem::ManualSelect, "/isimodem0", this);
//	m = new OfonoModem(this);
	connect(m, SIGNAL(validityChanged(bool)), this, SLOT(validityChanged(bool)));
    connect(m, SIGNAL(modemPathChanged(QString)),
	    this, SLOT(modemPathChanged(QString)));
	connect(m, SIGNAL(poweredChanged(bool)), this, SLOT(poweredChanged(bool)));
 	connect(m, SIGNAL(onlineChanged(bool)), this, SLOT(onlineChanged(bool)));
   connect(m, SIGNAL(nameChanged(QString)),
	    this, SLOT(nameChanged(QString)));
    connect(m, SIGNAL(manufacturerChanged(QString)),
	    this, SLOT(manufacturerChanged(QString)));
    connect(m, SIGNAL(modelChanged(QString)),
	    this, SLOT(modelChanged(QString)));
    connect(m, SIGNAL(revisionChanged(QString)),
	    this, SLOT(revisionChanged(QString)));
    connect(m, SIGNAL(serialChanged(QString)),
	    this, SLOT(serialChanged(QString)));
	connect(m, SIGNAL(featuresChanged(QStringList)), this, SLOT(featuresChanged(QStringList)));
	connect(m, SIGNAL(interfacesChanged(QStringList)), this, SLOT(interfacesChanged(QStringList)));
    }

    void testOfonoModem()
    {
	qDebug() << "validity" << m->isValid() << "modemPath" << m->modemPath();
    qDebug() << m->powered() << m->online() << m->manufacturer() << m->model() << m->revision() << m->serial() << m->features() << m->interfaces();
    m->setOnline(true);
    QTest::qWait(120000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoModem *m;
};

QTEST_MAIN(TestOfonoModem)
#include "test_ofonomodem.moc"
