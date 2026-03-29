#include <iostream>
#include "includes/bplustree/bplustree.h"
using namespace std;

int main() {
    int passed = 0;
    int failed = 0;

    // Test 1: Insert and contains
    {
        BPlusTree<int> tree;
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);
        if (tree.contains(10) && tree.contains(20) && tree.contains(30)) {
            cout << "[PASS] Insert and contains" << endl;
            passed++;
        } else {
            cout << "[FAIL] Insert and contains" << endl;
            failed++;
        }
    }

    // Test 2: Contains returns false for missing element
    {
        BPlusTree<int> tree;
        tree.insert(5);
        tree.insert(15);
        if (!tree.contains(99)) {
            cout << "[PASS] Contains returns false for missing element" << endl;
            passed++;
        } else {
            cout << "[FAIL] Contains returns false for missing element" << endl;
            failed++;
        }
    }

    // Test 3: Remove existing element
    {
        BPlusTree<int> tree;
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);
        tree.remove(20);
        if (!tree.contains(20) && tree.contains(10) && tree.contains(30)) {
            cout << "[PASS] Remove existing element" << endl;
            passed++;
        } else {
            cout << "[FAIL] Remove existing element" << endl;
            failed++;
        }
    }

    // Test 4: Empty tree
    {
        BPlusTree<int> tree;
        if (tree.empty()) {
            cout << "[PASS] Empty tree" << endl;
            passed++;
        } else {
            cout << "[FAIL] Empty tree" << endl;
            failed++;
        }
    }

    // Test 5: Tree not empty after insert
    {
        BPlusTree<int> tree;
        tree.insert(42);
        if (!tree.empty()) {
            cout << "[PASS] Not empty after insert" << endl;
            passed++;
        } else {
            cout << "[FAIL] Not empty after insert" << endl;
            failed++;
        }
    }

    // Test 6: Size tracking
    {
        BPlusTree<int> tree;
        tree.insert(1);
        tree.insert(2);
        tree.insert(3);
        if (tree.size() == 3) {
            cout << "[PASS] Size tracking" << endl;
            passed++;
        } else {
            cout << "[FAIL] Size tracking — expected 3, got " << tree.size() << endl;
            failed++;
        }
    }

    // Test 7: Insert many elements and validate structure
    {
        BPlusTree<int> tree;
        for (int i = 1; i <= 20; i++) {
            tree.insert(i);
        }
        if (tree.is_valid()) {
            cout << "[PASS] Insert 20 elements, tree is valid" << endl;
            passed++;
        } else {
            cout << "[FAIL] Insert 20 elements, tree is invalid" << endl;
            failed++;
        }
    }

    // Test 8: Iterator traversal in order
    {
        BPlusTree<int> tree;
        tree.insert(1);
        tree.insert(2);
        tree.insert(3);
        int expected = 1;
        bool ok = true;
        for (auto it = tree.begin(); it != tree.end(); ++it) {
            cout << "  got: " << *it << " expected: " << expected << endl;
            if (*it != expected++) {
                ok = false;
                break;
            }
        }
        if (ok) {
            cout << "[PASS] Iterator traversal in order" << endl;
            passed++;
        } else {
            cout << "[FAIL] Iterator traversal in order" << endl;
            failed++;
        }
    }

    // Test 9: Clear tree
    {
        BPlusTree<int> tree;
        tree.insert(10);
        tree.insert(20);
        tree.clear_tree();
        if (tree.empty()) {
            cout << "[PASS] Clear tree" << endl;
            passed++;
        } else {
            cout << "[FAIL] Clear tree" << endl;
            failed++;
        }
    }

    // Test 10: Copy constructor
    {
        BPlusTree<int> tree;
        tree.insert(5);
        tree.insert(10);
        tree.insert(15);
        BPlusTree<int> copy(tree);
        if (copy.contains(5) && copy.contains(10) && copy.contains(15)) {
            cout << "[PASS] Copy constructor" << endl;
            passed++;
        } else {
            cout << "[FAIL] Copy constructor" << endl;
            failed++;
        }
    }

        // Test 11: Remove from large tree
    {
        BPlusTree<int> tree;
        for (int i = 1; i <= 20; i++) {
            tree.insert(i);
        }
        tree.remove(5);
        tree.remove(10);
        tree.remove(15);
        bool ok = !tree.contains(5) && !tree.contains(10) && !tree.contains(15)
            && tree.contains(4) && tree.contains(6)
            && tree.contains(9) && tree.contains(11)
            && tree.contains(14) && tree.contains(16)
            && tree.is_valid();
        if (ok) {
            cout << "[PASS] Remove from large tree" << endl;
            passed++;
        } else {
            cout << "[FAIL] Remove from large tree" << endl;
            failed++;
        }
    }

    // Test 12: Remove all elements
    {
        BPlusTree<int> tree;
        for (int i = 1; i <= 10; i++) {
            tree.insert(i);
        }
        for (int i = 1; i <= 10; i++) {
            tree.remove(i);
        }
        if (tree.empty() && tree.is_valid()) {
            cout << "[PASS] Remove all elements" << endl;
            passed++;
        } else {
            cout << "[FAIL] Remove all elements" << endl;
            failed++;
        }
    }

    cout << "\n=== Results: " << passed << " passed, " << failed << " failed ===" << endl;
    return failed;
}