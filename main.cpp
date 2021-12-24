#include <iostream>
#include "tree.h"

using namespace std;

void example_1()
{
    cout << "Laba 2 Task 2\nDrobotun Juliya M8o-210B-20" << endl;
    //пример с ключем типа "int" и данными типа "string"
    cout << "Example 1:" << endl << "TKey - int, TValue - string" << endl;
    comparator<int> *comparator_int_1 = new comparator<int>;
    twothree_tree<int, string> *tree = new twothree_tree<int, string>(comparator_int_1);
    key_and_value<int, string> *node = new key_and_value<int, string>();
    try
    {
        cout << "Insert 8 : \"008\"" << endl;
        int key = 8;
        node->key = &key;
        node->value = "008";
        tree->insert(*node);
        int key_1 = 4;
        cout << "Insert 4 : \"004\"" << endl;
        node->key = &key_1;
        node->value = "004";
        tree->insert(*node);
        int key_2 = 20;
        cout << "Insert 20 : \"020\"" << endl;
        node->key = &key_2;
        node->value = "020";
        tree->insert(*node);
        int key_3 = 7;
        cout << "Insert 7 : \"007\"" << endl << endl;
        node->key = &key_3;
        node->value = "007";
        tree->insert(*node);
        tree->insert(*node);
        cout << "Remove 7 : \"007\"" << endl << endl;
        tree->remove(*node);
        int key_4 = 4;
        cout << "Search 4 : \"004\"" << endl;
        node->key = &key_4;
        node->value = "004";
        cout << *((tree->search(*node))->nodes[0].key) << " : " << (tree->search(*node))->nodes[0].value<< endl;
    }
    catch (tree_exception &exception)
    {
        cout << exception.what() << endl;
    }

    try
    {
        cout << "Remove 7 : \"007\"" << endl << endl;
        tree->remove(*node);
        int key_4 = 4;
        cout << "Search 4 : \"004\"" << endl;
        node->key = &key_4;
        node->value = "004";
        cout << *((tree->search(*node))->nodes[0].key) << " : " << (tree->search(*node))->nodes[0].value<< endl;
        cout << "Search 20 : \"020\"" << endl;
        int key_5 = 20;
        node->key = &key_5;
        node->value = "020";
        cout << *((tree->search(*node))->nodes[0].key) << " : " << (tree->search(*node))->nodes[0].value<< endl;
    }
    catch (tree_exception &exception)
    {
        cout << exception.what() << endl;
    }
}
int main()
{
    example_1();
    return 0;
}
