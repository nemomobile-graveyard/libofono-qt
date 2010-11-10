#include <QtTest/QtTest>
#include <QtCore/QObject>

#include <ofonomessagewaiting.h>

#include <QtDebug>

class TestOfonoMessageWaiting : public QObject
{
    Q_OBJECT

private slots:

    void voicemailWaitingChanged(bool waiting)
    {
	qDebug() << "voicemailWaitingChanged" << waiting;
    }

    void voicemailMessageCountChanged(int count)
    {
	qDebug() << "voicemailMessageCountChanged" << count;
    }

    void voicemailMailboxNumberChanged(QString mailboxnumber)
    {
	qDebug() << "voicemailMailboxNumberChanged" << mailboxnumber;
    }

    void validityChanged(bool validity)
    {
	qDebug() << "ValidityChanged" << validity;
    }

    void initTestCase()
    {
	m = new OfonoMessageWaiting(QString(), this);
	connect(m, SIGNAL(validityChanged(bool)), this, 
		SLOT(validityChanged(bool)));
	connect(m, SIGNAL(voicemailWaitingChanged(bool)), this, 
		SLOT(voicemailWaitingChanged(bool)));
	connect(m, SIGNAL(voicemailMessageCountChanged(int)), this, 
		SLOT(voicemailMessageCountChanged(int)));
	connect(m, SIGNAL(voicemailMailboxNumberChanged(QString)), this, 
		SLOT(voicemailMailboxNumberChanged(QString)));
    }

    void testOfonoMessageWaiting()
    {
	qDebug() << "validity:" << m->isValid();
	qDebug() << "voicemailWaiting():" << m->voicemailWaiting();
	qDebug() << "voicemailMessageCount():" << m->voicemailMessageCount();
	qDebug() << "voicemailMailboxNumber():" << m->voicemailMailboxNumber();
	QTest::qWait(120000);
    }


    void cleanupTestCase()
    {

    }


private:
    OfonoMessageWaiting *m;
};

QTEST_MAIN(TestOfonoMessageWaiting)
#include "test_ofonomessagewaiting.moc"
