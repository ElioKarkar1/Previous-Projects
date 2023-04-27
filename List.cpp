/*********************************************************************************
 * Karkar Elio
 * List.cpp
 * List ADT
 *********************************************************************************/

#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include "List.h"

// private Node struct
List::Node::Node(ListElement x)
{
    data = x;
    next = nullptr;
    prev = nullptr;
}

// Creates new List in the empty state.
List::List()
{
    frontDummy = new Node(0);
    backDummy = new Node(7);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
}
// Copy constructor.
List::List(const List &L)
{
    frontDummy = new Node(0);
    backDummy = new Node(7);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
    // Create a pointer to the first node of the old list
    Node *current = L.frontDummy->next;

    // Iterate through the old list
    while (current != L.backDummy)
    {
        // Create a new node with the same value as the current node of the old list
        Node *new_node = new Node(current->data);
        // Insert the new node at the back of the new list
        insertBefore(new_node->data);
        // Move to the next node of the old list
        current = current->next;
    }
}

List::~List()
{
    // Delete all nodes in the list
    Node *current = frontDummy;
    while (current != nullptr)
    {
        Node *next_node = current->next;
        delete current;
        current = next_node;
    }
}

int List::length() const
{
    return num_elements;
}

ListElement List::front() const
{
    if (num_elements == 0)
    {
        // If the list is empty, throw an exception
        throw std::out_of_range("List is empty");
    }
    // Return the data in the first node of the list
    return frontDummy->next->data;
}

ListElement List::back() const
{
    if (num_elements == 0)
    {
        // If the list is empty, throw an exception
        throw std::out_of_range("List is empty");
    }
    // Return the data in the last node of the list
    return backDummy->prev->data;
}

int List::position() const
{
    return pos_cursor;
}

ListElement List::peekNext() const
{
    if (pos_cursor >= num_elements)
    {
        // If the cursor is at the end of the list, throw an exception
        throw std::out_of_range("Cursor is at the end of the list");
    }
    // Return the data in the node after the cursor
    return afterCursor->data;
}

ListElement List::peekPrev() const
{
    if (pos_cursor <= 0)
    {
        // If the cursor is at the beginning of the list, throw an exception
        throw std::out_of_range("Cursor is at the beginning of the list");
    }
    // Return the data in the node before the cursor
    return beforeCursor->data;
}

void List::clear()
{
    // Traverse the list, deleting each node
    Node *current = frontDummy->next;
    while (current != backDummy)
    {
        Node *next = current->next;
        delete current;
        current = next;
    }
    // Reset the list fields to the empty state
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
}

void List::moveFront()
{
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
    pos_cursor = 0;
}

void List::moveBack()
{
    beforeCursor = backDummy->prev;
    afterCursor = backDummy;
    pos_cursor = num_elements;
}

ListElement List::moveNext()
{
    if (pos_cursor >= num_elements)
    {
        // if cursor is at back of list, can't move next
        throw std::out_of_range("Cursor is at the back of the list");
    }
    ListElement next_element = afterCursor->data;
    beforeCursor = afterCursor;
    afterCursor = afterCursor->next;
    pos_cursor++;
    return next_element;
}

ListElement List::movePrev()
{
    if (pos_cursor <= 0)
    {
        // if cursor is at front of list, can't move prev
        throw std::out_of_range("Cursor is at the front of the list");
    }
    ListElement prev_element = beforeCursor->data;
    afterCursor = beforeCursor;
    beforeCursor = beforeCursor->prev;
    pos_cursor--;
    return prev_element;
}

void List::insertAfter(ListElement x)
{
    Node *new_node = new Node(x);
    new_node->next = afterCursor;
    new_node->prev = beforeCursor;
    beforeCursor->next = new_node;
    afterCursor->prev = new_node;
    afterCursor = new_node;
    num_elements++;
}

void List::insertBefore(ListElement x)
{
    Node *new_node = new Node(x);
    new_node->next = afterCursor;
    new_node->prev = beforeCursor;
    beforeCursor->next = new_node;
    afterCursor->prev = new_node;
    beforeCursor = new_node;
    num_elements++;
    pos_cursor++;
}

void List::setAfter(ListElement x)
{
    if (position() == length())
    {
        throw "Error: Cannot call setAfter() when the cursor is at the end of the list.";
    }
    afterCursor->data = x;
}

void List::setBefore(ListElement x)
{
    if (position() == 0)
    {
        throw "Error: Cannot call setBefore() when the cursor is at the front of the list.";
    }
    beforeCursor->data = x;
}

void List::eraseAfter()
{
    if (position() >= num_elements)
    {
        throw "Error: Cannot call eraseAfter() when the cursor is at the back of the list.";
    }
    Node *oldNode = afterCursor;
    afterCursor = oldNode->next;
    beforeCursor->next = afterCursor;
    afterCursor->prev = beforeCursor;
    delete oldNode;
    num_elements--;
}

