#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QFileDialog>
#include <QDebug>
#include "unit_test.h"
#include "qtreelineedit.h"
#include "htmlgenerator.h"
#include "codewindow.h"
#include <QScrollBar>
#include <QHeaderView>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //load settings
    LoadSettings();

    //initialize helper classes
    parse = new HTMLParser();
    pgenie = new PHPGenerator();

    //initialize windows
    swindow = new SettingsWindow(this);
    twindow = new TreeViewWindow(this);


    //this allows for the htmltreeview to be added back as a tab
    connect(twindow,&TreeViewWindow::CloseWindow,this,&MainWindow::CloseTreeViewWindow);

    //relaying checkbox toggle to HTMLParser
    connect(swindow,&SettingsWindow::toggle_ResolvePostContent,parse,&HTMLParser::on_toggle_ResolvePostContent);
    connect(swindow,&SettingsWindow::toggle_CollapseAttributesIntoArray,parse,&HTMLParser::on_toggle_CollapseAttributesIntoArray);
    connect(swindow,&SettingsWindow::toggle_GenerateIDTags,parse,&HTMLParser::on_toggle_GenerateIDTags);
    connect(swindow,&SettingsWindow::toggle_IgnoreBreakTags,parse,&HTMLParser::on_toggle_IgnoreBreakTags);
    connect(swindow,&SettingsWindow::toggle_IgnoreClosingTags,parse,&HTMLParser::on_toggle_IgnoreClosingTags);

    //relaying checkbox toggle to PHPGenerator
    connect(swindow,&SettingsWindow::toggle_CollapseAttributesIntoArray,pgenie,&PHPGenerator::on_toggle_CollapseAttributesIntoArray);
    connect(swindow,&SettingsWindow::toggle_GenerateIDTags,pgenie,&PHPGenerator::on_toggle_GenerateIDTags);
    connect(swindow,&SettingsWindow::toggle_IgnoreBreakTags,pgenie,&PHPGenerator::on_toggle_IgnoreBreakTags);
    connect(swindow,&SettingsWindow::toggle_IgnoreClosingTags,pgenie,&PHPGenerator::on_toggle_IgnoreClosingTags);
    connect(swindow,&SettingsWindow::toggle_WrapInClass,pgenie,&PHPGenerator::on_toggle_WrapInClass);

    //relaying lineedit 'text edited' to PHPGenerator
    connect(swindow,&SettingsWindow::edited_className,pgenie,&PHPGenerator::on_edited_className);
    connect(swindow,&SettingsWindow::edited_autoIDExpression,pgenie,&PHPGenerator::on_edited_autoIDExpression);
    connect(swindow,&SettingsWindow::edited_typeIDExpression,pgenie,&PHPGenerator::on_edited_typeIDExpression);
    connect(swindow,&SettingsWindow::edited_variableNameExpression,pgenie,&PHPGenerator::on_edited_variableNameExpression);
    connect(swindow,&SettingsWindow::edited_variableNameEmptyExpression,pgenie,&PHPGenerator::on_edited_variableNameEmptyExpression);


    //move somewhere else?
    pgenie->workingFolder = workingFolder;

    //set initial values
    parse->typeIDExpression = swindow->typeIDExpression;
    parse->variableNameExpression = swindow->variableNameExpression;
    parse->variableNameEmptyExpression = swindow->variableNameEmptyExpression;
    parse->autoIDExpression =   swindow->autoIDExpression;
    parse->ResolvePostContent = swindow->ResolvePostContent;
    parse->CollapseAttributesIntoArray =  swindow->CollapseAttributesIntoArray;
    parse->IgnoreBreakTags =  swindow->IgnoreBreakTags;
    parse->IgnoreClosingTags = swindow->IgnoreClosingTags;
    parse->ExtractStyle = true;
    parse->GenerateIDTags = swindow->GenerateIDTags;

    pgenie->GenerateIDTags = swindow->GenerateIDTags;
    pgenie->typeIDExpression = swindow->typeIDExpression;
    pgenie->variableNameExpression = swindow->variableNameExpression;
    pgenie->variableNameEmptyExpression = swindow->variableNameEmptyExpression;
    pgenie->autoIDExpression =   swindow->autoIDExpression;

    pgenie->WrapInClass = swindow->WrapInClass;
    pgenie->className = swindow->className;

    pgenie->CollapseAttributesIntoArray = swindow->CollapseAttributesIntoArray;
    pgenie->ExtractStyle = true;
    pgenie->GenerateIDTags =swindow->GenerateIDTags;

    pgenie->IgnoreBreakTags =  swindow->IgnoreBreakTags;
    pgenie->IgnoreClosingTags = swindow->IgnoreClosingTags;


    mainTextMap.insert("HTML",ui->htmlTextEdit);
    mainTextMap.insert("Class",ui->classTextEdit);
    mainTextMap.insert("Generated",ui->generatedTextEdit);
    mainTextMap.insert("CSS",ui->CSSTextEdit);

    //TODO write GUI unit test and add more tests
    Unit_Test t = Unit_Test();

    //connect(ui->treeWidget->tree,&QTreeWidget::itemClicked,this,&MainWindow::itemClicked);

    //parser menu functions
    connect(ui->actionParse_HTML,&QAction::triggered,this,&MainWindow::ParseHTML);
    ui->actionParse_HTML->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));

    //generator menu actions
    connect(ui->actionGenerate_PHP,&QAction::triggered,this,&MainWindow::GeneratePHP);
    ui->actionGenerate_PHP->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_P));

    connect(ui->actionGenerate_HTML,&QAction::triggered,this,&MainWindow::GenerateHTML);
    ui->actionGenerate_HTML->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_G));

    //file menu actions
    connect(ui->actionSave_all,&QAction::triggered,this,&MainWindow::SaveAll);
    ui->actionSave_all->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    //
    connect(ui->actionOpen_Existing_Class,&QAction::triggered,this,&MainWindow::FindExistingClassFiles);
    ui->actionOpen_Existing_Class->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));

    connect(ui->actionSet_Working_Directory,&QAction::triggered,this,&MainWindow::SetWorkingDirectory);
    ui->actionSet_Working_Directory->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));
    //window
    connect(ui->actionOpen_HTML_in_New_Window,&QAction::triggered,this,&MainWindow::OpenHTMLWindow);

    connect(ui->actionOpen_Class_in_New_Window,&QAction::triggered,this,&MainWindow::OpenClassWindow);
    connect(ui->actionOpen_Generated_in_New_Window,&QAction::triggered,this,&MainWindow::OpenGeneratedWindow);
    connect(ui->actionOpen_CSS_in_New_Window,&QAction::triggered,this,&MainWindow::OpenCSSWindow);

    //regenerate tree
    connect(ui->actionGenerate_Tree,&QAction::triggered,this,&MainWindow::BuildMainElementTree);
    ui->actionGenerate_Tree->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));

    //settings
    connect(ui->actionOpen_Settings,&QAction::triggered,swindow,&SettingsWindow::show);

    //tree vieww
    connect(ui->actionOpen_Tree_View_in_New_Window,&QAction::triggered,this,&MainWindow::OpenTreeViewWindow);

}

