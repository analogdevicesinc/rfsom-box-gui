#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <QWidget>

namespace Ui {
class MainWindow;
}

class MenuPage : public QWidget
{
	Q_OBJECT
public:
	explicit MenuPage(QWidget *parent = 0);
	virtual ~MenuPage();
	void init(Ui::MainWindow *_ui);
	virtual void load();
	virtual void unload();

signals:

public slots:
	virtual void updateUi();

protected:
	virtual void initialize();
	Ui::MainWindow *ui;
};

#endif // MENUPAGE_H
