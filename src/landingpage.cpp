#include "landingpage.h"
#include <QDebug>
#include <QKeyEvent>
#include <QApplication>
#include <QJsonDocument>
#include <QFile>
#include "ui_mainwindow.h"
#include "readonlyelementui.h"
#include "scriptresult.h"
#include "common.h"

LandingPage::LandingPage(QWidget *parent) : MenuPage(parent)
{
}

void LandingPage::load()
{
	setFocus();

	for (auto timer : timers) {
		timer->start();
	}
}

void LandingPage::unload()
{
	for (auto timer : timers) {
		timer->stop();
	}
}

void LandingPage::initialize()
{
}

void LandingPage::wheelEvent(QWheelEvent *event)
{
	QKeyEvent *kev;
	kev=new QKeyEvent( QEvent::KeyPress,Qt::Key_Right,Qt::NoModifier);
	QApplication::sendEvent(this, kev);

}

void LandingPage::keyPressEvent(QKeyEvent *e)
{
	qDebug()<<"landingpage";
	MenuPage::keyPressEvent(e);
}

void LandingPage::loadJsonConfig(QString jsonFileName)
{
	QFile file(resolveFileLocation(jsonFileName));
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString jsonContent = file.readAll();
	file.close();
	QJsonDocument doc = QJsonDocument::fromJson(jsonContent.toUtf8());
	QJsonObject obj = doc.object();

	if (obj.contains("timers")) {
		for (QJsonValue val :obj["timers"].toArray()) {
			auto period = val.toInt(cMinimumTimerPeriod);

			if (period < cMinimumTimerPeriod) {
				period = cMinimumTimerPeriod;
			}

			QTimer *tim = new QTimer(this);
			timers.append(tim);
			tim->start(period);
		}

	} else {
		qDebug()<<"No timers defined in the configuration json. Creating default 1 second timer";
		QTimer *tim  = new QTimer(this);
		timers.append(tim);
		tim->start(1000); // default 1 second timer
	}

	for (auto element : obj["elements"].toArray()) {
		auto wid = new ReadOnlyElementUi(this);
		auto elObj = element.toObject();
		wid->setOneRow(true);
		wid->setIconPath(elObj["icon"].toString());
		ScriptResult *scr = new ScriptResult(elObj["cmd_read"].toString(),wid);
		wid->setStrVal(scr);
		wid->setDescription(elObj["description"].toString());
		wid->setIconSize(elObj["iconSize"].toInt());
		ui->devStatusLayout->addWidget(wid);

		int timerId = 0;

		if (elObj.contains("timer")) {
			timerId = elObj["timer"].toInt();
		}

		connect(timers[timerId],SIGNAL(timeout()),wid,SLOT(update()));
		wid->update();

	}
}

void LandingPage::updateUi()
{
}
