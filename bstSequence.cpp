/*
 * Name: Mathew Breland
 * Date Submitted: 4/4/2023
 * Lab Section: 001
 * Assignment Name: Lab 8 
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <assert.h>

#include "bstSequence.h"

using namespace std;

void fix_size(Node* T)
{
    T->size = 1;
    if (T->left) T->size += T->left->size;
    if (T->right) T->size += T->right->size;
}


// insert value v at rank r
Node* insert(Node* T, int v, int r)
{
    if (T == nullptr) {
        // if node T s equal to nullptr, returns a new node
        return new Node(v);
    }

    // given assert function 
    assert(r >= 0 && r <= T->size);

    int rankRoot = T->left ? T->left->size : 0; // creation of a rank root that is used many times during the lab 

    if (r <= rankRoot) {

        T->left = insert(T->left, v, r);    // if rank is less than rankRoot, recursively call insert function 
    }

    else {

        T->right = insert(T->right, v, r - rankRoot - 1);   // else (if r >= rankRoot) insert recursive call to right
    }

    fix_size(T);    // calling fix size function before returning T
    return T;
}


// returns a vector of key values corresponding to the inorder traversal of T (i.e., the contents of T in sorted order)
vector<int> inorder_traversal(Node* T)
{
    vector<int> inorder;
    vector<int> rhs;
    if (T == nullptr) return inorder;
    inorder = inorder_traversal(T->left);
    inorder.push_back(T->key);
    rhs = inorder_traversal(T->right);
    inorder.insert(inorder.end(), rhs.begin(), rhs.end());
    return inorder;
}

// return pointer to node of rank r (with r'th largest key; e.g. r=0 is the minimum)
Node* select(Node* T, int r)
{
    // given assert function 
    assert(T != nullptr && r >= 0 && r < T->size);

    int rankRoot = T->left ? T->left->size : 0; // same ranked function as before used in all of the functions 

    if (r == rankRoot) {
        // if rank is equal to rankRoot, return node T
        return T;
    }

    if (r < rankRoot) { 
        // if rank is less than rankRoot, recursively call select left (because <)
        return select(T->left, r);
    }

    else {    
        // if rank is more than rankRoot, recursively call select right (because >)
        return select(T->right, r - rankRoot - 1);
    }
}

// Split tree T on rank r into tree L (containing ranks < r) and 
// a tree R (containing ranks >= r)
void split(Node* T, int r, Node** L, Node** R)
{

    if (T == NULL) {    // if T is equal to null, set L and R to null as well and return (invalid)
        *L = NULL;
        *R = NULL;
        return;
    }

    // given assert function
    assert(r >= 0 && r <= T->size);

    int rankRoot = T->left ? T->left->size : 0; // same ranked function as before 

    if (r <= rankRoot) {    // if rank is less than rankRoot, recurisvely call split using left (because <)

        split(T->left, r, L, &T->left);

        *R = T;
    }

    else {                  // else (if rank is > rankRoot), recursively call split with right

        split(T->right, r - rankRoot - 1, &T->right, R);    // r - rankRoot - 1 because other variations were not working

        *L = T;
    }

    fix_size(T);    // calling fix size
}


Node* insert_random(Node* T, int v, int r)
{
    if (T == NULL) return new Node(v);  // if T is null, return a new node 

    if (rand() % (T->size + 1) == 0) {  // random function used to randomly insert 

        Node* newRoot = new Node(v);    // creating a new root at new node v 

        split(T, r, &newRoot->left, &newRoot->right);   // calling splitting function 

        fix_size(newRoot);

        return newRoot;
    }

    // otherwise insert recursively

    // given assert  function 
    assert(r >= 0 && r <= T->size);

    int rankRoot = T->left ? T->left->size : 0; // same ranked function used in the other parts 

    if (r <= rankRoot) T->left = insert_random(T->left, v, r);

    else T->right = insert_random(T->right, v, r - rankRoot - 1);

    fix_size(T);

    return T;
}

// Returns true if team x defeated team y
// Leave this function unmodified
bool did_x_beat_y(int x, int y)
{
    assert(x != y);
    if (x > y) return !did_x_beat_y(y, x);
    unsigned long long lx = x;
    unsigned long long ly = y;
    return ((17 + 8321813 * lx + 1861 * ly) % 1299827) % 2 == 0;
}

// Return a BST containing a valid ordering of n teams
Node* order_n_teams(int n)
{
    Node* T = nullptr;

    // start by inserting the first team
    T = insert_random(T, 0, 0);

    // now insert the other teams...
    for (int i = 1; i < n; i++) {
        // insert team i so the sequence encoded by the BST remains valid
        if (did_x_beat_y(i, select(T, 0)->key)) // can we insert at beginning?
            T = insert_random(T, i, 0);
        else if (did_x_beat_y(select(T, T->size - 1)->key, i)) // can we insert at end?
            T = insert_random(T, i, T->size);
        else {
            // 7 5 4 2 0 3 1 6    (when inserting team i=8)
            // W W W L L L W L
            int start = 0, end = T->size - 1;
            while (end - start != 1) {
                int mid = (start + end) / 2;
                if (did_x_beat_y(i, select(T, mid)->key))
                    end = mid;
                else
                    start = mid;
            }
            T = insert(T, i, end);
        }
    }
    return T;
}

void printVector(vector<int> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i] << endl;
    }
}

/*/ int main()
{
    Node* T = nullptr;
    std::vector<int> test;
    for (int i = 1; i <= 1000000; i++) test.push_back(i);
    for (int i = 0; i < 1000000; i++) T = insert_random(T, test[i], T ? T->size / 2 : 0);
    std::vector<int> test2;
    for (int i = 1; i <= 500000; i++) test2.push_back(i * 2);
    for (int i = 500000; i >= 1; i--) test2.push_back(i * 2 - 1);
    std::vector<int> inorder = inorder_traversal(T);

    //ASSERT_THAT(inorder, ElementsAreArray(test2));
    for (int i = 0; i < 1000000; i++)
    {
        if (inorder[i] != test2[i])
        {
            std::cout << "Mismatch at position " << i << ": expected/correct value " << test2[i] << ", actual value when testing " << inorder[i] << ".\n";
            return 1;
        }
    }
    std::cout << "Passed" << std::endl;
    return 0;
}/**/