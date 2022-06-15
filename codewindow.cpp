#include "codewindow.h"
#include "ui_codewindow.h"
#include "mainwindow.h"
CodeWindow::CodeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CodeWindow)
{
    ui->setupUi(this);
}

void CodeWindow::SetText(QString text)
{
    //ui->plainTextEdit->setPlainText(text);
}

void CodeWindow::SetTitle(QString window)
{
    this->setWindowTitle(window);
}



void CodeWindow::closeEvent(QCloseEvent *event)
{
     emit CloseWindow(this->windowTitle(),nullptr);
   // emit CloseWindow(windowName,ui->plainTextEdit->toPlainText());
}

void CodeWindow::BindShortcutKeys(MainWindow* mw)
{
    //parser menu functions

    connect(ui->actionParse_HTML,&QAction::triggered,mw,&MainWindow::ParseHTML);
    ui->actionParse_HTML->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));



    //generator menu actions
    connect(ui->actionGenerate_PHP,&QAction::triggered,mw,&MainWindow::GeneratePHP);
    ui->actionGenerate_PHP->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_P));

    connect(ui->actionGenerate_HTML,&QAction::triggered,mw,&MainWindow::GenerateHTML);
    ui->actionGenerate_HTML->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_G));
}

QPlainTextEdit *CodeWindow::GetPlainTextEdit()
{
    return nullptr;
}



CodeWindow::~CodeWindow()
{
    delete ui;
}
