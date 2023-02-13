#ifndef TWOPHASEBST_H
#define TWOPHASEBST_H

#include <iostream>
#include <string>
#include <stack>
#include <list>
// do not add any other library.
// modify parts as directed by assignment text and comments here.
/* */

// find_2 ye bak, print'i bitir.
using namespace std;
/* */
template <class T>
class TwoPhaseBST {
private: //do not change
    struct SecondaryNode {
        std::string key;
        T data;
        SecondaryNode *left;
        SecondaryNode *right;

        SecondaryNode(const std::string &k, const T &d, SecondaryNode *l, SecondaryNode *r);
    };

    struct PrimaryNode {
        std::string key;
        PrimaryNode *left;
        PrimaryNode *right;
        SecondaryNode *rootSecondaryNode;

        PrimaryNode(const std::string &k, PrimaryNode *l, PrimaryNode *r, SecondaryNode *rsn);
    };

public: // do not change.
    TwoPhaseBST();
    ~TwoPhaseBST();

    TwoPhaseBST &insert(const std::string &primaryKey, const std::string &secondaryKey, const T &data);
    TwoPhaseBST &remove(const std::string &primaryKey, const std::string &secondaryKey);
    TwoPhaseBST &print(const std::string &primaryKey = "", const std::string &secondaryKey = "");
    T *find(const std::string &primaryKey, const std::string &secondaryKey);

private: // you may add your own utility member functions here.
    void destructNode(PrimaryNode * &root);
    void destructNode(SecondaryNode * &root);
    bool find_if(const std::string &primaryKey);
    void secondaryfind(const std::string &primaryKey, const std::string &secondaryKey = "");


private: // do not change.
    PrimaryNode *root; //designated root.

    // do not provide an implementation. TwoPhaseBST's are not copiable.
    TwoPhaseBST(const TwoPhaseBST &);
    const TwoPhaseBST &operator=(const TwoPhaseBST &);
};

template <class T>
TwoPhaseBST<T>::SecondaryNode::SecondaryNode(const std::string &k, const T &d, SecondaryNode *l, SecondaryNode *r)
        : key(k), data(d), left(l), right(r) {}

template <class T>
TwoPhaseBST<T>::PrimaryNode::PrimaryNode(const std::string &k, PrimaryNode *l, PrimaryNode *r, SecondaryNode *rsn)
        : key(k), left(l), right(r), rootSecondaryNode(rsn) {}

template <class T>
TwoPhaseBST<T>::TwoPhaseBST() : root(NULL) {}

template <class T>
TwoPhaseBST<T>::~TwoPhaseBST() {
    destructNode(root);
}

template <class T>
TwoPhaseBST<T> &
TwoPhaseBST<T>::insert(const std::string &primaryKey, const std::string &secondaryKey, const T &data) {


  PrimaryNode * curser = root;
  PrimaryNode * prev ;
  SecondaryNode * prev_2;
  SecondaryNode* curser_2;
  if (!curser) {
    PrimaryNode * node = new PrimaryNode(primaryKey,NULL,NULL,NULL);

    root = node;
    curser = root;
  }
  while (curser) {
    if (primaryKey > curser->key) {
      prev = curser;
      curser = curser->right;
    }
    else if (primaryKey < curser->key) {
      prev = curser;
      curser = curser->left;
    }
    else {
      break;
    }
  }
  if (curser == NULL) {
    struct PrimaryNode *node = new PrimaryNode(primaryKey,NULL,NULL,NULL);
    curser = node;
    if(prev->key < primaryKey) {
      prev->right = node;
    }
    else {
      prev->left = node;
    }
  }
  curser_2 = curser->rootSecondaryNode;
  prev_2 = curser->rootSecondaryNode;
  if(!curser_2){
    struct SecondaryNode* node_2 = new SecondaryNode(secondaryKey,data,NULL,NULL);
    curser->rootSecondaryNode = node_2;
    return *this;
  }
  while (curser_2) {
    if (secondaryKey > curser_2->key) {
      prev_2 = curser_2;
      curser_2 = curser_2->right;
    }
    else if (secondaryKey < curser_2->key) {
      prev_2 = curser_2;
      curser_2 = curser_2->left;
    }
    else {
      break;
    }
  }
    if(curser_2 == NULL) {
      struct SecondaryNode *node_2 = new SecondaryNode(secondaryKey,data,NULL,NULL);
      if(prev_2->key > secondaryKey) {
        prev_2->left = node_2;
        return *this;
      }
      else {
        prev_2->right = node_2;
        return *this;
      }
    }
    return *this;
    /* IMPLEMENT THIS */

}



