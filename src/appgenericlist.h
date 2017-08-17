#ifndef APPGENERICLIST_H
#define APPGENERICLIST_H

#include <QWidget>
#include <QListWidget>
#include <QLayout>
#include <QJsonValue>
#include <QTimer>
#include <QProcess>
#include "scriptresult.h"
#include "app.h"

class AppGenericList : public App
{
	Q_OBJECT
	QListWidget *list;
	QJsonValue params;
	QList<QTimer *> timers;
	QList<ScriptResult *> tasks;
	const int cMinimumTimerPeriod = 100;
public:
	explicit AppGenericList(QJsonValue params, QLayout *lay, QWidget *parent = 0);
	~AppGenericList();
	bool eventFilter(QObject *watched, QEvent *event);


signals:

public slots:
	virtual void buildUi();
	virtual void destroyUi();
	virtual void load();
	virtual void unload();
	void confirmListSelection();

private:
	void setupTasks();
	void setupTimers();
	QWidget *setupReadOnlyElementUi(QJsonObject obj);
	QWidget *setupButtonElementUi(QJsonObject obj);
	QWidget *setupCheckboxElementUi(QJsonObject obj);
	QWidget *setupEditElementUi(QJsonObject obj);

};

#endif // APPGENERICLIST_H
