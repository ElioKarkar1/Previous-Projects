/*********************************************************************************
 * Elio Karkar
 * Dictionary.cpp
 * Dictionary ADT in C++
 *********************************************************************************/
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include "Dictionary.h"

Dictionary::Node::Node(keyType k, valType v)
{
    key = k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
    color = 0;
}

Dictionary::Dictionary()
{
    nil = new Node("", -4);
    root = nil;
    current = nil;
    num_pairs = 0;
}

Dictionary::Dictionary(const Dictionary &D)
{
    nil = new Node("", -4);
    root = nil;
    current = nil;
    num_pairs = 0;
    preOrderCopy(D.root, D.nil);
}

Dictionary::~Dictionary()
{
    postOrderDelete(root);
    delete (nil);
}

void Dictionary::inOrderString(std::string &s, Node *R) const
{
    if (R != nil && R != NULL)
    {
        inOrderString(s, R->left);
        s.append(R->key);
        s.append(" : ");
        s.append(std::to_string(R->val));
        s.append("\n");
        inOrderString(s, R->right);
    }
}

void Dictionary::preOrderString(std::string &s, Node *R) const
{
    if (R != nil && R != NULL)
    {
        s.append(R->key + "\n");
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}

void Dictionary::preOrderCopy(Node *R, Node *N)
{
    if (R != N)
    {
        if (R != nil && R != NULL)
        {
            setValue(R->key, R->val);  // copy the value of the current node
            preOrderCopy(R->left, N);  // recursively copy the left subtree
            preOrderCopy(R->right, N); // recursively copy the right subtree
        }
    }
}

void Dictionary::postOrderDelete(Node *R)
{
    if (R == nil || R == NULL)
    {
        return;
    }
    else if (R == root)
    {
        root = nil;
        current = nil;
    }

    postOrderDelete(R->left);  // recursively delete the left subtree
    postOrderDelete(R->right); // recursively delete the right subtree

    delete (R); // delete the current node
}

Dictionary::Node *Dictionary::search(Node *R, keyType k) const
{
    if (R->key == k || R == nil)
    {
        return R; // found the desired node
    }
    if (k < R->key)
    {
        return search(R->left, k); // recursively search the left subtree
    }
    else
    {
        return search(R->right, k); // recursively search the right subtree
    }
}

Dictionary::Node *Dictionary::findMin(Node *R)
{
    if (size() != 0)
    {
        Node *iter = R;
        while (iter->left != nil)
        {
            iter = iter->left;
        }
        return iter;
    }
    return nil;
}

Dictionary::Node *Dictionary::findMax(Node *R)
{
    if (size() != 0)
    {
        Node *iter = R;
        while (iter->right != nil)
        {
            iter = iter->right;
        }
        return iter;
    }
    return nil;
}

Dictionary::Node *Dictionary::findNext(Node *N)
{
    Node *iter = N;
    if (iter->right != nil)
    {
        return findMin(iter->right);
    }
    Node *iter2 = N->parent;
    while (iter2 != nil && iter == iter2->right)
    {
        iter = iter2;
        iter2 = iter2->parent;
    }
    return iter2;
}

Dictionary::Node *Dictionary::findPrev(Node *N)
{
    Node *iter = N;
    if (iter->left != nil)
    {
        return findMax(iter->left);
    }
    Node *iter2 = N->parent;
    while (iter2 != nil && iter == iter2->left)
    {
        iter = iter2;
        iter2 = iter2->parent;
    }
    return iter2;
}

void Dictionary::RightRotate(Node *x)
{
    // Set y to the left child of x
    Node *y = x->left;

    // Turn y's right subtree into x's left subtree
    x->left = y->right;
    if (y->right != nil)
    { // If using sentinel nil node, this check is not necessary
        y->right->parent = x;
    }

    // Link y's parent to x
    y->parent = x->parent;
    if (x->parent == nil)
    {
        root = y;
    }
    else if (x == x->parent->right)
    {
        x->parent->right = y;
    }
    else
    {
        x->parent->left = y;
    }

    // Put x on y's right
    y->right = x;
    x->parent = y;
}

void Dictionary::LeftRotate(Node *x)
{
    // set y
    Node *y = x->right;

    // turn y's left subtree into x's right subtree
    x->right = y->left;
    if (y->left != nil)
    { // not necessary if using sentinel nil node
        y->left->parent = x;
    }

    // link y's parent to x
    y->parent = x->parent;
    if (x->parent == nil)
    {
        root = y;
    }
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }

    // put x on y's left
    y->left = x;
    x->parent = y;
}

