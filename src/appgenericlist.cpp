#include "appgenericlist.h"
#include "common.h"
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include "scriptresult.h"
#include "readonlyelementui.h"
#include "buttonelementui.h"
#include "checkboxelementui.h"
#include "editboxelementui.h"
#include "numericelementui.h"
#include "ipaddresselementui.h"
#include "enumelementui.h"
#include <QDirIterator>
#include <QKeyEvent>
#include <QApplication>

AppGenericList::AppGenericList(QJsonValue params, QLayout *lay,
                               QWidget *parent) : App(parent) , params(params)
{
	list = new QListWidget(this);
	list->setMaximumWidth(maxWidth);
	list->setMaximumHeight(maxHeight);
	list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	// add stylesheet for item spacing

	auto filePath = resolveFileLocation("stylesheets/appGenericList.qss");
	QFile f(filePath);

	if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream s(&f);
		QString str = s.readAll();
		list->setStyleSheet(str);
	} else {
		qDebug()<<"Stylesheet not found @ " << filePath;
	}

	lay->addWidget(list);
	qDebug()<<"AppGenericListConstructor";
}

QWidget *AppGenericList::setupReadOnlyElementUi(QJsonObject obj)
{
	auto ui_element = new ReadOnlyElementUi(list);
	ui_element->setStrVal(new ScriptResult(obj["cmd"].toString(),ui_element));

	if (obj.contains("one_row")) {
		ui_element->setOneRow(obj["one_row"].toBool());
	}

	if (obj.contains("description")) {
		ui_element->setDescription(obj["description"].toString());
	}

	if (obj.contains("stylesheet")) {
		ui_element->setStyleSheet(obj["stylesheet"].toString());
	}

	/*if(obj.contains(""))
		ui_element->setIconRight(bool value);
	if(obj.contains("")
		ui_element->setIconPath(const QString &value);
	if(obj.contains("icon_size")
		ui_element->setIconSize(obj["icon_size"].toInt());*/

	ui_element->update();

	if (obj.contains("timer")) {
		bool readOnce=false;
		if(obj["timer"].isString())
		{
			if(obj["timer"].toString()=="read_once")
				readOnce=true;
		}

		if(!readOnce)
		{
			int timer_id = obj["timer"].toInt();
			if (!(timer_id >= 0 && timer_id < timers.count())) {
				timer_id = 0; // connects to the default timer 0
			}
			connect(timers[timer_id],SIGNAL(timeout()),ui_element,SLOT(update()));
		}

	}
	return ui_element;
}


QWidget *AppGenericList::setupButtonElementUi(QJsonObject obj)
{
	auto ui_element = new ButtonElementUi(list);

	ui_element->setScript(new ScriptResult(obj["cmd"].toString(),ui_element));

	if (obj.contains("description")) {
		ui_element->setDescription(obj["description"].toString());
	}

	if(obj.contains("background_task"))	{
		ui_element->setRunInBackground(obj["background_task"].toBool());
	}

	if (obj.contains("stylesheet")) {
		ui_element->setStyleSheet(obj["stylesheet"].toString());
	}


	return ui_element;
}

QWidget *AppGenericList::setupCheckboxElementUi(QJsonObject obj)
{
	auto ui_element = new CheckboxElementUi(list);

	//ui_element->setScript(new ScriptResult(obj["cmd"].toString(),ui_element));

	if (obj.contains("description")) {
		ui_element->setDescription(obj["description"].toString());
	}

	if (obj.contains("stylesheet")) {
		ui_element->setStyleSheet(obj["stylesheet"].toString());
	}

	if (obj.contains("cmd_read")) {
		ui_element->setRead(new ScriptResult(obj["cmd_read"].toString(),ui_element));
	}

	if (obj.contains("cmd_write")) {
		ui_element->setWrite(new ScriptResult(obj["cmd_write"].toString(),ui_element));
	}


	if (obj.contains("cmd_read_timer")) {
		bool readOnce=false;
		if(obj["cmd_read_timer"].isString())
		{
			if(obj["cmd_read_timer"].toString()=="read_once")
				readOnce=true;
		}

		if(!readOnce)
		{
			int timer_id = obj["cmd_read_timer"].toInt();

			if (!(timer_id >= 0 && timer_id < timers.count())) {
				timer_id = 0; // connects to the default timer 0
			}
			connect(timers[timer_id],SIGNAL(timeout()),ui_element,SLOT(update()));
		}

	}


	ui_element->update();
	return ui_element;
}

