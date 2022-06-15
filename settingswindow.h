#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::SettingsWindow *ui;
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();
    bool ResolvePostContent = false;
    bool IgnoreClosingTags  = false;
    bool IgnoreBreakTags  = false;

    bool GenerateIDTags  = false;

    bool CollapseAttributesIntoArray  = false;
    bool ExtractStyle = false;
   bool  WrapInClass = false;


   QString className = "";


  QString typeIDExpression = "";
  QString variableNameExpression = "";
  QString variableNameEmptyExpression = "";
  QString autoIDExpression = "";


private:

public: signals:
    void toggle_ResolvePostContent(bool value);
    void toggle_IgnoreClosingTags(bool value);
    void toggle_IgnoreBreakTags(bool value);
    void toggle_GenerateIDTags(bool value);
    void toggle_CollapseAttributesIntoArray(bool value);
    void toggle_ExtractStyle(bool value);
    void toggle_WrapInClass(bool value);

    void edited_className(QString str);

    void edited_typeIDExpression(QString str);
    void edited_variableNameExpression(QString str);
    void edited_variableNameEmptyExpression(QString str);
    void edited_autoIDExpression(QString str);

private slots:
    void on_collapseAttributesArrayCheckBox_toggled(bool checked);
    void on_ignoreClosingTagsCheckBox_toggled(bool checked);
    void on_resolvePostContentCheckBox_toggled(bool checked);
    void on_ignoreBrCheckBox_toggled(bool checked);
    void on_typeIDedit_textChanged(const QString &arg1);
    void on_variableNameEdit_textChanged(const QString &arg1);
    void on_variableNameEdit_2_textChanged(const QString &arg1);
    void on_autoIDEdit_textChanged(const QString &arg1);
    void on_checkBox_4_toggled(bool checked);
    void on_wrapInClassCheckBox_toggled(bool checked);
    void on_classEdit_textChanged(const QString &arg1);
};

#endif // SETTINGSWINDOW_H