// Insertion fix-up for red-black trees
void Dictionary::RB_InsertFixUp(Node *z)
{
    // Keep fixing up until the parent of the inserted node is black
    while (z->parent && z->parent->color == 1)
    {
        // If the parent of the inserted node is the left child of its parent
        if (z->parent == z->parent->parent->left)
        {
            Node *y = z->parent->parent->right; // Get the uncle of the inserted node
            // If the uncle of the inserted node is red
            if (y && y->color == 1)
            {
                z->parent->color = 0;         // Case 1: recolor the parent to black
                y->color = 0;                 // Case 1: recolor the uncle to black
                z->parent->parent->color = 1; // Case 1: recolor the grandparent to red
                z = z->parent->parent;        // Case 1: move up two levels to continue fixing up
            }
            else
            {
                // If the uncle of the inserted node is black
                if (z == z->parent->right)
                {
                    z = z->parent; // Case 2: move up one level and rotate left
                    LeftRotate(z);
                }
                z->parent->color = 0;           // Case 3: recolor the parent to black
                z->parent->parent->color = 1;   // Case 3: recolor the grandparent to red
                RightRotate(z->parent->parent); // Case 3: rotate right around the grandparent
            }
        }
        else
        {
            // If the parent of the inserted node is the right child of its parent (symmetric to the above case)
            Node *y = z->parent->parent->left;
            if (y && y->color == 1)
            {
                z->parent->color = 0;
                y->color = 0;
                z->parent->parent->color = 1;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    RightRotate(z);
                }
                z->parent->color = 0;
                z->parent->parent->color = 1;
                LeftRotate(z->parent->parent);
            }
        }
    }
    // Color the root node black
    root->color = 0;
}

