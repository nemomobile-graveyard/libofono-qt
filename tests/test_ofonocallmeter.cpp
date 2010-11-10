#include <QtTest/QtTest>
#include <QtCore/QObject>

#include <ofonocallmeter.h>

#include <QtDebug>


class TestOfonoCallMeter : public QObject
{
    Q_OBJECT

private slots:

    void validityChanged(bool validity)
    {
	qDebug() << "ValidityChanged" << validity;
    }

    void callMeterComplete(bool success, uint value)
    {
	qDebug() << "callMeterComplete" << success << value;
    }

    void callMeterChanged(uint value)
    {
	qDebug() << "callMeterChanged" << value;
    }

    void initTestCase()
    {
	m = new OfonoCallMeter(QString(), this);
	connect(m, SIGNAL(validityChanged(bool)), this, 
		SLOT(validityChanged(bool)));
	connect(m, SIGNAL(callMeterChanged(uint)), this, 
		SLOT(callMeterChanged(uint)));
	connect(m, SIGNAL(callMeterComplete(bool, uint)), this, 
		SLOT(callMeterComplete(bool, uint)));
    }

    void testOfonoCallMeter()
    {
	qDebug() << "validity:" << m->isValid();
	m->requestCallMeter();
	QTest::qWait(1000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoCallMeter *m;
};

QTEST_MAIN(TestOfonoCallMeter)
#include "test_ofonocallmeter.moc"
