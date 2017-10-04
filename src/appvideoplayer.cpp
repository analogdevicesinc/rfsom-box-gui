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
	post_cmd="";
	if(params.toObject().contains("post_cmd"))
	{
		post_cmd=params.toObject()["post_cmd"].toString();
	}
	cmd = params.toObject()["cmd"].toString();

	te=new QPlainTextEdit(parent);
	te->setPlainText("");
	lay->addWidget(te);
	te->setReadOnly(true);
	te->setFocusPolicy(Qt::NoFocus);
	te->installEventFilter(this);
	exitRequested=false;
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
	unload();
	if(te!=nullptr)
	{
		te->removeEventFilter(this);
		te->deleteLater();
		te=nullptr;
	}
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
		disconnect(proc,SIGNAL(finished(int)),this,SLOT(handleExitCode(int)));
		proc->write("q");
		proc->waitForFinished(1000);		
		proc->kill();
		proc->deleteLater();
		proc = new QProcess(this);
		proc->start("/bin/sh",QStringList() << "-c" <<  post_cmd);
		proc->waitForFinished(1000);
		proc->kill();
		proc->deleteLater();
		proc=nullptr;
		exitRequested=true;
	}
}

void AppVideoPlayer::load()
{
	//unload();
	proc = new QProcess(this);
	proc->setWorkingDirectory(sharedResPath);
	proc->start("/bin/sh",QStringList() << "-c" <<  cmd);
	qDebug()<<proc->readAll();
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

		if(!exitRequested)
		{
			QKeyEvent *kev = new QKeyEvent( QEvent::KeyPress,Qt::Key_Left,Qt::NoModifier);
			QApplication::sendEvent(this, kev);
		}
	}


}
