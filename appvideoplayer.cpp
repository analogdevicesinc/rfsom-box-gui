#include "appvideoplayer.h"
#include <QJsonObject>
#include <QCoreApplication>
#include <QDebug>
#include <QTime>

AppVideoPlayer::AppVideoPlayer(QJsonValue params, QLayout *lay,
                               QWidget *parent) : App(parent) , params(params)
{

	proc=nullptr;
	cmd = params.toObject()["cmd"].toString();
	/*setStyleSheet("background-color:red");
	canvas = new QWidget(this);
	canvas->setStyleSheet("background-color:red");
	lay->addWidget(canvas);*/
}

AppVideoPlayer::~AppVideoPlayer()
{
//	delete canvas;
}

void AppVideoPlayer::buildUi()
{
	load();
}

void AppVideoPlayer::destroyUi()
{
	unload();
}

void AppVideoPlayer::unload()
{
	if (proc!=nullptr) {
		proc->write("q");
		proc->waitForFinished();
		delete proc;
		proc=nullptr;
	}
}

void AppVideoPlayer::load()
{
	unload();
	proc = new QProcess(this);
	proc->setWorkingDirectory(QCoreApplication::applicationDirPath()+"/../");
	proc->start("/bin/sh",QStringList() << "-c" <<  cmd);
	qDebug()<<proc->readAll();

}

