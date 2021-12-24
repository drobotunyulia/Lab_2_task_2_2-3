#ifndef TREE_H
#define TREE_H
#include<iostream>
#include <string>
#include <sstream>
#include "node.h"
#include "treeexception.h"
using namespace std;
template<typename TKey, typename TValue>
class twothree_tree
{
protected:
    //вложенный класс исключения "ошибка поиска"
    class find_error_exception : public tree_exception
    {
    public:
        find_error_exception(TKey key);
    };
    //вложенный класс исключения "ошибка вставки"
    class insert_error_exception : public tree_exception
    {
    public:
        insert_error_exception(TKey key);
    };
    //вложенный класс исключения "ошибка удаления"
    class remove_error_exception : public tree_exception
    {
    public:
        remove_error_exception(TKey key);
    };
public:
    void clear();//oчистка дерева
    void insert(key_and_value<TKey, TValue> &node);//вставка в дерево;
    twothree_node<TKey,  TValue> *search(key_and_value<TKey, TValue> &node);//поиск в дереве;
    void *remove(key_and_value<TKey, TValue> &node);//удаление ключа из дерева
    twothree_node<TKey, TValue> *get_root();//возвращает корень дерева
    int height();
    comparator<TKey> *key_comparator;
    twothree_tree(comparator<TKey> *key_comparator);
    ~twothree_tree();//возвращает высоту дерева

    twothree_node<TKey, TValue> *root;//корень 2-3 дерева
    twothree_node<TKey, TValue> *merge(twothree_node<TKey, TValue> *leaf);//слияние используется при удалении;
    twothree_node<TKey, TValue> *redistribute(twothree_node<TKey, TValue> *leaf);//перераспределение также используется при удалении;
    twothree_node<TKey, TValue> *fix(twothree_node<TKey, TValue> *leaf);//используется после удаления для возвращения свойств дереву (использует merge или redistribute)
    twothree_node<TKey, TValue> *split(twothree_node<TKey, TValue> *item);//метод для разделение вершины при переполнении;
    void insert(key_and_value<TKey, TValue> &node, twothree_node<TKey, TValue> *node_2);//вспомогательная функция, используется при вставке
    void clear(twothree_node<TKey, TValue> *node);//вспомогательная функция, используется при очистке
    twothree_node<TKey, TValue> *search(key_and_value<TKey, TValue> &node, twothree_node<TKey, TValue> *node_2);//вспомогательная функция, используется при поиске
    twothree_node<TKey, TValue> *remove(key_and_value<TKey, TValue> &node, twothree_node<TKey, TValue> *node_2);//вспомогательная функуция, используется при удалении
    int height(twothree_node<TKey, TValue> *node);//вспомогательная функция, используется при вычислении высоты
    twothree_node<TKey, TValue> *find_min(twothree_node<TKey, TValue> *node);//поиск узла с минимальным значением ключа
};
template <typename TKey, typename TValue>
twothree_tree<TKey, TValue>::find_error_exception::find_error_exception(TKey key)
{
    std::stringstream key_string;
    key_string << key;
    std::string exception_message = "Find error. Element with key \"" + key_string.str() + "\" not found.";
    set_exception_message(exception_message);
}

template <typename TKey, typename TValue>
twothree_tree<TKey, TValue>::insert_error_exception::insert_error_exception(TKey key)
{
    std::stringstream key_string;
    key_string << key;
    set_exception_message("Insert error. Element with key \"" + key_string.str() + "\" already exists.");
}

