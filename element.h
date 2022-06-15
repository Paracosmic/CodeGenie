#ifndef ELEMENT_H
#define ELEMENT_H

#include <QMap>
#include <QString>

class Element
{
public:
    Element();

    //ex:
    //<div id = 'd1'><a id = 'a1'></a></div>
    //the parent of <a id = 'a1'> is <div id = 'd1'>
    Element* parent = nullptr;

    int parentLevel = 0;

    QVector<Element*> children = {};

    //if a closing tag, this will point to the owner
    //ex:
    //<div id = 'd1'><a id = 'a1'></a></div>
    //the element </a> will have tagOwner set to <a id = 'a1'>
    Element* tagOwner = nullptr;

    //if a tag that is closed, this will point to the closing element
    //ex:
    //<div id = 'd1'><a id = 'a1'></a></div>
    //the element <a id = 'a1'> will have closingTag set to </a>
    Element* closingTag = nullptr;

    //content that prepends the closing tag
    //ex:
    //<div id = 'd1'>normal content here<a id = 'a1'></a></div>

    QString content = "";
    //content that prepends the closing tag
    //ex:
    //<div id = 'd1'><a id = 'a1'></a>post content here</div>
    QString postContent = "";

    //ex:
    //<div id='div_1'>
    //the tag is set to 'div'
    QString tag;
    //container for attributes
    //ex :
    //<div id='div_1'>
    //atributes["id"] == "div_1";
    //above is TRUE
    QMap<QString,QString> attributes;

    //each element gets a unique id, this is generated from its index value from the stack of contained elements
    int uid = 0;
    //type id is set by a 'user defined expression' with the 'evalString' function
    int tid = 0;


    //PHP variable name set by a 'user defined expression'
    //ex:
    //<div id='div_1'>
    //var_name == "div_1"
    QString var_name = "";

    //PHP code that declares the element within the scope of a class
    //only used when 'generate class file' is set to true
    //ex:
    //<div id='div_1'>
    //declarte_str  == "$public $div_1;"
    //Note $div_1 may not be the variable name as this is set by a 'user defined expression' and is just an example
    //the real variable name is store in 'var_name' after the expression is evaluated
    QString php_declare_str = "";

    //PHP variable name set by a 'user defined expression'
    //ex:
    //<div id='div_1'>
    //php_var_name == "$div_1"
    //OR
    //php_var_name == "$" + var_name

    QString php_var_name = ""; //$tag_tid

    //name of PHP class that is set by the user
    QString php_class_name = "";

    //PHP function calls that set attributes
    //<div id='div_1'>
    //$div_1->setID('div_1');
    QVector<QString> php_html_attributes; //name='value'


    QString content_functionCall = "";
    QString postContent_functionCall = "";
    //PHP array that contains attribute data
    //<div id='div_1' class='foo'>
    //['id'=>'div_1','class'=>'foo']
    QString php_array_html_attributes = "";


    QString getAttributesAsStr();

    void set(QString name, QString value);



    //to html stuff
    int index_c = 0;

    bool closed = false; //bandaid :(


};

#endif // ELEMENT_H
