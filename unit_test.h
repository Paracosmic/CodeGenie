#ifndef UNIT_TEST_H
#define UNIT_TEST_H
#include "element.h"
#include "htmlparser.h"
#include "phpgenerator.h"

class Unit_Test
{
public:
    Unit_Test();

    //basic test
    //<div></div>

    void TestTemplate(HTMLParser& parse, PHPGenerator& pgenie,QString HTML, QString& code)
    {
        code = "";

        pgenie.elementTypeCounter.clear();


        parse.ParseHTML(HTML);

        if(parse.resolveElements())
        {

            pgenie.evaluateTypeID(parse.elements, "%tag%");
            pgenie.generatePHPElementCode(parse.elements);

            if(parse.ExtractStyle)
                QString style = parse.extractStyle();
        }

        code = pgenie.generateCode(parse.elements);

    }


};

#endif // UNIT_TEST_H
