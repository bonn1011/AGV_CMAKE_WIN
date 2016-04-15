/********************
*AGV自动导航软件
* Author:Bo.Z
* Date:20160304
*********************/


#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(close()));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