void MainWindow::LoadSettings()
{
    //load settings
    QFile file("settings.dat");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);    // read the data serialized from the file
    QString wd;
    in >> wd;
    workingDir = wd;

    //gets the working folder from the working directory
    QRegExp rx("(\\/)"); //RegEx for ' ' or ',' or '.' or ':' or '\t'
    QStringList query = wd.split(rx);
    //the folder is scraped off the end of the directory
    workingFolder = query[query.size()-1];
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTreeItemTextEdit(QString str, QTreeLineEdit *caller, QTreeWidgetItem *owner)
{
    QString t = "";
    Element* e = nullptr;

    //find 'Element* e' from 'QTreeWidgetItem* owner' using 'emap'
    if(emap.find(owner)!= emap.end())
    {
        e = emap[owner];
        t = emap[owner]->tag;
    } else return;



    QString column = "";

    //lookup the current 'column' using 'QTreeLineEdit* caller' as the key for lmap, the value is a pair, second value (int) is the column index
    if(lmap.find(caller) != lmap.end())
        column = labels[lmap[caller].second];

    //pass 'str' using the 'column' as the name to the 'element'
    e->set(column,str);

    //generate live changes from the htmltreeview
    if(ui->treeWidget->liveEditCheckBox->isChecked())
        if(parsePass)
            GenerateHTML();



}



