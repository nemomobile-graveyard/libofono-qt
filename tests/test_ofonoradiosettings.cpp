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

#include <ofonoradiosettings.h>

#include <QtDebug>


class TestOfonoRadioSettings : public QObject
{
    Q_OBJECT

private slots:

    void technologyPreferenceChanged(const QString pref)
    {
	qDebug() << "technologyPreferenceChanged" << pref;
    }

    void validityChanged(bool validity)
    {
	qDebug() << "ValidityChanged" << validity;
    }

    void initTestCase()
    {
	m = new OfonoRadioSettings(OfonoModem::AutomaticSelect, QString(), this);
	connect(m, SIGNAL(validityChanged(bool)), this, 
		SLOT(validityChanged(bool)));
	connect(m, SIGNAL(technologyPreferenceChanged(QString)), 
		this, 
		SLOT(technologyPreferenceChanged(QString)));
    }

    void testOfonoRadioSettings()
    {
	qDebug() << "validity:" << m->isValid();
	qDebug() << "technologyPreference():" << m->technologyPreference();
	QTest::qWait(120000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoRadioSettings *m;
};

QTEST_MAIN(TestOfonoRadioSettings)
#include "test_ofonoradiosettings.moc"
