/**
 * Project 2 - Binary Trees
 * UserTree.h
 * Implementation for the UTree class.
 * Author: Daniel Wu
 * Date: 11/6/2024
 * section: 597
 * E-mail: danielwu1510@tamu.edu
 */

#include "utree.h"

/**
 * Destructor, deletes all dynamic memory.
 */
UTree::~UTree() {
    clear();
}

/**
 * Sources a .csv file to populate Account objects and insert them into the UTree.
 * @param infile path to .csv file containing database of accounts
 * @param append true to append to an existing tree structure or false to clear before importing
 */ 
void UTree::loadData(string infile, bool append) {
    std::ifstream instream(infile);
    string line;
    char delim = ',';
    const int numFields = 5;
    string fields[numFields];

    /* Check to make sure the file was opened */
    if(!instream.is_open()) {
        std::cerr << __FUNCTION__ << ": File " << infile << " could not be opened or located" << endl;
        exit(-1);
    }

    /* Should we append or clear? */
    if(!append) this->clear();

    /* Read in the data from the .csv file and insert into the UTree */
    while(std::getline(instream, line)) {
        std::stringstream buffer(line);

        /* Quick check to make sure each line is formatted correctly */
        int delimCount = 0;
        for(unsigned int c = 0; c < buffer.str().length(); c++) if(buffer.str()[c] == delim) delimCount++;
        if(delimCount != numFields - 1) {
            throw std::invalid_argument("Malformed input file detected - ensure each line contains 5 fields deliminated by a ','");
        }

        /* Populate the account attributes - 
         * Each line always has 5 sections of data */
        for(int i = 0; i < numFields; i++) {
            std::getline(buffer, line, delim);
            fields[i] = line;
        }
        Account newAcct = Account(fields[0], std::stoi(fields[1]), std::stoi(fields[2]), fields[3], fields[4]);
        this->insert(newAcct);
    }
}

/**
 * Dynamically allocates a new UNode in the tree and passes insertion into DTree. 
 * Should also update heights and detect imbalances in the traversal path after
 * an insertion.
 * @param newAcct Account object to be inserted into the corresponding DTree
 * @return true if the account was inserted, false otherwise
 */
bool UTree::insert(Account newAcct) {
    return insert(_root, newAcct);
}

bool UTree::insert(UNode*& node, Account newAcct){
    //base case of creating a new node at the right place
    if (node == nullptr) {
        node = new UNode();
        node->getDTree()->insert(newAcct);
        node->_height = 0;
        return true;
    }
    //insert into the right subtree
    else if(newAcct.getUsername() > node->getUsername()){
        bool didInsert = insert(node->_right, newAcct);
        updateHeight(node);
        int heightDifference = checkImbalance(node);
        if (heightDifference > 1 || heightDifference < -1) {
            rebalance(node);
        }
        return didInsert;
    }
    //insert into the left subtree
    else if(newAcct.getUsername() < node->getUsername()){
        bool didInsert = insert(node->_left, newAcct);
        updateHeight(node);
        int heightDifference = checkImbalance(node);
        if (heightDifference > 1 || heightDifference < -1) {
            rebalance(node);
        }
        return didInsert;
    }
    else{
        //insert into the DTree since the username already exists
        return node->getDTree()->insert(newAcct);
    }
}

/**
 * Removes a user with a matching username and discriminator.
 * @param username username to match
 * @param disc discriminator to match
 * @param removed DNode object to hold removed account
 * @return true if an account was removed, false otherwise
 */

bool UTree::removeUser(string username, int disc, DNode*& removed) {
    return removeUser(_root, username, disc, removed);
}