void Dictionary::RB_Transplant(Node *u, Node *v)
{
    // If u is the root of the tree, replace it with v
    if (u->parent == nil)
    {
        root = v;
    }
    // Otherwise, replace u with v as u's parent's child
    else if (u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    // Set the parent of v to be the parent of u
    v->parent = u->parent;
}

void Dictionary::RB_DeleteFixUp(Node *x)
{
    // While x is not the root and its color is black
    while (x != root && x->color == 0)
    {
        // If x is the left child of its parent
        if (x == x->parent->left)
        {
            Node *w = x->parent->right; // Set w as x's sibling

            // Case 1: w is red
            if (w->color == 1)
            {
                w->color = 0;          // Set w's color to black
                x->parent->color = 1;  // Set x's parent's color to red
                LeftRotate(x->parent); // Rotate left at x's parent
                w = x->parent->right;  // Update w to x's new sibling
            }

            // Case 2: w and its children are black
            if (w->left->color == 0 && w->right->color == 0)
            {
                w->color = 1;  // Set w's color to red
                x = x->parent; // Move up the tree to x's parent
            }
            else
            {
                // Case 3: w's right child is black
                if (w->right->color == 0)
                {
                    w->left->color = 0;   // Set w's left child to black
                    w->color = 1;         // Set w's color to red
                    RightRotate(w);       // Rotate right at w
                    w = x->parent->right; // Update w to x's new sibling
                }

                // Case 4: w's right child is red
                w->color = x->parent->color; // Set w's color to x's parent's color
                x->parent->color = 0;        // Set x's parent's color to black
                w->right->color = 0;         // Set w's right child to black
                LeftRotate(x->parent);       // Rotate left at x's parent
                x = root;                    // Terminate the loop
            }
        }
        else
        {                              // If x is the right child of its parent
            Node *w = x->parent->left; // Set w as x's sibling

            // Case 5: w is red
            if (w->color == 1)
            {
                w->color = 0;           // Set w's color to black
                x->parent->color = 1;   // Set x's parent's color to red
                RightRotate(x->parent); // Rotate right at x's parent
                w = x->parent->left;    // Update w to x's new sibling
            }

            if (w->right->color == 0 && w->left->color == 0)
            {
                w->color = 1;  // case 6
                x = x->parent; // case 6
            }
            else
            {
                if (w->left->color == 0)
                {
                    w->right->color = 0; // case 7
                    w->color = 1;        // case 7
                    LeftRotate(w);       // case 7
                    w = x->parent->left; // case 7
                }
                w->color = x->parent->color; // case 8
                x->parent->color = 0;        // case 8
                w->left->color = 0;          // case 8
                RightRotate(x->parent);      // case 8
                x = root;                    // case 8
            }
        }
    }
    x->color = 0;
}

void Dictionary::RB_Delete(Node *z)
{
    Node *y = z;
    int y_original_color = y->color; // Store the original color of y
    Node *x;
    if (z->left == nil)
    {                               // Case 1: z has no left child, replace z with its right child
        x = z->right;               // Set x to z's right child
        RB_Transplant(z, z->right); // Replace z with z's right child
    }
    else if (z->right == nil)
    {                              // Case 2: z has no right child, replace z with its left child
        x = z->left;               // Set x to z's left child
        RB_Transplant(z, z->left); // Replace z with z's left child
    }
    else
    {                                // Case 3: z has both left and right children, replace z with its successor y
        y = findMin(z->right);       // Find z's successor y
        y_original_color = y->color; // Store the original color of y
        x = y->right;                // Set x to y's right child
        if (y->parent == z)
        {                  // Case 3.1: y is z's right child, replace z with y
            x->parent = y; // Set x's parent to y
        }
        else
        {                               // Case 3.2: y is not z's right child, replace y with its right child and replace z with y
            RB_Transplant(y, y->right); // Replace y with its right child
            y->right = z->right;        // Set y's right child to z's right child
            y->right->parent = y;       // Set y's right child's parent to y
        }
        RB_Transplant(z, y); // Replace z with y
        y->left = z->left;   // Set y's left child to z's left child
        y->left->parent = y; // Set y's left child's parent to y
        y->color = z->color; // Copy the color of z to y
    }
    if (y_original_color == 0)
    { // If the original color of y was black, fix the tree to maintain red-black properties
        RB_DeleteFixUp(x);
    }
}

int Dictionary::size() const
{
    return num_pairs;
}

bool Dictionary::contains(keyType k) const
{
    if (search(root, k) != nil)
    {
        return true;
    }
    else
    {
        return false;
    }
}

valType &Dictionary::getValue(keyType k) const
{
    Node *found = search(root, k);
    if (found != nil)
    {
        return found->val;
    }
    else
    {
        throw std::logic_error("key " + k + " does not exist");
    }
    return nil->val;
}

bool Dictionary::hasCurrent() const
{
    if (current != nil)
    {
        return true;
    }
    else
    {
        return false;
    }
}

keyType Dictionary::currentKey() const
{
    if (hasCurrent() == true)
    {
        return current->key;
    }
    else if (hasCurrent() == false)
    {
        throw std::logic_error("current not defined");
    }
    return nil->key;
}

valType &Dictionary::currentVal() const
{
    if (hasCurrent() == true)
    {
        return current->val;
    }
    else if (hasCurrent() == false)
    {
        throw std::logic_error("current not defined");
    }
    return nil->val;
}

void Dictionary::clear()
{
    postOrderDelete(root);
    num_pairs = 0;
    root = nil;
    current = nil;
}

void Dictionary::setValue(keyType k, valType v)
{
    Node *newNode = new Node(k, v);
    newNode->left = nil;
    newNode->right = nil;
    newNode->parent = nil;
    Node *currentNode = root;
    Node *parentNode = nil;

    while (currentNode != nil)
    {
        parentNode = currentNode;
        if (k == currentNode->key)
        {
            currentNode->val = v;
            delete newNode;
            return;
        }
        else if (k < currentNode->key)
        {
            currentNode = currentNode->left;
        }
        else
        {
            currentNode = currentNode->right;
        }
    }

    newNode->parent = parentNode;
    if (parentNode == nil)
    {
        root = newNode;
    }
    else if (newNode->key < parentNode->key)
    {
        parentNode->left = newNode;
    }
    else
    {
        parentNode->right = newNode;
    }
    newNode->color = 1;
    num_pairs = num_pairs + 1;
    RB_InsertFixUp(newNode);
}

void Dictionary::remove(keyType key)
{
    // Check if the key exists in the dictionary
    if (contains(key) == false)
    {
        throw std::logic_error("remove() Error:  \"" + key + "\" does not exist");
    }
    else
    {
        // Find the node containing the key to be removed
        Node *nodeToDelete = search(root, key);

        // Update the current pointer if it points to the node being deleted
        if (current == nodeToDelete)
        {
            current = nil;
        }

        // Delete the node using the Red-Black Tree delete algorithm
        RB_Delete(nodeToDelete);

        // Decrement the number of key-value pairs in the dictionary
        num_pairs--;
    }
}

void Dictionary::begin()
{
    if (size() > 0)
    {
        current = findMin(root);
    }
}

void Dictionary::end()
{
    if (size() > 0)
    {
        current = findMax(root);
    }
}

void Dictionary::next()
{
    if (hasCurrent() == false)
    {
        return;
    }
    if (current == findMax(root))
    {
        current = nil;
    }
    else if (hasCurrent())
    {
        current = findNext(current);
    }
}

void Dictionary::prev()
{
    if (hasCurrent() == false)
    {
        throw std::logic_error("current is not defined");
    }
    if (current == findMin(root))
    {
        current = nil;
    }
    else if (hasCurrent())
    {
        current = findPrev(current);
    }
}

std::string Dictionary::to_string() const
{
    std::string s = "";
    inOrderString(s, root);
    return s;
}

std::string Dictionary::pre_string() const
{
    std::string s = "";
    preOrderString(s, root);
    return s;
}

bool Dictionary::equals(const Dictionary &D) const
{
    Dictionary first = *this;
    Dictionary second = D;
    int first_size = first.size();
    int second_size = second.size();
    // First, check if the sizes are the same
    if (first_size != second_size)
    {
        return false;
    }
    std::string a = first.to_string();
    std::string b = second.to_string();
    if (a != b)
    {
        return false;
    }
    // If we made it this far, all keys and values are equal
    return true;
}

std::ostream &operator<<(std::ostream &stream, Dictionary &D)
{
    return stream << D.Dictionary::to_string();
}

bool operator==(const Dictionary &A, const Dictionary &B)
{
    return A.Dictionary::equals(B);
}

Dictionary &Dictionary::operator=(const Dictionary &D)
{
    if (this != &D)
    {
        if (num_pairs > 0)
        {
            postOrderDelete(root);
        }
        preOrderCopy(D.root, D.nil);
    }
    return *this;
}
