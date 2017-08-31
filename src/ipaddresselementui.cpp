#include "ipaddresselementui.h"
#include "ui_editboxElement.h"

IPAddressElement::IPAddressElement(QWidget *parent) : NumericElementUi(parent)
{
	separator = '.';
	nrOfDigits=4;
	cursorPosition = 0;
}

IPAddressElement::~IPAddressElement()
{
}

void IPAddressElement::increment()
{
	ip[cursorPosition]++;
	formatForEditing();
}

void IPAddressElement::decrement()
{
	ip[cursorPosition]--;
	formatForEditing();
}


bool IPAddressElement::cursorRight()
{
	if (cursorPosition>0) {
		cursorPosition--;
	}

	updateCursorPosition();
	return true;
}

bool IPAddressElement::cursorLeft()
{
	if (cursorPosition<nrOfDigits) {
		cursorPosition++;
	}

	updateCursorPosition();
	return true;
}

void IPAddressElement::updateCursorPosition()
{
	auto cursorPos = ui->editbox->text().length() - ((cursorPosition+1)*3 +
	                 cursorPosition);
	ui->editbox->setCursorPosition(cursorPos);
	ui->editbox->setSelection(cursorPos,3);
	setFocus();
}

void IPAddressElement::formatForEditing()
{
	ipStr.sprintf("%03d.%03d.%03d.%03d",ip[3],ip[2],ip[1],ip[0]);
	ui->editbox->setText(ipStr);
	updateCursorPosition();
}

void IPAddressElement::formatForReading()
{
	ipStr.sprintf("%d.%d.%d.%d",ip[3],ip[2],ip[1],ip[0]);
	ui->editbox->setText(ipStr);
}
void IPAddressElement::strToIp()
{
	QStringList ipList = ipStr.split(".");

	if (ipList.count()==4) {
		ip[3] = ipList.at(0).toInt();
		ip[2] = ipList.at(1).toInt();
		ip[1] = ipList.at(2).toInt();;
		ip[0] = ipList.at(3).toInt();;
	} else {
		ip[3] = 0;
		ip[2] = 0;
		ip[1] = 0;
		ip[0] = 0;
	}
}

void IPAddressElement::update()
{
	if (!selected) {
		ipStr = read->run();
		strToIp();
		formatForReading();
	}

	ui->topRight->setText(description);
	ui->topLeft->setText(iconString());
}


void IPAddressElement::click()
{
	if (selected) {
		formatForReading();
		write->vars["EDIT_VAL"]=ipStr;
		write->run();
		setSelected(false);
		update();

	} else {
		cursorPosition = 0;
		updateCursorPosition();
		setSelected(true);
		formatForEditing();
		//setFocus();
	}
}
