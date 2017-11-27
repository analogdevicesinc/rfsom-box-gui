#ifndef MAINMENU_H
#define MAINMENU_H
#include <QStackedWidget>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QJsonObject>
#include <QJsonArray>
#include "menupage.h"
#include "landingpage.h"
#include "launcherpage.h"
#include "apppage.h"
#include "app.h"

namespace Ui {
class MainWindow;
}

class MainMenu : public QStackedWidget
{
	Q_OBJECT
public:
	enum pages {
		landingPageId,
		launcherPageId,
		appPageId
	};

	MainMenu(QWidget *parent);
	void initialize(Ui::MainWindow *_ui);
	void keyPressEvent(QKeyEvent *e);
	bool eventFilter(QObject *watched, QEvent *event);


public slots:
private:
	QList<MenuPage *> pages;
	LandingPage *landingPage;
	LauncherPage *launcherPage;
	AppPage *appPage;
	Ui::MainWindow *ui;
};

#endif // MAINMENU_H
