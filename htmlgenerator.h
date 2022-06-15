#ifndef HTMLGENERATOR_H
#define HTMLGENERATOR_H
#include "element.h"
#include <QDebug>
class HTMLGenerator
{
public:
    HTMLGenerator();
    QString generateAttributes(Element* e);

    QVector<Element*> wstack ={};
    QVector<QString> rstack = {};

 void Push(Element* e);

void End(Element* e);
//$$
bool StackStep(Element* e);
//##


  void StackLoop();



      QString generateHTML(Element* e);
};

#endif // HTMLGENERATOR_H
