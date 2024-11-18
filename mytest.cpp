/**
 * Author: Daniel Wu
 * Date: 10/10/2024
 * Section: 597
 * E-mail: Danielwu1510@tamu.edu
 * Project 2 - Binary Trees
 * mytest.cpp
 * A test file for the DTree class.
 */

#include <iostream>
#include "dtree.h"
#include "utree.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <iomanip> // For std::setw

using std::cout, std::endl, std::string, std::ostream;

// Test the destructor
void testDestructor() {
    // Create a DTree object
    DTree* tree = new DTree();

    tree->insert(Account("user1", 1, false, "badge1", "online"));
    tree->insert(Account("user2", 2, true, "badge2", "offline"));
    tree->insert(Account("user3", 3, false, "badge3", "online"));
    tree->insert(Account("user4", 4, true, "badge4", "offline"));
    tree->insert(Account("user5", 7, false, "badge5", "online"));
    tree->insert(Account("user6", 5, true, "badge6", "offline"));
    tree->insert(Account("user7", 15, false, "badge7", "online"));
    tree->insert(Account("user8", 6, true, "badge8", "offline"));

    tree->printAccounts();

    tree->dump(tree->getRoot());

    delete tree;
}

// Test the copy constructor
void testCopyConstructor() {
    // Create a DTree object
    DTree* tree = new DTree();

    // Insert some nodes
    tree->insert(Account("user1", 1, false, "badge1", "online"));
    tree->insert(Account("user2", 2, true, "badge2", "offline"));
    tree->insert(Account("user3", 3, false, "badge3", "online"));
    tree->insert(Account("user4", 4, true, "badge4", "offline"));
    tree->insert(Account("user5", 7, false, "badge5", "online"));
    tree->insert(Account("user6", 5, true, "badge6", "offline"));
    tree->insert(Account("user7", 15, false, "badge7", "online"));
    tree->insert(Account("user8", 6, true, "badge8", "offline"));

    // Print the tree before deletion
    std::cout << "Tree before deletion:" << std::endl;
    tree->printAccounts();

    tree->dump(tree->getRoot());

    cout << "divider: ____________________________" << endl;
    // Create a new DTree object using the copy constructor
    DTree* tree2 = tree;
    tree2->printAccounts();
    tree2->dump(tree2->getRoot());
    delete tree;
}

// Test the insert function and its edge cases
void testInsert() {
    DTree tree;

    // Test regular insert
    tree.insert(Account("user1", 1, false, "badge1", "online"));
    tree.insert(Account("user2", 2, true, "badge2", "offline"));
    tree.insert(Account("user3", 3, false, "badge3", "online"));
    tree.insert(Account("user4", 4, true, "badge4", "offline"));
    tree.insert(Account("user5", 7, false, "badge5", "online"));
    tree.insert(Account("user6", 5, true, "badge6", "offline"));
    tree.insert(Account("user7", 15, false, "badge7", "online"));
    tree.insert(Account("user8", 6, true, "badge8", "offline"));

    // Print the tree before deletion
    std::cout << "Tree before deletion:" << std::endl;
    tree.printAccounts();

    tree.dump(tree.getRoot());
}

// Test the remove function and its edge cases
void testRemove() {
    DTree tree;

    // Test regular insert
    tree.insert(Account("user1", 1, false, "badge1", "online"));
    tree.insert(Account("user2", 2, true, "badge2", "offline"));
    tree.insert(Account("user3", 3, false, "badge3", "online"));
    tree.insert(Account("user4", 4, true, "badge4", "offline"));
    tree.insert(Account("user5", 7, false, "badge5", "online"));
    tree.insert(Account("user6", 5, true, "badge6", "offline"));
    tree.insert(Account("user7", 15, false, "badge7", "online"));
    tree.insert(Account("user8", 6, true, "badge8", "offline"));

    // Test remove
    DNode* removed;
    cout << "Node removal was:" << tree.remove(2, removed) << endl;
    cout << "Node removal was:" << tree.remove(7, removed) << endl;

    tree.dump(tree.getRoot());
}

