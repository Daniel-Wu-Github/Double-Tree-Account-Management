
/**
 * Author: Daniel Wu
 * Date: 10/10/2024
 * Section: 597
 * E-mail: Danielwu1510@tamu.edu
 * Project 2 - Binary Trees
 * DiscriminatorTree.cpp
 * Implementation for the DTree class.
 */
#include <iostream>
#include <vector>
#include "dtree.h"

using namespace std;
/**
 * Destructor, deletes all dynamic memory.
 */
DTree::~DTree() {
    clear();
}

/**
 * Overloaded assignment operator, makes a deep copy of a DTree.
 * @param rhs Source DTree to copy
 * @return Deep copy of rhs
 */
DTree& DTree::operator=(const DTree& rhs) {
    if (this == &rhs) {
        return *this;
    }
    clear(_root);
    _root = subTreeCopy(rhs._root);
    return *this;
}
DNode* DTree::subTreeCopy(const DNode* rhsNode) {
    if (rhsNode == nullptr) {
        return nullptr;
    }
    DNode* newNode = new DNode(rhsNode->getAccount());
    newNode->_size = rhsNode->_size;
    newNode->_numVacant = rhsNode->_numVacant;
    newNode->_vacant = rhsNode->_vacant;
    newNode->_left = subTreeCopy(rhsNode->_left);
    newNode->_right = subTreeCopy(rhsNode->_right);
    return newNode;
}

/**
 * Dynamically allocates a new DNode in the tree. 
 * Should also update heights and detect imbalances in the traversal path
 * an insertion.
 * @param newAcct Account object to be contained within the new DNode
 * @return true if the account was inserted, false otherwise
 */
bool DTree::insert(Account newAcct) {
    if (_root == nullptr) {
        _root = new DNode(newAcct);
        return true;
    }
    // If the discriminator already exists
    if (retrieve(newAcct.getDiscriminator()) != nullptr) {
        return false;
    }
    if (newAcct.getDiscriminator() < MIN_DISC || newAcct.getDiscriminator() > MAX_DISC) {
        return false;
    }
    if (newAcct.getDiscriminator() == INVALID_DISC) {
        return false;
    }
    return insert(_root, newAcct);
}

bool DTree::insert(DNode*& node, Account newAcct) {
    if (node == nullptr) {
        node = new DNode(newAcct);
        // Update the size and numVacant of the parent nodes
        updateNumVacant(node);
        updateSize(node);
        if (checkImbalance(node)) {
            rebalance(node);
        }
        return true;
    }

    if (newAcct.getDiscriminator() < node->_account.getDiscriminator()) {
        if (node->_left == nullptr) {
            node->_left = new DNode(newAcct);
            updateNumVacant(node->_left);
            updateSize(node->_left);
            return true;
        } 
        // If the left child is vacant, check if the new account can be inserted
        else if (node->_left->_vacant) {
            if ((node->_left->_left == nullptr || newAcct.getDiscriminator() > getMaxDiscriminator(node->_left->_left)) &&
                (node->_left->_right == nullptr || newAcct.getDiscriminator() < getMinDiscriminator(node->_left->_right))) {
                replaceVacantNode(node->_left, newAcct);
                updateNumVacant(node->_left);
                updateSize(node->_left);
                return true;
            } 
            else {
                return insert(node->_left, newAcct);
            }
        } 
        else {
            return insert(node->_left, newAcct);
        }
    } 
    else if (newAcct.getDiscriminator() > node->_account.getDiscriminator()) {
        if (node->_right == nullptr) {
            node->_right = new DNode(newAcct);
            updateNumVacant(node->_right);
            updateSize(node->_right);
            return true;
        } // If the right child is vacant, check if the new account can be inserted
        else if (node->_right->_vacant) {
            if ((node->_right->_left == nullptr || newAcct.getDiscriminator() > getMaxDiscriminator(node->_right->_left)) &&
                (node->_right->_right == nullptr || newAcct.getDiscriminator() < getMinDiscriminator(node->_right->_right))) {
                replaceVacantNode(node->_right, newAcct);
                updateNumVacant(node->_right);
                updateSize(node->_right);
                return true;
            } 
            else {
                return insert(node->_right, newAcct);
            }
        } 
        else {
            return insert(node->_right, newAcct);
        }
    } 
    else {
        // Duplicate discriminator, insertion fails
        return false;
    }
}
// Helper function to get the maximum discriminator in a subtree
int DTree::getMaxDiscriminator(DNode* node) const {
    while (node->_right != nullptr) {
        node = node->_right;
    }
    return node->_account.getDiscriminator();
}

