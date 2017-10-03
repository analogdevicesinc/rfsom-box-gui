#include "checkboxelementui.h"
#include <ui_checkboxElement.h>

CheckboxElementUi::CheckboxElementUi(QWidget *parent) : UiElement(parent)
{
	read=nullptr;
	write=nullptr;
	ui = new Ui::checkboxElement;
	ui->setupUi(this);
}
CheckboxElementUi::~CheckboxElementUi()
{
	delete ui;
}

ScriptResult *CheckboxElementUi::getWrite() const
{
	return write;
}

void CheckboxElementUi::setWrite(ScriptResult *value)
{
	write = value;
}

ScriptResult *CheckboxElementUi::getRead() const
{
	return read;
}

void CheckboxElementUi::setRead(ScriptResult *value)
{
	read = value;
}

QString CheckboxElementUi::getDescription() const
{
	return description;
}

void CheckboxElementUi::setDescription(const QString& value)
{
	description = value;
}

void CheckboxElementUi::update()
{
	bool ok;
	auto str = read->run();
	auto val = str.toLong(&ok);
	if(!ok)
		val=str.toLong(&ok,16); // try hex
	ui->topLeft->setText(description);
	ui->checkBox->setChecked(ok && val);
}

void CheckboxElementUi::click()
{
	ui->checkBox->toggle();
	write->vars["CHECKBOX_VAL"]=QString::number(ui->checkBox->isChecked());
	write->run();
	update();
}
