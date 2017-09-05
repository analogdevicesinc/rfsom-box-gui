#ifndef SCRIPTRESULT_H
#define SCRIPTRESULT_H

#include <QString>
#include "stringvalue.h"
#include <QProcess>
#include <QMap>

class ScriptResult : public StringValue
{
	Q_OBJECT
	QString cmd;
	QProcess *proc;
	QString setVars();
	bool runInBackground;
signals:
	//void getScriptResult(QString);
public:
	QMap<QString, QString> vars;

	ScriptResult(QString cmd, QObject *parent=0);
	virtual ~ScriptResult();
	QString get();
	QString getCmd() const;
	void setCmd(const QString& value);

	bool getRunInBackground() const;
	void setRunInBackground(bool value);

public slots:
	virtual QString run();
	virtual void scriptFinished(int);

protected:
	virtual QString run_script();
	virtual QString convert(QString);
};


#endif // SCRIPTRESULT_H
