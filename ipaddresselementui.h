#ifndef IPADDRESSELEMENTUI_H
#define IPADDRESSELEMENTUI_H

#include "numericelementui.h"

class IPAddressElement : public NumericElementUi
{
	quint8 ip[4];
	QString ipStr;
public:

	IPAddressElement(QWidget *parent=0);
	virtual ~IPAddressElement();

public slots:
	virtual void increment();
	virtual void decrement();
	virtual bool cursorRight();
	virtual bool cursorLeft();
	virtual void update();
	virtual void click();
	virtual void formatForEditing();
	virtual void formatForReading();

protected:
	virtual void updateCursorPosition();
	virtual void strToIp();
};

#endif // IPADDRESSELEMENTUI_H