void MainWindow::SetWorkingDirectory()
{
    QFileDialog* fd = new QFileDialog(this);
    fd->setFileMode(QFileDialog::Directory);
    QString dir = fd->getExistingDirectory(this,
                                           tr("Choose Directory"), "C:/");



    QRegExp rx("(\\/)"); //RegEx for ' ' or ',' or '.' or ':' or '\t'
    QStringList query = dir.split(rx);
    workingFolder = query[query.size()-1];
    pgenie->workingFolder = workingFolder;

    //save settings
    QFile file("settings.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);   // we will serialize the data into the file
    out << QString(dir);   // working directory
}

void MainWindow::FindExistingClassFiles()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image"),workingDir+"/Classes/", tr("PHP Files (*.php)"));
    //gets actual name
    QRegExp rx("(\\/)"); //RegEx for ' ' or ',' or '.' or ':' or '\t'
    QStringList query = fileName.split(rx);
    QString file = query[query.size()-1];
    className = file;

    QString sourceFile = workingDir + "/HTMLSource/" + file;
    QString styleFile = workingDir + "/Styles/" + file;
    QString generatedFile = workingDir + "/Generated/" + file;

    sourceFile = sourceFile.replace(".php","-source.php");
    styleFile = styleFile.replace(".php","-style.php");
    generatedFile = generatedFile.replace(".php","-generated.php");


    QFile phpClass;
    phpClass.setFileName(fileName);
    phpClass.open(QIODevice::ReadWrite);

    QTextStream phpin(&phpClass);
    mainTextMap["Class"]->setPlainText(phpin.readAll());
    phpClass.close();


    QFile phpGen;
    phpClass.setFileName(generatedFile);
    phpClass.open(QIODevice::ReadWrite);

    QTextStream phpGenin(&phpGen);
    mainTextMap["Generated"]->setPlainText(phpin.readAll());
    phpClass.close();


    QFile CSS;
    CSS.setFileName(styleFile);
    CSS.open(QIODevice::ReadWrite);
    QTextStream cssin(&CSS);
    mainTextMap["CSS"]->setPlainText(cssin.readAll());
    CSS.close();

    // if(ui->createHTMLCheckBox->isChecked())
    if(true)
    {
        QFile html;
        html.setFileName(sourceFile);
        html.open(QIODevice::ReadWrite);

        QTextStream htmlin(&html);
        mainTextMap["HTML"]->setPlainText(htmlin.readAll());
        html.close();

    }


}

void MainWindow::SaveAll()
{

    QString sourceFile = workingDir + "/HTMLSource/" + className;
    QString styleFile = workingDir + "/Styles/"  + className;
    QString generatedFile = workingDir + "/Generated/"+ className;
    QString classFile = workingDir + "/Classes/"  + className;

    sourceFile = sourceFile.replace(".php","-source.php");
    styleFile = styleFile.replace(".php","-style.php");
    generatedFile = generatedFile.replace(".php","-generated.php");

    QFile phpClass;
    phpClass.setFileName(classFile);
    phpClass.open(QIODevice::ReadWrite);
    phpClass.resize(0);

    QString code = mainTextMap["Class"]->toPlainText();
    phpClass.write(code.toStdString().c_str(),code.length());
    phpClass.close();


    //
    QFile phpGen;
    phpGen.setFileName(generatedFile);
    phpGen.open(QIODevice::ReadWrite);
    phpGen.resize(0);

    code = mainTextMap["Generated"]->toPlainText();
    phpGen.write(code.toStdString().c_str(),code.length());
    phpGen.close();
    //

    QFile CSS;
    CSS.setFileName(styleFile);
    CSS.open(QIODevice::ReadWrite);
    CSS.resize(0);

    code = mainTextMap["CSS"]->toPlainText();
    CSS.write(code.toStdString().c_str(),code.length());
    CSS.close();

    // if(ui->createHTMLCheckBox->isChecked())
    if(true)
    {
        QFile html;
        html.setFileName(sourceFile);
        html.open(QIODevice::ReadWrite);
        html.resize(0);

        code = mainTextMap["HTML"]->toPlainText();
        html.write(code.toStdString().c_str(),code.length());
        html.close();

    }


}

