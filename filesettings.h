#ifndef FILESETTINGS_H
#define FILESETTINGS_H

#include <QMainWindow>

namespace Ui {
class FileSettings;
}

class FileSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit FileSettings(QWidget *parent = nullptr);
    ~FileSettings();

private:
    Ui::FileSettings *ui;
};

#endif // FILESETTINGS_H
