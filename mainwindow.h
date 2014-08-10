#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QSqlDatabase>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_actionOpen_triggered();

	void on_actionOpenUrl_triggered();

	void on_actionCopyLink_triggered();

private:
	Ui::MainWindow *ui;
	QSqlQueryModel *bookmarkModel;
	QSqlDatabase db;
};

#endif // MAINWINDOW_H
