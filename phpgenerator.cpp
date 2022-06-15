#include "phpgenerator.h"
#include <QDebug>
#include <QRegularExpression>
PHPGenerator::PHPGenerator()
{

}

void PHPGenerator::reset()
{

}

QString PHPGenerator::evalString(Element &e, QString string)
{





    QString ret = string;
    if(string.contains("%tag"))
        ret = ret.replace("%tag%",e.tag);


    if(string.contains("%attributes%"))
    {


            ret = ret.replace("%attributes%",e.getAttributesAsStr());

    }

    if(string.contains("%postcontent%"))
    {

        if(e.parent)
            ret = ret.replace("%postcontent%",e.postContent);

    }

    if(string.contains("%content%"))
    {

        if(e.parent)
            ret = ret.replace("%content%",e.content);

    }



    if(string.contains("%ptag%"))
    {

        if(e.parent)
            ret = ret.replace("%ptag%",e.parent->tag);

    }

    if(string.contains("%tid"))
        ret = ret.replace("%tid%",QString::number(e.tid));

    if(string.contains("%vname"))
        ret = ret.replace("%vname%",e.var_name);

    for(auto ee = e.attributes.begin();ee!= e.attributes.end();ee++)
    {
        QString attr = "%"+ee.key()+"%";
        if(ret.contains(attr))
            ret = ret.replace(attr,ee.value());
       // qDebug() << ee.key() << " ; " << ee.value();
    }

    QString temp_ret ="";
    bool parse = false;
    for(auto ch : ret)
    {
        if(ch=='%' && !parse)
        {
            parse = true;
            continue;
        }
        if(ch=='%' && parse)
        {
            parse = false;
            continue;
        }
        if(parse)
        {
            continue;
        }

        temp_ret+=ch;

    }

    ret = temp_ret;
    return ret;



}

QStringList PHPGenerator::evalStrings(Element &e, QString string)
{


    return QStringList();

}



void PHPGenerator::evaluateTypeID(QVector<Element> &elements, QString typeIDExpression)
{
    for(Element& e: elements)
    {

        QString typeID = evalString(e,typeIDExpression);

        if(elementTypeCounter.find(typeID) != elementTypeCounter.end())
        {
            elementTypeCounter[typeID]++;
        } else
        {
            elementTypeCounter[typeID] = 0;
        }

        e.tid = elementTypeCounter[typeID];

    }


}

