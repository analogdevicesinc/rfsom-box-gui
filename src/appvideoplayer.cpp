#include "appvideoplayer.h"
#include <QJsonObject>
#include <QCoreApplication>
#include <QDebug>
#include "common.h"
#include <QTime>

AppVideoPlayer::AppVideoPlayer(QJsonValue params, QLayout *lay,
                               QWidget *parent) : App(parent) , params(params)
{
	proc=nullptr;
	cmd = params.toObject()["cmd"].toString();
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
	proc->setWorkingDirectory(sharedResPath);
	proc->start("/bin/sh",QStringList() << "-c" <<  cmd);
	qDebug()<<proc->readAll();

}

