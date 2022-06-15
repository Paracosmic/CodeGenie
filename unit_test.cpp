#include "unit_test.h"


Unit_Test::Unit_Test()
{
    HTMLParser parse;
    PHPGenerator pgenie;
    QString code;

    parse.ExtractStyle = true;
    pgenie.typeIDExpression = "%tag%";
    pgenie.variableNameExpression = "%id%";
    pgenie.variableNameEmptyExpression = "%tag%_%tid%";
    pgenie.autoIDExpression = "%vname%";
    pgenie.className = "unit_test";

    pgenie.CollapseAttributesIntoArray = true;
    pgenie.ExtractStyle = true;
    pgenie.GenerateIDTags = true;
    pgenie.IgnoreBreakTags = false;
    pgenie.IgnoreClosingTags = true;
    pgenie.WrapInClass = true;




    TestTemplate(parse,pgenie,"<div class = 'foo'>1<a>2</ a>3<a>4< / a >5</div>",code);

    if(parse.elements.size() == 6)
    {
        if(parse.elements[0].tag == "div" &&
                parse.elements[1].tag == "a" &&
                parse.elements[2].tag == "/a" &&
                parse.elements[3].tag == "a" &&
                parse.elements[4].tag == "/a" &&
                parse.elements[5].tag == "/div" )
        {
            qDebug() << "Test 1.0 - Element Order and Tags - Pass!";
        }else
        {
            qDebug() << "Test 1.0 - Element Order and Tags - Fail!";

        }

        if (parse.elements[0].attributes["id"] == "div_0" &&
                parse.elements[1].attributes["id"] == "a_0" &&
                parse.elements[3].attributes["id"] == "a_1")
        {
            qDebug() << "Test 1.1 - Auto Generate ID - Pass!";
        } else
        {
            qDebug() << "Test 1.1 - Auto Generate ID - Fail!";

        }

        if(parse.elements[0].content == "1" &&
                parse.elements[1].content == "2" &&
                parse.elements[1].postContent == "3" &&
                parse.elements[3].content == "4" &&
                parse.elements[3].postContent == "5")
        {
            qDebug() << "Test 1.2 - Content / Post Content - Pass!";

        }else
        {
            qDebug() << "Test 1.2 Content / Post Content - Fail!";

        }


        if(parse.elements[0].php_declare_str == "$this->div_0 = new Div($this->NULL, ['class' => 'foo', 'id' => 'div_0'] );")
        {
            qDebug() << "Test 1.3 - PHP Declare String - Pass!";
        }else
        {
            qDebug() << "Test 1.3 - PHP Declare String - Fail! ";
        }

    }


    parse.reset();
     pgenie.reset();

   code = "";





    TestTemplate(parse,pgenie,"<div class = 'foo'><style>.foo {color:red;}</style>1<a>2</ a>3<a>4< / a >5</div>",code);
    QString style = parse.extractStyle();
  //  qDebug() << style;
    if(style.contains("<style>") && style.contains(".foo") && style.contains("{color:red;}") && style.contains("</style>"))
    {
        qDebug() << "Test 2.0 - Style Extraction - Pass!";


    } else
    {
          qDebug() << "Test 2.0 - Style Extraction - Fail!";


    }

    if(parse.elements[0].tag == "div" && parse.elements[0].content == "1")
    {
        qDebug() << "Test 2.1 - Style Extraction Post Content Resolved - Pass!";


    } else
    {
          qDebug() << "Test 2.1 - Style Extraction Post Content Resolved - Fail!";


    }




    parse.reset();
     pgenie.reset();

   code = "";


   parse.ExtractStyle = false;
   pgenie.ExtractStyle = false;


    TestTemplate(parse,pgenie,"<div class = 'foo'><style>.foo {color:red;}</style>1<a>2</ a>3<a>4< / a >5</div>",code);
    style = parse.extractStyle();
  //  qDebug() << style;
    if(parse.elements[1].tag == "style" && parse.elements[1].content == ".foo {color:red;}")
    {
        qDebug() << "Test 2.2 - Style Inline - Pass!";


    } else
    {
          qDebug() << "Test 2.2 - Style Inline - Fail!";


    }




}
