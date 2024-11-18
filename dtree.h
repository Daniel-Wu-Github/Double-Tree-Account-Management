/**
 * Author: Daniel Wu
 * Date: 10/10/2024
 * Section: 597
 * E-mail: Danielwu1510@tamu.edu
 * Project 2 - Binary Trees
 * DiscriminatorTree.h
 * An interface for the DTree class.
 */

#pragma once

#include <iostream>
#include <string>
#include <exception>
#include <vector>

//for debugging
#include <iomanip> // For std::setw

using std::cout;
using std::endl;
using std::string;
using std::ostream;
using std::vector;

#define DEFAULT_USERNAME ""
#define INVALID_DISC -1
#define MIN_DISC 0000
#define MAX_DISC 9999
#define DEFAULT_BADGE ""
#define DEFAULT_STATUS ""

#define DEFAULT_SIZE 1
#define DEFAULT_NUM_VACANT 0

class Grader;   /* For grading purposes */
class Tester;   /* Forward declaration for testing class */

class Account {
public:
    friend class Grader;
    friend class Tester;
    friend class DNode;
    friend class DTree;
    Account() {
        _username = DEFAULT_USERNAME;
        _disc = INVALID_DISC;
        _nitro = false;
        _badge = DEFAULT_BADGE;
        _status = DEFAULT_STATUS;
    }

    Account(string username, int disc, bool nitro, string badge, string status) {
        if(disc < MIN_DISC || disc > MAX_DISC) {
            throw std::out_of_range("Discriminator out of valid range (" + std::to_string(MIN_DISC) 
                                    + "-" + std::to_string(MAX_DISC) + ")");
        }
        _username = username;
        _disc = disc;
        _nitro = nitro;
        _badge = badge;
        _status = status;
    }

    /* Getters */
    string getUsername() const {return _username;}
    int getDiscriminator() const {return _disc;}
    bool hasNitro() const {return _nitro;}
    string getBadge() const {return _badge;}
    string getStatus() const {return _status;}

private:
    string _username;
    int _disc;
    bool _nitro;
    string _badge;
    string _status;
};

/* Overloaded << operator to print Accounts */
ostream& operator<<(ostream& sout, const Account& acct);

class DNode {
    friend class Grader;
    friend class Tester;
    friend class DTree;

public:
    DNode() {
        _size = DEFAULT_SIZE;
        _numVacant = DEFAULT_NUM_VACANT;
        _vacant = false;
        _left = nullptr;
        _right = nullptr;
    }

    DNode(Account account) {
        _account = account;
        _size = DEFAULT_SIZE;
        _numVacant = DEFAULT_NUM_VACANT;
        _vacant = false;
        _left = nullptr;
        _right = nullptr;
    }

    /* Getters */
    Account getAccount() const {return _account;}
    int getSize() const {return _size;}
    int getNumVacant() const {return _numVacant;}
    bool isVacant() const {return _vacant;}
    string getUsername() const {return _account.getUsername();}
    int getDiscriminator() const {return _account.getDiscriminator();}
    DNode* getLeft() const {return _left;}
    DNode* getRight() const {return _right;}
    int getNumVacant(DNode* node) const {
        if (node == nullptr) {
            return 0;
        }
        else{
            return node->_numVacant;
        }
    }
    int getSize(DNode* node) const {
        if (node == nullptr) {
            return 0;
        }
        else{
            return node->_size;
        }
    }

private:
    Account _account;
    int _size;
    int _numVacant;
    bool _vacant;
    DNode* _left;
    DNode* _right;

    /* IMPLEMENT (optional): any other helper functions */
};

class DTree {
    friend class Grader;
    friend class Tester;

public:
    DTree(): _root(nullptr) {}

    /* IMPLEMENT: destructor and assignment operator*/
    ~DTree();
    DTree& operator=(const DTree& rhs);

    /* IMPLEMENT: Basic operations */

    bool insert(Account newAcct);
    bool remove(int disc, DNode*& removed);
    DNode* retrieve(int disc);
    void clear();
    void printAccounts() const;
    void dump() const {dump(_root);}
    void dump(DNode* node) const;

    /* IMPLEMENT: "Helper" functions */
    
    int getNumUsers() const;
    string getUsername() const {return _root->getUsername();}
    void updateSize(DNode* node);
    void updateNumVacant(DNode* node);
    bool checkImbalance(DNode* node);
    //----------------
    void rebalance(DNode*& node);
    // -- OR --
    //DNode* rebalance(DNode*& node);
    //----------------
    DNode* getRoot() const {return _root;}

    //debugging
void printTreeStructure(DNode* node, int depth = 0, const std::string& prefix = "", bool isLeft = true) const {
            if (node == nullptr) {
                return;
            }
        
            // Print the current node with indentation based on depth
            std::cout << prefix;
        
            // Add the appropriate branch symbol
            std::cout << (isLeft ? "/-- " : "\\-- ");
        
            // Print the node's discriminator
            std::cout << node->getAccount().getDiscriminator();
        
            // Indicate if the node is vacant
            if (node->_vacant) {
                std::cout << " (vacant)";
            }
        
            std::cout << std::endl;
        
            // Prepare the prefix for the children
            std::string newPrefix = prefix + (isLeft ? "|   " : "    ");
        
            // Recursively print the left and right subtrees
            if (node->_left || node->_right) {
                if (node->_left) {
                    printTreeStructure(node->_left, depth + 1, newPrefix, true);
                } else {
                    std::cout << newPrefix << "/-- " << "null" << std::endl;
                }
                if (node->_right) {
                    printTreeStructure(node->_right, depth + 1, newPrefix, false);
                } else {
                    std::cout << newPrefix << "\\-- " << "null" << std::endl;
                }
            }
        }

private:
    DNode* _root;

    /* IMPLEMENT (optional): any additional helper functions here */
    void clear(DNode* node);
    DNode* subTreeCopy(const DNode* rhsNode);
    void printAccounts(DNode* node) const;
    int getNumUsers(DNode* node) const;
    int intUpdateSize(DNode* node);
    int intUpdateNumVacant(DNode* node);
    //DNode** arraySort(DNode* node, DNode**& sortedArray, int& index);
    void fillArray(DNode* node, vector<DNode*>& nodeArray);
    DNode* sortedNewTree(vector<DNode*>& nodeArray, int start, int end);
    bool insert(DNode*& node, Account newAcct);
    void replaceVacantNode(DNode* node, Account newAcct);
    int getMaxDiscriminator(DNode* node) const;
    int getMinDiscriminator(DNode* node) const;
};