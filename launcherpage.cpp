#include "launcherpage.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include "appgenericlist.h"
#include "appvideoplayer.h"
#include <QTime>

LauncherPage::LauncherPage(QWidget *parent) : MenuPage(parent)
{
	currentApp = new App(this);
	oldIndex = -1;
}

void LauncherPage::loadJsonConfig(QString jsonFileName)
{
	QString val;
	QFile file;
	//qDebug() << "App path : " << qApp->applicationDirPath();
	file.setFileName(jsonFileName);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	val = file.readAll();
	file.close();
	QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());

	launcher = d.array();

	for (auto val:launcher) {
		auto name = val.toObject()["name"];
		ui->listWidget->addItem(name.toString());
	}
}

void LauncherPage::buildUi(int index)
{
	if (oldIndex != index) {
		//auto key = item->text();
		auto uiJson = launcher[index].toObject();
		auto name = uiJson["name"];
		auto type = uiJson["type"];
		auto params = uiJson["params"];

		qDebug()<<"name "<<name;
		qDebug()<<"type "<<type;

		QStringList myOptions;
		myOptions << "list" << "video" <<"imu";// << "goRegister";

		switch (myOptions.indexOf(type.toString())) {
		case 0:
			currentApp->destroyUi();
			delete currentApp;
			currentApp = new AppGenericList(params,ui->appLayout,ui->appPage);
			// set keys
			break;

		case 1:
			currentApp->destroyUi();
			delete currentApp;
			currentApp = new AppVideoPlayer(params,ui->appLayout, ui->appPage);

		default:
			break;
		}

		currentApp->buildUi();
		oldIndex = index;

	} else {
		currentApp->load();
	}
}

void LauncherPage::unload()
{
	qInfo()<<"unload";
}

void LauncherPage::load()
{
	qInfo()<<"load";
	currentApp->unload();
	list->setFocus();
	qDebug()<<updatesEnabled();
	update();
	QCoreApplication::processEvents();
}

void LauncherPage::initialize()
{
	list = ui->listWidget;
	list->setCurrentRow(0);
}

void LauncherPage::keyPressEvent(QKeyEvent *e)
{
	switch (e->key()) {
	case Qt::Key_Return:
	case Qt::Key_Right:
		buildUi(ui->listWidget->currentRow());
	}

	MenuPage::keyPressEvent(e);
}

QListWidget *LauncherPage::getList() const
{
	return list;
}
