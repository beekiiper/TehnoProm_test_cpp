#include "widget.h"
#include "ui_widget.h"

#include <QDesktopServices>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    model = new QFileSystemModel(this);
    model->setFilter(QDir::QDir::AllEntries);
    model->setRootPath("");
    ui->listView->setModel(model);
    connect(ui->listView, SIGNAL(dobleClicked(QModelIndex)),this,SLOT(on_listView_doubleClicked(QModelIndex)));

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_listView_doubleClicked(const QModelIndex &index)
{
    QListView* listView = (QListView*)sender();
    QFileInfo fileInfo = model->fileInfo(index);
    QDir dir = fileInfo.dir();
    if (fileInfo.fileName() == "..") {
        dir.cdUp();
        listView->setRootIndex(model->index(dir.absolutePath()));
        ui->lineEdit->setText(dir.path());

    }
    else if (fileInfo.fileName()=="."){
        listView->setRootIndex(model->index(""));
        ui->lineEdit->setText("");
    }
    else if (fileInfo.isDir()) {
        listView->setRootIndex(index);
        ui->lineEdit->setText(model->filePath(listView->currentIndex()));
    }
    else {
        QDesktopServices::openUrl(QUrl::fromLocalFile(model->filePath(index)));
    }
}


void Widget::on_pushButton_clicked()
{
    QFileDialog path;
    auto p =path.getExistingDirectory();
    ui->listView->setRootIndex(model->index(p));
    ui->lineEdit->setText(p);
}


void Widget::on_lineEdit_editingFinished()
{
    ui->listView->setRootIndex(model->index(ui->lineEdit->text()));

}

