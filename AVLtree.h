
#ifndef AVLTREE_AVLTREE_H
#define AVLTREE_AVLTREE_H

#include <exception>
#include <cstdio>
#include "NodeMapTree.h"
#include "exceptions.h"

template<class T, class F>
class AVLtree {

    NodeMapTree<T, F> *root;
    int size;

    NodeMapTree<T, F> *removeLeaf(NodeMapTree<T, F> *nodeToRemove);

    NodeMapTree<T, F> *removeOneSon(NodeMapTree<T, F> *nodeToRemove);

    NodeMapTree<T, F> *
    removeNodeInTheMiddle(NodeMapTree<T, F> *nodeToRemove);

    void
    changeParentByNode(NodeMapTree<T, F> *source, NodeMapTree<T, F> *des);

    void changePlacesParentRightSon(NodeMapTree<T, F> *parent,
                                    NodeMapTree<T, F> *son);

    void
    changeNodePlaces(NodeMapTree<T, F> *node1, NodeMapTree<T, F> *node2);

public:

    /**
     * Constructor for AVLtree structure - initialize the head to nullptr
     * and size to zero.
     */
    AVLtree();

    /**
     * Copy constructor for tree structure.
     * @param tree the tree object to copy from
     */
    AVLtree(const AVLtree &tree) = default;

    /**
     * Destructor for tree structure - frees all the tree's nodes
     */
    ~AVLtree();

    /**
     * Get the tree's size
     * @return an int - the number of the nodes in the tree.
     */
    int getSize() const;

    /**
     * Find the node with the provided key by user
     * @param key the node's key you wish to find
     * @return a pointer to the tree's node with the provided key
     */
    NodeMapTree<T, F> *findNode(const T &key) const;

    /**
     * Get the value of the node with the provided key
     * @param key the node's key you wish to find
     * @return the value of the wanted node
     */
    F findGetValue(const T &key);

    /**
     * Add a new node to the tree.
     * @param key the new node's key that will be added to tree
     * @param value the new node's value that will be added to tree
     * @return a pointer to the new node that was added
     */
    NodeMapTree<T, F> *addNode(const T &key, const F &value);

    /**
     * Remove the node with the key provided from the tree
     * @param key the key of the node to remove
     */
    void removeNode(const T &key);

    /**
     * Remove a node from the tree - by the pointer provided by user
     * @param node a pointer to the node to remove
     */
    void removeByPointer(NodeMapTree<T, F> *node);

    /**
     * Delete the tree nodes using post order algorithm
     * @param node a pointer to the tree's head/root
     */
    void deletePostOrder(NodeMapTree<T, F> *node);

    /**
     * Check the balance of the tree (after removal/addition of a node to
     * the tree) and call the right rollout.
     * @param p a pointer to the tree's node to check it's balance
     */
    void checkBalance(NodeMapTree<T, F> *p);

    /**
     * Left-left rollout algorithm
     * @param B a pointer to the tree's node to balance
     * @return a pointer to the new node replaced with provided one
     */
    NodeMapTree<T, F> *reBalanceLL(NodeMapTree<T, F> *B);

    /**
     * Left-right rollout algorithm
     * @param B a pointer to the tree's node to balance
     * @return a pointer to the new node replaced with provided one
     */
    NodeMapTree<T, F> *reBalanceLR(NodeMapTree<T, F> *C);

    /**
     * Right-left rollout algorithm
     * @param B a pointer to the tree's node to balance
     * @return a pointer to the new node replaced with provided one
     */
    NodeMapTree<T, F> *reBalanceRL(NodeMapTree<T, F> *C);

    /**
     * Right-right rollout algorithm
     * @param B a pointer to the tree's node to balance
     * @return a pointer to the new node replaced with provided one
     */

    NodeMapTree<T, F> *reBalanceRR(NodeMapTree<T, F> *B);

    /**
     * Get the tree's head
     * @return a NodeMapTree pointer to the tree's head
     */
    NodeMapTree<T, F> *getRoot() const;

    /**
     * Set the root for the avl tree
     * @param root the new root pointer to update
     */
    void setRoot(NodeMapTree<T, F> *root);

    /**
     * set the avl tree size
     * @param size the new size of the tree
     */
    void setSize(int size);

