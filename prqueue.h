/*---------------------------------------
Program 4: Priority Queue
Course: CS 251, Spring 2024, UIC
System: VSCode
Author: Mohammad Yazdani
----------------------------------------*/

#pragma once

#include <iostream>  // For debugging
#include <sstream>   // For as_string

using namespace std;

template <typename T>
class prqueue {
   private:
    struct NODE {
        int priority;
        T value;
        NODE* parent;
        NODE* left;
        NODE* right;
        NODE* link;  // Link to duplicates -- Part 2 only
    };

    NODE* root;
    size_t sz;

    // Utility pointers for begin and next.
    NODE* curr;
    NODE* temp;  // Optional

    // TODO_STUDENT: add private helper function definitions here

    // Recursive Helper for clear().
    void emptyBST(NODE* node) {
        // Check if the current node is not null.
        if (node != nullptr) {
            // Recursively empty the left subtree.
            emptyBST(node->left);
            // Recursively empty the right subtree.
            emptyBST(node->right);

            // Delete all the links connceted to the node.
            while (node->link != nullptr) {
                NODE* temp = node->link;
                node->link = temp->link;
                delete temp;
                sz--;
            }

            // Delete the current node.
            delete(node);
            // Decrement the size of the prqueue.
            sz--;
        }
    }

    // Recursive Helper for enqueue().
    NODE* InsertNode(NODE* rootNode, T value, int priority) {
        // If the current node is null, create a new node and assign its values.
        if (rootNode == nullptr) {
            rootNode = new NODE;
            rootNode->value = value;
            rootNode->priority = priority;
            rootNode->parent = nullptr;
            rootNode->left = nullptr;
            rootNode->right = nullptr;
            rootNode->link = nullptr;
        }
        // If the priority of the value to be inserted is less than the priority of the current node,
        // recursively insert it into the left subtree.
        else if (priority < rootNode->priority) {
            rootNode->left = InsertNode(rootNode->left, value, priority);
            rootNode->left->parent = rootNode; // Update the parent pointer of the left child.
        }
        // If the priority of the value to be inserted is greater than the priority of the current node,
        // recursively insert it into the right subtree.
        else if (priority > rootNode->priority) {
            rootNode->right = InsertNode(rootNode->right, value, priority);
            rootNode->right->parent = rootNode; // Update the parent pointer of the right child.
        }
        // If the priority of the value to be inserted is equal to the priority of the current node,
        // insert it as a link to the existing node.
        else {
            NODE* temp = rootNode;
            while (temp->link != nullptr) {
                temp = temp->link; // Move to the last node in the link chain.
            }
            NODE* newNode = new NODE;
            newNode->value = value;
            newNode->priority = priority;
            newNode->left = nullptr;
            newNode->right = nullptr;
            newNode->parent = nullptr;
            newNode->link = nullptr;
            temp->link = newNode; // Link the new node to the last node in the chain.
        }
        // Return the updated root node of the subtree.
        return rootNode;
    }


    // Recursive Helper for peek() and dequeue().
    NODE* peekNode() const {
        // Check if the root is null, indicating an empty prqueue.
        if (root == nullptr) {
            return nullptr; // If empty, return nullptr.
        }
        else {
            NODE* curr = root; // Start traversal from the root node.
            // Traverse to the leftmost node to find the node with the smallest priority.
            while (curr->left != nullptr) {
                curr = curr->left;
            }
            // Return the node with the smallest priority.
            return curr;
        }
    }

    // Recurvive helper for as_string().
    void AsStringHelper(NODE* node, ostream& output) const {
        // Check if the current node is not null.
        if (node != nullptr) {
            // Recursively traverse the left subtree.
            AsStringHelper(node->left, output);
            // Output the priority and value of the current node.
            output << node->priority << " value: " << node->value << endl;

            // Gett all the links connected to the node.
            NODE* temp = node->link;
            while (temp != nullptr) {
                output << temp->priority << " value: " << temp->value << endl;
                temp = temp->link;
            }

            // Recursively traverse the right subtree.
            AsStringHelper(node->right, output);
        }
    }

    // Recursive helper function to copy nodes from another prqueue.
    void copyNodes(NODE* node) {
        if (node != nullptr) {
            // Insert the node into the new prqueue.
            enqueue(node->value, node->priority);
            // Recursively copy nodes from the left and right subtrees.

            NODE* temp = node->link;
            while (temp != nullptr) {
                enqueue(temp->value, temp->priority);
                temp = temp->link;
            }

            copyNodes(node->left);
            copyNodes(node->right);
        }
    }

