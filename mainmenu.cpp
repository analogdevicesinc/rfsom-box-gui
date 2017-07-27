#include <QDebug>
#include <QJsonDocument>
#include "mainmenu.h"
#include "ui_mainwindow.h"

MainMenu::MainMenu(QWidget *parent) : QStackedWidget(parent)
{
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
	launcherPage->loadJsonConfig(jsonFileName);
	setCurrentIndex(landingPageId);
	landingPage->load();
}