// Helper function to get the minimum discriminator in a subtree
int DTree::getMinDiscriminator(DNode* node) const {
    while (node->_left != nullptr) {
        node = node->_left;
    }
    return node->_account.getDiscriminator();
}


//helper function to replace a vacant node with a new account
void DTree::replaceVacantNode(DNode* node, Account newAcct){
    node->_account = newAcct;
    node->_vacant = false;
}
    

/**
 * Removes the specified DNode from the tree.
 * @param disc discriminator to match
 * @param removed DNode object to hold removed account
 * @return true if an account was removed, false otherwise
 * 
 * 
 */
bool DTree::remove(int disc, DNode*& removed) {
    if(disc == INVALID_DISC){
        return false;
    }
    if(disc < MIN_DISC || disc > MAX_DISC){
        return false;
    }
    if(_root == nullptr){
        return false;
    }

    if(retrieve(disc) == nullptr){
        return false;
    }



    DNode* parent = nullptr;
    DNode* current = _root;

    // Find the node to be removed and its parent
    while (current != nullptr && current->_account.getDiscriminator() != disc) {
        parent = current;
        if (disc < current->_account.getDiscriminator()) {
            current = current->_left;
        } 
        else {
            current = current->_right;
        }
    }

    // If the node was not found
    if (current == nullptr) {
        return false;
    }

    // Mark the node as vacant
    current->_vacant = true;
    removed = current;

    // Update the size and numVacant of the parent nodes
    while (parent != nullptr) {
        updateSize(parent);
        updateNumVacant(parent);
        if (parent->_account.getDiscriminator() > disc) {
            parent = parent->_left;
        } 
        else {
            parent = parent->_right;
        }
    }

    return true;
}

/**
 * Retrieves the specified Account within a DNode.
 * @param disc discriminator int to search for
 * @return DNode with a matching discriminator, nullptr otherwise
 */
DNode* DTree::retrieve(int disc) {
    if(disc == INVALID_DISC){
        return nullptr;
    }
    if(disc < MIN_DISC || disc > MAX_DISC){
        return nullptr;
    }
    if(_root == nullptr){
        return nullptr;
    }

    //start traversal process
    DNode* current = _root;

    //traversal
    while(current!= nullptr){
        if(disc > current->_account.getDiscriminator()){
            current = current->_right;
        }
        else if(disc < current->_account.getDiscriminator()){
            current = current->_left;
        }
        else if(disc == current->_account.getDiscriminator()){
            if(current->_vacant == true){
                return nullptr;
            }
            return current;
        }
    }
    //if the node is not found
    return nullptr;
}

/**
 * Helper for the destructor to clear dynamic memory.
 */
void DTree::clear() {
    clear(_root);
    _root = nullptr;
}

void DTree::clear(DNode* node) {
    if (node == nullptr) {
        return;
    }
    clear(node->_left);
    clear(node->_right);
    delete node;
}

/**
 * Prints all accounts' details within the DTree. in-order traversal
 */
void DTree::printAccounts() const {
    printAccounts(_root);
}

void DTree::printAccounts(DNode* node) const {
    if(_root == nullptr){
        return;
    }
    if(node == nullptr){
        return;
    }
    printAccounts(node->_left);
    if(node->_vacant == false){
        cout << node->getAccount() << endl;
    }
    printAccounts(node->_right);
}

/**
 * Dump the DTree in the '()' notation.
 */
void DTree::dump(DNode* node) const {
    if(node == nullptr) return;
    cout << "(";
    dump(node->_left);
    cout << node->getAccount().getDiscriminator() << ":" << node->getSize() << ":" << node->getNumVacant();
    dump(node->_right);
    cout << ")";
    //added for debugging
}

/**
 * Returns the number of valid users in the tree.
 * @return number of non-vacant nodes
 */
int DTree::getNumUsers() const {
    return getNumUsers(_root);
}
int DTree::getNumUsers(DNode* node) const {
    if(node == nullptr){
        return 0;
    }
    //if the node is vacant, return the sum of the left and right children
    if(node->_vacant == true){
        return getNumUsers(node->_left) + getNumUsers(node->_right);
    }
    return 1 + getNumUsers(node->_left) + getNumUsers(node->_right);
}

/**
 * Updates the size of a node based on the immediate children's sizes
 * @param node DNode object in which the size will be updated
 */
void DTree::updateSize(DNode* node) {
    if (node == nullptr) {
        return;
    }

    // Recursively update the size for left and right children
    updateSize(node->_left);
    updateSize(node->_right);

    // Update the size of the current node
    node->_size = intUpdateSize(node);
}

