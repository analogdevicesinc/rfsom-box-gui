#ifndef LISTPAGE_H
#define LISTPAGE_H

#include "menupage.h"
#include <QListWidget>
#include <QJsonArray>
#include "app.h"

class LauncherPage : public MenuPage
{
	Q_OBJECT
public:
	explicit LauncherPage(QWidget *parent = 0);

	virtual void unload();
	virtual void load();
	virtual void keyPressEvent(QKeyEvent *);
	void buildUi(int index);
	void loadJsonConfig(QString);
	QListWidget *getList() const;

public slots:
	//void scrollUp();
	//void scrollDown();
	void loadApp();


private:
	QJsonArray launcher;
	virtual void initialize();
	QListWidget *list;
	int oldIndex;
	App *currentApp;

};

#endif // LISTPAGE_H
