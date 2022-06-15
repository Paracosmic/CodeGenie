#ifndef HTMLPARSER_H
#define HTMLPARSER_H
#include <QVector>
#include <QMap>
#include "element.h"
#include <QDebug>

class HTMLParser : public QObject
{
    Q_OBJECT
public:
    HTMLParser();

    void reset();

    QVector<Element> elements;
    QMap<QString,int> elementTypeCounter; //keeps track of the count of each attribute type


    QMap<QString,QString> tokenize(QString input);
    QString extractTag(QString& input);

  Element tokenizeElement(QString input);

    bool resolveTest();
    void resolveParents();
    void resolveParentLevel();
 //   void computeElements(QVector<Element> &elements);
    void resolvePostContent();


    //php - move to phpgenerator
 //   void resolveTypeID(QVector<Element> &elements);

    QString extractStyle();

 //   QString evalString(Element& e,QString string);


    //move to phpgenerator
    QString typeIDExpression = "";
    QString variableNameExpression = "";
    //if above evaluates to am empty string (above) then return the evaulated expresion (below)
    QString variableNameEmptyExpression = "";
    QString autoIDExpression = "";

    QMap<QString,QString> settings;

    bool ResolvePostContent = false;
    bool IgnoreClosingTags  = false;
    bool IgnoreBreakTags  = false;
    bool GenerateIDTags  = false;
    bool CollapseAttributesIntoArray  = false;
    bool ExtractStyle = false;

 //   QString ExtractedStyle = "";

    //parses all html into a stack of elements
    void ParseHTML(QString str);
    bool resolveElements();

    static bool checkIfEmpty(const QString contents)
     {
          if(contents.trimmed()=="") return true;
          else return false;
     }
public slots:
    void on_toggle_ResolvePostContent(bool value);
    void on_toggle_IgnoreClosingTags(bool value);
    void on_toggle_IgnoreBreakTags(bool value);
    void on_toggle_GenerateIDTags(bool value);
    void on_toggle_CollapseAttributesIntoArray(bool value);
    void on_toggle_ExtractStyle(bool value);


};

#endif // HTMLPARSER_H