QWidget *AppGenericList::setupEditElementUi(QJsonObject obj)
{
	EditboxElementUi *ui_element;
	NumericElementUi *num_element = nullptr;
	IPAddressElement *ip_element = nullptr;
	EnumElementUi *enum_element=nullptr;

	if (obj.contains("subtype")) {
		if (obj["subtype"].toString()=="numeric") {
			ui_element = new NumericElementUi(list);
			num_element = dynamic_cast<NumericElementUi *>(ui_element);
		} else if (obj["subtype"].toString()=="ip") {
			ui_element = new IPAddressElement(list);
			ip_element = dynamic_cast<IPAddressElement *>(ui_element);
		} else if (obj["subtype"].toString()=="enum") {
			ui_element = new EnumElementUi(list);
			enum_element = dynamic_cast<EnumElementUi *>(ui_element);
		} else {
			ui_element = new EditboxElementUi(list);
		}
	} else {
		ui_element = new EditboxElementUi(list);
	}


	if (obj.contains("description")) {
		ui_element->setDescription(obj["description"].toString());
	}

	if (obj.contains("stylesheet")) {
		ui_element->setStyleSheet(obj["stylesheet"].toString());
	}

	if (obj.contains("cmd_read")) {
		ui_element->setRead(new ScriptResult(obj["cmd_read"].toString(),ui_element));
	}

	if (obj.contains("cmd_write")) {
		ui_element->setWrite(new ScriptResult(obj["cmd_write"].toString(),ui_element));
	}

	if (obj.contains("cmd_read_only")) {
		ui_element->setReadOnly(new ScriptResult(obj["cmd_read_only"].toString(),
		                        ui_element));
	}

	if (obj.contains("cmd_read_timer")) {
		bool readOnce=false;
		if(obj["cmd_read_timer"].isString())
		{
			if(obj["cmd_read_timer"].toString()=="read_once")
				readOnce=true;
		}

		if(!readOnce)
		{
			int timer_id = obj["cmd_read_timer"].toInt();

			if (!(timer_id >= 0 && timer_id < timers.count())) {
				timer_id = 0; // connects to the default timer 0
			}
			connect(timers[timer_id],SIGNAL(timeout()),ui_element,SLOT(update()));
		}

	}

	if (num_element!=nullptr) {

		if (obj.contains("nr_of_digits")) {
			auto nr_of_digits = obj["nr_of_digits"].toInt();
			num_element->setNrOfDigits(nr_of_digits);

			auto max_value_s = QString(nr_of_digits,'9');
			auto max_value = max_value_s.toLongLong();
			num_element->setMaxValue(max_value);

		}

		if (obj.contains("max_value")) {
			auto max_value_d = obj["max_value"].toDouble();
			auto max_value = (qint64) max_value_d;

			num_element->setMaxValue(max_value);
			auto nr_of_digits = QString::number(max_value).length();
			num_element->setNrOfDigits(nr_of_digits);
		}

		if (obj.contains("min_value")) {
			auto min_value_d = obj["min_value"].toDouble();
			auto min_value = (qint64) min_value_d;
			num_element->setMinValue(min_value);
		}
	}

	if (enum_element!=nullptr) {
		if (obj.contains("enum")) {
			QStringList strlist;

			if (obj["enum"].isArray()) {
				for (QJsonValue val : obj["enum"].toArray()) {
					if (val.isString()) {
						strlist.append(val.toString());
					}
				}
			}

			enum_element->setElements(strlist);
		}

	}

	ui_element->update();
	return ui_element;

}

void AppGenericList::setupTasks()
{
	if (params.toObject().contains("tasks")) {
		for (QJsonValue val : params.toObject()["tasks"].toArray()) {
			auto obj = val.toObject();

			if (obj.contains("cmd")) {
				ScriptResult *res = new ScriptResult(obj["cmd"].toString(),this);
				//res->setProc(proc);
				res->setRunInBackground(true);
				auto timerid = obj["timer"].toInt();

				if (timerid == -1) {
					res->run();
				} else {
					connect(timers[timerid],SIGNAL(timeout()),res,SLOT(run()));
				}

			}
		}
	}
}

