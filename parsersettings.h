#ifndef PARSERSETTINGS_H
#define PARSERSETTINGS_H

#include <QMainWindow>

namespace Ui {
class ParserSettings;
}

class ParserSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit ParserSettings(QWidget *parent = nullptr);
    ~ParserSettings();

private:
    Ui::ParserSettings *ui;
};

#endif // PARSERSETTINGS_H
