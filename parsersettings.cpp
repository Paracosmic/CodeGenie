#include "parsersettings.h"
#include "ui_parsersettings.h"

ParserSettings::ParserSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ParserSettings)
{
    ui->setupUi(this);
}

ParserSettings::~ParserSettings()
{
    delete ui;
}
