#include "treeviewwindow.h"
#include "ui_treeviewwindow.h"

TreeViewWindow::TreeViewWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TreeViewWindow)
{
    ui->setupUi(this);
    //tree = ui->treeWidget->tree;
}



void TreeViewWindow::closeEvent(QCloseEvent *event)
{
    emit CloseWindow();
}

void TreeViewWindow::InsertMainTree(QTreeWidget *tw)
{
   // ui->centralwidget->layout()->removeWidget(ui->treeWidget);
    ui->centralwidget->layout()->addWidget(tw);
}

TreeViewWindow::~TreeViewWindow()
{
    delete ui;
}

void TreeViewWindow::on_liveEditCheckBox_toggled(bool checked)
{
    emit toggle_liveEditCheckBox(checked);
}

void TreeViewWindow::toggleliveEditCheckBox()
{
   // qDebug() << "HERE";
 //    ui->treeWidget->liveEditCheckBox->setChecked(!ui->treeWidget->liveEditCheckBox->isChecked());
}

