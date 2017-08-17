#ifndef READONLYELEMENTUI_h
#define READONLYELEMENTUI_h

#include <QWidget>
#include "stringvalue.h"
#include "uielement.h"

namespace Ui {
class ReadOnlyValueUi;
}

class ReadOnlyElementUi : public UiElement
{
	Q_OBJECT
	QString description;
	QString iconPath;
	StringValue *strVal;

	quint16 iconSize;
	bool icon;
	bool oneRow;
	bool iconRight;
	Ui::ReadOnlyValueUi *ui;

private:
	QString iconString();

public:
	explicit ReadOnlyElementUi(QWidget *parent=0);
	virtual ~ReadOnlyElementUi();

	QString getIconPath() const;
	void setIconPath(const QString& value);
	bool isOneRow() const;
	void setOneRow(bool value);
	QString getDescription() const;
	void setDescription(const QString& value);
	bool getIconRight() const;
	void setIconRight(bool value);
	quint16 getIconSize() const;
	void setIconSize(const quint16& value);


	StringValue *getStrVal() const;
	void setStrVal(StringValue *value);

public slots:
	virtual void update();
	virtual void click();
};

#endif // READONLYELEMENTUI_h