// Helper function for updateSize of int type
int DTree::intUpdateSize(DNode* node) {
    if (node == nullptr) {
        return 0;
    }
    return 1 + intUpdateSize(node->_left) + intUpdateSize(node->_right);
}


/**
 * Updates the number of vacant nodes in a node's subtree based on the immediate children
 * @param node DNode object in which the number of vacant nodes in the subtree will be updated
 */
void DTree::updateNumVacant(DNode* node) {
    node->_numVacant = intUpdateNumVacant(node);
}
//helper function for updateNumVacant of int type
int DTree::intUpdateNumVacant(DNode* node){
    if(node == nullptr){
        return 0;
    }
    
    // Recursively update the number of vacant nodes for left and right children
    int numVacantLeft = intUpdateNumVacant(node->_left);
    int numVacantRight = intUpdateNumVacant(node->_right);

    if(node->_vacant){
        node->_numVacant = 1 + numVacantLeft + numVacantRight;
    }
    else{
        node->_numVacant = numVacantLeft + numVacantRight;
    }
    return node->_numVacant;
}

/**
 * Checks for an imbalance, defined by 'Discord' rules, at the specified node.
 * @param checkImbalance DNode object to inspect for an imbalance
 * @return (can change) returns true if an imbalance occured, false otherwise
 */
bool DTree::checkImbalance(DNode* node) {
    //debugging
    cout << "Checking imbalance at node...... " << node->_account.getDiscriminator() << endl;
    if(node == nullptr){
        return false;
    }
    updateSize(node);
    updateNumVacant(node);

    if(node->_left == nullptr && node->_right == nullptr){
        return false;
    }

    //check if the node is a leaf, initialize leftSize and rightSize

    int leftSize;
    int rightSize;

    if(node->_left == nullptr){
        leftSize = 0;
    }
    else{
        leftSize = node->_left->_size;
    }
    
    if(node->_right == nullptr){
        rightSize = 0;
    }

    else{
        rightSize = node->_right->_size;
    }

    if(leftSize < 4 && rightSize < 4){
        return false;
    }
    //check if the node is imbalanced
    if(leftSize > 1.5 * rightSize || rightSize > 1.5 * leftSize){
        return true;
    }
    return false;
}

//----------------
/**
 * Begins and manages the rebalancing process for a 'Discrd' tree (pass by reference).
 * @param node DNode root of the subtree to balance
 */
//void DTree::rebalance(DNode*& node) {
//}

// -- OR --

/**
 * Begins and manages the rebalancing process for a 'Discrd' tree (returns a pointer).
 * @param node DNode root of the subtree to balance
 * @return DNode root of the balanced subtree
 */
void DTree::rebalance(DNode*& node) {
    //debugging
    cout << "imbalance found, rebalancing at node...... " << node->_account.getDiscriminator() << endl;
    if(node == nullptr){
        return;
    }
    //update size and numVacant
    updateSize(node);
    updateNumVacant(node);

    std::vector<DNode*> sortedArray;

    // Fill the array with non-vacant nodes using in-order traversal
    fillArray(node, sortedArray);

    // Create a new balanced tree from the sorted array
    DNode* balancedRoot = sortedNewTree(sortedArray, 0, sortedArray.size() - 1);

    node = balancedRoot;

    //debugging
    cout << "Rebalancing complete at node...... " << node->_account.getDiscriminator() << endl;
}

void DTree::fillArray(DNode* node, std::vector<DNode*>& nodeArray) {
    if (node == nullptr) {
        return;
    }
    fillArray(node->_left, nodeArray);
    // Add the node to the array if it is not vacant
    if (!node->_vacant) {
        nodeArray.push_back(node);
    }
    fillArray(node->_right, nodeArray);
}

DNode* DTree::sortedNewTree(std::vector<DNode*>& nodeArray, int start, int end){
    if (start > end){
        return nullptr;
    }
    int mid = (start + end) / 2;
    DNode* root = nodeArray[mid];
    root->_left = sortedNewTree(nodeArray, start, mid - 1);
    root->_right = sortedNewTree(nodeArray, mid + 1, end);
    updateSize(root);
    updateNumVacant(root);
    return root;
}
    
    

//----------------

/**
 * Overloaded << operator for an Account to print out the account details
 * @param sout ostream object
 * @param acct Account objec to print
 * @return ostream object containing stream of account details
 */
ostream& operator<<(ostream& sout, const Account& acct) {
    sout << "Account name: " << acct.getUsername() << 
            "\n\tDiscriminator: " << acct.getDiscriminator() <<
            "\n\tNitro: " << acct.hasNitro() << 
            "\n\tBadge: " << acct.getBadge() << 
            "\n\tStatus: " << acct.getStatus();
    return sout;
}