#include <iostream>
#include <cassert>
#include "includes/bplustree/bplustree.h"

int main() {
    std::cout << "Testing B+ Tree remove...\n\n";

    // Test 1: Basic remove - verify tree clears only one entry
    std::cout << "=== Test 1: Basic remove ===\n";
    BPlusTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    std::cout << "Before remove - contains:\n";
    std::cout << "  10: " << tree.contains(10) << "\n";
    std::cout << "  20: " << tree.contains(20) << "\n";
    std::cout << "  30: " << tree.contains(30) << "\n";

    tree.remove(20);

    std::cout << "After remove(20) - contains:\n";
    std::cout << "  10: " << tree.contains(10) << " (expected 1)\n";
    std::cout << "  20: " << tree.contains(20) << " (expected 0 - REMOVED)\n";
    std::cout << "  30: " << tree.contains(30) << " (expected 1)\n";

    assert(tree.contains(10) == true);
    assert(tree.contains(20) == false);  // Should be REMOVED
    assert(tree.contains(30) == true);

    // Test 2: Remove root element
    std::cout << "\n=== Test 2: Remove root element ===\n";
    tree.remove(10);
    std::cout << "After remove(10) - contains:\n";
    std::cout << "  10: " << tree.contains(10) << " (expected 0 - REMOVED)\n";
    std::cout << "  30: " << tree.contains(30) << " (expected 1)\n";
    assert(!tree.contains(10));  // Removed
    assert(tree.contains(30));

    // Test 3: Remove and add new
    std::cout << "\n=== Test 3: Remove and add new ===\n";
    tree.insert(40);
    tree.insert(50);
    std::cout << "After inserting 40, 50:\n";
    std::cout << "  30: " << tree.contains(30) << "\n";
    std::cout << "  40: " << tree.contains(40) << "\n";
    std::cout << "  50: " << tree.contains(50) << "\n";
    assert(tree.contains(30));
    assert(tree.contains(40));
    assert(tree.contains(50));

    tree.remove(40);
    std::cout << "After remove(40):\n";
    std::cout << "  30: " << tree.contains(30) << "\n";
    std::cout << "  40: " << tree.contains(40) << " (expected 0 - REMOVED)\n";
    std::cout << "  50: " << tree.contains(50) << "\n";
    assert(!tree.contains(40));  // Removed
    assert(tree.contains(50));

    // Test 4: String keys
    std::cout << "\n=== Test 4: String keys ===\n";
    BPlusTree<std::string> strTree;
    strTree.insert("apple");
    strTree.insert("banana");
    strTree.insert("cherry");
    
    strTree.remove("banana");
    std::cout << "After remove('banana'):\n";
    std::cout << "  apple: " << strTree.contains("apple") << "\n";
    std::cout << "  banana: " << strTree.contains("banana") << " (expected 0)\n";
    std::cout << "  cherry: " << strTree.contains("cherry") << "\n";
    assert(strTree.contains("apple"));
    assert(!strTree.contains("banana"));  // Removed
    assert(strTree.contains("cherry"));

    // Test 5: is_valid after remove
    std::cout << "\n=== Test 5: Tree validation after remove ===\n";
    BPlusTree<int> validTree;
    for (int i = 1; i <= 10; i++) {
        validTree.insert(i * 10);
    }
    std::cout << "Initial tree is_valid: " << validTree.is_valid() << "\n";
    assert(validTree.is_valid());

    validTree.remove(50);
    validTree.remove(30);
    validTree.remove(10);
    std::cout << "After 3 removes, is_valid: " << validTree.is_valid() << "\n";
    assert(validTree.is_valid());

    // Test 6: Empty tree operations
    std::cout << "\n=== Test 6: Empty tree ===\n";
    BPlusTree<int> emptyTree;
    std::cout << "Empty tree contains 99: " << emptyTree.contains(99) << "\n";
    std::cout << "Empty tree empty(): " << emptyTree.empty() << "\n";
    emptyTree.remove(99);  // Should not crash
    std::cout << "After remove on empty tree, still empty: " << emptyTree.empty() << "\n";
    assert(emptyTree.empty());

    std::cout << "\n=======================\n";
    std::cout << "All B+ Tree remove tests PASSED!\n";
    std::cout << "Note: size() has pre-existing bugs in this B+ tree.\n";
    std::cout << "The key test is that remove() now removes specific\n";
    std::cout << "entries instead of clearing the entire tree.\n";
    std::cout << "=======================\n";

    return 0;
}
