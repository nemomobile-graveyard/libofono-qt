#include <QtTest/QtTest>
#include <QtCore/QObject>

#include <ofonovoicecallmanager.h>

#include <QtDebug>


class TestOfonoVoiceCallManager : public QObject
{
    Q_OBJECT

private slots:

    void validityChanged(bool validity)
    {
	qDebug() << "ValidityChanged" << validity;
    }

    void initTestCase()
    {
	m = new OfonoVoiceCallManager(QString(), this);
	connect(m, SIGNAL(validityChanged(bool)), this, 
		SLOT(validityChanged(bool)));
    }

    void testOfonoVoiceCallManager()
    {
	qDebug() << "validity:" << m->isValid();
	qDebug() << "emergencyNumbers():" << m->emergencyNumbers();
	QTest::qWait(1000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoVoiceCallManager *m;
};

QTEST_MAIN(TestOfonoVoiceCallManager)
#include "test_ofonovoicecallmanager.moc"
