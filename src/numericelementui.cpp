
#include "numericelementui.h"
#include "ui_editboxElement.h"
#include <QDebug>
#include <QLocale>

NumericElementUi::NumericElementUi(QWidget *parent) : EditboxElementUi(parent)
{
	separator = ' ';
}

qint64 NumericElementUi::getValue() const
{
	return value;
}

void NumericElementUi::setValue(const qint64& val)
{
	value = val;
}

qint8 NumericElementUi::getNrOfDigits() const
{
	return nrOfDigits;
}

void NumericElementUi::setNrOfDigits(const qint8& value)
{
	nrOfDigits = value;
}

QString NumericElementUi::getInputMask() const
{
	return inputMask;
}

void NumericElementUi::setInputMask(const QString& value)
{
	inputMask = value;
	ui->editbox->setInputMask(inputMask);
}

void NumericElementUi::increment()
{
	auto exp = (pow(10,cursorPosition));
	auto oldvalue = value;
	value+=exp;

	if (QString::number(value).length()>nrOfDigits) {
		value = oldvalue;
	}

	formatForEditing();
}

void NumericElementUi::decrement()
{
	auto exp = (pow(10,cursorPosition));

	if (exp > value) {
		return;
	}

	value-=exp;

	if (value < 0) {
		value = 0;
	}

	formatForEditing();

}

int NumericElementUi::getCursorPosition()
{
	return cursorPosition;
}

bool NumericElementUi::cursorLeft()
{
	if (cursorPosition<nrOfDigits) {
		cursorPosition++;
	}

	//qDebug()<<"number of spaces in between " << cursorPosition/3;
	updateCursorPosition();
	return true;
}

bool NumericElementUi::cursorRight()
{
	if (cursorPosition>0) {
		cursorPosition--;
	}

	updateCursorPosition();
	return true;
}

void NumericElementUi::formatForReading()
{

	auto text = QString::number(value);

	for (auto i=text.length()-3; i>0; i-=3) {
		text.insert(i,separator);
	}

	ui->editbox->setText(text);

}

void NumericElementUi::updateCursorPosition()
{
	auto cursorPos = ui->editbox->text().length() - (cursorPosition +
	                 (cursorPosition/3))-1;
	ui->editbox->setCursorPosition(cursorPos);
	ui->editbox->setSelection(cursorPos,1);

}

void NumericElementUi::formatForEditing()
{

	auto text = QString::number(value);
	auto textlen = text.length();
	text.prepend(QString(nrOfDigits - textlen,'0'));

	for (auto i=nrOfDigits-3; i>0; i-=3) {
		text.insert(i,separator);
	}

	ui->editbox->setText(text);
	updateCursorPosition();
}

void NumericElementUi::update()
{
	if (!selected) {
		value = read->get().toLongLong();
		formatForReading();
	} else {

	}

	ui->topRight->setText(description);
	ui->topLeft->setText(iconString());
}

void NumericElementUi::click()
{
	if (selected) {

		write->vars["EDIT_VAL"]=QString::number(value);
		write->run();
		setSelected(false);
		update();
	} else {
		cursorPosition = 0;
		updateCursorPosition();
		setSelected(true);
		formatForEditing();
		setFocus();
	}
}
