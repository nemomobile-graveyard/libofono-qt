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

#include <ofonointerface.h>

#include <QtDebug>

class TestOfonoInterface : public QObject
{
    Q_OBJECT

private slots:

    void onPropertyChanged(const QString& name, const QVariant& property)
    {
    qDebug() << name << property;
    qDebug () << oi->properties();
    }

    void initTestCase()
    {
	oi = new OfonoInterface("/isimodem1", "org.ofono.Modem", OfonoInterface::GetAllOnStartup, this);
	connect(oi, SIGNAL(propertyChanged(const QString&, const QVariant&)), this, SLOT(onPropertyChanged(const QString&, const QVariant&)));
    }

    void testOfonoInterface()
    {
    oi->setProperty("Online", qVariantFromValue(true));
    QTest::qWait(30000);
//    qDebug() << oi->properties();
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoInterface *oi;
};

QTEST_MAIN(TestOfonoInterface)
#include "test_ofonointerface.moc"