void MainWindow::OpenHTMLWindow()
{
    OpenCodeWindow("HTML");
}

void MainWindow::OpenClassWindow()
{
    OpenCodeWindow("Class");
}

void MainWindow::OpenGeneratedWindow()
{
    OpenCodeWindow("Generated");
}

void MainWindow::OpenCSSWindow()
{
    OpenCodeWindow("CSS");
}

void MainWindow::OpenTreeViewWindow()
{
    disconnect(ui->treeWidget->liveEditCheckBox,&QCheckBox::toggled,twindow,&TreeViewWindow::toggleliveEditCheckBox);

    connect(twindow,&TreeViewWindow::toggle_liveEditCheckBox,this,&MainWindow::toggle_liveEditCheckBox);


    twindow->show();

    twindow->centralWidget()->layout()->addWidget(ui->treeWidget);
    //  BuildElementTree(twindow->tree);


    //remove tab

    for(int i = 0; i < ui->tabWidget_2->count(); ++i)
    {
        if("Tree View" == ui->tabWidget_2->tabText(i))
        {


            QWidget* w = ui->tabWidget_2->widget(i);

            ui->tabWidget_2->removeTab(i);

            tabMap.insert("Tree View",w);
        }
    }

}

void MainWindow::CloseTreeViewWindow()
{

    disconnect(twindow,&TreeViewWindow::toggle_liveEditCheckBox,this,&MainWindow::toggle_liveEditCheckBox);

    connect(ui->treeWidget->liveEditCheckBox,&QCheckBox::toggled,twindow,&TreeViewWindow::toggleliveEditCheckBox);

    if(tabMap.find("Tree View") != tabMap.end())
        ui->tabWidget_2->insertTab(0,tabMap["Tree View"],"Tree View");

    //   BuildElementTree(ui->treeWidget->tree);

    ui->tab->layout()->addWidget(ui->treeWidget);
    // twindow->centralWidget()->layout()->addWidget(ui->treeWidget);



}

void MainWindow::OpenCodeWindow(QString window)
{
    for(int i = 0; i < ui->tabWidget_2->count(); ++i)
    {
        if(window == ui->tabWidget_2->tabText(i))
        {
            CodeWindow* c = new CodeWindow(this);
            //move textEdit to the newly created window
            c->centralWidget()->layout()->addWidget(mainTextMap[window]);
            c->BindShortcutKeys(this);
            c->setWindowTitle(window);
            c->show();

            //get widget
            QWidget* w = ui->tabWidget_2->widget(i);
            //add to map
            tabMap.insert(window,w);
            //remove tab
            ui->tabWidget_2->removeTab(i);

            //connection allows for the tab to be added back after the window is closed
            connect(c,&CodeWindow::CloseWindow,this,&MainWindow::CloseCodeWindow);


        }
    }
}

void MainWindow::CloseCodeWindow(QString window, QString text)
{
    //insert tab widget from tabMap using window as the key
    if(tabMap.find(window) != tabMap.end())
        ui->tabWidget_2->insertTab(0,tabMap[window],window);

    //add textEdit back to the tab
    tabMap[window]->layout()->addWidget(mainTextMap[window]);

}

