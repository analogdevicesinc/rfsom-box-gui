#include "scriptresult.h"
#include <QDebug>
#include <QCoreApplication>

ScriptResult::ScriptResult(QString cmd, QObject *parent) : StringValue(parent),
	cmd(cmd)
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
	proc->setWorkingDirectory(QCoreApplication::applicationDirPath()+"/../");

	QProcessEnvironment qpe;

	for (auto key : vars.keys()) {
		qpe.insert(key,vars[key]);
	}

	proc->setProcessEnvironment(qpe);

	proc->start("/bin/sh",QStringList() << "-c" <<  cmd);
	proc->waitForFinished(100);
	QString ret = proc->readAll();
	delete proc;
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
