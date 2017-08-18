#ifndef NUMERICELEMENTUI_H
#define NUMERICELEMENTUI_H

#include "editboxelementui.h"

class NumericElementUi : public EditboxElementUi
{
public:
	NumericElementUi(QWidget *parent=0);

	qint64 getValue() const;
	void setValue(const qint64& val);

	qint8 getNrOfDigits() const;
	void setNrOfDigits(const qint8& value);

	QString getInputMask() const;
	void setInputMask(const QString& value);

	qint64 getMaxValue() const;
	void setMaxValue(const qint64 &value);

	qint64 getMinValue() const;
	void setMinValue(const qint64 &value);

protected:
	qint64 value;
	qint64 maxValue;
	qint64 minValue;

	qint8 cursorPosition;
	QString inputMask;

	qint8 nrOfDigits;
	QChar separator;
	virtual void updateCursorPosition();

public slots:

	virtual void increment();
	virtual void decrement();
	virtual int getCursorPosition();
	virtual bool cursorLeft();
	virtual bool cursorRight();
	virtual void formatForReading();
	virtual void formatForEditing();
	virtual void update();
	virtual void click();

};

#endif // NUMERICELEMENTUI_H
