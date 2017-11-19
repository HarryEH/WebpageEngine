#include <Arduino.h>// Required in .cxx
#include "WebpageEngine.h"

////////////////////////////////////////////////////////////////////////////////
////////////////// Methods defined for the class LinkedList ////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Get head of the linked list
node* LinkedList::getHead()
{
  return head;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// number of nodes in the list
int LinkedList::count()
{
  int counter = 0;
  node *temp = head;
  while(temp)
  {
    temp = temp->next;
    counter++;
  }
  return counter;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// This method creates a node and appends it on the linked list
void LinkedList::addNode(Element value)
{
  node *temp = new node;
  temp->data = value;
  temp->next = NULL;
  if(head == NULL)
  {
    head = temp;
    tail = temp;
    temp = NULL;
  }
  else
  {
    tail->next = temp;
    tail = temp;
  }
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Method to delete the last Element in a
void LinkedList::deleteLast()
{
  node *current  = new node;
  node *previous = new node;
  current        = head;

  while(current->next!=NULL)
  {
    previous=current;
    current=current->next;
  }

  tail=previous;
  previous->next=NULL;
  delete current;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Method to delete en element in the list
void LinkedList::removeElement(String tag, String content, int attribNum, int childNum)
{
  node *tmp = getHead();// account for the case where we delete the head
  if ( (tmp->data).tag.equals(tag)
    && (tmp->data).content.equals(content)
    && (tmp->data).attribNum == attribNum
    && (tmp->data).childNum == childNum )
  {
    node *vTmp = NULL;
    if(tmp->next != NULL)
    {
       vTmp = tmp->next;// gets the
    } else
    {
      tail == NULL;
    }
    delete tmp;
    this->head = vTmp;// set to either null or the element after the one we deleted
  } else { // it wasn't the head!
    while(tmp->next != NULL)
    {
      if ( (tmp->next->data).tag.equals(tag)
        && (tmp->next->data).content.equals(content)
        && (tmp->next->data).attribNum == attribNum
        && (tmp->next->data).childNum  == childNum )
      {
        node *vTmp = NULL;
        if(tmp->next->next != NULL)
        {
           vTmp = tmp->next->next;// gets the next next :o
        } else
        {
          tail = tmp;// if the next next doesn't exist then next is the tail
        }
        delete tmp->next;
        tmp->next = vTmp;// set to either null or the element after the one we deleted
      }
      tmp = tmp->next;
    }
  }
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///////////////////// Methods defined for the class WebPage/////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Method for inserting an HTML Element into either the body or head
void WebPage::insertHtmlElement(Section sec, Element e)
{
  switch (sec)
  {
    case HEAD:
      head.addNode(e);
      break;
    case BODY:
      body.addNode(e);
      break;
  }
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Method implementation for inserting CSS
void WebPage::insertCss(String tag, Attribute *Attributes, int aLen)
{

  // Construct content string
  String content = tag; // Sorry for using string - time constraints
  content.concat(" { ");
  for (int i = 0; i < aLen; i++) {
    content.concat(Attributes[i].name);
    content.concat(" : ");
    content.concat(Attributes[i].value);
    content.concat("; ");
  }

  Element e;
  e.tag       = "style"; // ALWAYS STYLE BECAUSE IT IS CSS
  e.attribNum = 0;
  e.childNum  = 0;
  e.content   = content.c_str();

  insertHtmlElement(HEAD, e);
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Method for converting the linked lists into one page string
String WebPage::renderPage()
{
  // Sorry for using string - time constraints
  String page = "<html>\n"; // Start the HTML page

  const char *endline = "\n";

  node *headTemp = head.getHead();// Get head of the linked list for head
  page.concat("<head>\n"); // Start of head

  page.concat("<title>");
  page.concat(title);
  page.concat("</title>\n");

  while(headTemp != NULL)
  {
    page.concat(htmlElementToString(&(headTemp->data)));
    page.concat(endline);
    headTemp = headTemp -> next;
  }
  page.concat("</head>\n"); // End of head

  node *bodyTemp = body.getHead();// Get head of the linked list for body
  page.concat("<body>\n"); // Start of body

  while(bodyTemp != NULL)
  {
    page.concat(htmlElementToString(&(bodyTemp->data)));
    page.concat(endline);
    bodyTemp = bodyTemp -> next;
  }
  page.concat("</body>\n");// End of body

  page.concat("</html>");// End the HTML page

  return page;

}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Method to convert an Element struct to a string
String WebPage::htmlElementToString(Element *e)
{

  // Sorry for using string - time constraints
  String s = "<";
  s.concat(e->tag);

  for(int i = 0; i< e->attribNum; i++)
  {
    Attribute temp = e->attributes[i];
    s.concat(" ");
    s.concat(temp.name);
    s.concat("=\"");
    s.concat(temp.value);
    s.concat("\"");
  }

  s.concat(">");

  // Issue here is always doing content before the nested Elements and their content...
  s.concat(e->content);
  for(int i = 0; i < e->childNum; i++)
  {
    s.concat(htmlElementToString(&(e->children[i])));
    s.concat("\n");
  }
  s.concat("</");
  s.concat(e->tag);
  s.concat(">");
  s.concat("\n");

  return s;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Method to create an Attribute to give to an html or css Element
Attribute WebPage::attributeCreator(String name, String val)
{
  Attribute a;
  a.name = name;
  a.value = val;
  return a;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Method to create an html Element. This is a recursive data structure
Element WebPage::htmlElementCreator(String tag, Attribute *at, int atNum, String content, Element *child, int childNum)
{

  Element e;
  e.tag     = tag;
  e.attribNum   = atNum;
  if(atNum > 0)
  {
    e.attributes = at;
  }
  e.content  = content;
  e.childNum = childNum;
  if(childNum > 0)
  {
    e.children = child;
  }

  return e;
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Remove html element based on the tag
void WebPage::removeHtmlElement(Section sec, String tag, String content, int attribNum, int childNum)
{
  switch (sec)
  {
    case HEAD:
      head.removeElement(tag, content, attribNum, childNum);
      break;
    case BODY:
      body.removeElement(tag, content, attribNum, childNum);
      break;
  }
}
////////////////////////////////////////////////////////////////////////////////