   public:
    /// Creates an empty `prqueue`.
    ///
    /// Runs in O(1).
    prqueue() {
        root = nullptr;
        curr = nullptr;
        temp = nullptr;
        sz = 0;
    }

    /// Copy constructor.
    ///
    /// Copies the value-priority pairs from the provided `prqueue`.
    /// The internal tree structure must be copied exactly.
    ///
    /// Runs in O(N), where N is the number of values in `other`.
    prqueue(const prqueue& other) {
        root = nullptr; // Initialize root to nullptr for the new prqueue.
        sz = 0; // Initialize size with 0;
        copyNodes(other.root); // Copy the elements from the other prqueue.
    }


    /// Assignment operator; `operator=`.
    ///
    /// Clears `this` tree, and copies the value-priority pairs from the
    /// provided `prqueue`. The internal tree structure must be copied exactly.
    ///
    /// Runs in O(N + O), where N is the number of values in `this`, and O is
    /// the number of values in `other`.
    prqueue& operator=(const prqueue& other) {
        if (this != &other) {
            clear();
            copyNodes(other.root);
        }
        return *this;
    }

    /// Empties the `prqueue`, freeing all memory it controls.
    ///
    /// Runs in O(N), where N is the number of values.
    void clear() {
        emptyBST(root);
        root = nullptr;
    }

    /// Destructor, cleans up all memory associated with `prqueue`.
    ///
    /// Runs in O(N), where N is the number of values.
    ~prqueue() {
        clear();
    }

    /// Adds `value` to the `prqueue` with the given `priority`.
    ///
    /// Uses the priority to determine the location in the underlying tree.
    ///
    /// Runs in O(H + M), where H is the height of the tree, and M is
    /// the number of duplicate priorities.
    void enqueue(T value, int priority) {
        root = InsertNode(root, value, priority);
        sz++; // Increment size.
    }

    /// Returns the value with the smallest priority in the `prqueue`, but does
    /// not modify the `prqueue`.
    ///
    /// If the `prqueue` is empty, returns the default value for `T`.
    ///
    /// Runs in O(H + M), where H is the height of the tree, and M is
    /// the number of duplicate priorities.
    T peek() const {
        // Check if the BST is empty.
        if (root == nullptr) {
            return T{}; // Return default value for T.
        }
        return peekNode()->value;
    }

    /// Returns the value with the smallest priority in the `prqueue` and
    /// removes it from the `prqueue`.
    ///
    /// If the `prqueue` is empty, returns the default value for `T`.
    ///
    /// Runs in O(H + M), where H is the height of the tree, and M is
    /// the number of duplicate priorities.
    T dequeue() {
        // Check if the prqueue is empty.
        if (root == nullptr) {
            return T{}; // Return the default value of type T.
        }

        // Retrieve the node with the smallest priority.
        NODE* minNode = peekNode();
        // Retrieve the value of the node with the smallest priority.
        T value = peek();

        if (minNode->link != nullptr) {
            // If the minNode has links, dequeue the first node in the linked list.
            NODE* temp = minNode->link;
            minNode->value = temp->value;
            minNode->priority = temp->priority;
            minNode->link = temp->link; // Update the link.
            delete temp; // Delete the dequeued node.
        }
        else {
            // Remove the node from the binary search tree.
            if (minNode->parent != nullptr) {
                if (minNode->parent->left == minNode) {
                    // If the minimum node is a left child, update the left child pointer of its parent.
                    minNode->parent->left = minNode->right;
                } 
                else {
                    // If the minimum node is a right child, update the right child pointer of its parent.
                    minNode->parent->right = minNode->right;
                }
            } else {
                // If the minimum node is the root, update the root pointer.
                root = minNode->right;
            }

            // Update the parent pointer of the right child, if it exists.
            if (minNode->right != nullptr) {
                minNode->right->parent = minNode->parent;
            }

            delete minNode; // Delete the minimum node.
        }

        sz--; // Decrease the size of the prqueue.
        return value; // Return the value of the removed node.
    }

    /// Returns the number of elements in the `prqueue`.
    ///
    /// Runs in O(1).
    size_t size() const {
        return sz;
    }

    /// Resets internal state for an iterative inorder traversal.
    ///
    /// See `next` for usage details.
    ///
    /// O(H), where H is the maximum height of the tree.
    void begin() {
        curr = peekNode();
    }