template <typename TKey, typename TValue>
twothree_tree<TKey, TValue>::remove_error_exception::remove_error_exception(TKey key)
{
    std::stringstream key_string;
    key_string << key;
    std::string exception_message = "Remove error. Element with key \"" + key_string.str() + "\" not found.";
    set_exception_message(exception_message);
}
template<typename TKey, typename TValue>
twothree_tree<TKey ,TValue>::twothree_tree(comparator<TKey> *key_comparator)
{
    this->key_comparator = key_comparator;
    root = nullptr;
}
template<typename TKey, typename TValue>
twothree_tree<TKey ,TValue>::~twothree_tree()
{
    clear();
}
template<typename TKey, typename TValue>
void twothree_tree<TKey, TValue>::clear()
{
  clear(this->root);
  this->root = nullptr;
}
template<typename TKey, typename TValue>
void twothree_tree<TKey, TValue>::clear(twothree_node<TKey, TValue> *node)
{
  if (node != nullptr)
  {
    clear(node->first);
    clear(node->second);
    clear(node->third);
    delete [] node;
  }
}
template<typename TKey, typename TValue>
void twothree_tree<TKey, TValue>::insert(key_and_value<TKey, TValue> &node)
{
  if (this->root != nullptr)
  {
    insert(node, root);
  }
  else
  {
    this->root = new twothree_node<TKey, TValue>(node);
  }
}
template<typename TKey, typename TValue>
void twothree_tree<TKey, TValue>::insert(key_and_value<TKey, TValue> &node, twothree_node<TKey, TValue> *node_2)
{
  compare_t compare_result_1 = EQUAL, compare_result_2 = EQUAL;
  if(node_2->nodes[0].key)
  {
     if(*(node_2->nodes[0].key) == *(node.key))
     {
         throw insert_error_exception(*(node.key));
     }
     compare_result_1 = (*key_comparator)(*(node.key), *(node_2->nodes[0].key));
  }
  if(node_2->nodes[1].key)
  {
      if(*(node_2->nodes[1].key) == *(node.key))
      {
          throw insert_error_exception(*(node.key));
      }
     compare_result_2 = (*key_comparator)(*(node.key), *(node_2->nodes[1].key));
  }
  if (node_2->is_leaf())
  {
    node_2->insert_to_node(node, key_comparator);
  }
  else if (compare_result_1 == LESS || compare_result_1 == EQUAL)
  {
    insert(node, node_2->first);
  }
  else if ((node_2->size == 1) || ((node_2->size == 2) && (compare_result_2 == LESS || compare_result_2 == EQUAL)))
  {
    insert(node, node_2->second);
  }
  else
  {
    insert(node, node_2->third);
  }
  this->root = split(node_2);
}
template<typename TKey, typename TValue>
twothree_node<TKey, TValue> *twothree_tree<TKey, TValue>::split(twothree_node<TKey, TValue> *item)
{
  if (item->size < 3)
      return item;

  twothree_node<TKey, TValue> *x = new twothree_node<TKey, TValue>(item->nodes[0], item->first, item->second, nullptr, nullptr, item->parent);//создаем две новые вершины,
  twothree_node<TKey, TValue> *y = new twothree_node<TKey, TValue>(item->nodes[2], item->third, item->fourth, nullptr, nullptr, item->parent);//которые имеют такого же родителя, как и разделяющийся элемент.
  if (x->first)
      x->first->parent = x;//правильно устанавливаем "родителя" "сыновей".
  if (x->second)
      x->second->parent = x;//после разделения, "родителем" "сыновей" является "дедушка",
  if (y->first)
      y->first->parent = y;//поэтому нужно правильно установить указатели.
  if (y->second)
      y->second->parent = y;

  if (item->parent)
  {
      item->parent->insert_to_node(item->nodes[1], key_comparator);
      if (item->parent->first == item)
          item->parent->first = nullptr;
      else if (item->parent->second == item)
          item->parent->second = nullptr;
      else if (item->parent->third == item)
          item->parent->third = nullptr;
    //дальше происходит сортировка ключей при разделении
    if (item->parent->first == nullptr)
    {
        item->parent->fourth = item->parent->third;
        item->parent->third = item->parent->second;
        item->parent->second = y;
        item->parent->first = x;
    }
    else if (item->parent->second == nullptr)
    {
        item->parent->fourth = item->parent->third;
        item->parent->third = y;
        item->parent->second = x;
    }
    else
    {
        item->parent->fourth = y;
        item->parent->third = x;
    }
    twothree_node<TKey, TValue> *tmp = item->parent;
    delete [] item;
    return tmp;
  }
  else
  {
      x->parent = item;   // Так как в эту ветку попадает только корень,
      y->parent = item;   // то мы "родителем" новых вершин делаем разделяющийся элемент.
      item->become_node_2(item->nodes[1], x, y);
      return item;
  }
}
template<typename TKey, typename TValue>
twothree_node<TKey, TValue> *twothree_tree<TKey, TValue>::search(key_and_value<TKey, TValue> &node)
{
    return search(node, this->root);
}
template<typename TKey, typename TValue>
twothree_node<TKey, TValue> *twothree_tree<TKey, TValue>::search(key_and_value<TKey, TValue> &node, twothree_node<TKey, TValue> *node_2)
{
  compare_t compare_result_1 = EQUAL, compare_result_2 = EQUAL;
  if(node_2->nodes[0].key)
  {
      compare_result_1 = (*key_comparator)(*(node.key), *(node_2->nodes[0].key));
  }
  if(node_2->nodes[1].key)
  {
      compare_result_2 = (*key_comparator)(*(node.key), *(node_2->nodes[1].key));
  }
  if(node_2 == nullptr)
  {
    throw find_error_exception(*node.key);
    return nullptr;
  }
  if(node_2->find_node(node, key_comparator))
  {
    return node_2;
  }
  if(compare_result_1 == LESS)
  {
    return search(node, node_2->first);
  }
  if(((node_2->size == 2) && (compare_result_2 == LESS)) || (node_2->size == 1))
  {
    return search(node, node_2->second);
  }
  if(node_2->size == 2)
  {
    return search(node, node_2->third);
  }
  throw find_error_exception(*node.key);
  return nullptr;
}
template<typename TKey, typename TValue>
void *twothree_tree<TKey, TValue>::remove(key_and_value<TKey, TValue> &node)
{
  this->root = remove(node, this->root);
}
template<typename TKey, typename TValue>
twothree_node<TKey, TValue> *twothree_tree<TKey, TValue>::remove(key_and_value<TKey, TValue> &node, twothree_node<TKey, TValue> *node_2)
{
    compare_t compare_result;
    twothree_node<TKey, TValue> *item = nullptr;
    try
    {
        item = search(node, node_2);
    }
    catch (tree_exception &exception)
    {
        throw remove_error_exception(*(node.key));
    } // Ищем узел, где находится ключ k
    if (!item)
        return node_2;
    twothree_node<TKey, TValue> *min = nullptr;
    compare_result = (*key_comparator)(*(item->nodes[0].key), *(node.key));
    if (compare_result == EQUAL)
        min = find_min(item->second); // Ищем эквивалентный ключ
    else
    {
        min = find_min(item->third);
    }

    if (min)
    { // Меняем ключи местами
      key_and_value<TKey, TValue> &z = (*(node.key) == *(item->nodes[0].key) ? item->nodes[0]: item->nodes[1]);
      item->swap(z, min->nodes[0]);
      item = min; // Перемещаем указатель на лист, т.к. min - всегда лист
    }

    item->remove_from_node(node, key_comparator); // И удаляем требуемый ключ из листа
    return fix(item);// Вызываем функцию для восстановления свойств дерева.
}
template<typename TKey, typename TValue>
twothree_node<TKey, TValue> *twothree_tree<TKey, TValue>::find_min(twothree_node<TKey, TValue> *node)
{
  if (!node)
  {
    return node;
  }
  if (!(node->first))
  {
    return node;
  }
  else
  {
    return find_min(node->first);
  }
}
template<typename TKey, typename TValue>
twothree_node<TKey, TValue> *twothree_tree<TKey, TValue>::fix(twothree_node<TKey, TValue> *leaf)
{
    // после удаления может произойти 4 случая, которые нарушат свойства дерева
  if (leaf->size == 0 && leaf->parent == nullptr)
  { // Случай 0, когда удаляем единственный ключ в дереве
    delete leaf;
    return nullptr;
  }
  if (leaf->size != 0)
  {
      // Случай 1, когда вершина, в которой удалили ключ, имела два ключа
    if (leaf->parent)
        return fix(leaf->parent);
    else
        return leaf;
  }

  twothree_node<TKey, TValue> *parent = leaf->parent;
  if (parent->first->size == 2 || parent->second->size == 2 || parent->size == 2)
  {
      leaf = redistribute(leaf); // Случай 2, когда достаточно перераспределить ключи в дереве
  }
  else if (parent->size == 2 && parent->third->size == 2)
  {
      leaf = redistribute(leaf); // Аналогично
  }
  else
  {
      leaf = merge(leaf); // Случай 3, когда нужно произвести склеивание и пройтись вверх по дереву как минимум на еще одну вершину
  }

  return fix(leaf);
}
template<typename TKey, typename TValue>
twothree_node<TKey, TValue> *twothree_tree<TKey, TValue>::redistribute(twothree_node<TKey, TValue> *leaf)
{
  twothree_node<TKey, TValue> *parent = leaf->parent;
  twothree_node<TKey, TValue> *first = parent->first;
  twothree_node<TKey, TValue> *second = parent->second;
  twothree_node<TKey, TValue> *third = parent->third;

  if ((parent->size == 2) && (first->size < 2) && (second->size < 2) && (third->size < 2))
  {
    if (first == leaf)
    {
      parent->first = parent->second;
      parent->second = parent->third;
      parent->third = nullptr;
      parent->first->insert_to_node(parent->nodes[0], key_comparator);
      parent->first->third = parent->first->second;
      parent->first->second = parent->first->first;

      if (leaf->first != nullptr)
          parent->first->first = leaf->first;
      else if (leaf->second != nullptr)
          parent->first->first = leaf->second;

      if (parent->first->first != nullptr)
          parent->first->first->parent = parent->first;
      parent->remove_from_node(parent->nodes[0], key_comparator);
      delete [] first;
    }
    else if (second == leaf)
    {
      first->insert_to_node(parent->nodes[0], key_comparator);
      parent->remove_from_node(parent->nodes[0], key_comparator);
      if (leaf->first != nullptr)
          first->third = leaf->first;
      else if (leaf->second != nullptr)
          first->third = leaf->second;
      if (first->third != nullptr)
          first->third->parent = first;
      parent->second = parent->third;
      parent->third = nullptr;
      delete [] second;
    }
    else if (third == leaf)
    {
      second->insert_to_node(parent->nodes[1], key_comparator);
      parent->third = nullptr;
      parent->remove_from_node(parent->nodes[1], key_comparator);
      if (leaf->first != nullptr)
          second->third = leaf->first;
      else if (leaf->second != nullptr)
          second->third = leaf->second;
      if (second->third != nullptr)  second->third->parent = second;
      delete [] third;
    }
  }
  else if ((parent->size == 2) && ((first->size == 2) || (second->size == 2) || (third->size == 2)))
  {
    if (third == leaf)
    {
      if (leaf->first != nullptr)
      {
        leaf->second = leaf->first;
        leaf->first = nullptr;
      }
      leaf->insert_to_node(parent->nodes[1], key_comparator);
      if (second->size == 2)
      {
        parent->nodes[1] = second->nodes[1];
        second->remove_from_node(second->nodes[1], key_comparator);
        leaf->first = second->third;
        second->third = nullptr;
        if (leaf->first != nullptr)
            leaf->first->parent = leaf;
      }
      else if (first->size == 2)
      {
        parent->nodes[1] = second->nodes[0];
        leaf->first = second->second;
        second->second = second->first;
        if (leaf->first != nullptr)
            leaf->first->parent = leaf;
        second->nodes[0] = parent->nodes[0];
        parent->nodes[0] = first->nodes[1];
        first->remove_from_node(first->nodes[1], key_comparator);
        second->first = first->third;
        if (second->first != nullptr)
            second->first->parent = second;
        first->third = nullptr;
      }
    }
    else if (second == leaf)
    {
      if (third->size == 2)
      {
        if (leaf->first == nullptr)
        {
          leaf->first = leaf->second;
          leaf->second = nullptr;
        }
        second->insert_to_node(parent->nodes[1], key_comparator);
        parent->nodes[1] = third->nodes[0];
        third->remove_from_node(third->nodes[0], key_comparator);
        second->second = third->first;
        if (second->second != nullptr) second->second->parent = second;
        third->first = third->second;
        third->second = third->third;
        third->third = nullptr;
      }
      else if (first->size == 2)
      {
        if (leaf->second == nullptr)
        {
          leaf->second = leaf->first;
          leaf->first = nullptr;
        }
        second->insert_to_node(parent->nodes[0], key_comparator);
        parent->nodes[0] = first->nodes[1];
        first->remove_from_node(first->nodes[1], key_comparator);
        second->first = first->third;
        if (second->first != nullptr)
            second->first->parent = second;
        first->third = nullptr;
      }
    }
    else if (first == leaf)
    {
      if (leaf->first == nullptr)
      {
        leaf->first = leaf->second;
        leaf->second = nullptr;
      }
      first->insert_to_node(parent->nodes[0], key_comparator);
      if (second->size == 2)
      {
        parent->nodes[0] = second->nodes[0];
        second->remove_from_node(second->nodes[0], key_comparator);
        first->second = second->first;
        if (first->second != nullptr)
            first->second->parent = first;
        second->first = second->second;
        second->second = second->third;
        second->third = nullptr;
      }
      else if (third->size == 2)
      {
        parent->nodes[0] = second->nodes[0];
        second->nodes[0] = parent->nodes[1];
        parent->nodes[1] = third->nodes[0];
        third->remove_from_node(third->nodes[0], key_comparator);
        first->second = second->first;
        if (first->second != nullptr)
            first->second->parent = first;
        second->first = second->second;
        second->second = third->first;
        if (second->second != nullptr)
            second->second->parent = second;
        third->first = third->second;
        third->second = third->third;
        third->third = nullptr;
      }
    }
  }
  else if (parent->size == 1)
  {
    leaf->insert_to_node(parent->nodes[0], key_comparator);
    if (first == leaf && second->size == 2)
    {
      parent->nodes[0] = second->nodes[0];
      second->remove_from_node(second->nodes[0], key_comparator);
      if (leaf->first == nullptr)
          leaf->first = leaf->second;
      leaf->second = second->first;
      second->first = second->second;
      second->second = second->third;
      second->third = nullptr;
      if (leaf->second != nullptr)
          leaf->second->parent = leaf;
    }
    else if (second == leaf && first->size == 2)
    {
      parent->nodes[0] = first->nodes[1];
      first->remove_from_node(first->nodes[1], key_comparator);
      if (leaf->second == nullptr)
          leaf->second = leaf->first;
      leaf->first = first->third;
      first->third = nullptr;
      if (leaf->first != nullptr)
          leaf->first->parent = leaf;
    }
  }
  return parent;
}
template<typename TKey, typename TValue>
twothree_node<TKey, TValue> *twothree_tree<TKey, TValue>::merge(twothree_node<TKey, TValue> *leaf)
{
    twothree_node<TKey, TValue> *parent = leaf->parent;
    if (parent->first == leaf)
    {
      parent->second->insert_to_node(parent->nodes[0], key_comparator);
      parent->second->third = parent->second->second;
      parent->second->second = parent->second->first;
      if (leaf->first != nullptr)
          parent->second->first = leaf->first;
      else if (leaf->second != nullptr)
          parent->second->first = leaf->second;
      if (parent->second->first != nullptr)
          parent->second->first->parent = parent->second;
      parent->remove_from_node(parent->nodes[0], key_comparator);
      delete parent->first;
      parent->first = nullptr;
    }
    else if (parent->second == leaf)
    {
      parent->first->insert_to_node(parent->nodes[0], key_comparator);

      if (leaf->first != nullptr)
          parent->first->third = leaf->first;
      else if (leaf->second != nullptr)
          parent->first->third = leaf->second;

      if (parent->first->third != nullptr)
          parent->first->third->parent = parent->first;

      parent->remove_from_node(parent->nodes[0], key_comparator);
      delete [] parent->second;
      parent->second = nullptr;
    }
    if (parent->parent == nullptr)
    {
      twothree_node<TKey, TValue> *tmp = nullptr;
      if (parent->first != nullptr) tmp = parent->first;
      else tmp = parent->second;
      tmp->parent = nullptr;
      delete [] parent;
      return tmp;
    }
    return parent;
}
template<typename TKey, typename TValue>
twothree_node<TKey, TValue> *twothree_tree<TKey, TValue>::get_root()
{
  return this->root;
}
template<typename TKey, typename TValue>
int twothree_tree<TKey, TValue>::height()
{
  return height(this->root);
}
template<typename TKey, typename TValue>
int twothree_tree<TKey, TValue>::height(twothree_node<TKey, TValue> *node)
{
  if (node == nullptr)
  {
    return 0;
  }

  const int first_height = height(node->first);

  return first_height + 1;
}
#endif // TREE_H