void PHPGenerator::generatePHPElementCode(QVector<Element> &elements)
{
    QString code,debug;

    for(Element& ee : elements)
    {
        auto e = &ee;



        if(e->tag.contains("!--"))
            continue;

        //    qDebug() <<"| " << e->tag;
        code = "";

        if(IgnoreClosingTags)
            if(e->tag.contains("/"))
                continue;




        if(IgnoreBreakTags)
            if(e->tag == "br")
                continue;


        QString temp = e->tag;
        temp[0] = temp[0].toUpper();

        e->php_class_name = temp;

        e->var_name = evalString(*e,variableNameExpression);
        if(e->var_name.isEmpty())
        {
            e->var_name = evalString(*e,variableNameEmptyExpression);
        }
      //  if(WrapInClass)
       // e->php_var_name = "$this->" +  e->var_name;
     //   else
        e->php_var_name = "$" +  e->var_name;

        if(e->parent)
            code = "$# = new @($" + e->parent->var_name + ");";
        else
            code = "$# = new @();";

        code = code.replace("@",temp);
        code = code.replace("$#",e->php_var_name);

        if(WrapInClass)
            code = code.replace("$","$this->");

        e->php_declare_str = code;

        if(GenerateIDTags)
            if(e->attributes.find("id") == e->attributes.end())
            {
                e->attributes["id"]=evalString(*e,autoIDExpression);
            }


        for(auto a = e->attributes.begin();a != e->attributes.end(); a++)
        {

            QString temp = a.key();
            temp = temp.replace(' ',"");
            temp[0] = temp[0].toUpper();

            e->php_html_attributes.push_back("$this->"+e->var_name + "->set" + temp + "('" +a.value() + "');");
        }

        if(CollapseAttributesIntoArray)
        {
            if(e->attributes.size() > 0)
            {
                //open php array
                QString temp = ", [";

                if(e->parent == nullptr)
                    temp = temp.replace(",","$NULL,");

                        if(WrapInClass)
                            temp = temp.replace("$","$this->");

                //populate array
                for(auto a = e->attributes.begin();a != e->attributes.end(); a++)
                {
                    auto temp2 = a.key() ;
                    temp2 = temp2.replace(' ',"");

                    temp += + "'" + temp2 + "' => '" + a.value() + "'";

                    if(a+1 != e->attributes.end())
                        temp += ", ";
                }
                //close array
                temp += "] );";

                //    e->array_html_attributes = temp;
                e->php_declare_str =  e->php_declare_str.replace(");",temp);

            }

        }


        if(e->content != "" && e->content !="\n")
        {
          //  e->php_html_attributes.push_back("\n" + e->php_var_name + "->setContent('" +e->content + "');\n");
         //
            e->content_functionCall = e->php_var_name + "->setContent('" +e->content + "');\n";

                    if(WrapInClass)
                 e->content_functionCall = "$this->" + e->var_name + "->setContent('" +e->content + "');\n";

            if(e->postContent != "" && e->postContent !="\n")
            {

          //   e->php_html_attributes[e->php_html_attributes.size()-1]+= "\n" + e->php_var_name + "->setPostContent('" +e->postContent + "');\n";

             e->postContent_functionCall = "\n" + e->php_var_name + "->setPostContent('" +e->postContent + "');\n";

                     if(WrapInClass)
                         e->postContent_functionCall  =  "$this->" + e->var_name + "->setPostContent('" +e->postContent + "');\n";

            }

        }



    }


}