    /**
     *
     * @param node
     * @param valuesArray
     * @param size
     */
    void getValuesByInOrder(NodeMapTree<T, F> *node, F* valuesArray, int* index);

};

/* Description: constructor of the tree
*/
template<class T, class F>
AVLtree<T, F>::AVLtree(): root(nullptr), size(0) {}

/* Description:   delete the nodes's tree in postorder recursive way.
   * Input:       node- usually the root of the tree.
   * Return Values: --
   */
template<class T, class F>
void AVLtree<T, F>::deletePostOrder(NodeMapTree<T, F> *node) {
    if (node == nullptr) return;
    deletePostOrder(node->getSonLeft());
    deletePostOrder(node->getSonRight());
    this->removeLeaf(node);
}

/* Description:  destructor of the tree
   */
template<class T, class F>
AVLtree<T, F>::~AVLtree() {
    deletePostOrder(root);
    root = nullptr;
}

/* Description:   get the size of the tree (number of nodes).
   * Input:       --
   * Return Values: number of nodes.
   */
template<class T, class F>
int AVLtree<T, F>::getSize() const {
    return this->size;
}

/* Description:   Finds a specific node in the tree.
* Input:             key - The node to find.
* Return Values: if the node exists - a pointer to the node,
             else, a pointer to the last node in the search.
*/
template<class T, class F>
NodeMapTree<T, F> *AVLtree<T, F>::findNode(const T &key) const {
    if (!this->root) { // if root is null the tree is empty
        return root;
    }
    NodeMapTree<T, F> *note = this->root;
    while (note->getKey() != key) {
        if (note->getKey() > key) {
            if (note->getSonLeft()) {
                note = note->getSonLeft();
                continue;
            } else break;
        }
        if (note->getKey() < key) {
            if (note->getSonRight()) {
                note = note->getSonRight();
                continue;
            } else break;
        }
    }
    return note;
}

/* Description:   gets the value of a specific node in the tree.
    * Input:       key - The node to find.
    * Return Values: if the node exists - the value of the node,
                 else- throw error.
    */
template<class T, class F>
F AVLtree<T, F>::findGetValue(const T &key) {
    if (root == nullptr) {
        throw KeyDoesNotExist();
    }
    NodeMapTree<T, F> *note = findNode(key);
    if (note->getKey() != key) {
        throw KeyDoesNotExist();
    } else {
        return note->getValue();
    }
}

/* Description:   Add a new node to the tree.
* Input:          key and value of the new node.
* Return Values:  A pointer to the new node.
*/
template<class T, class F>
NodeMapTree<T, F> *AVLtree<T, F>::addNode(const T &key, const F &value) {

    NodeMapTree<T, F> *newNode;

    if (!root) { // if the tree is empty

        newNode = new NodeMapTree<T, F>(key, value);
        root = newNode;
        this->size++;
        return root;
    }

    NodeMapTree<T, F> *lastNode = this->findNode(key);

    if(key == lastNode->getKey()){

        lastNode->updateValue(value);
        newNode = lastNode;

    } if (key > lastNode->getKey()) {

        newNode = new NodeMapTree<T, F>(key, value);
        lastNode->updateRightSon(newNode);
        this->size++;
    } if(key < lastNode->getKey()) {

        newNode = new NodeMapTree<T, F>(key, value);
        lastNode->updateLeftSon(newNode);
        this->size++;
    }

    lastNode->updateHeight();

    if (lastNode->getParent()) {
        checkBalance(lastNode->getParent());
    }
    return newNode;
}

template<class T, class F>
void AVLtree<T, F>::removeNode(const T &key) {
    if (root == nullptr) {
        throw KeyDoesNotExist();
    }
    NodeMapTree<T, F> *nodeToRemove = this->findNode(key);
    if (nodeToRemove->getKey() != key) {
        throw KeyDoesNotExist();
    } // the node is not exist
    NodeMapTree<T, F> *parentNodeToRemove = nullptr;
    int numOfSons = nodeToRemove->getNumOfSons();
    if (numOfSons == 0) { // if its a leaf- remove
        parentNodeToRemove = removeLeaf(nodeToRemove);
    }
    if (numOfSons == 1) {
        parentNodeToRemove = removeOneSon(nodeToRemove);
    }
    if (numOfSons > 1) {
        parentNodeToRemove = removeNodeInTheMiddle(nodeToRemove);
    }
    if (parentNodeToRemove) {
        checkBalance(parentNodeToRemove);
    }

}

