#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include "element.h"
#include "htmlparser.h"
#include "phpgenerator.h"
#include "settingswindow.h"
#include "treeviewwindow.h"
#include <QPair>
#include <QDir>
#include <QPlainTextEdit>
#include <QSettings>
#include <QTreeWidget>

QT_BEGIN_NAMESPACE

namespace Ui { class MainWindow; }
QT_END_NAMESPACE


typedef Element element;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void LoadSettings();
    ~MainWindow();


    HTMLParser* parse = nullptr;
    PHPGenerator* pgenie = nullptr;
    SettingsWindow* swindow = nullptr;
    TreeViewWindow* twindow  = nullptr;

    QString workingDir;
    QString workingFolder = "";
    QString className = "foo.php";

    static bool checkIfEmpty(const QString contents)
    {
        if(contents.trimmed()=="") return true;
        else return false;
    }

    class QTreeWidgetItem* current = nullptr;
    class QLineEdit* tree_edit = nullptr;

    QMap<QPair<QTreeWidgetItem*,int>,class QTreeLineEdit*> tmap;
    QMap<QTreeLineEdit*,QPair<QTreeWidgetItem*,int>> lmap;
    QMap<QTreeWidgetItem*,Element*> emap;

    void onTreeItemTextEdit(QString str,QTreeLineEdit* caller,QTreeWidgetItem* owner);

    int column = 0;
    QStringList labels = {};
    QStringList values = {};

    void SetWorkingDirectory();
    void FindExistingClassFiles();
    void SaveAll();




    QMap<QString,QWidget*> tabMap;
    //QString is the name of what the plaintextedit is for,  ex html plaintext edit has a string of HTML

    QMap<QString,QPlainTextEdit*> mainTextMap;
    //which ever text edit is active (main tab vs window)

 //   QMap<QString,QPlainTextEdit*> handleTextMap;
    //opens a new window from one of the code tabs



    void OpenCodeWindow(QString window);
    void OpenHTMLWindow();
    void OpenClassWindow();
    void OpenGeneratedWindow();
    void OpenCSSWindow();

    void OpenTreeViewWindow();
    void CloseTreeViewWindow();
    void CloseCodeWindow(QString window,QString text);

    void ParseHTML();
    bool parsePass = false;
    void GenerateHTML();
    void GeneratePHP();
    //0 = not generated, 1 = generated, but not current, 2 = generated and current
    void SetPHPStatus(int i);



private slots:


    void on_typeIDedit_textChanged(const QString &arg1);

    void on_variableNameEdit_textChanged(const QString &arg1);

    void on_variableNameEdit_2_textChanged(const QString &arg1);

    void on_autoIDEdit_textChanged(const QString &arg1);



    void on_wrapInClassCheckBox_toggled(bool checked);

    void toggle_liveEditCheckBox();


    QStringList TokenizeExpression(QString expr);
    QStringList TokenizeExpressionAsStr(QString expr);
    void BuildElementTree(QTreeWidget* tree);
    void BuildMainElementTree();

    void on_liveEditCheckBox_toggled(bool checked);




private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
