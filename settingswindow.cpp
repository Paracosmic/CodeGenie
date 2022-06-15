#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    ResolvePostContent = ui->resolvePostContentCheckBox->isChecked();
    IgnoreClosingTags  = ui->ignoreClosingTagsCheckBox->isChecked();
    IgnoreBreakTags  = ui->ignoreBrCheckBox->isChecked();
    CollapseAttributesIntoArray  = ui->collapseAttributesArrayCheckBox->isChecked();
    GenerateIDTags = ui->checkBox_4->isChecked();
    WrapInClass = ui->wrapInClassCheckBox->isChecked();

    typeIDExpression = ui->typeIDedit->text();
    variableNameExpression = ui->variableNameEdit->text();
    variableNameEmptyExpression = ui->variableNameEdit_2->text();
    autoIDExpression = ui->autoIDEdit->text();


    className = ui->classEdit->text();

}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_collapseAttributesArrayCheckBox_toggled(bool checked)
{
    emit toggle_CollapseAttributesIntoArray(checked);
}


void SettingsWindow::on_ignoreClosingTagsCheckBox_toggled(bool checked)
{
    emit toggle_IgnoreClosingTags(checked);
}


void SettingsWindow::on_resolvePostContentCheckBox_toggled(bool checked)
{
    emit toggle_ResolvePostContent(checked);
}


void SettingsWindow::on_ignoreBrCheckBox_toggled(bool checked)
{
    emit toggle_IgnoreBreakTags(checked);
}


void SettingsWindow::on_typeIDedit_textChanged(const QString &arg1)
{
    emit edited_typeIDExpression(arg1);
}


void SettingsWindow::on_variableNameEdit_textChanged(const QString &arg1)
{
    emit edited_variableNameExpression(arg1);
}


void SettingsWindow::on_variableNameEdit_2_textChanged(const QString &arg1)
{
    emit edited_variableNameEmptyExpression(arg1);
}


void SettingsWindow::on_autoIDEdit_textChanged(const QString &arg1)
{
    emit edited_autoIDExpression(arg1);
}


void SettingsWindow::on_checkBox_4_toggled(bool checked)
{
    emit toggle_GenerateIDTags(checked);
}


void SettingsWindow::on_wrapInClassCheckBox_toggled(bool checked)
{
    emit toggle_WrapInClass(checked);
}


void SettingsWindow::on_classEdit_textChanged(const QString &arg1)
{
    className = arg1;
    emit edited_className(arg1);
}