template<class T, class F>
void AVLtree<T, F>::removeByPointer(NodeMapTree<T, F> *node) {
    if (!node) {
        throw InvalidInput();
    }
    removeNode(node->getKey());
    return;
}

template<class T, class F>
void AVLtree<T, F>::checkBalance(NodeMapTree<T, F> *p) {
    while (p) {
        if (p->isBalanced()) {
            p->updateHeight();
            p = p->getParent();
            continue;
        }
        int BF = p->getBalanceFactor();
        int highTemp = p->getHeight();
        NodeMapTree<T, F> *localRoot;
        if (BF == 2) {
            NodeMapTree<T, F> *leftSon = p->getSonLeft();
            if (leftSon->getBalanceFactor() >= 0) {
                localRoot = reBalanceLL(p);
            } else {
                localRoot = reBalanceLR(p);
            }
        } else {
            NodeMapTree<T, F> *rightSon = p->getSonRight();
            if (rightSon->getBalanceFactor() == 1) {
                localRoot = reBalanceRL(p);
            } else {
                localRoot = reBalanceRR(p);
            }
        }
        if (localRoot->getHeight() == highTemp) {
            break;
        }
        p = localRoot->getParent();
    }
    return;
}

template<class T, class F>
NodeMapTree<T, F> *AVLtree<T, F>::reBalanceLL(NodeMapTree<T, F> *B) {
    NodeMapTree<T, F> *A = B->getSonLeft();
    B->updateLeftSon(A->getSonRight());
    changeParentByNode(B, A);
    A->updateRightSon(B);
    B->updateHeight();
    A->updateHeight();
    return A;
}

template<class T, class F>
NodeMapTree<T, F> *AVLtree<T, F>::reBalanceLR(NodeMapTree<T, F> *C) {
    NodeMapTree<T, F> *A = C->getSonLeft();
    NodeMapTree<T, F> *B = A->getSonRight();
    C->updateLeftSon(B->getSonRight());
    A->updateRightSon(B->getSonLeft());
    changeParentByNode(C, B);
    B->updateLeftSon(A);
    B->updateRightSon(C);
    A->updateHeight();
    C->updateHeight();
    B->updateHeight();
    return B;
}

template<class T, class F>
NodeMapTree<T, F> *AVLtree<T, F>::reBalanceRL(NodeMapTree<T, F> *C) {
    NodeMapTree<T, F> *A = C->getSonRight();
    NodeMapTree<T, F> *B = A->getSonLeft();
    C->updateRightSon(B->getSonLeft());
    A->updateLeftSon(B->getSonRight());
    changeParentByNode(C, B);
    B->updateLeftSon(C);
    B->updateRightSon(A);
    A->updateHeight();
    C->updateHeight();
    B->updateHeight();
    return B;
}

template<class T, class F>
NodeMapTree<T, F> *AVLtree<T, F>::reBalanceRR(NodeMapTree<T, F> *B) {
    NodeMapTree<T, F> *A = B->getSonRight();
    B->updateRightSon(A->getSonLeft());
    changeParentByNode(B, A);
    A->updateLeftSon(B);
    B->updateHeight();
    A->updateHeight();
    return A;
}

template<class T, class F>
void AVLtree<T, F>::changeParentByNode(NodeMapTree<T, F> *source,
                                       NodeMapTree<T, F> *des) {
    if (source->getParent()) { // if source parent is not null
        if (source->isRightSon()) { // if source is a right son
            source->getParent()->updateRightSon(des);
        } else {
            source->getParent()->updateLeftSon(des);
        }
    } else { // if source parent is null its a root
        des->updateParent(nullptr);
        this->root = des;
    }
}

template<class T, class F>
void AVLtree<T, F>::changePlacesParentRightSon(NodeMapTree<T, F> *parent,
                                               NodeMapTree<T, F> *son) {
    son->updateLeftSon(parent->getSonLeft());
    if (!parent->getParent()) { // if the parent is a root
        son->updateParent(nullptr);
        root = son;
    } else {
        if (parent->isRightSon()) {
            parent->getParent()->updateRightSon(son);
        } else {
            parent->getParent()->updateLeftSon(son);
        }
    }
    parent->nodeAllNull(); //(**)
    parent->updateRightSon(son->getSonRight());
    son->updateRightSon(parent);
    return;
}

