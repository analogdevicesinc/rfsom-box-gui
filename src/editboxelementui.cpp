#include "editboxelementui.h"
#include <ui_editboxElement.h>


bool EditboxElementUi::getSelected() const
{
	return selected;
}

void EditboxElementUi::setSelected(bool value)
{
	selected = value;
}

QString EditboxElementUi::getIconPath() const
{
	return iconPath;
}

void EditboxElementUi::setIconPath(const QString& value)
{
	if (value == 0) {
		icon=false;
	} else {
		icon=true;
		iconPath = value;
	}
}

void EditboxElementUi::increment()
{
	auto text = ui->editbox->text();
	auto position = cursorPosition;

	if (position<text.length()) {
		auto charAt = ui->editbox->text().at(position);
		auto ascii = charAt.toLatin1();

		if (ascii<0x7E) {
			ascii++;
		}

		QChar newchar = QChar(ascii);
		text.replace(position,1,newchar);
		ui->editbox->setText(text);
	}

	updateCursorPosition();
}

void EditboxElementUi::decrement()
{
	auto text = ui->editbox->text();
	auto position = cursorPosition;

	if (position<text.length()) {
		auto charAt = ui->editbox->text().at(position);
		auto ascii = charAt.toLatin1();

		if (ascii>0x20) {
			ascii--;
		}

		QChar newchar = QChar(ascii);
		text.replace(position,1,newchar);
		ui->editbox->setText(text);
	}

	updateCursorPosition();

}

quint16 EditboxElementUi::getIconSize() const
{
	return iconSize;
}

void EditboxElementUi::setIconSize(const quint16& value)
{
	iconSize = value;
}

ScriptResult *EditboxElementUi::getReadOnly() const
{
	return readOnly;
}

void EditboxElementUi::setReadOnly(ScriptResult *value)
{
	readOnly = value;
}

bool EditboxElementUi::isReadOnly()
{
	if (readOnly==nullptr) {
		return (write==nullptr);
	}

	auto result = readOnly->get();

	if (result=="" || result == "0") {
		return false;
	}

	return true;
}

quint8 EditboxElementUi::getTextLength() const
{
	return textLength;
}

void EditboxElementUi::setTextLength(const quint8& value)
{
	textLength = value;
}

EditboxElementUi::EditboxElementUi(QWidget *parent) : UiElement(parent)
{
	read=nullptr;
	write=nullptr;
	readOnly=nullptr;
	ui = new Ui::editboxElement;
	ui->setupUi(this);
	icon=false;
	iconSize=12;
	textLength = 12;
	selected = false;
}
EditboxElementUi::~EditboxElementUi()
{
	delete ui;
}

ScriptResult *EditboxElementUi::getWrite() const
{
	return write;
}

void EditboxElementUi::setWrite(ScriptResult *value)
{
	write = value;
}

ScriptResult *EditboxElementUi::getRead() const
{
	return read;
}

void EditboxElementUi::setRead(ScriptResult *value)
{
	read = value;
}

QString EditboxElementUi::getDescription() const
{
	return description;
}

void EditboxElementUi::setDescription(const QString& value)
{
	description = value;
}

QString EditboxElementUi::iconString()
{
	if (icon) {
		return "<img src=\"" + iconPath +"\" height=\"" + QString::number(
		               iconSize) +"\" width=\"" + QString::number(iconSize) +"\">";
	} else {
		return "";
	}
}

void EditboxElementUi::updateCursorPosition()
{
	ui->editbox->setCursorPosition(cursorPosition-1);
	ui->editbox->setSelection(cursorPosition,1);
	setFocus();
}

void EditboxElementUi::update()
{
	if (!selected) {
		ui->editbox->setText(read->get());
		ui->topRight->setText(description);
		ui->topLeft->setText(iconString());
	}
}

void EditboxElementUi::click()
{
	if (selected) {

		write->vars["EDIT_VAL"]=ui->editbox->text();
		write->run();
		setSelected(false);
		update();

	} else {
		//setFocus();
		cursorPosition=0;
		updateCursorPosition();
		setSelected(true);
	}
}