// Test update functions
void testUpdateFunctions() {
    DTree tree;

    // Insert nodes
    tree.insert(Account("user1", 1, false, "badge1", "online"));
    tree.insert(Account("user2", 2, true, "badge2", "offline"));
    tree.insert(Account("user3", 3, false, "badge3", "online"));
    tree.insert(Account("user4", 4, true, "badge4", "offline"));
    tree.insert(Account("user5", 7, false, "badge5", "online"));
    tree.insert(Account("user6", 5, true, "badge6", "offline"));
    tree.insert(Account("user7", 15, false, "badge7", "online"));
    tree.insert(Account("user8", 6, true, "badge8", "offline"));

    // Mark some nodes as vacant
    DNode* removed;
    tree.remove(2, removed); // Mark node with discriminator 2 as vacant
    tree.remove(7, removed); // Mark node with discriminator 7 as vacant
    tree.remove(15, removed); // Mark node with discriminator 15 as vacant

    // Update size and number of vacant nodes
    tree.updateSize(tree.getRoot());
    tree.updateNumVacant(tree.getRoot());

    // Print the tree structure
    tree.dump(tree.getRoot());

    // Verify the results
    cout << "Verifying updateSize and updateNumVacant functions:" << endl;

    // Check the size and number of vacant nodes at various points
    DNode* root = tree.getRoot();
    if (root) {
        cout << "Root size: " << root->getSize() << ", expected: 8" << endl;
        cout << "Root numVacant: " << root->getNumVacant() << ", expected: 3" << endl;
    }
    cout << "size of the right child of the root: " << root->getSize(root->getRight()) << ", expected: 7" << endl;
    cout << "size of the left child of the root: " << root->getSize(root->getLeft()) << ", expected: 0" << endl;
    cout << "numVacant of the right child of the root: " << root->getNumVacant(root->getRight()) << ", expected: 3" << endl;
    cout << "numVacant of the left child of the root: " << root->getNumVacant(root->getLeft()) << ", expected: 0" << endl;
    cout << "numVacant of the right child of the right child of the root: " << root->getNumVacant(root->getRight()->getRight()) << ", expected: 2" << endl; 
}

// Similar test but for another tree
void testUpdateFunctions2() {
    DTree tree;

    // Insert nodes
    tree.insert(Account("user1", 10, false, "badge1", "online"));
    tree.insert(Account("user2", 20, true, "badge2", "offline"));
    tree.insert(Account("user3", 5, false, "badge3", "online"));
    tree.insert(Account("user4", 15, true, "badge4", "offline"));
    tree.insert(Account("user5", 25, false, "badge5", "online"));
    tree.insert(Account("user6", 6, true, "badge6", "offline"));
    tree.insert(Account("user7", 4, false, "badge7", "online"));
    
    // Mark some nodes as vacant
    DNode* removed;
    tree.remove(20, removed); // Mark node with discriminator 20 as vacant
    tree.remove(15, removed); // Mark node with discriminator 15 as vacant
    tree.remove(6, removed); // Mark node with discriminator 6 as vacant

    // Testing inserting at vacant nodes
    tree.insert(Account("user8", 55, false, "badge8", "online"));
    tree.insert(Account("user9", 21, false, "badge9", "online"));
    tree.insert(Account("user10", 16, false, "badge10", "online"));
    tree.insert(Account("user11", 99, false, "badge11", "online")); 

    DNode* root = tree.getRoot();
    tree.updateSize(root);
    tree.updateNumVacant(root);

    tree.dump(root);

    // Rebalance
    if (tree.checkImbalance(root)) {
        tree.rebalance(root);
    }

    tree.dump(root);
}

// Test for duplicate insertions
void testDuplicateInsertions() {
    DTree tree;

    // Insert nodes
    tree.insert(Account("user1", 1, false, "badge1", "online"));
    bool result = tree.insert(Account("user1", 1, false, "badge1", "online")); // Duplicate insertion

    // Print the result
    cout << "Duplicate insertion result (should be false): " << result << endl;
}

// Test for edge cases (min and max discriminators)
void testEdgeCases() {
    DTree tree;

    // Insert nodes with min and max discriminators
    tree.insert(Account("userMin", MIN_DISC, false, "badgeMin", "online"));
    tree.insert(Account("userMax", MAX_DISC, true, "badgeMax", "offline"));

    // Print the tree structure
    tree.dump(tree.getRoot());
}

