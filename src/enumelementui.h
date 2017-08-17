#ifndef ENUMELEMENTUI_H
#define ENUMELEMENTUI_H

#include "editboxelementui.h"


class EnumElementUi : public EditboxElementUi
{
	QStringList elements;
	int current;
public:
	EnumElementUi(QWidget *parent=0);
	virtual ~EnumElementUi();

	QStringList getElements() const;
	void setElements(const QStringList& value);

public slots:
	virtual void increment();
	virtual void decrement();
	virtual bool cursorRight()
	{
		return true;
	}
	virtual bool cursorLeft()
	{
		return true;
	}
	void updateCursorPosition();

};

#endif // ENUMELEMENTUI_H
