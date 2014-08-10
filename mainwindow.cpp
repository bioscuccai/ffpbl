#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QSqlRecord>
#include <QClipboard>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow){
	ui->setupUi(this);

	db=QSqlDatabase::addDatabase("QSQLITE");

	connect(ui->actionCopyLink, SIGNAL(triggered()), this, SLOT(on_actionCopyLink_triggered()));
	connect(ui->actionOpenUrl, SIGNAL(triggered()), this, SLOT(on_actionOpenUrl_triggered()));

	ui->placeTable->addAction(ui->actionCopyLink);
	//ui->placeTable->addAction(ui->actionOpenUrl);

}

MainWindow::~MainWindow(){
	delete ui;
}

void MainWindow::on_actionOpen_triggered(){
	QString fn=QFileDialog::getOpenFileName();
	if(fn=="") return;
	db.setDatabaseName(fn);
	db.open();

	if(!db.isOpen()){
		QMessageBox::warning(this,"Invalid database", "The database file is invalid.");
		return;
	}

	//QSqlQueryModel *model=new QSqlQueryModel();
	//model->setQuery("select id, url, title, datetime(substr(last_visit_date,0,11),'unixepoch') from moz_places limit 200");

	bookmarkModel=new QSqlQueryModel();
	bookmarkModel->setQuery("select title, (select url from moz_places where id=fk) as url, (select title from moz_bookmarks as cat where cat.id=bmark.parent) as folder, datetime(substr(dateAdded,0,11),'unixepoch') as added from moz_bookmarks as bmark where type=1 order by folder, title");
	ui->placeTable->setModel(bookmarkModel);

	//ui->placeTable->setModel(model);
	ui->placeTable->setSelectionBehavior(QAbstractItemView::SelectRows);


	ui->placeTable->setColumnWidth(0, 350);
	ui->placeTable->setColumnWidth(1, 300);
	ui->placeTable->setColumnWidth(2, 200);
	ui->placeTable->setColumnWidth(3, 180);
}

void MainWindow::on_actionOpenUrl_triggered(){

}

void MainWindow::on_actionCopyLink_triggered(){
	QModelIndexList ilist=ui->placeTable->selectionModel()->selectedRows();
	for(int i=0;i<ilist.length();i++){
		QModelIndex ind=ilist.at(i);
		QApplication::clipboard()->setText(bookmarkModel->record(ind.row()).value(1).toString());
	}
}