void List::eraseBefore()
{
    if (pos_cursor <= 0)
    {
        throw std::out_of_range("Cursor is at the beginning of the list");
    }
    beforeCursor->next->prev = beforeCursor->prev;
    beforeCursor->prev->next = beforeCursor->next;
    delete (beforeCursor);
    beforeCursor = afterCursor->prev;
    num_elements--;
    pos_cursor--;
}

int List::findNext(ListElement x)
{
    while (pos_cursor < num_elements)
    {                         // while cursor is not at end of list
        int cur = moveNext(); // move to next element
        if (cur == x)
        {                      // if next element is the target element
            return position(); // return its position
        }
    }
    moveFront(); // reset cursor to front of list
    return -1;   // target element not found, return -1
}

int List::findPrev(ListElement x)
{
    while (pos_cursor > 0)
    {                         // while cursor is not at front of list
        int cur = movePrev(); // move to previous element
        if (cur == x)
        {                      // if previous element is the target element
            return position(); // return its position
        }
    }
    moveBack(); // reset cursor to back of list
    return -1;  // target element not found, return -1
}

void List::cleanup()
{
    // Pointer to current node being examined
    Node *current_node = frontDummy->next;

    // Pointer to previous node
    Node *previous_node = nullptr;

    // Pointer to a duplicate node
    Node *duplicate_node = nullptr;

    // Current position in the list
    int current_position = 0;

    // Position of previous node
    int previous_position = -1;

    // Iterate through the list
    while (current_node != backDummy && current_node->next != backDummy)
    {
        previous_node = current_node;
        previous_position = current_position;

        // Check for duplicates of current node
        while (previous_node->next != backDummy)
        {
            if (current_node->data == previous_node->next->data)
            {
                // Found a duplicate node
                if (previous_node->next == afterCursor)
                {
                    // If the duplicate node is after the cursor, update the cursor
                    duplicate_node = previous_node->next;
                    previous_node->next->next->prev = previous_node;
                    previous_node->next = previous_node->next->next;
                    afterCursor = previous_node->next;
                    num_elements--;
                    delete duplicate_node;
                }
                else if (previous_node->next == beforeCursor)
                {
                    // If the duplicate node is before the cursor, update the cursor
                    duplicate_node = previous_node->next;
                    previous_node->next->next->prev = previous_node;
                    previous_node->next = previous_node->next->next;
                    beforeCursor = beforeCursor->prev;
                    num_elements--;
                    delete duplicate_node;
                }
                else
                {
                    // If the duplicate node is neither before nor after the cursor
                    duplicate_node = previous_node->next;
                    previous_node->next->next->prev = previous_node;
                    previous_node->next = previous_node->next->next;
                    num_elements--;
                    delete duplicate_node;
                }

                // If the cursor moved due to deleting the duplicate node, adjust the cursor position
                if (previous_position + 1 < pos_cursor)
                {
                    pos_cursor--;
                }
            }
            else
            {
                previous_node = previous_node->next;
                previous_position++;
            }
        }

        current_node = current_node->next;
        current_position++;
    }
}

List List::concat(const List &L) const
{
    List newList;

    // Traverse the first list and add each element to the new list
    newList.moveFront();
    Node *curr = frontDummy->next;
    while (curr != backDummy)
    {
        newList.insertBefore(curr->data);
        curr = curr->next;
    }

    // Traverse the second list and add each element to the new list
    curr = L.frontDummy->next;
    while (curr != L.backDummy)
    {
        newList.insertBefore(curr->data);
        curr = curr->next;
    }
    newList.moveFront();
    return newList;
}

std::string List::to_string() const
{
    std::ostringstream oss;
    oss << "(";

    // Traverse the list and append each element to the string
    Node *curr = frontDummy->next;
    while (curr != backDummy)
    {
        oss << curr->data;
        curr = curr->next;
        if (curr != backDummy)
        {
            oss << ", ";
        }
    }

    oss << ")";
    return oss.str();
}

bool List::equals(const List &R) const
{
    // If the two lists have different lengths, they are not equal
    if (num_elements != R.num_elements)
    {
        return false;
    }

    // Traverse both lists and compare each element
    Node *curr_this = frontDummy->next;
    Node *curr_R = R.frontDummy->next;

    while (curr_this != backDummy)
    {
        if (curr_this->data != curr_R->data)
        {
            return false;
        }
        curr_this = curr_this->next;
        curr_R = curr_R->next;
    }

    // If all elements are equal, the lists are equal
    return true;
}

std::ostream &operator<<(std::ostream &stream, const List &L)
{
    stream << L.to_string();
    return stream;
}

bool operator==(const List &A, const List &B)
{
    return A.equals(B);
}

// operator=()
// Overwrites the state of this List with state of L.
List &List::operator=(const List &L)
{
    // check for self-assignment
    if (this != &L)
    {
        // clear the current list
        clear();

        // copy each element from L to this list
        Node *curr = L.frontDummy->next;
        while (curr != L.backDummy)
        {
            insertBefore(curr->data);
            curr = curr->next;
        }
    }
    return *this;
}
