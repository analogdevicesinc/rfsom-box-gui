#ifndef STRINGVALUE_h
#define STRINGVALUE_h


#include <QObject>
#include <QString>
class StringValue : public QObject
{
	Q_OBJECT

public:
	StringValue(QObject *parent = 0,QString val="") : QObject(parent), val(val) {}
	virtual ~StringValue() {}
	virtual QString get()
	{
		return val;
	}
protected:
	QString val;
};


#endif // STRINGVALUE_h
