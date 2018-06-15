#ifndef ENUMELEMENTUI_H
#define ENUMELEMENTUI_H

#include "editboxelementui.h"


class EnumElementUi : public EditboxElementUi
{
	QStringList elements;
	ScriptResult *elementsCmd;
	QString elementsSep;
	int current;
public:
	EnumElementUi(QWidget *parent=0);
	virtual ~EnumElementUi();

	QStringList getElements() const;
	void setElements(const QStringList& value);

	ScriptResult *getElementsCmd() const;
	void setElementsCmd(ScriptResult *value);

	QString getElementsSep() const;
	void setElementsSep(const QString &value);

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
	void update();
	void updateElements();

};

#endif // ENUMELEMENTUI_H
