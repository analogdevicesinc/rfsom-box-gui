#ifndef SYSFSVALUE_H
#define SYSFSVALUE_H

#include <QObject>
#include <QString>
#include "stringvalue.h"


class SysFSValue : public StringValue
{
	Q_OBJECT
	QString raw_val;
	QString path;

public:
	SysFSValue(QString path, QObject *parent=0);
	virtual ~SysFSValue();
	QString get();
	QString getPath() const;
	void setPath(const QString& value);

protected:
	QString read();
	virtual QString convert(QString);

};

#endif // SYSFSVALUE_H
