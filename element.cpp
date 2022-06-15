#include "element.h"
#include <QDebug>
Element::Element()
{

}

QString Element::getAttributesAsStr()
{
    QString ret = "";
    QMap<QString, QString>::const_iterator i = attributes.constBegin();
    while (i !=  attributes.constEnd()) {

        ret += i.key() + " = " + i.value();

        ++i;

        if(i !=  attributes.constEnd())
            ret += ", ";

    }

    return ret;

}

void Element::set(QString name, QString value)
{
qDebug() << name;
    if(name.toLower() == "tag")
    {     tag = value;

        if(closingTag)
        {
            closingTag->tag = "/" + value;
        }

    }


    else
    if(name.toLower() == "content")
        content = value;
    else
    if(name.toLower() == "postcontent")
        postContent = value;
   else
    if(name.toLower() == "vname")
        var_name = value;
else if(attributes.find(name) != attributes.end())
    {
        attributes[name] = value;

    }else
    {
        attributes.insert(name.toLower(),value);
    }

}
