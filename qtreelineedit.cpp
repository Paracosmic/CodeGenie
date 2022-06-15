#include "qtreelineedit.h"

QTreeLineEdit::QTreeLineEdit(QWidget* parent) :  QLineEdit(parent)
{

connect(this,&QLineEdit::textEdited,this,&QTreeLineEdit::call_textEdited);
}

void QTreeLineEdit::mousePressEvent(QMouseEvent *event)
{
   // QLineEdit::mousePressEvent(event);

  //  this->setStyleSheet("border: 1px solid black");

}

void QTreeLineEdit::focusInEvent(QFocusEvent *event)
{
    focused = true;
    QLineEdit::focusInEvent(event);
    style = "border : 1px solid black;";
    this->setStyleSheet(style);

}

void QTreeLineEdit::focusOutEvent(QFocusEvent *event)
{
     focused = false;
    QLineEdit::focusOutEvent(event);
      style = "border : none;border-bottom: 1px dashed black;border-right: 1px dashed black;";
    this->setStyleSheet(style);
}

void QTreeLineEdit::enterEvent(QEvent *event)
{

    QLineEdit::enterEvent(event);


    QString s = style + "background-color :  rgb(254, 255, 169);border:none;border-bottom: 1px dashed black;border-right: 1px dashed black;";

if(focused)
s=style;
      this->setStyleSheet(s);

}

void QTreeLineEdit::leaveEvent(QEvent *event)
{
       QLineEdit::leaveEvent(event);
    QString s = style;
    if(!focused)
        s+="border:none;border-bottom: 1px dashed black;border-right: 1px dashed black;";
         this->setStyleSheet(s);


}

void QTreeLineEdit::call_textEdited(QString str)
{
    emit textEdited(str,this,itemOwner);

}
