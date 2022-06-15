#include "filesettings.h"
#include "ui_filesettings.h"

FileSettings::FileSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FileSettings)
{
    ui->setupUi(this);
}

FileSettings::~FileSettings()
{
    delete ui;
}
