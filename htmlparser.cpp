#include "htmlparser.h"

HTMLParser::HTMLParser()
{
    elements.clear();
}

void HTMLParser::reset()
{
     elements.clear();
}

QMap<QString, QString> HTMLParser::tokenize(QString input)
{



    //step 1
    //remove < and >
    //ex: form id='abc'  class='foo'
    QString str = input.replace('<',' ');
    str = str.replace('>',' ');

    //remove spaces, this is a band-aid / fix parser
    str = str.replace(" = ","=");
    str = str.replace("= ","=");
    str = str.replace(" =","=");

    //step 2
    //extract first string until ' ' (space) is reached, tag is set to this strings value
    QString tag =  extractTag(str);
    //remaining string should be attributes
    //ex: id='abc'  class='foo'
    //ex: the tag is 'form'

    //step 3
    //read in  alphanumeric char until '=' is reached, attribute name read in is complete, reading in value
    //REPEAT UNTIL INPUT IS EMPTY



    QMap<QString,QString> map;


    tag = tag.replace(' ',"");
    map["tag"]=tag;

    QString token = "";
    QString value = "";

    bool tokenDone = false;

    foreach(auto c, str)
    {
        if(c == '=')
        {
            tokenDone = true;


            token = token.replace(' ',"");

            continue;

        }

        if(c == "'" || c == "\"")
        {
            if(!value.isEmpty())
            {
                map[token]=value;
                //     qDebug() << token + "='"+value+"'";
                tokenDone = false;
                value = "";
                token = "";
            }
            continue;
        }

        if(!tokenDone)
            token += c;


        if(tokenDone)
        {
            value += c;
        }













    }



    //  qDebug() << temp;
    return map;
}

QString HTMLParser::extractTag(QString &input)
{
    QString tag = "";
    int substr = 0;

    foreach(auto c, input)
    {

        if(tag == "")
            if(c == ' ')
                continue;

        if(tag == "/")
            if(c == ' ')
                continue;



        substr++;

        if(c == ' ')
            break;

        tag += c;

    }



    input = input.mid(substr,input.count());

    return tag;
}

Element HTMLParser::tokenizeElement(QString input)
{
    auto map = tokenize(input);
    Element e;
    e.tag = map["tag"];
    map.remove("tag");
    e.attributes = map;

    return e;
}

bool HTMLParser::resolveTest()
{

    QVector<QString> stack;

    for(Element& e : elements)
    {

        //has no closing tag!
        if(e.tag.contains("input"))
            continue;
        if(e.tag.contains("br"))
            continue;
        if(e.tag.contains("!--"))
            continue;
        if(e.tag.contains("img"))
            continue;
        if(e.tag.contains("hr"))
            continue;
        if(e.tag.contains("meta"))
            continue;

        //find tag in stack with closing tag
        //set tagOwner as well
        if(e.tag.contains('/'))
        {
         //   qDebug() << e.tag;
            int i = stack.size() - 1;
            auto copy = stack;

            for(int i = stack.size()-1; i > -1;--i)
            {
                QString t = e.tag;
                QString temp = t.replace('/',"");
                if(stack[i] == temp)
                {
                //    qDebug() << i;


                    copy.remove(i);

                }

            }
            stack = copy;

        }
        else
        {
            stack.push_back(e.tag);

        }




    }
    //qDebug() <<"I should be zero : " << stack.size();
    return stack.isEmpty();
}

void HTMLParser::resolveParents()
{
    QVector<Element*> stack;
    Element* current_parent = nullptr;

    for(Element& e : elements)
    {

        if(e.tag == "input" || e.tag == "br" || e.tag.contains("!--") || e.tag.contains("img") || e.tag.contains("hr") || e.tag.contains("meta"))
        {

            if(current_parent == nullptr)
             continue;

            e.parent = current_parent;//parent is null in this case
            current_parent->children.push_back(&e);
            continue;
        }

        if(e.parent == nullptr && current_parent == nullptr)
        {
            current_parent = &e;
            stack.push_back(&e);
            continue;

        }

        e.parent = current_parent;
   current_parent->children.push_back(&e);

        if(e.tag.contains("/"))
        {
            QString temp = e.tag;temp=temp.replace('/',"");


            //            auto copy = stack;
            //            int i = stack.size();
            //            for(QVector<element*>::iterator itr = copy.end(); itr != copy.begin(); itr--)
            //            {
            //                if((*itr)->tag == temp)
            //                {
            //                    current_parent = copy[i]->parent;
            //                    copy.remove(i);
            //                    break;
            //                }
            //                i--;
            //            }
            //            stack = copy;
            if(stack.size() > 0)
                if(stack[stack.size()-1]->tag == temp)
                {
              //      qDebug() << stack[stack.size()-1]->tag;
                    current_parent = stack[stack.size()-1]->parent;
                    stack.remove(stack.size()-1);

                }


            continue;
        }


        stack.push_back(&e);

        current_parent = &e;
    }
}

