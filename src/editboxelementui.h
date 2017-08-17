#ifndef EDITBOXELEMENTUI_H
#define EDITBOXELEMENTUI_H

#include <QWidget>
#include "scriptresult.h"
#include "uielement.h"

namespace Ui {
class editboxElement;
}

class EditboxElementUi : public UiElement
{
	Q_OBJECT

protected:
	QString description;
	ScriptResult *write;
	ScriptResult *read;
	ScriptResult *readOnly;
	Ui::editboxElement *ui;

	bool icon;
	QString iconPath;
	QString readOnlyResult;
	quint16 iconSize;
	quint8 cursorPosition;
	quint8 textLength;
	bool selected;

public:
	explicit EditboxElementUi(QWidget *parent=0);
	virtual ~EditboxElementUi();

	QString getDescription() const;
	void setDescription(const QString& value);

	ScriptResult *getWrite() const;
	void setWrite(ScriptResult *value);

	ScriptResult *getRead() const;
	void setRead(ScriptResult *value);

	bool getSelected() const;
	void setSelected(bool value);
	QString iconString();

	QString getIconPath() const;
	void setIconPath(const QString& value);

	quint16 getIconSize() const;
	void setIconSize(const quint16& value);

	ScriptResult *getReadOnly() const;
	void setReadOnly(ScriptResult *value);

	virtual bool isReadOnly();
	bool getReadOnlyFlag() const;
	void setReadOnlyFlag(bool value);

	quint8 getTextLength() const;
	void setTextLength(const quint8& value);

protected:
	virtual void updateCursorPosition();
public slots:
	virtual void update();
	virtual void click();
	virtual void increment();
	virtual void decrement();
	virtual bool cursorLeft()
	{
		if (cursorPosition>0) {
			cursorPosition--;
		}

		updateCursorPosition();
		return true;
	}
	virtual bool cursorRight()
	{
		if (cursorPosition<textLength) {
			cursorPosition++;
		}

		updateCursorPosition();
		return true;
	}
};

#endif // EditboxELEMENTUI_H
