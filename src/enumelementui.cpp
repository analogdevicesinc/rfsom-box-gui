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

ScriptResult *EnumElementUi::getElementsCmd() const
{
	return elementsCmd;
}

void EnumElementUi::setElementsCmd(ScriptResult *value)
{
	elementsCmd = value;
}

QString EnumElementUi::getElementsSep() const
{
	return elementsSep;
}

void EnumElementUi::setElementsSep(const QString &value)
{
	elementsSep = value;
}

EnumElementUi::EnumElementUi(QWidget *parent) : EditboxElementUi(parent)
{
	current=0;
	elementsCmd = nullptr;
	elementsSep = " ";
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

void EnumElementUi::update()
{
	EditboxElementUi::update();
	if(!selected)
	{
		updateElements();
		if(elements.contains(ui->editbox->text()))
			current = elements.indexOf(ui->editbox->text());
	}
}
void EnumElementUi::updateElements()
{
	QStringList val;
	if(elementsCmd)
	{
		val = elementsCmd->run().split(' ',QString::SkipEmptyParts);
		setElements(val);
	}
}
