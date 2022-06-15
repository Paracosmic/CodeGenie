#ifndef EVALUATE_H
#define EVALUATE_H

#include <QString>
#include "element.h"
class Evaluate
{
public:
    Evaluate();

    QString typeIDExpression = "";
    QString variableNameExpression = "";
    //if above evaluates to am empty string (above) then return the evaulated expresion (below)
    QString variableNameEmptyExpression = "";

    QString autoIDExpression = "";


};

#endif // EVALUATE_H
