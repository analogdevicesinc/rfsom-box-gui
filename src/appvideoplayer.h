#ifndef APPVIDEOPLAYER_H
#define APPVIDEOPLAYER_H
#include <QJsonValue>
#include <QProcess>
#include <QLayout>
#include <QPlainTextEdit>
#include "app.h"

class AppVideoPlayer : public App
{
	Q_OBJECT
	QJsonValue params;
	QProcess *proc;
	QString cmd;
	QString post_cmd;
	QLayout *lay;
	QPlainTextEdit *te;
	bool scrollToBottom;
	QWidget *parent;
	bool exitRequested;
	bool hangOnFinish;
	bool consoleEnabled;
	bool scrollConsole;
	QMap<inputKeys,QString> inputMap;

public:
	explicit AppVideoPlayer(QJsonValue params, QLayout *lay, QWidget *parent = 0);
	~AppVideoPlayer();
	bool isKeyMapped(int key);
	bool eventFilter(QObject *watched, QEvent *event);
	void keyPressEvent(QKeyEvent *event);

signals:

public slots:
	virtual void buildUi();
	virtual void destroyUi();
	virtual void load();
	virtual void unload();
	virtual void handleExitCode(int);
	virtual void readStdErr();

private:

};

#endif // APPVIDEOPLAYER_H