// Test for tree balancing
void testTreeBalancing() {
    DTree tree;

    // Insert nodes in a way that requires rebalancing
    tree.insert(Account("user1", 1, false, "badge1", "online"));
    tree.insert(Account("user2", 2, true, "badge2", "offline"));
    tree.insert(Account("user3", 3, false, "badge3", "online"));
    tree.insert(Account("user4", 4, true, "badge4", "offline"));
    tree.insert(Account("user5", 5, false, "badge5", "online"));
    tree.insert(Account("user6", 6, true, "badge6", "offline"));
    tree.insert(Account("user7", 7, false, "badge7", "online"));
    tree.insert(Account("user8", 8, true, "badge8", "offline"));

    // Check for imbalance and rebalance if necessary
    DNode* root = tree.getRoot();
    if (tree.checkImbalance(root)) {
        tree.rebalance(root);
    }

    // Print the tree structure
    tree.dump(root);
}
//Divider ------------------------------------------------------------------------------------------------------------

// test for UTree.cpp

// Test the destructor
void testUTreeDestructor() {
   UTree tree;

    // Populate the tree with some data
    tree.insert(Account("user1", 1, 100, "email1", "phone1"));
    cout << "UTree insert one complete." << endl;
    tree.insert(Account("user2", 2, 200, "email2", "phone2"));
    cout << "UTree insert two complete." << endl;
    tree.insert(Account("user3", 3, 300, "email3", "phone3"));
    cout << "UTree insert three complete." << endl;

    std::cout << "UTree destructor test passed." << std::endl;
}

void testUTreeUpdateHeight() {
    UTree tree;

    // Populate the tree with some data
    tree.insert(Account("user1", 1, 100, "email1", "phone1"));
    tree.insert(Account("user2", 2, 200, "email2", "phone2"));
    tree.insert(Account("user3", 3, 300, "email3", "phone3"));

    // Print the tree structure
    tree.dump();
}   

// divider ------------------------------------------------------------------------------------------------------------ 

void testUTreeRemoveUser() {
    UTree tree;

    // Populate the tree with some data
    tree.insert(Account("user1", 1, 100, "email1", "phone1"));
    tree.insert(Account("user2", 2, 200, "email2", "phone2"));
    tree.insert(Account("user3", 3, 300, "email3", "phone3"));
    tree.insert(Account("user4", 4, 400, "email4", "phone4"));
    tree.insert(Account("user5", 5, 500, "email5", "phone5"));

    // Print the tree before removal
    cout << "Tree before removal:" << endl;
    tree.dump();

    DNode* removed = nullptr;

    // Test removing a user that exists
    cout << "Removing user2 with discriminator 2:" << endl;
    bool result = tree.removeUser("user2", 2, removed);
    cout << "Remove result: " << result << " (expected: true)" << endl;
    tree.dump();
    cout << "Test " << (result ? "PASSED" : "FAILED") << endl;

    // Test removing a user that does not exist
    cout << "Removing user6 with discriminator 6 (should not exist):" << endl;
    result = tree.removeUser("user6", 6, removed);
    cout << "Remove result: " << result << " (expected: false)" << endl;
    tree.dump();
    cout << "Test " << (!result ? "PASSED" : "FAILED") << endl;

    // Test removing a user with a different discriminator
    cout << "Removing user3 with discriminator 4 (wrong discriminator):" << endl;
    result = tree.removeUser("user3", 4, removed);
    cout << "Remove result: " << result << " (expected: false)" << endl;
    tree.dump();
    cout << "Test " << (!result ? "PASSED" : "FAILED") << endl;

    // Test removing the last user in a DTree
    cout << "Removing user1 with discriminator 1:" << endl;
    result = tree.removeUser("user1", 1, removed);
    cout << "Remove result: " << result << " (expected: true)" << endl;
    tree.dump();
    cout << "Test " << (result ? "PASSED" : "FAILED") << endl;

    // Test removing a user that causes a node replacement
    cout << "Removing user3 with discriminator 3:" << endl;
    result = tree.removeUser("user3", 3, removed);
    cout << "Remove result: " << result << " (expected: true)" << endl;
    tree.dump();
    cout << "Test " << (result ? "PASSED" : "FAILED") << endl;

    // Test removing the last user in the tree
    cout << "Removing user4 with discriminator 4:" << endl;
    result = tree.removeUser("user4", 4, removed);
    cout << "Remove result: " << result << " (expected: true)" << endl;
    tree.dump();
    cout << "Test " << (result ? "PASSED" : "FAILED") << endl;

    cout << "Removing user5 with discriminator 5:" << endl;
    result = tree.removeUser("user5", 5, removed);
    cout << "Remove result: " << result << " (expected: true)" << endl;
    tree.dump();
    cout << "Test " << (result ? "PASSED" : "FAILED") << endl;
}

