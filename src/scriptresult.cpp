#include "scriptresult.h"
#include <QDebug>
#include "common.h"
#include <QCoreApplication>

bool ScriptResult::getRunInBackground() const
{
	return runInBackground;
}

void ScriptResult::setRunInBackground(bool value)
{
	runInBackground = value;
}

ScriptResult::ScriptResult(QString cmd, QObject *parent) : StringValue(parent),
	cmd(cmd),runInBackground(false)

{

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

	proc = new QProcess(this);
	proc->setWorkingDirectory(sharedResPath);

	QProcessEnvironment qpe;

	for (auto key : vars.keys()) {
		qpe.insert(key,vars[key]);
	}

	proc->setProcessEnvironment(qpe);
	proc->start("/bin/sh",QStringList() << "-c" <<  cmd);
	QString ret="";

	if (!runInBackground) {
		proc->waitForFinished(100);
		ret = proc->readAll();
		delete proc;
		proc=nullptr;
	} else {
		ret = proc->readAll();
		qDebug()<<ret;
		connect(proc,SIGNAL(finished(int)),proc,SLOT(deleteLater()));
	}

	return ret;
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
