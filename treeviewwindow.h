#ifndef TREEVIEWWINDOW_H
#define TREEVIEWWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include "htmlparser.h"

namespace Ui {
class TreeViewWindow;
}

class TreeViewWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TreeViewWindow(QWidget *parent = nullptr);
      void closeEvent(QCloseEvent *event) override;
    HTMLParser* parse = nullptr;
    QTreeWidget* tree = nullptr;

    void InsertMainTree(QTreeWidget* tw);

    ~TreeViewWindow();


public slots:
    void on_liveEditCheckBox_toggled(bool checked);
    void toggleliveEditCheckBox();

public: signals:
    void toggle_liveEditCheckBox(bool checked);
    void CloseWindow();


private:
    Ui::TreeViewWindow *ui;
};

#endif // TREEVIEWWINDOW_H
