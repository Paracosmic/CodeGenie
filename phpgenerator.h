#ifndef PHPGENERATOR_H
#define PHPGENERATOR_H

#include <QObject>
#include "element.h"
#include <QStringList>
class PHPGenerator : public QObject
{
    Q_OBJECT
public:
    PHPGenerator();
    void reset();
    QString evalString(Element& e,QString string);
   QStringList evalStrings(Element& e,QString string);

    QMap<QString,int> elementTypeCounter; //keeps track of the count of each attribute type


    void evaluateTypeID(QVector<Element> &elements, QString typeIDExpression);


    void generatePHPElementCode(QVector<Element> &elements);

    QString generateCode(QVector<Element> &elements);

    QStringList generateCodes(QVector<Element> &elements);

    //move to phpgenerator
    QString typeIDExpression = "";
    QString variableNameExpression = "";
    //if above evaluates to am empty string (above) then return the evaulated expresion (below)
    QString variableNameEmptyExpression = "";
    QString autoIDExpression = "";
    QString className = "";
    QString workingFolder = "";

    bool IgnoreClosingTags = false;
    bool IgnoreBreakTags= false;
    bool GenerateIDTags= false;
    bool CollapseAttributesIntoArray = false;
    bool ExtractStyle= false;
    bool WrapInClass = false;

public slots:

    void on_toggle_IgnoreClosingTags(bool value);
    void on_toggle_IgnoreBreakTags(bool value);
    void on_toggle_GenerateIDTags(bool value);
    void on_toggle_CollapseAttributesIntoArray(bool value);
    void on_toggle_ExtractStyle(bool value);
    void on_toggle_WrapInClass(bool value);

       void on_edited_className(QString str);

    void on_edited_typeIDExpression(QString str);
    void on_edited_variableNameExpression(QString str);
    void on_edited_variableNameEmptyExpression(QString str);
    void on_edited_autoIDExpression(QString str);
};

#endif // PHPGENERATOR_H
