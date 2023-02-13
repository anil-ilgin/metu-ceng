#ifndef LINKEDLIST_HPP
#define	LINKEDLIST_HPP

#include <iostream>
#include "Node.hpp"

using namespace std;

/*....DO NOT EDIT BELOW....*/
template <class T>
class LinkedList {
private:
    Node<T>* head;
    int length;
public:

    LinkedList();
    LinkedList(const LinkedList<T>& ll);
    LinkedList<T>& operator=(const LinkedList<T>& ll);
    ~LinkedList();


    Node<T>* getHead() const;
    Node<T>* first() const;
    Node<T>* findPrev(const T& data) const;
    Node<T>* findNode(const T& data) const;
    void insertNode(Node<T>* prev, const T& data);
    void deleteNode(Node<T>* prevNode);
    void clear();
    size_t getLength() const;
    void print() const;
    void swap(int index1, int index2);
};

template <class T>
void LinkedList<T>::print() const {
    const Node<T>* node = first();
    while (node) {
        std::cout << node->getData();
        node = node->getNext();
    }
    cout << std::endl;
}

/*....DO NOT EDIT ABOVE....*/

/* TO-DO: method implementations below */
template <class T>
LinkedList<T>::LinkedList() {
  head = new Node<T>();
  length = 0;
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& ll) {
  head = new Node<T>();
  *this = ll;
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& ll) {
  if (this != &ll){
        clear();
        const Node<T>* r = ll.first();
        Node<T>* p = getHead();

        while (r) {
            insertNode(p, r->getData());
            r = r->getNext();
            p = p->getNext();
        }
    }
    return *this;
}

template <class T>
LinkedList<T>::~LinkedList() {
  clear();
  delete head;
}
template <class T>
Node<T>* LinkedList<T>::getHead() const {
    return head;
  }

template <class T>
Node<T>* LinkedList<T>::first() const {
  return head->getNext();
}

template <class T>
Node<T>* LinkedList<T>::findPrev(const T& data) const {        /*NO_IDEA_IF_IT_WORKS*/
  Node<T>* dummy = getHead();
  while (dummy->getNext()) {
    if (data == dummy->getNext()->getData() ) { return dummy; }
    dummy = dummy->getNext();
  }
  return NULL;
}

template <class T>
Node<T>* LinkedList<T>::findNode(const T& data) const {
  Node<T>* dummy = first();
  while (dummy) {
    if (data == dummy->getData() ) { return dummy; }
    else { dummy = dummy->getNext(); }
  }
  return NULL;
}

template <class T>
void LinkedList<T>::insertNode(Node<T>* prev, const T& data) {
  if(prev == NULL) {
    return;
  }
  Node<T>* newNode = new Node<T>(data);
  if (prev->getNext() == NULL) {
    prev->setNext(newNode);
    return;
  }
  newNode->setNext(prev->getNext());
  prev->setNext(newNode);
  length++;
  return;
}

template <class T>
void LinkedList<T>::deleteNode(Node<T>* prevNode) {
  Node <T>* start = getHead();
  while (start != prevNode) {
    start = start->getNext();
    if (start == NULL) { return;}
  }
  Node<T>* temp_2 = start->getNext();
  Node<T>* temp_1 = temp_2->getNext();
  prevNode->setNext(temp_1);
  delete temp_2;
  length--;
}

template <class T>
void LinkedList<T>::clear() {
  Node<T>* start = getHead();
  while (start->getNext()) {
    deleteNode(start);
  }
  length = 0;
}

template <class T>
size_t LinkedList<T>::getLength() const {
  return length;
}

template <class T>
void LinkedList<T>::swap(int index1, int index2){
	Node<T>* t1 = head ;
	Node<T>* t2 = head ;
	Node<T>* d1 ;
	int i;
	for(i = 0; i<index1; i++){
		t1 = t1->getNext();
	}
	for(i = 0; i<index2; i++){
		t2 = t2->getNext();
	}
	d1 = t1->getNext();
	t1->setNext(t2->getNext());
	t2->setNext(d1);
	d1 = t1->getNext()->getNext();
	t1->getNext()->setNext(t2->getNext()->getNext());
	t2->getNext()->setNext(d1);
}
/*
template <class T>
void LinkedList<T>::swap(int index1, int index2) {

  if (index1 > index2) {
    int temp1 = index1;
    index1 = index2;
    index2 = temp1;
  }
  Node<T>* curser_1 = getHead();
  Node<T>* curser_2 = getHead();
  for (int i=0; i<index1; i++) {
    curser_1 = curser_1->getNext();
  }
  for (int j=0; j<index2; j++) {
    curser_2 = curser_2->getNext();
  }
  Node<T>* temp_1 = new Node<T>(curser_1->getNext()->getData());
  temp_1->setNext(curser_1->getNext()->getNext());
  Node<T>* temp_2 = new Node<T>(curser_2->getNext()->getData());
  temp_2->setNext(curser_2->getNext()->getNext());
  insertNode(curser_1,temp_2->getData());
  insertNode(curser_2,temp_1->getData());
  deleteNode(curser_2->getNext());
  deleteNode(curser_1->getNext());
  delete temp_1;
  delete temp_2;
}
*/



/* end of your implementations*/
#endif
