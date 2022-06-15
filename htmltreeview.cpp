#include "htmltreeview.h"
#include "ui_htmltreeview.h"

HTMLTreeView::HTMLTreeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HTMLTreeView)
{
    ui->setupUi(this);

    liveEditCheckBox = ui->liveEditCheckBox;
    tree = ui->treeWidget;
    treeColExpr = ui->treeColumnExpr;

}

HTMLTreeView::~HTMLTreeView()
{
    delete ui;
}
