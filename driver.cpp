#define ASSERT_EQUALS(x, y) {\
    if (x != y) \
        std::cout << __FUNCTION__ << " failed equality on line " << __LINE__\
        << "; expected: " << x << ", got: " << y << std::endl;}


#define ASSERT_NOT_EQUALS(x, y) {\
    if (x == y) \
        std::cout << __FUNCTION__ << " failed inequality on line " << __LINE__\
        << "; expected: not " << x << ", got: " << y << std::endl;}

#include "dtree.h"

class MyTest {
public:
    void testRoot();
    void testInsert();
    void testInsert2();
    void testInsertVacancies();
    void testVacancySizes();

    void testRebalance();

    void testRebalanceDepthOne();

    void testRebalanceVacancies();
};

void MyTest::testRoot() {
    // create a root
    DTree* dtree = new DTree;
    ASSERT_EQUALS(dtree->getRoot(), nullptr);
    ASSERT_EQUALS(dtree->getNumUsers(), 0);

    // add an account
    Account account("", 1, false, "", "");
    dtree->insert(account);
    ASSERT_NOT_EQUALS(nullptr, dtree->getRoot());
    ASSERT_EQUALS(1, dtree->getNumUsers());
    ASSERT_EQUALS(0, dtree->getRoot()->getNumVacant());

    // attempt a false deletion
    DNode* removed;
    ASSERT_EQUALS(dtree->remove(0, removed), false);

    // actually delete the account
    bool removeSuccess = dtree->remove(1, removed);
    ASSERT_EQUALS(removeSuccess, true);
    ASSERT_EQUALS(dtree->getNumUsers(), 0);
    ASSERT_EQUALS(dtree->getRoot()->getNumVacant(), 1);
    ASSERT_EQUALS(removed->getDiscriminator(), 1);

    // attempt another false deletion
    ASSERT_EQUALS(dtree->remove(0, removed), false);
}

void MyTest::testInsert() {
    DTree* dtree = new DTree;
    Account account("", 1, false, "", "");
    dtree->insert(account);
    Account left("", 0, false, "", "");
    dtree->insert(left);
    ASSERT_EQUALS(2, dtree->getNumUsers());

    ASSERT_EQUALS(nullptr, dtree->getRoot()->getRight());
    ASSERT_NOT_EQUALS(nullptr, dtree->getRoot()->getLeft());

    ASSERT_EQUALS(0, dtree->getRoot()->getNumVacant());
    ASSERT_EQUALS(0, dtree->getRoot()->getLeft()->getNumVacant());
    ASSERT_EQUALS(1, dtree->getRoot()->getLeft()->getSize());

    Account right("", 2, false, "", "");
    dtree->insert(right);
    ASSERT_NOT_EQUALS(nullptr, dtree->getRoot()->getRight());
    ASSERT_EQUALS(3, dtree->getNumUsers());
    ASSERT_EQUALS(1, dtree->getRoot()->getLeft()->getSize());
    ASSERT_EQUALS(1, dtree->getRoot()->getRight()->getSize());
}

// I love proof by induction!!!
void MyTest::testInsert2() {
    DTree* dtree = new DTree;
    Account account("", 5, false, "", "");
    dtree->insert(account);
    Account left("", 3, false, "", "");
    dtree->insert(left);
    Account right("", 7, false, "", "");
    dtree->insert(right);

    Account leftleft("", 2, false, "", "");
    dtree->insert(leftleft);
    ASSERT_EQUALS(4, dtree->getNumUsers());

    Account leftright("", 1, false, "", "");
    dtree->insert(leftright);
    ASSERT_EQUALS(5, dtree->getNumUsers());

    Account rightleft("", 6, false, "", "");
    dtree->insert(rightleft);
    ASSERT_EQUALS(6, dtree->getNumUsers());

    Account rightright("", 8, false, "", "");
    dtree->insert(rightright);
    ASSERT_EQUALS(7, dtree->getNumUsers());
}