void HTMLParser::resolveParentLevel()
{
    for(Element& ee : elements)
    {

        Element* curr_p = ee.parent;



        while(curr_p !=nullptr)
        {
            ee.parentLevel+=1;

            curr_p = curr_p->parent;
        }



   //     qDebug() << ee.parentLevel;

    }
}

//void HTMLParser::computeElements(QVector<Element> &elements)
//{
//    QString code,debug;

//    for(Element& ee : elements)
//    {
//        auto e = &ee;



//        if(e->tag.contains("!--"))
//            continue;

//        //    qDebug() <<"| " << e->tag;
//        code = "";

//        if(IgnoreClosingTags)
//            if(e->tag.contains("/"))
//                continue;




//        if(IgnoreBreakTags)
//            if(e->tag == "br")
//                continue;


//        QString temp = e->tag;
//        temp[0] = temp[0].toUpper();

//        e->php_class_name = temp;

//        e->var_name = evalString(*e,variableNameExpression);
//        if(e->var_name.isEmpty())
//        {
//            e->var_name = evalString(*e,variableNameEmptyExpression);
//        }
//        e->php_var_name = "$" +  e->var_name;

//        if(e->parent)
//            code = "$# = new @($" + e->parent->var_name + ");";
//        else
//            code = "$# = new @();";

//        code = code.replace("@",temp);
//        code = code.replace("$#",e->php_var_name);

//        e->php_declare_str = code;

//        if(GenerateIDTags)
//            if(e->attributes.find("id") == e->attributes.end())
//            {
//                e->attributes["id"]=evalString(*e,autoIDExpression);
//            }


//        for(auto a = e->attributes.begin();a != e->attributes.end(); a++)
//        {

//            QString temp = a.key();
//            temp = temp.replace(' ',"");
//            temp[0] = temp[0].toUpper();
//            e->php_html_attributes.push_back(e->php_var_name + "->set" + temp + "('" +a.value() + "');");
//        }

//        if(CollapseAttributesIntoArray)
//        {
//            if(e->attributes.size() > 0)
//            {
//                //open php array
//                QString temp = ", [";

//                if(e->parent == nullptr)
//                    temp = temp.replace(",","$NULL,");

//                //populate array
//                for(auto a = e->attributes.begin();a != e->attributes.end(); a++)
//                {
//                    auto temp2 = a.key() ;
//                    temp2 = temp2.replace(' ',"");

//                    temp += + "'" + temp2 + "' => '" + a.value() + "'";

//                    if(a+1 != e->attributes.end())
//                        temp += ", ";
//                }
//                //close array
//                temp += "] );";

//                //    e->array_html_attributes = temp;
//                e->php_declare_str =  e->php_declare_str.replace(");",temp);

//            }

//        }


//        if(e->content != "" || e->content !="\n")
//        {
//            e->php_html_attributes.push_back("\n" + e->php_var_name + "->setContent('" +e->content + "');\n");
//            if(e->postContent != "" && e->postContent !="\n")
//        e->php_html_attributes[e->php_html_attributes.size()-1]+= "\n" + e->php_var_name + "->setPostContent('" +e->postContent + "');\n";

//        }



//    }

//}

void HTMLParser::resolvePostContent()
{

    QVector<Element*> estack;
  //  if(!ResolvePostContent)
      //  return;

    for(Element& e: elements)
    {
   //     qDebug() << e.tag << "  ::  " << e.content;
        if(e.tag.contains("/"))
        {
            if(e.tagOwner == nullptr)
            {
                if(estack.size() > -1)
                {

                    for(int i = estack.size() - 1; i > -1; --i)
                    {
                        QString t = e.tag;
                        t.replace("/","");

                        if(estack[i]->tag == t)
                        {
                            if(estack[i]->tagOwner != nullptr)
                                continue;

                            e.tagOwner = estack[i];

                            estack[i]->closingTag = &e;
                            //postcontent
                            estack[i]->postContent = e.content;
                            estack.remove(i);
                            break;
                        }

                    }
                }
            }


            //style's postContent needs to be resolved to the closing tag owners, parent
            if(ExtractStyle)
            if(e.tag.contains("style"))
            {

                e.tagOwner->parent->content = e.content;

            }

        } else
        {

            estack.push_back(&e);

        }

        //resolve postcontent for a style that was extracted



    }

    //   qDebug() <<"I should be zero : " << estack.size();

}

//void HTMLParser::resolveTypeID(QVector<Element> &elements)
//{
//    for(Element& e: elements)
//    {

//        QString typeID = evalString(e,typeIDExpression);