template <class T>
TwoPhaseBST<T> &
TwoPhaseBST<T>::remove(const std::string &primaryKey, const std::string &secondaryKey) {
  PrimaryNode * p_node = root;
  SecondaryNode * s_node;
  SecondaryNode * parent;
  SecondaryNode * min;
  SecondaryNode * parent_2;
  std::string s_key ;
  while(p_node) {
    if (primaryKey > p_node->key) {
      p_node = p_node->right;
    }
    else if (primaryKey < p_node->key) {
      p_node = p_node->left;
    }
    else {
      break;
    }
  }
  if (p_node == NULL) {
    return *this;
  }
  s_node = p_node->rootSecondaryNode;
  parent = s_node;
  while (s_node) {
    if (secondaryKey > s_node->key) {
      parent = s_node;
      s_node = s_node->right;
    }
    else if (secondaryKey < s_node->key) {
      parent = s_node;
      s_node = s_node->left;
    }
    else {
      break;
    }
  }
  if ( s_node == NULL) {
      return *this;
    }
  if(s_node->left == NULL && s_node->right == NULL) {
    if (s_node->key == p_node->rootSecondaryNode->key) {
      p_node->rootSecondaryNode = NULL;
      delete s_node;
      return *this;
    }
    else if(parent->left == s_node) {
      parent->left = NULL;
      delete s_node;
      return *this;
    }
    else {
      parent->right = NULL;
      delete s_node;
      return *this;
    }
  }

  else if (s_node->left == NULL && s_node->right != NULL) {
    if (s_node->key == p_node->rootSecondaryNode->key) {
      p_node->rootSecondaryNode = s_node->right;
      delete s_node;
      return *this;
    }
    else if (parent->left == s_node) {
      parent->left = s_node->right;
      delete s_node;
      return *this;
    }
    else {
      parent->right = s_node->right;
      delete s_node;
      return *this;
    }
  }

  else if (s_node->left != NULL && s_node->right == NULL) {
    if (s_node->key == p_node->rootSecondaryNode->key) {
      p_node->rootSecondaryNode = s_node->left;
      delete s_node;
      return *this;
    }
    else if (parent->left == s_node) {
      parent->left = s_node->left;
      delete s_node;
      return *this;
    }
    else {
      parent->right = s_node->left;
      delete s_node;
      return *this;
    }
  }

  else {
    min = s_node->right;
    parent_2 = s_node;
    while (min->left) {
      parent_2 = min;
      min = min->left;
    }
    if (parent->left == s_node) {
      if (s_node->key != p_node->rootSecondaryNode->key) {parent->left = min;}
      else { p_node->rootSecondaryNode = min;}
      if (parent_2 == s_node) {
        min->right = s_node->right;
      }
      else {

      parent_2->left = min->right;
      min->left = s_node->left;
      min->right = s_node->right;

    }
      if (min->left == min) {
        min->left = NULL;
      }
      if (min->right == min) {
        min->right = NULL;
      }
      delete s_node;
      return *this;
    }
    else {
      if (s_node->key != p_node->rootSecondaryNode->key) {parent->right = min; }
      else { p_node->rootSecondaryNode = min;}
      if (s_node == parent_2) {
        min->left = s_node->left;
      }
      else {
        parent_2->left = min->right;
        min->left = s_node->left;
        min->right = s_node->right;
      }
      if (min->left == min) {
        min->left = NULL;
      }
      if (min->right == min) {
        min->right = NULL;
      }
      delete s_node;
      return *this;
    }
  }

  return *this;
    /* IMPLEMENT THIS */
}

template <class T>
bool TwoPhaseBST<T>::find_if(const std::string &primaryKey) {
  PrimaryNode * curser = root;
  while (curser) {
    if (primaryKey > curser->key) {
      curser = curser->right;
    }
    else if (primaryKey < curser->key) {
      curser = curser->left;
    }
    else {
      break;
    }
  }
  if (curser == NULL) {return 0;}
  else {return 1;}

}

