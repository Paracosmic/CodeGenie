#ifndef QTREELINEEDIT_H
#define QTREELINEEDIT_H

#include <QLineEdit>
#include <QObject>
#include <QWidget>

class QTreeLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    QTreeLineEdit(QWidget* parent = nullptr);

    class QTreeWidgetItem* itemOwner = nullptr;

    QString style = "";
    bool focused = false;
    int column = -1; //-1 is not set value

    virtual void	mousePressEvent(QMouseEvent *event) override;
    virtual void	focusInEvent(QFocusEvent *event) override;
    virtual void	focusOutEvent(QFocusEvent *event) override;
    virtual void	enterEvent(QEvent *event);
    virtual void	leaveEvent(QEvent *event);

    void call_textEdited(QString str);


public: signals:

    virtual void textEdited(QString str,QTreeLineEdit* caller,QTreeWidgetItem* owner);


};

#endif // QTREELINEEDIT_H
