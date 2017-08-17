#ifndef CHECKBOXELEMENTUI_H
#define CHECKBOXELEMENTUI_H

#include <QWidget>
#include "scriptresult.h"
#include "uielement.h"

namespace Ui {
class checkboxElement;
}

class CheckboxElementUi : public UiElement
{
	Q_OBJECT
	QString description;
	ScriptResult *write;
	ScriptResult *read;
	Ui::checkboxElement *ui;

public:
	explicit CheckboxElementUi(QWidget *parent=0);
	~CheckboxElementUi();

	QString getDescription() const;
	void setDescription(const QString& value);

	ScriptResult *getWrite() const;
	void setWrite(ScriptResult *value);

	ScriptResult *getRead() const;
	void setRead(ScriptResult *value);

public slots:
	virtual void update();
	virtual void click();
};

#endif // CHECKBOXELEMENTUI_H
