#ifndef Webpage_Engine_H
#define Webpage_Engine_H

enum Section {HEAD,BODY};

struct Attribute {
  String name;
  String value;
};

struct Element {
  String tag;
  Attribute *attributes; // for example href, class, src... the list goes on
  int attribNum; // to make implementation easier
  String content;
  Element *children;
  int childNum;
};

////////////////////////////////////////////////////////////////////////////////
// Linked List definition for elements
// Basic implementation of a singly linked list
struct node
{
  Element data; // Sorry that this isn't generic
  node *next;
};

class LinkedList
{
  private:
  node *head, *tail;
  public:

  LinkedList()
  {
    head = NULL;
    tail = NULL;
  }

  ~LinkedList()
  {
    // Clear nodes
    while(head->next != NULL)
    {
      deleteLast();
    }
    delete head;
    head = NULL;
    tail = NULL;
  }
  // Get head of the linked list
  node* getHead();
  // number of nodes in the list
  int count();
  // This method creates a node and appends it on the linked list
  void addNode(Element value);
  // Method to delete the last element in a
  void deleteLast();
  // Method to delete en element in the list
  void removeElement(String, String, int, int);

}; // End of the LinkedList class definition
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// WebPage class definition
class WebPage
{
  private:
    String title;
  public:
    LinkedList head, body; // These are public so you can manipulate
    // them how you please - ordering matters to some people apparently.

    WebPage(String pageTitle){title = pageTitle;}// Constructor

    void insertHtmlElement(Section, Element);
    void removeHtmlElement(Section, String, String, int, int);
    void insertCss(String , Attribute *, int);
    String renderPage();
    String htmlElementToString(Element*);
    static Attribute attributeCreator(String, String);
    static Element htmlElementCreator(String, Attribute *,int, String, Element *, int);

}; // End of the LinkedList class definition
////////////////////////////////////////////////////////////////////////////////

#endif
