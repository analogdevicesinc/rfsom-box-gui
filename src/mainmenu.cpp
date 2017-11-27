#include <QDebug>
#include <QJsonDocument>
#include <QFile>
#include "mainmenu.h"
#include "common.h"
#include "appvideoplayer.h"
#include "ui_mainwindow.h"

MainMenu::MainMenu(QWidget *parent) : QStackedWidget(parent)
{
	installEventFilter(this);
}

bool MainMenu::eventFilter(QObject *watched, QEvent *event)
{
	qDebug()<<event<<watched;
	if(event->type()==QEvent::KeyPress)
	{
		QKeyEvent *key = static_cast<QKeyEvent *>(event);
		qDebug()<<key;
		auto videoApp = dynamic_cast<AppVideoPlayer*>(launcherPage->getCurrentApp());
		if(videoApp && currentIndex()==appPageId)
		{
			if(videoApp->isKeyMapped(key->key()))
			{
				QApplication::sendEvent(launcherPage->getCurrentApp(),key);
				return true;
			}
		}
		keyPressEvent(key);
		return true;
	}
	return false;

}

void MainMenu::keyPressEvent(QKeyEvent *e)
{
	switch (e->key()) {
	case Qt::Key_Left:
		pages[currentIndex()]->unload();
		setCurrentIndex(currentIndex()-1);
		pages[currentIndex()]->load();
		return;

	case Qt::Key_Return:
	case Qt::Key_Right:
		pages[currentIndex()]->unload();
		setCurrentIndex(currentIndex()+1);
		pages[currentIndex()]->load();
		return;

	case Qt::Key_Q:
		QCoreApplication::quit();
		return;
	}

	QStackedWidget::keyPressEvent(e);
}

void MainMenu::initialize(Ui::MainWindow *_ui)
{
	ui=_ui;
	landingPage = ui->landingPage;
	launcherPage = ui->listPage;
	appPage = ui->appPage;
	pages.append(landingPage);
	pages.append(launcherPage);
	pages.append(appPage);

	for (auto& page : pages) {
		page->init(ui);
	}

	QFile file(resolveFileLocation("main.json"));

	file.open(QIODevice::ReadOnly);
	QString jsonContent = file.readAll();
	file.close();
	QJsonDocument main = QJsonDocument::fromJson(jsonContent.toUtf8());
	QJsonObject obj = main.object();

	launcherPage->loadJsonConfig(obj["launcher"].toString());
	landingPage->loadJsonConfig(obj["landing"].toString());

	setCurrentIndex(landingPageId);
	landingPage->load();
}
