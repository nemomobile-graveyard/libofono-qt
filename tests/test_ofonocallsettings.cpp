#include <QtTest/QtTest>
#include <QtCore/QObject>

#include <ofonocallsettings.h>

#include <QtDebug>


class TestOfonoCallSettings : public QObject
{
    Q_OBJECT

private slots:

    void validityChanged(bool validity)
    {
	qDebug() << "ValidityChanged" << validity;
    }

    void callingLinePresentationComplete(bool success, QString setting)
    {
	qDebug() << "callingLinePresentationComplete" << success << setting;
    }
    void calledLinePresentationComplete(bool success, QString setting)
    {
	qDebug() << "calledLinePresentationComplete" << success << setting;
    }
    void calledLineRestrictionComplete(bool success, QString setting)
    {
	qDebug() << "calledLineRestrictionComplete" << success << setting;
    }
    void callingLineRestrictionComplete(bool success, QString setting)
    {
	qDebug() << "callingLineRestrictionComplete" << success << setting;
    }
    void hideCallerIdComplete(bool success, QString setting)
    {
	qDebug() << "hideCallerIdComplete" << success << setting;
    }
    void voiceCallWaitingComplete(bool success, QString setting)
    {
	qDebug() << "voiceCallWaitingComplete" << success << setting;
    }

    void callingLinePresentationChanged(QString setting)
    {
	qDebug() << "callingLinePresentationChanged" << setting;
    }
    void calledLinePresentationChanged(QString setting)
    {
	qDebug() << "calledLinePresentationChanged" << setting;
    }
    void calledLineRestrictionChanged(QString setting)
    {
	qDebug() << "calledLineRestrictionChanged" << setting;
    }
    void callingLineRestrictionChanged(QString setting)
    {
	qDebug() << "callingLineRestrictionChanged" << setting;
    }
    void hideCallerIdChanged(QString setting)
    {
	qDebug() << "hideCallerIdChanged" << setting;
    }
    void voiceCallWaitingChanged(QString setting)
    {
	qDebug() << "voiceCallWaitingChanged" << setting;
    }

    void setHideCallerIdFailed()
    {
	qDebug() << "setHideCallerIdFailed";
    }
    void setVoiceCallWaitingFailed()
    {
	qDebug() << "setVoiceCallWaitingFailed";
    }

    void initTestCase()
    {
	m = new OfonoCallSettings(QString(), this);
	connect(m, SIGNAL(validityChanged(bool)), this, 
		SLOT(validityChanged(bool)));
	connect(m, SIGNAL(callingLinePresentationChanged(QString)), this, 
		SLOT(callingLinePresentationChanged(QString)));
	connect(m, SIGNAL(calledLinePresentationChanged(QString)), this, 
		SLOT(calledLinePresentationChanged(QString)));
	connect(m, SIGNAL(calledLineRestrictionChanged(QString)), this, 
		SLOT(calledLineRestrictionChanged(QString)));
	connect(m, SIGNAL(callingLineRestrictionChanged(QString)), this, 
		SLOT(callingLineRestrictionChanged(QString)));
	connect(m, SIGNAL(hideCallerIdChanged(QString)), this, 
		SLOT(hideCallerIdChanged(QString)));
	connect(m, SIGNAL(voiceCallWaitingChanged(QString)), this, 
		SLOT(voiceCallWaitingChanged(QString)));
	connect(m, SIGNAL(setHideCallerIdFailed()), this, 
		SLOT(setHideCallerIdFailed()));
	connect(m, SIGNAL(setVoiceCallWaitingFailed()), this, 
		SLOT(setVoiceCallWaitingFailed()));

	connect(m, SIGNAL(callingLinePresentationComplete( bool, QString)), this, 
		SLOT(callingLinePresentationComplete( bool, QString)));
	connect(m, SIGNAL(calledLinePresentationComplete( bool, QString)), this, 
		SLOT(calledLinePresentationComplete( bool, QString)));
	connect(m, SIGNAL(calledLineRestrictionComplete( bool, QString)), this, 
		SLOT(calledLineRestrictionComplete( bool, QString)));
	connect(m, SIGNAL(callingLineRestrictionComplete( bool, QString)), this, 
		SLOT(callingLineRestrictionComplete( bool, QString)));
	connect(m, SIGNAL(hideCallerIdComplete( bool, QString)), this, 
		SLOT(hideCallerIdComplete( bool, QString)));
	connect(m, SIGNAL(voiceCallWaitingComplete( bool, QString)), this, 
		SLOT(voiceCallWaitingComplete( bool, QString)));

    }

    void testOfonoCallSettings()
    {
	qDebug() << "validity:" << m->isValid();
	m->requestCallingLinePresentation();
	QTest::qWait(1000);
	m->requestCalledLinePresentation();
	QTest::qWait(1000);
	m->requestCalledLineRestriction();
	QTest::qWait(1000);
	m->requestCallingLineRestriction();
	QTest::qWait(1000);
	m->requestHideCallerId();
	QTest::qWait(1000);
	m->requestVoiceCallWaiting();
	QTest::qWait(1000);
	qDebug() << "setHideCallerId(enabled):";
	m->setHideCallerId("enabled");
	qDebug() << "setVoiceCallWaiting(enabled):";
	m->setVoiceCallWaiting("enabled");
	qDebug() << "setHideCallerId(disabled):";
	m->setHideCallerId("disabled");
	qDebug() << "setVoiceCallWaiting(disabled):";
	m->setVoiceCallWaiting("disabled");

	QTest::qWait(120000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoCallSettings *m;
};

QTEST_MAIN(TestOfonoCallSettings)
#include "test_ofonocallsettings.moc"