//        if(elementTypeCounter.find(typeID) != elementTypeCounter.end())
//        {
//            elementTypeCounter[typeID]++;
//        } else
//        {
//            elementTypeCounter[typeID] = 0;
//        }

//        e.tid = elementTypeCounter[typeID];

//    }

//}

QString HTMLParser::extractStyle()
{
    if(ExtractStyle)
    {
        QString style = "<style>\n";

        for(Element& e : elements)
        {
            //qDebug() << "print " << e.tag;
            if(e.tag == "style")
            {
                style += e.content;

            }



        }
        style += "</style>";

        //ui->plainTextEdit_3->setPlainText(style);
        return style;
    }

    return "";
}

//QString HTMLParser::evalString(Element &e, QString string)
//{



//        QString ret = string;
//        if(string.contains("%tag"))
//            ret = ret.replace("%tag%",e.tag);

//        if(string.contains("%ptag%"))
//        {

//            if(e.parent)
//                ret = ret.replace("%ptag%",e.parent->tag);

//        }

//        if(string.contains("%tid"))
//            ret = ret.replace("%tid%",QString::number(e.tid));

//        if(string.contains("%vname"))
//            ret = ret.replace("%vname%",e.var_name);

//        for(auto ee = e.attributes.begin();ee!= e.attributes.end();ee++)
//        {
//            QString attr = "%"+ee.key()+"%";
//            if(ret.contains(attr))
//                ret = ret.replace(attr,ee.value());
//            qDebug() << ee.key() << " ; " << ee.value();
//        }

//        QString temp_ret ="";
//        bool parse = false;
//        for(auto ch : ret)
//        {
//            if(ch=='%' && !parse)
//            {
//                parse = true;
//                continue;
//            }
//            if(ch=='%' && parse)
//            {
//                parse = false;
//                continue;
//            }
//            if(parse)
//            {
//                continue;
//            }

//            temp_ret+=ch;

//        }

//        ret = temp_ret;
//        return ret;



//}

void HTMLParser::ParseHTML(QString str)
{

    int MODE = 0;

    //0 outside of tag
    //1 inside tag
    //2 end of tag
    const int OUTSIDE_OF_TAG = 0;
    const int INSIDE_OF_TAG = 1;
    const int END_OF_TAG = 2;

    QString current  = "";
    QString last_content = "";

    elements.clear();

    QVector<QString> stack;

    elementTypeCounter.clear();
    Element* last = nullptr;

    //parse for <element>'content'</element>
    //content is assiocated with the last element parsed
    foreach(auto curr_char , str)
    {


        if(curr_char == '<')
        {
            MODE = INSIDE_OF_TAG;



            if(last)
                last->content = current;

            current = "";
            last_content = current;

        }
        else if(curr_char == '>')
            MODE = END_OF_TAG;



        switch(MODE)
        {
        case OUTSIDE_OF_TAG:
        {
            if(current.isEmpty())
                if(checkIfEmpty(curr_char))
                    continue;

            current += curr_char;
            break;
        }

        case INSIDE_OF_TAG:
        {
            current += curr_char;
            break;
        }

        case END_OF_TAG:

        {



            current += curr_char;

            if(current == '\n')
                continue;

            stack.push_back(current);

            Element e = tokenizeElement(current);
            e.uid = 100 + elements.size();


            //            QString typeID = evalString(e,typeIDExpression);

            //            if(elementTypeCounter.find(typeID) != elementTypeCounter.end())
            //            {
            //                elementTypeCounter[typeID]++;
            //            } else
            //            {
            //                elementTypeCounter[typeID] = 0;
            //            }

            //            e.tid = elementTypeCounter[typeID];

            elements.push_back(e);

            last = &elements[elements.size()-1];

            current = "";
            MODE = 0;
            continue;
            break;
        }

        default:

        {

            break;
        }

        }//end of MODE switch

    }//end of reading in text loop



}

bool HTMLParser::resolveElements()
{

    if((elements.size() > 0) && resolveTest())
    {

        resolveParents();
        resolveParentLevel();
        resolvePostContent();

        return true;
    }
    else
    {
        qDebug() << "Resolve failed!";

        return false;
    }
}

void HTMLParser::on_toggle_ResolvePostContent(bool value)
{
    ResolvePostContent = value;
}

void HTMLParser::on_toggle_IgnoreClosingTags(bool value)
{
    IgnoreClosingTags = value;
}

void HTMLParser::on_toggle_IgnoreBreakTags(bool value)
{
    IgnoreBreakTags = value;

}

void HTMLParser::on_toggle_GenerateIDTags(bool value)
{
    GenerateIDTags= value;
}

void HTMLParser::on_toggle_CollapseAttributesIntoArray(bool value)
{
    CollapseAttributesIntoArray = value;
}

void HTMLParser::on_toggle_ExtractStyle(bool value)
{
    ExtractStyle= value;
}


