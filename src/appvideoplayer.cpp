#include "appvideoplayer.h"
#include <QJsonObject>
#include <QCoreApplication>
#include <QScrollBar>
#include <QApplication>
#include <QDebug>
#include "common.h"
#include <QTime>
#include <QPlainTextEdit>

AppVideoPlayer::AppVideoPlayer(QJsonValue params, QLayout *lay, //QPlainTextEdit* te,
			       QWidget *parent) : App(parent) , params(params), lay(lay), te(nullptr)
{
	proc=nullptr;
	cmd = params.toObject()["cmd"].toString();
	te=new QPlainTextEdit(parent);
	te->setPlainText("");
	lay->addWidget(te);
	te->setReadOnly(true);
	te->setFocusPolicy(Qt::NoFocus);
	te->installEventFilter(this);
}

AppVideoPlayer::~AppVideoPlayer()
{

}

void AppVideoPlayer::buildUi()
{
	load();
}

void AppVideoPlayer::destroyUi()
{
	if(te!=nullptr)
	{
		te->removeEventFilter(this);
		delete te;
		te=nullptr;
	}
	unload();
}

bool AppVideoPlayer::eventFilter(QObject *watched, QEvent *event)
{
	if(watched==te && event->type()==QEvent::Wheel)
	{
		QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);

		auto val = te->verticalScrollBar()->value();
		if (wheelEvent->delta()>0) {
			val--;
		} else {
			val++;
		}
		te->verticalScrollBar()->setValue(val);
	}
	return false;
}

void AppVideoPlayer::unload()
{
	if (proc!=nullptr) {
		proc->write("q");
		proc->waitForFinished(1000);
		delete proc;
		proc=nullptr;
	}
}

void AppVideoPlayer::load()
{
	unload();
	proc = new QProcess(this);
	proc->setWorkingDirectory(sharedResPath);
	proc->start("/bin/sh",QStringList() << "-c" <<  cmd);
	qDebug()<<proc->readAll();
	/*te=new QPlainTextEdit(this);
	te->setWordWrapMode(QTextOption::WrapAnywhere);
	te->setReadOnly(true);
	lay->addWidget(te);*/
	connect(proc,SIGNAL(finished(int)),this,SLOT(handleExitCode(int)));
}

void AppVideoPlayer::handleExitCode(int exitCode)
{
	if(exitCode!=0)
	{
		QString text="Errorcode "+QString::number(exitCode,16);
		te->setPlainText(text);
		te->appendPlainText(proc->errorString());
		te->appendPlainText(proc->readAllStandardError());
		setFocus();
	}
	else
	{
		qDebug()<<"App finished";

		QKeyEvent *kev = new QKeyEvent( QEvent::KeyPress,Qt::Key_Left,Qt::NoModifier);
		QApplication::sendEvent(this, kev);
	}


}
