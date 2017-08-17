#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	bool shown;
public:
	enum {
		UpKey,
		DownKey,
		LeftKey,
		RightKey,
		ReturnKey
	} RotaryKeys;
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;
protected:
	void keyPressEvent(QKeyEvent *e);

};

#endif // MAINWINDOW_H
