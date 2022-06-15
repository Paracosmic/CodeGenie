#ifndef HTMLTREEVIEW_H
#define HTMLTREEVIEW_H

#include <QWidget>
#include <QCheckBox>
#include <QTreeWidget>
#include <QLineEdit>
namespace Ui {
class HTMLTreeView;
}

class HTMLTreeView : public QWidget
{
    Q_OBJECT

public:
    explicit HTMLTreeView(QWidget *parent = nullptr);
    ~HTMLTreeView();


    QCheckBox* liveEditCheckBox;
    QTreeWidget* tree;
    QLineEdit* treeColExpr;


private:
    Ui::HTMLTreeView *ui;
};

#endif // HTMLTREEVIEW_H
