#include "sysfsvalue.h"
#include <QFile>
#include <QTextStream>


QString SysFSValue::getPath() const
{
	return path;
}

void SysFSValue::setPath(const QString& value)
{
	path = value;
}

SysFSValue::SysFSValue(QString path, QObject *parent) : StringValue(parent),
	path(path)
{

}

SysFSValue::~SysFSValue() {}

QString SysFSValue::read()
{
	QFile file(path);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return "Cannot open file";
	}

	QTextStream in(&file);
	return in.readAll();
}

QString SysFSValue::get()
{
	raw_val = read();
	val = convert(raw_val);
	return val;
}

QString SysFSValue::convert(QString v)
{
	return v.simplified();
}

