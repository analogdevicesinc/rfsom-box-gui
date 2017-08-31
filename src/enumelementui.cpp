#include "editboxelementui.h"
#include "ui_editboxElement.h"
#include "enumelementui.h"


QStringList EnumElementUi::getElements() const
{
	return elements;
}

void EnumElementUi::setElements(const QStringList& value)
{
	elements = value;
}

EnumElementUi::EnumElementUi(QWidget *parent) : EditboxElementUi(parent)
{
	current=0;
}

EnumElementUi::~EnumElementUi()
{

}

void EnumElementUi::increment()
{
	if (current < elements.count()-1) {
		current++;
	} else {
		current = elements.count()-1;
	}

	ui->editbox->setText(elements[current]);
	updateCursorPosition();
}

void EnumElementUi::decrement()
{
	if (current > 0) {
		current--;
	} else {
		current=0;
	}

	ui->editbox->setText(elements[current]);
	updateCursorPosition();
}

void EnumElementUi::updateCursorPosition()
{
	ui->editbox->selectAll();
	setFocus();
}
