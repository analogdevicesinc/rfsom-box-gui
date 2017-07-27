#ifndef APPVIDEOPLAYER_H
#define APPVIDEOPLAYER_H
#include <QJsonValue>
#include <QProcess>
#include <QLayout>
#include "app.h"

class AppVideoPlayer : public App
{
	Q_OBJECT
	//QWidget *canvas;
	QJsonValue params;
	QProcess *proc;
	QString cmd;
	//const int cMinimumTimerPeriod = 100;
public:
	explicit AppVideoPlayer(QJsonValue params, QLayout *lay, QWidget *parent = 0);
	~AppVideoPlayer();
	//bool eventFilter(QObject *watched, QEvent *event);

signals:

public slots:
	virtual void buildUi();
	virtual void destroyUi();
	virtual void load();
	virtual void unload();
//	void confirmListSelection();
//	virtual void keyPressEvent(QKeyEvent *e);

private:
	/*void setupTimers();
	QWidget *setupReadOnlyElementUi(QJsonObject obj);
	QWidget *setupButtonElementUi(QJsonObject obj);
	QWidget *setupCheckboxElementUi(QJsonObject obj);
	QWidget *setupEditElementUi(QJsonObject obj);*/

};

#endif // APPVIDEOPLAYER_H
