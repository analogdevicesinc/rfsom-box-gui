#include "scriptresult.h"
#include <QDebug>
#include "common.h"
#include <QCoreApplication>
#include "toastnotification.h"


bool ScriptResult::getRunInBackground() const
{
	return runInBackground;
}

void ScriptResult::setRunInBackground(bool value)
{
	runInBackground = value;
}

int ScriptResult::getReturnVal() const
{
	return proc->exitCode();
}

bool ScriptResult::getErrEnabled() const
{
	return errEnabled;
}

void ScriptResult::setErrEnabled(bool value)
{
	errEnabled = value;
}

ScriptResult::ScriptResult(QString cmd, QObject *parent,bool muted) : StringValue(parent),
	cmd(cmd),proc(nullptr),runInBackground(false),errEnabled(false),muted(muted)

{

}

bool ScriptResult::getMuted() const
{
	return muted;
}

void ScriptResult::setMuted(bool value)
{
	muted = value;
}

ScriptResult::~ScriptResult()
{

}

QString ScriptResult::get()
{
	auto raw_val = run_script();
	val = convert(raw_val);
	return val;
}

QString ScriptResult::run()
{
	return get();
}

QString ScriptResult::run_script()
{
	QString ret="";
	if(proc!=nullptr)
	{
		ret=proc->readAll();
		proc->waitForFinished(1000);
		qDebug()<<ret;		
		proc->deleteLater();
		proc=nullptr;
	}

	proc = new QProcess(this);
	proc->setWorkingDirectory(sharedResPath);

	QProcessEnvironment qpe;

	for (auto key : vars.keys()) {
		qpe.insert(key,vars[key]);
	}

	proc->setProcessEnvironment(qpe);
	proc->start("/bin/sh",QStringList() << "-c" <<  cmd);

	if (!runInBackground) {
		proc->waitForFinished(100);
		ret = proc->readAll();
		proc->deleteLater();
		proc=nullptr;	
	} else {
		connect(proc,SIGNAL(finished(int)),this,SLOT(scriptFinished(int)));
		ret = proc->readAll();
		qDebug()<<ret;
		//connect(proc,SIGNAL(finished(int)),proc,SLOT(deleteLater()));
	}
	if(errEnabled && ! muted)
	{
		if(proc->exitCode()!=0)
		       ToastNotification *tn= new ToastNotification("Error 0x"+QString::number(proc->exitCode(),16),500);
	}

	return ret;
}

void ScriptResult::scriptFinished(int)
{
	if(proc)
	{
		qDebug()<<(proc->readAll());
		auto exit=proc->exitCode();
		QString err = proc->readAllStandardError();
		qDebug()<<exit<<" code "<< err;

		if(!muted)
		{
			if(exit==0)
			{
				if(err=="")
				{
					ToastNotification *tn= new ToastNotification("Success",500);
				}
				else
				{
					ToastNotification *tn= new ToastNotification(err,500);
				}
			}
			else
			{
				ToastNotification *tn= new ToastNotification("err: 0x"+QString::number(exit)+" "+err,500);
			}
		}
	}
}

QString ScriptResult::convert(QString v)
{
	return v.simplified();
}

QString ScriptResult::getCmd() const
{
	return cmd;
}

void ScriptResult::setCmd(const QString& value)
{
	cmd = value;
}