void MainWindow::ParseHTML()
{
    pgenie->className = className;
    pgenie->className.replace(".php","");

    pgenie->elementTypeCounter.clear();

    parse->ParseHTML(  mainTextMap["HTML"]->toPlainText() );

    parsePass = parse->resolveElements();
    if(parsePass)
    {

        BuildElementTree(ui->treeWidget->tree);

        ui->htmlStatusLabel->setText("HTML: <span style=\"color:green;\">parsed</span>");
    } else
    {
        ui->htmlStatusLabel->setText("HTML: <span style=\"color:red;\">error, unable to parse, check syntax!</span>");

    }


}

void MainWindow::GenerateHTML()
{
    if(parsePass)
    {
        for(Element& e : parse->elements)
        {
            e.index_c = 0;
            e.closed = false;
        }

        HTMLGenerator htmlgen;
        int hvalue =  mainTextMap["HTML"]->verticalScrollBar()->value();

        mainTextMap["HTML"]->setPlainText(htmlgen.generateHTML(& parse->elements[0]));

        mainTextMap["HTML"]->verticalScrollBar()->setValue(hvalue);

        SetPHPStatus(1);
    }

}

void MainWindow::GeneratePHP()
{
    if(parsePass)
    {
        pgenie->elementTypeCounter.clear();

        pgenie->evaluateTypeID(parse->elements, swindow->typeIDExpression) ;
        pgenie->generatePHPElementCode( parse->elements);

        if( parse->ExtractStyle)
            mainTextMap["CSS"]->setPlainText( parse->extractStyle());


        QStringList code = pgenie->generateCodes( parse->elements);

        if( mainTextMap["Class"]->toPlainText().isEmpty())
            mainTextMap["Class"]->setPlainText(code[1]);


        mainTextMap["Generated"]->setPlainText(code[0]);

        ui->treeWidget->tree->clear();
        BuildElementTree(ui->treeWidget->tree);


        SetPHPStatus(2);

    }

}

void MainWindow::SetPHPStatus(int i)
{
    switch(i)
    {
    case 0:
        ui->phpStatusLabel->setText("PHP: <span style=\"color:red;\">not generated</span>");
        break;
    case 1:

        ui->phpStatusLabel->setText("PHP: <span style=\"color:red;\">generated, but not current</span>");
        break;
    case 2:
        ui->phpStatusLabel->setText("PHP: <span style=\"color:green;\">generated</span>");
        break;



    }

}


void MainWindow::on_typeIDedit_textChanged(const QString &arg1)
{
    pgenie->typeIDExpression = arg1;
}


void MainWindow::on_variableNameEdit_textChanged(const QString &arg1)
{
    pgenie->variableNameExpression = arg1;
}


void MainWindow::on_variableNameEdit_2_textChanged(const QString &arg1)
{
    pgenie->variableNameEmptyExpression = arg1;
}


void MainWindow::on_autoIDEdit_textChanged(const QString &arg1)
{
    pgenie->autoIDExpression = arg1;
}


void MainWindow::on_wrapInClassCheckBox_toggled(bool checked)
{
    pgenie->WrapInClass =  checked;

}

void MainWindow::toggle_liveEditCheckBox()
{
    ui->treeWidget->liveEditCheckBox->setChecked(!ui->treeWidget->liveEditCheckBox->isChecked());

}

