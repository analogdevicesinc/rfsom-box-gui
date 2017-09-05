#include "buttonelementui.h"
#include <ui_buttonElement.h>

bool ButtonElementUi::getIconRight() const
{
	return iconRight;
}

void ButtonElementUi::setIconRight(bool value)
{
	iconRight = value;
}

quint16 ButtonElementUi::getIconSize() const
{
	return iconSize;
}

void ButtonElementUi::setIconSize(const quint16& value)
{
	iconSize = value;
}

ScriptResult *ButtonElementUi::getScript() const
{
	return script;
}

void ButtonElementUi::setScript(ScriptResult *value)
{
	script = value;
	script->setRunInBackground(runInBackground);
}

bool ButtonElementUi::getRunInBackground() const
{
	return runInBackground;
}

void ButtonElementUi::setRunInBackground(bool value)
{
	runInBackground = value;
	if(script)
		script->setRunInBackground(value);
}

QString ButtonElementUi::iconString()
{
	if (icon) {
		return "<img src=\"" + iconPath +"\" height=\"" + QString::number(
		               iconSize) +"\" width=\"" + QString::number(iconSize) +"\">";
	} else {
		return "";
	}
}

ButtonElementUi::ButtonElementUi(QWidget *parent) : UiElement(parent)
{
	icon = false;
	iconRight = false;
	iconSize = 16;
	script=nullptr;
	ui = new Ui::buttonElement;
	ui->setupUi(this);
}

void ButtonElementUi::click()
{
	ui->btn->animateClick(100);
	script->run();
}

QString ButtonElementUi::getDescription() const
{
	return description;
}

void ButtonElementUi::setDescription(const QString& value)
{
	description = value;
	ui->btn->setText(value);
}

QString ButtonElementUi::getIconPath() const
{
	return iconPath;
}

void ButtonElementUi::setIconPath(const QString& value)
{
	if (value == 0) {
		icon=false;
	} else {
		icon=true;
		iconPath = value;
//		ui->btn->setIcon(new QIcon(value));
	}
}

ButtonElementUi::~ButtonElementUi()
{
	delete ui;
}