bool UTree::removeUser(UNode*& node, string username, int disc, DNode*& removed) {
    //user not found
    if (node == nullptr) {
        return false;
    }
    //continue to the right subtree
    if (username > node->getUsername()) {
        bool didRemove = removeUser(node->_right, username, disc, removed);
        if (didRemove) {
            updateHeight(node);
            int heightDifference = checkImbalance(node);
            if (heightDifference > 1 || heightDifference < -1) {
                rebalance(node);
            }
            return true;
        }
        return false;
    }
    //continue to the left subtree 
    else if (username < node->getUsername()) {
        bool didRemove = removeUser(node->_left, username, disc, removed);
        if (didRemove) {
            updateHeight(node);
            int heightDifference = checkImbalance(node);
            if (heightDifference > 1 || heightDifference < -1) {
                rebalance(node);
            }
            return true;
        }
        return false;
    }
    //found the user 
    else {
        //remove the user from the DTree
        bool didRemoveDTree = node->getDTree()->remove(disc, removed);
        if (didRemoveDTree) {
            if (node->getDTree()->getNumUsers() == 0) {
                replaceVacantNode(node);
            }
            updateHeight(node);
            int heightDifference = checkImbalance(node);
            if (heightDifference > 1 || heightDifference < -1) {
                rebalance(node);
            }
            return true;
        }
        return false;
    }
}

void UTree::replaceVacantNode(UNode*& node) {
    if (node == nullptr) {
        return;
    }
    //leaf node
    if (node->_left == nullptr && node->_right == nullptr) {
        delete node;
        node = nullptr;
    }
    //only right child
    else if (node->_left == nullptr) {
        UNode* temp = node;
        node = node->_right;
        delete temp;
    } 
    //only left child
    else if (node->_right == nullptr) {
        UNode* temp = node;
        node = node->_left;
        delete temp;
    }
    //both children exist
    else{
        //find the rightmost node in the left subtree
        DTree* rightMost = nullptr;
        deleteRightMost(node->_left, rightMost);

        delete node->_dtree;
        node->_dtree = rightMost;

        updateHeight(node);
        int heightDifference = checkImbalance(node);
        if (heightDifference > 1 || heightDifference < -1) {
            rebalance(node);
        }
    }
}

void UTree::deleteRightMost(UNode*& node, DTree*& rightMost) {
    if (node == nullptr) {
        rightMost = nullptr;
        return;
    }
    //continue to the right subtree
    if (node->_right != nullptr) {
        deleteRightMost(node->_right, rightMost);
        updateHeight(node);
        int heightDifference = checkImbalance(node);
        if (heightDifference > 1 || heightDifference < -1) {
            rebalance(node);
        }
    }
    //found the rightmost node
    else {
        rightMost = node->_dtree;
        UNode* temp = node;
        node = node->_left;
        delete temp;
    }
}
    

/**
 * Retrieves a set of users within a UNode.
 * @param username username to match
 * @return UNode with a matching username, nullptr otherwise
 */
UNode* UTree::retrieve(string username) {
    if (_root == nullptr) {
        return nullptr;
    }
    //start traversal process
    UNode* current = _root;

    while (current != nullptr) {
        if (username > current->getUsername()) {
            current = current->_right;
        } else if (username < current->getUsername()) {
            current = current->_left;
        } else if (username == current->getUsername()) {
            return current;
        }
    }
    //node is not found
    return nullptr;
}

/**
 * Retrieves the specified Account within a DNode.
 * @param username username to match
 * @param disc discriminator to match
 * @return DNode with a matching username and discriminator, nullptr otherwise
 */
DNode* UTree::retrieveUser(string username, int disc) {
    UNode* user = retrieve(username);
    if (user == nullptr) {
        return nullptr;
    }
    return user->getDTree()->retrieve(disc);
}

/**
 * Returns the number of users with a specific username.
 * @param username username to match
 * @return number of users with the specified username
 */
int UTree::numUsers(string username) {
    UNode* user = retrieve(username);
    if (user == nullptr) {
        return 0;
    }
    return user->getDTree()->getNumUsers();
}

/**
 * Helper for the destructor to clear dynamic memory.
 */
void UTree::clear() {
    clear(_root);
    _root = nullptr;
}

void UTree::clear(UNode* node){
    if(node == nullptr){
        return;
    }
    clear(node->_left);
    clear(node->_right);
    delete node;
}

