#include "htmlgenerator.h"


HTMLGenerator::HTMLGenerator()
{




}

QString HTMLGenerator::generateAttributes(Element *e)
{
    QString ret = "";

  QMapIterator<QString, QString> i(e->attributes);

  while (i.hasNext()) {
      i.next();
      ret += i.key() + "=" + "'"+ i.value() +"' ";
  }
  return ret;
}

void HTMLGenerator::Push(Element *e)
{
 //   echo "Pushing : " . $tree_element->tag . "<br>";


    wstack.push_back(e);
QString str_header  = "";


if(e->tagOwner)
{
    for(int i = 0; i < e->tagOwner->parentLevel; i++)
        str_header += "     ";
} else
{
    for(int i = 0; i < e->parentLevel; i++)
        str_header += "     ";

}



rstack.push_back(str_header+'<' + e->tag  + " " + generateAttributes(e) + ">" + e->content + "\n");

}

void HTMLGenerator::End(Element *e)

{

  wstack.pop_back();

}

bool HTMLGenerator::StackStep(Element *e)
{

    qDebug() << e->tag;

int index_c =  e->index_c;

   if(e->children.size() != 0)
   {
       if(e->children.size() > 1)
       {
  //null


           if(e->children.size() > index_c)
           {

        //   if(!$tree_element->child[$index_c]->ignore) //skip ignored elements
               Push(e->children[index_c]);

                 e->index_c = index_c+1;

               return true;

           }else  //Onendorhasnochildren
           {



               End(e);

               e->index_c = index_c+1;

               return true;


           }

       }
       else
       {
               if(1 > index_c)
               {

       //    if(!$tree_element->child->ignore) //skip ignored elements
                   Push(e->children[index_c]);

                     e->index_c = index_c+1;



                   return true;
               }
               else  //Onendorhasnochildren
               {
                 //  echo 1 ." ". intval($tree_element->index_count) . "<br>";
                   End(e);

                   e->index_c = index_c+1;
                //   echo "<br>ret<br>";
                   return true;
               }

       }


   } else //Onendorhasnochildren
   {
    //  echo 1 ." ". intval($tree_element->index_count) . "<br>";
       End(e);

       e->index_c = index_c+1;
    //   echo "<br>ret<br>";
       return true;
   }

}

void HTMLGenerator::StackLoop()
{


 while(!wstack.isEmpty())
   {
    //   echo "Looping On : " . $wstack[(count($wstack))-1]->tag . "<br>";

 //      $ref =& $wstack[(count($wstack))-1]; //grab most recent element pushed to the stack
Element* e = wstack[wstack.size()-1];
   //    $this->StackStep($wstack,$rstack,$ref); //step with that element
StackStep(e);
   }

}

QString HTMLGenerator::generateHTML(Element *e)
{

    QString ret = "";

e->index_c = 0; //paranoid??? o.0

wstack = {e};
rstack = {"<" , e->tag  , " " , generateAttributes(e) , ">" , e->content, "\n"};

StackLoop();

for(QString str : rstack)
  ret += str;

return ret;

}