template<class T, class F>
void AVLtree<T, F>::changeNodePlaces(NodeMapTree<T, F> *node1,
                                     NodeMapTree<T, F> *node2) {
    NodeMapTree<T, F> *Parent2 = node2->getParent();
    /* int sonright2= 0; //(***) is left son
     if (node2->isRightSon()){
         sonright2=1;
     }*/
    // NodeMapTree<T,F>* LeftSon2= node2->getSonLeft(); //(**)
    NodeMapTree<T, F> *RightSon2 = node2->getSonRight();
    int high2 = node2->getHeight();
    if (!node1->getParent()) { //if node1 was a root
        node2->updateParent(nullptr);
        root = node2;
    } else {
        if (node1->isRightSon()) {
            node1->getParent()->updateRightSon(node2);
        } else {
            node1->getParent()->updateLeftSon(node2);
        }
    }
    node2->updateLeftSon(node1->getSonLeft());
    node2->updateRightSon(node1->getSonRight());
    node2->updateHeight(node1->getHeight());
    Parent2->updateLeftSon(node1);
    node1->updateLeftSon(nullptr);
    node1->updateRightSon(RightSon2);
    node1->updateHeight(high2);
    return;
}

template<class T, class F>
NodeMapTree<T, F> *
AVLtree<T, F>::removeLeaf(NodeMapTree<T, F> *nodeToRemove) {
    NodeMapTree<T, F> *Parent = nodeToRemove->getParent();
    if (!Parent) { // if its a root
        delete nodeToRemove;
        size--;
        root = nullptr;
        return nullptr;
    }
    if (nodeToRemove->isRightSon()) {
        Parent->updateRightSon(nullptr);
    } else {
        Parent->updateLeftSon(nullptr);
    }
    nodeToRemove->updateParent(nullptr);
    delete nodeToRemove;
    size--;
    return Parent;
}

template<class T, class F>
NodeMapTree<T, F> *
AVLtree<T, F>::removeOneSon(NodeMapTree<T, F> *nodeToRemove) {
    NodeMapTree<T, F> *son = nodeToRemove->getSonRight();
    if (!son) { // if rightSon IS NULL
        son = nodeToRemove->getSonLeft();
    }
    NodeMapTree<T, F> *nodeParent = nodeToRemove->getParent();
    if (!nodeParent) { // if nodeToRemove is root
        son->updateParent(nullptr);
        this->root = son;
    } else {
        if (nodeToRemove->isRightSon()) {
            nodeParent->updateRightSon(son);
        } else {
            nodeParent->updateLeftSon(son);
        }
    }
    nodeToRemove->nodeAllNull();
    delete nodeToRemove;
    size--;
    return nodeParent;
}

template<class T, class F>
NodeMapTree<T, F> *
AVLtree<T, F>::removeNodeInTheMiddle(NodeMapTree<T, F> *nodeToRemove) {
    NodeMapTree<T, F> *son = nodeToRemove->getSonRight();
    NodeMapTree<T, F> *Parent;
    if (son->getSonLeft() == nullptr) {
        changePlacesParentRightSon(nodeToRemove, son);
    } else {
        while (son->getSonLeft()) {
            son = son->getSonLeft();
        }
        changeNodePlaces(nodeToRemove, son);
    }
    if (nodeToRemove->getNumOfSons() == 0) {
        Parent = removeLeaf(nodeToRemove);
    } else {
        Parent = removeOneSon(nodeToRemove);
    }
    return Parent;
}

template<class T, class F>
NodeMapTree<T, F> *AVLtree<T, F>::getRoot() const {
    return root;
}

template<class T, class F>
void AVLtree<T, F>::setRoot(NodeMapTree<T, F> *root) {
    this->root = root;
}

template<class T, class F>
void AVLtree<T, F>::setSize(int size) {
    this->size = size;
}

template<class T, class F>
void AVLtree<T, F>::getValuesByInOrder(NodeMapTree<T, F> *node, F* valuesArray, int* index){

    if (node == nullptr) return;
    getValuesByInOrder(node->getSonLeft(), valuesArray, index);
    valuesArray[*index] = *(node->getValue());
    (*index)++;
    getValuesByInOrder(node->getSonRight(), valuesArray, index);

}

#endif //AVLTREE_AVLTREE_H