void MyTest::testInsertVacancies() {
    DTree* dtree = new DTree;
    Account account("", 4, false, "", "");
    dtree->insert(account);
    Account left("", 2, false, "", "");
    dtree->insert(left);
    Account right("", 6, false, "", "");
    dtree->insert(right);
    Account leftleft("", 1, false, "", "");
    dtree->insert(leftleft);
    Account leftright("", 3, false, "", "");
    dtree->insert(leftright);
    Account rightleft("", 5, false, "", "");
    dtree->insert(rightleft);
    Account rightright("", 7, false, "", "");
    dtree->insert(rightright);

    // let us remove at depth = 1
    DNode* removed;
    dtree->remove(2, removed);
    ASSERT_EQUALS(6, dtree->getNumUsers());
    ASSERT_EQUALS(1, dtree->getRoot()->getNumVacant());
    ASSERT_EQUALS(2, removed->getDiscriminator());

    dtree->insert(left);
    ASSERT_EQUALS(7, dtree->getNumUsers());
    ASSERT_EQUALS(0, dtree->getRoot()->getNumVacant());

    // let us remove at depth = 1 and depth = 2
    dtree->remove(2, removed);
    ASSERT_EQUALS(6, dtree->getNumUsers());
    ASSERT_EQUALS(1, dtree->getRoot()->getNumVacant());
    ASSERT_EQUALS(2, removed->getDiscriminator());

    // now @ depth = 2
    dtree->remove(1, removed);
    ASSERT_EQUALS(5, dtree->getNumUsers());
    ASSERT_EQUALS(2, dtree->getRoot()->getNumVacant());
    ASSERT_EQUALS(1, removed->getDiscriminator());

    // re insert depth 2
    dtree->insert(leftleft);
    ASSERT_EQUALS(6, dtree->getNumUsers());
    ASSERT_EQUALS(1, dtree->getRoot()->getNumVacant());

    // re insert depth 1
    dtree->insert(left);
    ASSERT_EQUALS(7, dtree->getNumUsers());
    ASSERT_EQUALS(0, dtree->getRoot()->getNumVacant());
}

void MyTest::testVacancySizes() {
    DTree* dtree = new DTree;
    Account account("", 1, false, "", "");
    dtree->insert(account);
    Account left("", 0, false, "", "");
    dtree->insert(left);
    Account right("", 2, false, "", "");
    dtree->insert(right);

    DNode* removed;
    dtree->remove(0, removed);
    ASSERT_EQUALS(2, dtree->getRoot()->getSize());
    ASSERT_EQUALS(1, dtree->getRoot()->getNumVacant());
    ASSERT_EQUALS(0, removed->getDiscriminator());
}

void MyTest::testRebalance() {
    DTree* dtree = new DTree;
    Account five("", 5, false, "", "");
    dtree->insert(five);
    Account four("", 4, false, "", "");
    dtree->insert(four);
    Account three("", 3, false, "", "");
    dtree->insert(three);
    Account two("", 2, false, "", "");
    dtree->insert(two);
    Account one("", 1, false, "", "");
    dtree->insert(one);

    Account six("", 6, false, "", "");
    dtree->insert(six);
    Account seven("", 7, false, "", "");
    dtree->insert(seven);
    Account eight("", 8, false, "", "");
    dtree->insert(eight);
    Account nine("", 9, false, "", "");
    dtree->insert(nine);
    Account ten("", 10, false, "", "");
    dtree->insert(ten);
    Account eleven("", 11, false, "", "");
    dtree->insert(eleven);
    // rebalancing starts here
    Account twelve("", 12, false, "", "");
    dtree->insert(twelve);
}

