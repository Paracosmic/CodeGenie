#ifndef CODEWINDOW_H
#define CODEWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>

namespace Ui {
class CodeWindow;
}

class CodeWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit CodeWindow(QWidget *parent = nullptr);
    void SetText(QString text);
    void SetTitle(QString window);
    void closeEvent(QCloseEvent *event);
    QString windowName = "";
    QString className = "";
    void BindShortcutKeys(class MainWindow* mw);
    QPlainTextEdit* GetPlainTextEdit();

    class MainWindow* parent = nullptr;



    ~CodeWindow();

public: signals:
    void CloseWindow(QString window, QString text);

private:
    Ui::CodeWindow *ui;
};

#endif // CODEWINDOW_H