template <class T>
void TwoPhaseBST<T>::secondaryfind(const std::string &primaryKey,const std::string &secondaryKey) {
    PrimaryNode * curser = root;
    SecondaryNode * node;
    stack<SecondaryNode*> stacks;
    while (curser) {
      if (primaryKey > curser->key) {
        curser = curser->right;
      }
      else if (primaryKey < curser->key) {
        curser = curser->left;
      }
      else {
        break;
      }
    }
    node = curser->rootSecondaryNode;
    if (node == NULL ) {std::cout << "}" ; return;}
    while(node || stacks.empty() == 0) {
      while(node) {
        stacks.push(node);
        node = node->left;
      }
      node = stacks.top();
      stacks.pop();
      std::cout << "\"" << node->key << "\""<< " : " << "\""<< node->data << "\"";
      if (!stacks.empty() || node->right) {std::cout << ", "; }
      node = node->right;
    }
    std::cout << "}";
    return ;

}

template <class T>
TwoPhaseBST<T> &TwoPhaseBST<T>::print(const std::string &primaryKey, const std::string &secondaryKey) {
    PrimaryNode * node = root;
    stack<PrimaryNode*> stacks;
    if(primaryKey == "" && secondaryKey == "" ) {
      if(root == NULL) {
        std::cout << "{}"<<std::endl;
        return *this;
      }
      std::cout << "{" ;
      while(node || stacks.empty() == 0) {
        while(node) {
          stacks.push(node);
          node = node->left;
        }
        node = stacks.top();
        stacks.pop();
        std::cout << "\"" << node->key << "\""<< " : " << "{";
        secondaryfind(node->key,node->rootSecondaryNode->key);
        if (!stacks.empty() || node->right) {std::cout << ", "; }
        node = node->right;
        }
        std::cout << "}" << std::endl;
        return *this;
      }

    else if (find_if(primaryKey) == 0) {
      std::cout << "{}"<<std::endl;
      return *this;
    }

    else if(secondaryKey == "") {
      std::cout << "{" <<"\""<< primaryKey << "\"" << " : "<< "{" ;
      secondaryfind(primaryKey,secondaryKey);
      std::cout << "}" << std::endl;
      return *this;
    }

    else if (find(primaryKey,secondaryKey) != NULL) {
      std::cout << "{" << "\"" << primaryKey << "\"" << " : " << "{" << "\"" << secondaryKey << "\"" << " : " << "\"" << *(find(primaryKey,secondaryKey)) << "\"" << "}}" << std::endl;
      return *this;
    }
    else if (find(primaryKey,secondaryKey) == NULL) {
      std::cout << "{}" << std::endl;
    }

    return *this;
}



template <class T>
T *TwoPhaseBST<T>::find(const std::string &primaryKey, const std::string &secondaryKey) {
  PrimaryNode * curser = root;
  SecondaryNode* curser_2;
  while (curser) {
    if (primaryKey > curser->key) {
      curser = curser->right;
    }
    else if (primaryKey < curser->key) {
      curser = curser->left;
    }
    else {
      break;
    }
  }
  if (curser == NULL) {return NULL;}
  curser_2 = curser->rootSecondaryNode;
  while (curser_2) {
    if (secondaryKey > curser_2->key) {
      curser_2 = curser_2->right;
    }
    else if (secondaryKey < curser_2->key) {
      curser_2 = curser_2->left;
    }
    else {
      break;
    }
  }
  if(curser_2 ==NULL) {return NULL;}
  return &(curser_2->data);

    /* IMPLEMENT THIS */
}

template <class T>
void TwoPhaseBST<T>::destructNode(TwoPhaseBST::PrimaryNode * &root)
{
    if (root == NULL)
        return;

    destructNode(root->left);
    destructNode(root->right);

    destructNode(root->rootSecondaryNode);

    delete root;

    root = NULL;
}

template <class T>
void TwoPhaseBST<T>::destructNode(TwoPhaseBST::SecondaryNode * &root)
{
    if (root == NULL)
        return;

    destructNode(root->left);
    destructNode(root->right);

    delete root;

    root = NULL;
}

#endif //TWOPHASEBST_H