void AppGenericList::setupTimers()
{
	// search for timers
	if (params.toObject().contains("timers")) {
		for (QJsonValue val : params.toObject()["timers"].toArray()) {
			auto period = val.toInt(cMinimumTimerPeriod);

			if (period < cMinimumTimerPeriod) {
				period = cMinimumTimerPeriod;
			}

			QTimer *tim = new QTimer(this);
			timers.append(tim);
			tim->start(period);
		}
	} else {
		qDebug()<<"No timers defined in the configuration json. Creating default 1 second timer";
		QTimer *tim  = new QTimer(this);
		timers.append(tim);
		tim->start(1000); // default 1 second timer
	}
}

void AppGenericList::load()
{
	for(auto timer:timers)
	{
		timer->start();
	}
}

void AppGenericList::unload()
{
	for(auto timer:timers)
	{
		timer->stop();
	}
}

void AppGenericList::buildUi()
{

	setupTimers();
	setupTasks();

	// search for Ui elements
	if (!params.toObject().contains("elements")) {
		qDebug()<<"No elements defined in the configuration json.";
		return;
	}

	for (auto val : params.toObject()["elements"].toArray()) {
		auto obj = val.toObject();
		QWidget *element = nullptr;

		if (obj["type"].toString()=="cmd_read") {
			element = setupReadOnlyElementUi(obj);
		} else if (obj["type"].toString()=="btn") {
			element = setupButtonElementUi(obj);
		} else if (obj["type"].toString()=="checkbox") {
			element = setupCheckboxElementUi(obj);
		} else if (obj["type"].toString()=="edit")	{
			element = setupEditElementUi(obj);
		}


		if (element!=nullptr) {
			int cnt = list->count();//get number of items before adding
			list->addItem("");

			if (cnt % 2 == 1) {
				element->setStyleSheet("background-color:black");
			}

			list->setItemWidget(list->item(cnt),element);
		}

	}

	list->setFocus();
	list->installEventFilter(this);
}

bool AppGenericList::eventFilter(QObject *watched, QEvent *event)
{
	if (event->type()==QEvent::Wheel) {
		watched=list->itemWidget(list->currentItem());
	}

	if (watched == list) {

		if (event->type()==QEvent::KeyPress) {
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

			switch (keyEvent->key()) {
			case Qt::Key_Up:
			case Qt::Key_Down:
				return false;

			case Qt::Key_Return:
				confirmListSelection();
				return true;

			case Qt::Key_Left:
			case Qt::Key_Right:
				return false;

			default:
				return true;
			}
		}
	}

	auto editElement = dynamic_cast<EditboxElementUi *>(watched);

	if (editElement!=nullptr&&editElement->getSelected()) {


		if (event->type()==QEvent::KeyPress) {
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			qDebug()<<keyEvent;

			switch (keyEvent->key()) {
			case Qt::Key_Left:
				return editElement->cursorLeft();

			case Qt::Key_Right:
				return editElement->cursorRight();

			case Qt::Key_Up:
			case Qt::Key_Down:
			case Qt::Key_Return:
				editElement->click();
				editElement->removeEventFilter(this);
				list->setFocus();
				return true;

			case Qt::Key_PageUp:
				editElement->increment();
				return true;

			case Qt::Key_PageDown:
				editElement->decrement();
				return true;

			}
		}

		if (event->type()==QEvent::Wheel) {
			QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);

			if (wheelEvent->delta()>0) {
				editElement->increment();
			} else {
				editElement->decrement();
			}
		}
	}

	return false;
}

void AppGenericList::destroyUi()
{

}

AppGenericList::~AppGenericList()
{
	delete list;
}

void AppGenericList::confirmListSelection()
{
	auto current = list->currentItem();
	auto wid = list->itemWidget(current);
	ButtonElementUi *btnElement = dynamic_cast<ButtonElementUi *>(wid);
	CheckboxElementUi *chkElement = dynamic_cast<CheckboxElementUi *>(wid);
	EditboxElementUi *editElement = dynamic_cast<EditboxElementUi *>(wid);


	// create listelement class
	if (btnElement!=nullptr) {
		btnElement->click();
	}

	if (chkElement!=nullptr) {
		chkElement->click();
	}

	if (editElement!=nullptr) {
		if (!editElement->isReadOnly()) {
			editElement->installEventFilter(this);
			editElement->click();
		}
		else
		{
			editElement->update();
		}
	}

}