QStringList MainWindow::TokenizeExpression(QString expr)
{

    QStringList exprs;

    int MODE = 0;

    //0 outside of tag
    //1 inside tag
    //2 end of tag
    const int OUTSIDE_OF_TAG = 0;
    const int INSIDE_OF_TAG = 1;
    const int END_OF_TAG = 2;

    QString current  = "";

    QVector<QString> stack;

    //parse for <element>'content'</element>
    //content is assiocated with the last element parsed
    foreach(auto curr_char , expr)
    {


        if(curr_char == '%' && MODE != INSIDE_OF_TAG)
        {
            MODE = INSIDE_OF_TAG;





            current = "";


        }
        else if(curr_char == '%' && MODE == INSIDE_OF_TAG)
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
            exprs.append(current);
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



    return exprs;
}

QStringList MainWindow::TokenizeExpressionAsStr(QString expr)
{
    QStringList exprs= TokenizeExpression(expr);

    for(QString& str : exprs)
    {
        str = str.replace("%","");
        str[0] = str[0].toUpper();

    }
    return exprs;

}

void MainWindow::BuildElementTree(QTreeWidget* tree)
{
    if(parsePass)
    {
        tree->clear();
        tmap.clear();
        lmap.clear();
        emap.clear();
        QString Expr = ui->treeWidget->treeColExpr->text();
        if(Expr.isEmpty())
            Expr = "%id%%content%%class%%type%%action%%method%%value%%href%";


        QTreeWidgetItem* first = new QTreeWidgetItem(tree);
        first->setExpanded(true);

        tree->insertTopLevelItem(0,first);

        int i = 0;
        QMap<Element*, QTreeWidgetItem*> treemap = {{& parse->elements[0],first}};
        QStringList exprs = TokenizeExpression(Expr);

        labels.clear();
        labels = TokenizeExpressionAsStr(Expr);
        labels.insert(0,"Tag");



        tree->setColumnCount(labels.count());
        tree->setHeaderLabels(labels);


        values.clear();
        values.append(parse->elements[0].tag);

        for(QString str: exprs)
        {
            values.append(pgenie->evalString(parse->elements[0],str));
        }
        for(int ix = 0; ix < tree->columnCount(); ++ix)
        {
            QTreeLineEdit* le = new QTreeLineEdit(tree);
            le->itemOwner = first;
            tree->setItemWidget(first,ix,le);
            le->setStyleSheet("border: none;border-bottom: 1px dashed black;border-right: 1px dashed black;");
            if(ix < values.size())
                le->setText(values[ix]);

            QPair<QTreeWidgetItem*,int> p = {first,ix};

            tmap.insert(p,le);
            lmap.insert(le,p);
            emap.insert(first,&parse->elements[0]);
            connect(le,&QTreeLineEdit::textEdited,this,&MainWindow::onTreeItemTextEdit);


        }

        for(Element& e: parse->elements)
        {
            if(i == 0)
            {
                ++i;
                continue;
            }


            if(e.tag.contains("/") || e.tag.contains("style"))
            {
                ++i;
                continue;

            }

            QTreeWidgetItem* parent = nullptr;
            if(treemap.find(e.parent) != treemap.end())
            {
                parent = treemap[e.parent];

            }

            if(parent != nullptr)
            {

                values.clear();
                values.append(e.tag);

                for(QString str: exprs)
                {
                    values.append(pgenie->evalString(e,str));
                }
                QTreeWidgetItem* current  = new QTreeWidgetItem(parent,values);

                for(int ix = 0; ix < tree->columnCount(); ++ix)
                {
                    QTreeLineEdit* le = new QTreeLineEdit(tree);
                    le->itemOwner = current;
                    tree->setItemWidget(current,ix,le);
                    le->setStyleSheet("border: none;border-bottom: 1px dashed black;border-right: 1px dashed black;");
                    if(ix < values.size())
                        le->setText(values[ix]);

                    QPair<QTreeWidgetItem*,int> p = {current,ix};

                    tmap.insert(p,le);
                    lmap.insert(le,p);
                    emap.insert(current,&e);
                    connect(le,&QTreeLineEdit::textEdited,this,&MainWindow::onTreeItemTextEdit);


                }

                current->setExpanded(true);


                treemap.insert(&e,current);



            }
            else { qDebug() << "NULL";}
            ++i;

        }

        for(int ix = 0; ix < tree->columnCount(); ++ix)
        {
            ui->treeWidget->tree->resizeColumnToContents(ix);

        }




    }
}

void MainWindow::BuildMainElementTree()
{
    BuildElementTree(ui->treeWidget->tree);
}


void MainWindow::on_liveEditCheckBox_toggled(bool checked)
{
    if(checked)
        GenerateHTML();

    qDebug() << "live edit toggled";
}