    /// Uses the internal state to return the next in-order value and priority
    /// by reference, and advances the internal state. Returns true if the
    /// reference parameters were set, and false otherwise.
    ///
    /// Example usage:
    ///
    /// ```c++
    /// pq.begin();
    /// T value;
    /// int priority;
    /// while (pq.next(value, priority)) {
    ///   cout << priority << " value: " << value << endl;
    /// }
    /// ```
    ///
    /// Runs in worst-case O(H + M) or O(H), depending on implementation, where
    /// H is the height of the tree, and M is the number of duplicate
    /// priorities.
    bool next(T& value, int& priority) {
        // Check if the current node pointer is not null.
        if (curr != nullptr) {
            // Set the value and priority references to the values of the current node.
            value = curr->value;
            priority = curr->priority;

            // If the current node has links, move to the next linked node.
            if (curr->link != nullptr) {
                curr = curr->link;
            }
            // If the current node doesn't have links, navigate up the tree to find the next node to visit.
            else {
                // Traverse up the tree until a parent with a different priority is found or until the root is reached.
                while (curr->parent != nullptr) {
                    if (curr->parent->priority != curr->priority) {
                        curr = curr->parent;
                    }
                }

                // If the current node has a right child, navigate down the leftmost path from it.
                if (curr->right != nullptr) {
                    curr = curr->right;
                    while (curr->left != nullptr) {
                        curr = curr->left;
                    }
                }
                // If the current node doesn't have a right child, move up until finding a parent with a higher priority.
                else {
                    while (curr->parent != nullptr) {
                        if (curr->parent->priority > curr->priority) {
                            curr = curr->parent;
                        }
                    }
                    // If the current node's parent is not null, move to it, else set current node to null.
                    if (curr->parent != nullptr) {
                        curr = curr->parent;
                    } else {
                        curr = nullptr;
                    }
                }
            }
        }
        // Return true if the current node is not null, indicating there are more nodes to visit.
        return (curr != nullptr);
    }


    /// Converts the `prqueue` to a string representation, with the values
    /// in-order by priority.
    ///
    /// Example:
    ///
    /// ```c++
    /// prqueue<string> names;
    /// names.enqueue("Gwen", 3);
    /// names.enqueue("Jen", 2);
    /// names.enqueue("Ben", 1);
    /// names.enqueue("Sven", 2);
    /// ```
    ///
    /// Calling `names.as_string()` would return the following multi-line
    /// string:
    ///
    /// ```text
    /// 1 value: Ben
    /// 2 value: Jen
    /// 2 value: Sven
    /// 3 value: Gwen
    /// ```
    ///
    /// Runs in O(N), where N is the number of values.
    string as_string() const {
        ostringstream oss;
        AsStringHelper(root, oss);
        return oss.str(); // Return the string representation from the output stringstream.
    }

    /// Checks if the contents of `this` and `other` are equivalent.
    ///
    /// Two `prqueues` are equivalent if they have the same priorities and
    /// values, as well as the same internal tree structure.
    ///
    /// These two `prqueue`s would be considered equivalent, because
    /// they have the same internal tree structure:
    ///
    /// ```c++
    /// prqueue<string> a;
    /// a.enqueue("2", 2);
    /// a.enqueue("1", 1);
    /// a.enqueue("3", 3);
    /// ```
    ///
    /// and
    ///
    /// ```c++
    /// prqueue<string> b;
    /// a.enqueue("2", 2);
    /// a.enqueue("3", 3);
    /// a.enqueue("1", 1);
    /// ```
    ///
    /// While this `prqueue`, despite containing the same priority-value pairs,
    /// would not be considered equivalent, because the internal tree structure
    /// is different.
    ///
    /// ```c++
    /// prqueue<string> c;
    /// a.enqueue("1", 1);
    /// a.enqueue("2", 2);
    /// a.enqueue("3", 3);
    /// ```
    ///
    /// Runs in O(N) time, where N is the maximum number of nodes in
    /// either `prqueue`.
    ///
    bool operator==(const prqueue& other) const {
        string str1 = this->as_string();
        string str2 = other.as_string();
        return (str1 == str2);
    }

    /// Returns a pointer to the root node of the BST.
    ///
    /// Used for testing the internal structure of the BST. Do not edit or
    /// change.
    ///
    /// Runs in O(1).
    void* getRoot() {
        return root;
    }
};