QString PHPGenerator::generateCode(QVector<Element> &elements)
{
    QString code = "";
    if(WrapInClass) //wrap in class
    {

        //class $name {
        //
        //
        //public $vars;
        //
        //public __construct()
        //{
        //$vars = new class();
        //}
        //}
 //user code class


//generated code class
        QString classInclude =  "require_once $_SERVER['DOCUMENT_ROOT'].\"/" + workingFolder + "/Classes/class.php\";\n";
        QString styleInclude = "require_once $_SERVER['DOCUMENT_ROOT'].\"/" + workingFolder + "/Styles/" + className + "-style.php" + "\";\n";
        QString classBegin = "class " + className + "_generated {\n\n";

        QString declared_public_php_vars = "";

        //generate element variable declarations
        foreach(auto e , elements)
        {

            if(IgnoreClosingTags
                    && e.tag.contains("/"))
                continue;

            if(IgnoreBreakTags
                    && e.tag == "br")
                continue;

            if(ExtractStyle
                    && e.tag == "style")
                continue;

            declared_public_php_vars += " public " + e.php_var_name + ";\n";
        }

        //generate constructor
        //public __construct()
        //{
        QString constructorBegin = "\n  public function __construct() \n  {\n";

        QString define_php_public_vars = "";

        foreach(auto e , elements)
        {

//            if(postcontent)
//            {
//                if(e.tag.contains("/"))
//                    if(e.php_html_attributes.size()>0)
//                        define_php_public_vars += e.php_html_attributes[e.php_html_attributes.size()-1] + "\n\n";

//            }

            if(IgnoreClosingTags
                    && e.tag.contains("/"))
                continue;

            if(IgnoreBreakTags
                    && e.tag == "br")
                continue;

            if(ExtractStyle
                    && e.tag == "style")
                continue;



            QString temp = e.php_declare_str;
            temp += "\n";

            if(CollapseAttributesIntoArray) //compact attrbutes into array
            {
//                if(e.php_html_attributes.size()>0)
//                {
//                    if(!e.content.isEmpty())
//                        temp += e.php_html_attributes[e.php_html_attributes.size()-1];//content is stored here
//                }
            }
            else //print all attribute set functions
            {
                for(auto a = e.php_html_attributes.begin();a != e.php_html_attributes.end(); a++)
                {
                    temp += +"   " +*a + "\n";
                }

            }

            temp += e.content_functionCall;
            temp += e.postContent_functionCall;

       //    temp = temp.replace('$',"   $this->");

            temp+= "\n";
            define_php_public_vars += temp;
        }


        QString generateFunction = "\n  function generateHTML()\n  {\n   return ";
        generateFunction += elements[0].php_var_name + "->generateHTML();\n  }\n";
        generateFunction = generateFunction.replace('$',"$this->");
        code = "<?php\n";

        code += classInclude;

        if(ExtractStyle)
            code += styleInclude;

        code +=  classBegin + declared_public_php_vars + constructorBegin + define_php_public_vars + "  }\n" + generateFunction + "\n}\n \n?>";

    }
    //$vars = new class();


    else

    {
        //generate code
        foreach(  auto e , elements)
        {
            if(IgnoreClosingTags
                    && e.tag.contains("/"))
                continue;

            if(IgnoreBreakTags
                    && e.tag == "br")
                continue;

            if(ExtractStyle
                    && e.tag == "style")
                continue;




            code += e.php_declare_str + "";
            //if
            if(CollapseAttributesIntoArray) //compact attrbutes into array
            {
//                if(e.php_html_attributes.size()>0)
//                {

//                    // code += e.array_html_attributes + "";


////                   if(!e.content.isEmpty())
////                 {    code += e.php_html_attributes[e.php_html_attributes.size()-1];//content is stored here
////                   qDebug() <<"YO"<<  e.php_html_attributes[e.php_html_attributes.size()-1];
////                   }
//                }
            }
            else //print all attribute set functions
            {
                for(auto a = e.php_html_attributes.begin();a != e.php_html_attributes.end(); a++)
                {
                    code += +"   " +*a + "\n";
                }

            }
            code += "\n";


        }
    }
    return code;
}

QStringList PHPGenerator::generateCodes(QVector<Element> &elements)
{
    QStringList code = {generateCode(elements)};


    if(WrapInClass)
    {

       QString include = "require_once $_SERVER['DOCUMENT_ROOT'].\"/CodeGenie/Generated/"+className+"-generated.php\";\n\n";
       QString constructorCode = "\n	public function __construct()\n	{\n		parent::__construct();\n	}";
       QString classCode = "class " + className + " extends "+ className+"_generated {\n "+constructorCode+" \n\n\n}";

       code.append("<?php\n\n "+include + classCode + " \n\n?>");

    }

return code;

}

void PHPGenerator::on_toggle_IgnoreClosingTags(bool value)
{
    IgnoreClosingTags = value;
}

void PHPGenerator::on_toggle_IgnoreBreakTags(bool value)
{
    IgnoreBreakTags = value;
}

void PHPGenerator::on_toggle_GenerateIDTags(bool value)
{
    GenerateIDTags = value;
}

void PHPGenerator::on_toggle_CollapseAttributesIntoArray(bool value)
{
    CollapseAttributesIntoArray = value;
}

void PHPGenerator::on_toggle_ExtractStyle(bool value)
{
    ExtractStyle = value;
}

void PHPGenerator::on_toggle_WrapInClass(bool value)
{
    WrapInClass = value;
}

void PHPGenerator::on_edited_className(QString str)
{
    className = str;
}

void PHPGenerator::on_edited_typeIDExpression(QString str)
{
    typeIDExpression = str;
}

void PHPGenerator::on_edited_variableNameExpression(QString str)
{
    variableNameExpression = str;
}

void PHPGenerator::on_edited_variableNameEmptyExpression(QString str)
{
    variableNameEmptyExpression = str;
}

void PHPGenerator::on_edited_autoIDExpression(QString str)
{
    autoIDExpression = str;
}
