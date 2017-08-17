#include "readonlyelementui.h"
#include "ui_readonlyvalue.h"
#include <QDebug>
#include "common.h"

bool ReadOnlyElementUi::getIconRight() const
{
	return iconRight;
}

void ReadOnlyElementUi::setIconRight(bool value)
{
	iconRight = value;
}

quint16 ReadOnlyElementUi::getIconSize() const
{
	return iconSize;
}

void ReadOnlyElementUi::setIconSize(const quint16& value)
{
	iconSize = value;
}

StringValue *ReadOnlyElementUi::getStrVal() const
{
	return strVal;
}

void ReadOnlyElementUi::setStrVal(StringValue *value)
{
	strVal = value;
}

QString ReadOnlyElementUi::iconString()
{
	if (icon) {
		return "<img src=\"" + resolveFileLocation(iconPath) +"\" height=\"" + QString::number(
		               iconSize) +"\" width=\"" + QString::number(iconSize) +"\">";
	} else {
		return "";
	}
}

ReadOnlyElementUi::ReadOnlyElementUi(QWidget *parent) : UiElement(parent)
{
	icon = false;
	iconRight = false;
	oneRow = false;
	iconSize = 16;
	strVal=nullptr;
	ui = new Ui::ReadOnlyValueUi;
	ui->setupUi(this);
}

void ReadOnlyElementUi::update()
{
	if (strVal==nullptr) {
		qDebug()<<"No strVal specified";
		return;
	}

	ui->bot->setVisible(!oneRow);

	//qDebug()<<iconString();
	if (oneRow) {
		if (iconRight) {
			ui->topLeft->setText(strVal->get());
			ui->topRight->setText(description+iconString());
		} else {
			ui->topRight->setText(strVal->get());
			ui->topLeft->setText(iconString()+description);
		}
	} else {
		ui->bot->setText(strVal->get());

		if (iconRight) {
			ui->topLeft->setText(description);
			ui->topRight->setText(iconString());
		} else {
			ui->topRight->setText(description);
			ui->topLeft->setText(iconString());
		}
	}
}

void ReadOnlyElementUi::click()
{
	update();
}

bool ReadOnlyElementUi::isOneRow() const
{
	return oneRow;
}

void ReadOnlyElementUi::setOneRow(bool value)
{
	oneRow = value;
}

QString ReadOnlyElementUi::getDescription() const
{
	return description;
}

void ReadOnlyElementUi::setDescription(const QString& value)
{
	description = value;
}

QString ReadOnlyElementUi::getIconPath() const
{
	return iconPath;
}

void ReadOnlyElementUi::setIconPath(const QString& value)
{
	if (value == 0) {
		icon=false;
	} else {
		icon=true;
		iconPath = value;
	}
}

ReadOnlyElementUi::~ReadOnlyElementUi()
{
	delete ui;
}