void MyTest::testRebalanceDepthOne() {
    DTree* dtree = new DTree;
    Account thirteen("", 13, false, "", "");
    dtree->insert(thirteen);
    // goal is to rebalance at 5
    ASSERT_EQUALS(1, dtree->getNumUsers());
    Account five("", 5, false, "", "");
    dtree->insert(five);
    ASSERT_EQUALS(2, dtree->getNumUsers());
    Account fourteen("", 14, false, "", "");
    dtree->insert(fourteen);
    ASSERT_EQUALS(3, dtree->getNumUsers());
    Account four("", 4, false, "", "");
    dtree->insert(four);
    ASSERT_EQUALS(4, dtree->getNumUsers());
    Account three("", 3, false, "", "");
    dtree->insert(three);
    ASSERT_EQUALS(5, dtree->getNumUsers());
    Account two("", 2, false, "", "");
    dtree->insert(two);
    ASSERT_EQUALS(6, dtree->getNumUsers());
    Account one("", 1, false, "", "");
    dtree->insert(one);
    ASSERT_EQUALS(7, dtree->getNumUsers());
    Account six("", 6, false, "", "");
    dtree->insert(six);
    ASSERT_EQUALS(8, dtree->getNumUsers());
    Account seven("", 7, false, "", "");
    dtree->insert(seven);
    ASSERT_EQUALS(9, dtree->getNumUsers());
    Account eight("", 8, false, "", "");
    dtree->insert(eight);
    ASSERT_EQUALS(10, dtree->getNumUsers());
    Account nine("", 9, false, "", "");
    dtree->insert(nine);
    ASSERT_EQUALS(11, dtree->getNumUsers());
    Account ten("", 10, false, "", "");
    dtree->insert(ten);
    ASSERT_EQUALS(12, dtree->getNumUsers());
    Account eleven("", 11, false, "", "");
    dtree->insert(eleven);
    ASSERT_EQUALS(13, dtree->getNumUsers());
    Account twelve("", 12, false, "", "");
    dtree->insert(twelve);
    ASSERT_EQUALS(14, dtree->getNumUsers());
}

void MyTest::testRebalanceVacancies() {
    DTree* dtree = new DTree;
    Account thirteen("", 13, false, "", "");
    dtree->insert(thirteen);
    // goal is to rebalance at 5
    ASSERT_EQUALS(1, dtree->getNumUsers());
    Account five("", 5, false, "", "");
    dtree->insert(five);
    ASSERT_EQUALS(2, dtree->getNumUsers());
    DNode* removed;
    dtree->remove(5, removed);
    ASSERT_EQUALS(1, dtree->getNumUsers());
    Account fourteen("", 14, false, "", "");
    dtree->insert(fourteen);
    ASSERT_EQUALS(2, dtree->getNumUsers());
    Account four("", 4, false, "", "");
    dtree->insert(four);
    ASSERT_EQUALS(3, dtree->getNumUsers());
    Account three("", 3, false, "", "");
    dtree->insert(three);
    ASSERT_EQUALS(4, dtree->getNumUsers());
    Account two("", 2, false, "", "");
    dtree->insert(two);
    ASSERT_EQUALS(5, dtree->getNumUsers());
    Account one("", 1, false, "", "");
    dtree->insert(one);
    ASSERT_EQUALS(6, dtree->getNumUsers());
    Account six("", 6, false, "", "");
    dtree->insert(six);
    ASSERT_EQUALS(7, dtree->getNumUsers());
    Account seven("", 7, false, "", "");
    dtree->insert(seven);
    ASSERT_EQUALS(8, dtree->getNumUsers());
    Account eight("", 8, false, "", "");
    dtree->insert(eight);
    ASSERT_EQUALS(9, dtree->getNumUsers());
    Account nine("", 9, false, "", "");
    dtree->insert(nine);
    ASSERT_EQUALS(10, dtree->getNumUsers());
    Account ten("", 10, false, "", "");
    dtree->insert(ten);
    ASSERT_EQUALS(11, dtree->getNumUsers());
    Account eleven("", 11, false, "", "");
    dtree->insert(eleven);
    ASSERT_EQUALS(12, dtree->getNumUsers());
    Account twelve("", 12, false, "", "");
    dtree->insert(twelve);
    ASSERT_EQUALS(13, dtree->getNumUsers());
}

int main() {
    MyTest root;
    root.testRoot();
    root.testInsert();
    root.testInsert2();
    root.testVacancySizes();
    root.testInsertVacancies();
    // rebalance at depth = 0
    root.testRebalance();
    // rebalance at depth = 1
    root.testRebalanceDepthOne();
    // rebalance with vacant nodes
    root.testRebalanceVacancies();
}