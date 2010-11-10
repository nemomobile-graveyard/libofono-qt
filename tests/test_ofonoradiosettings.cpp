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
	m = new OfonoRadioSettings(QString(), this);
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