void testUTreeRetrieve() {
    UTree tree;
    tree.insert(Account("user1", 1, 100, "email1", "phone1"));
    tree.insert(Account("user2", 2, 200, "email2", "phone2"));
    tree.insert(Account("user3", 3, 300, "email3", "phone3"));
    UNode* node = tree.retrieve("user1");
    cout << "Retrieve user1: " << (node != nullptr ? "PASSED" : "FAILED") << endl;
    node = tree.retrieve("user2");
    cout << "Retrieve user2: " << (node != nullptr ? "PASSED" : "FAILED") << endl;
    node = tree.retrieve("user3");
    cout << "Retrieve user3: " << (node != nullptr ? "PASSED" : "FAILED") << endl;
    node = tree.retrieve("user4");
    cout << "Retrieve user4: " << (node == nullptr ? "PASSED" : "FAILED") << endl;
}

void testUTreeRetrieveUser() {
    UTree tree;
    tree.insert(Account("user1", 1, 100, "email1", "phone1"));
    tree.insert(Account("user2", 2, 200, "email2", "phone2"));
    tree.insert(Account("user3", 3, 300, "email3", "phone3"));
    DNode* dnode = tree.retrieveUser("user1", 1);
    cout << "Retrieve user1 with disc 1: " << (dnode != nullptr ? "PASSED" : "FAILED") << endl;
    dnode = tree.retrieveUser("user2", 2);
    cout << "Retrieve user2 with disc 2: " << (dnode != nullptr ? "PASSED" : "FAILED") << endl;
    dnode = tree.retrieveUser("user3", 3);
    cout << "Retrieve user3 with disc 3: " << (dnode != nullptr ? "PASSED" : "FAILED") << endl;
    dnode = tree.retrieveUser("user4", 4);
    cout << "Retrieve user4 with disc 4: " << (dnode == nullptr ? "PASSED" : "FAILED") << endl;
}

void testUTreeNumUsers() {
    UTree tree;
    tree.insert(Account("user1", 1, 100, "email1", "phone1"));
    tree.insert(Account("user1", 2, 200, "email2", "phone2"));
    tree.insert(Account("user2", 3, 300, "email3", "phone3"));
    int numUsers = tree.numUsers("user1");
    cout << "Number of users with username 'user1': " << numUsers << " (expected: 2)" << endl;
    numUsers = tree.numUsers("user2");
    cout << "Number of users with username 'user2': " << numUsers << " (expected: 1)" << endl;
    numUsers = tree.numUsers("user3");
    cout << "Number of users with username 'user3': " << numUsers << " (expected: 0)" << endl;
}

void testUTreePrintUsers() {
    UTree tree;
    tree.insert(Account("user1", 1, 100, "email1", "phone1"));
    tree.insert(Account("user2", 2, 200, "email2", "phone2"));
    tree.insert(Account("user3", 3, 300, "email3", "phone3"));
    cout << "Printing all users:" << endl;
    tree.printUsers();
}

void testUTreeDump() {
    UTree tree;
    tree.insert(Account("user1", 1, 100, "email1", "phone1"));
    tree.insert(Account("user2", 2, 200, "email2", "phone2"));
    tree.insert(Account("user3", 3, 300, "email3", "phone3"));
    cout << "Dumping the tree structure:" << endl;
    tree.dump(tree.getRoot());
}

int main() {
    /*testDestructor();
    testCopyConstructor();
    testInsert();
    testRemove();
    testUpdateFunctions();
    testUpdateFunctions2();
    testDuplicateInsertions();
    testEdgeCases();
    testTreeBalancing();
    return 0;*/
    testUTreeDestructor();
    testUTreeUpdateHeight();
    testUTreeRemoveUser();
    testUTreeRetrieve();
    testUTreeRetrieveUser();
    testUTreeNumUsers();
    testUTreePrintUsers();
    testUTreeDump();
    return 0;
}