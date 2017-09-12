#include "appvideoplayer.h"
#include <QJsonObject>
#include <QCoreApplication>
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
		delete te;
		te=nullptr;
	}
	unload();
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

	}

}