/**
 * Prints all accounts' details within every DTree.
 */
void UTree::printUsers() const {
    printUsers(_root);
}

void UTree::printUsers(UNode* node) const {
    if(node == nullptr){
        return;
    }
    printUsers(node->_left);
    node->getDTree()->printAccounts();
    printUsers(node->_right);
}

/**
 * Dumps the UTree in the '()' notation.
 */
void UTree::dump(UNode* node) const {
    if(node == nullptr) return;
    cout << "(";
    dump(node->_left);
    cout << node->getUsername() << ":" << node->getHeight() << ":" << node->getDTree()->getNumUsers();
    dump(node->_right);
    cout << ")";
}

/**
 * Updates the height of the specified node.
 * @param node UNode object in which the height will be updated
 */
void UTree::updateHeight(UNode* node) {
    //use a post-order traversal to update the height of the tree
    if(node == nullptr){
        return;
    }
    updateHeight(node->_left);
    updateHeight(node->_right);


    // Check if _left or _right is nullptr before calling getHeight()
    int leftHeight = (node->_left != nullptr) ? node->_left->getHeight() : 0;
    int rightHeight = (node->_right != nullptr) ? node->_right->getHeight() : 0;

    // Set node->_height based on the maximum of left and right heights
    node->_height = 1 + std::max(leftHeight, rightHeight);
}

/**
 * Checks for an imbalance, defined by AVL rules, at the specified node.
 * @param node UNode object to inspect for an imbalance
 * @return (can change) returns true if an imbalance occured, false otherwise
 */
int UTree::checkImbalance(UNode* node) {
    if (node == nullptr) {
        return 0;
    }
    //calculate the height difference
    int leftHeight = (node->_left != nullptr) ? node->_left->_height : -1;
    int rightHeight = (node->_right != nullptr) ? node->_right->_height : -1;
    return leftHeight - rightHeight;
}

//----------------
/**
 * Begins and manages the rebalance procedure for an AVL tree (pass by reference).
 * @param node UNode object where an imbalance occurred
 */
void UTree::rebalance(UNode*& node) {
    if (node == nullptr) {
        return;
    }

    updateHeight(node);

    //calculate the height difference
    int heightDifference = ((node->_left != nullptr)? node->_left->_height : -1) - ((node->_right != nullptr)? node->_right->_height : -1);
    
    //left heavy
    if (heightDifference > 1) {
        int leftDiff = ((node->_left->_left != nullptr)? node->_left->_left->_height : -1) - ((node->_left->_right != nullptr)? node->_left->_right->_height : -1);

        //left left
        if (leftDiff >= 0) {
            zigRight(node);
        }
        //left right
        else {
            //first zig the left child to the left, then zig the node to the right
            zigLeft(node->_left);
            zigRight(node);
        } 
    }
    //right heavy
    else if(heightDifference < -1){
        int rightDiff = ((node->_right->_left != nullptr)? node->_right->_left->_height : -1) - ((node->_right->_right != nullptr)? node->_right->_right->_height : -1);

        //right right
        if (rightDiff <= 0) {
            zigLeft(node);
        }
        //right left
        else {
            //first zig the right child to the right, then zig the node to the left
            zigRight(node->_right);
            zigLeft(node);
        }
    }
}

void UTree::zigLeft(UNode*& node) {
    //rotate the node to the left
    UNode* temp = node->_right;
    node->_right = temp->_left;
    temp->_left = node;

    updateHeight(node);
    updateHeight(temp);

    node = temp;
}

void UTree::zigRight(UNode*& node) {
    //rotate the node to the right
    UNode* temp = node->_left;
    node->_left = temp->_right;
    temp->_right = node;

    updateHeight(node);
    updateHeight(temp);

    node = temp;
}

/**
 * Begins and manages the rebalance procedure for an AVL tree (returns a pointer).
 * @param node UNode object where an imbalance occurred
 * @return UNode object replacing the unbalanced node's position in the tree
 */
//UNode* UTree::rebalance(UNode* node) {

//}
//----------------
