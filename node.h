#ifndef NODE_H
#define NODE_H
#include <iostream>
#include "comparator.h"
#include "treeexception.h"
#include <string>
#include <sstream>

template<typename TKey, typename TValue>
struct key_and_value
{
    TKey *key;
    TValue value;
};

template<typename TKey, typename TValue>
struct twothree_node
{
    int size; // количество ключей в узле
    key_and_value<TKey, TValue> nodes[3]; // массив с ключами и значениями
    twothree_node *first{nullptr}; // ссылка на первого ребенка
    twothree_node *second{nullptr}; // ссылка на второго ребенка
    twothree_node *third{nullptr}; // ссылка на третьего ребенка
    twothree_node *fourth{nullptr}; // ссылка на четвертого ребенка
    twothree_node *parent{nullptr}; // ссылка на родителя
    /*comparator<TKey> *key_comparator;
    twothree_node(comparator<TKey> *key_comparator)
    {
        this->key_comparator = key_comparator;
    }*/
    twothree_node()
    {

    }
    explicit twothree_node (key_and_value<TKey, TValue> node,  twothree_node<TKey, TValue> *first_ = nullptr,  twothree_node<TKey, TValue> *second_ = nullptr,
                                  twothree_node<TKey, TValue> *third_ = nullptr,  twothree_node<TKey, TValue> *fourth_ = nullptr,
                                                                   twothree_node<TKey, TValue> *parent_ = nullptr);
    bool find_node(key_and_value<TKey, TValue> &node, comparator<TKey> *key_comparator); // поиск ноды в вершине
    void swap(key_and_value<TKey, TValue> &x, key_and_value<TKey, TValue> &y); // вспомогательная операция , используется в сортировке
    void sort_2(key_and_value<TKey, TValue> &x, key_and_value<TKey, TValue> &y, comparator<TKey> *key_comparator); // вспомогательная операция, используется в сортировке
    void sort_3(key_and_value<TKey, TValue> &x, key_and_value<TKey, TValue> &y, key_and_value<TKey, TValue> &z, comparator<TKey> *key_comparator); // вспомогательная операция, используется в сортировк
    void sort(comparator<TKey> *key_comparator); // сортировка ключей в вершине
    void insert_to_node(key_and_value<TKey, TValue> &node, comparator<TKey> *key_comparator); // вставка ноды в вершину
    void remove_from_node(key_and_value<TKey, TValue> &node, comparator<TKey> *key_comparator); // удаление ноды из вершины
    void become_node_2(key_and_value<TKey, TValue> &node, twothree_node<TKey, TValue> *first, twothree_node<TKey, TValue> *second); // преобразование в 2-вершину
    bool is_leaf(); // является ли вершина листом
    TValue search_value(TKey key, comparator<TKey> *key_comparator);
};
template<typename TKey, typename TValue>
twothree_node<TKey, TValue>::twothree_node(key_and_value<TKey, TValue> node,
                                           twothree_node<TKey, TValue> *first_,
                                           twothree_node<TKey, TValue> *second_,
                                           twothree_node<TKey, TValue> *third_,
                                           twothree_node<TKey, TValue> *fourth_,
                                           twothree_node<TKey, TValue> *parent_):
    size(1),
    nodes{node, 0},
    first(first_),
    second(second_),
    third(third_),
    fourth(fourth_),
    parent(parent_)
{

}
template<typename TKey, typename TValue>
bool twothree_node<TKey, TValue>::find_node(key_and_value<TKey, TValue> &node, comparator<TKey> *key_comparator)//возвращает true, если ключ k находится в вершине, иначе false.
{
   compare_t compare_result;
   TKey key = *(node.key);
   for (int i = 0; i < size; ++i)
   {
     compare_result = (*key_comparator)(*(nodes[i].key), key);
     if (compare_result == EQUAL)
     {
       return true;
     }
   }
   return false;
}
template<typename TKey, typename TValue>
void twothree_node<TKey, TValue>::swap(key_and_value<TKey, TValue> &x, key_and_value<TKey, TValue> &y)//поменять ключи местами, используется в сортировке
{
  TKey *curr_key = x.key;
  x.key = y.key;
  y.key = curr_key;
  TValue curr_value = x.value;
  x.value = y.value;
  y.value = curr_value;
}
template<typename TKey, typename TValue>
void twothree_node<TKey, TValue>::sort_2(key_and_value<TKey, TValue> &x, key_and_value<TKey, TValue> &y, comparator<TKey> *key_comparator)// сортировка вершины с 2 ключами, используется в сортировке
{
  compare_t compare_result;
  compare_result = (*key_comparator)(*(x.key), *(y.key));
  if(compare_result == GREAT)
  {
    swap(x, y);
  }
}
template<typename TKey, typename TValue>
void twothree_node<TKey, TValue>::sort_3(key_and_value<TKey, TValue> &x, key_and_value<TKey, TValue> &y, key_and_value<TKey, TValue> &z, comparator<TKey> *key_comparator)//сортировка вершины с 3 ключами, используется в сортировке
{
  compare_t compare_result;
  compare_result = (*key_comparator)(*(x.key), *(y.key));
  if(compare_result == GREAT)
  {
    swap(x, y);
  }
  compare_result = (*key_comparator)(*(x.key), *(z.key));
  if(compare_result == GREAT)
  {
    swap(x, z);
  }
  compare_result = (*key_comparator)(*(y.key), *(z.key));
  if(compare_result == GREAT)
  {
    swap(y, z);
  }
}
template<typename TKey, typename TValue>
void twothree_node<TKey, TValue>::sort(comparator<TKey> *key_comparator)//cортировка ключей в вершине по возрастанию
{
  if(size == 1)
  {
    return;
  }
  if(size == 2)
  {
    sort_2(nodes[0], nodes[1], key_comparator);
  }
  if(size == 3)
  {
    sort_3(nodes[0], nodes[1], nodes[2], key_comparator);
  }
}
template<typename TKey, typename TValue>
void twothree_node<TKey, TValue>::insert_to_node(key_and_value<TKey, TValue> &node, comparator<TKey> *key_comparator)//вставляем ноду в вершину (не в дерево)
{
  nodes[size] = node;
  size++;
  sort(key_comparator);
}
template<typename TKey, typename TValue>
void twothree_node<TKey, TValue>::remove_from_node(key_and_value<TKey, TValue> &node, comparator<TKey> *key_comparator)//yдаляем ноду из вершины (не из дерева)
{
    compare_t compare_result_1, compare_result_2;
    compare_result_1 = (*key_comparator)(*(nodes[0].key), *(node.key));
    compare_result_2 = (*key_comparator)(*(nodes[1].key), *(node.key));
    if (size >= 1 && compare_result_1 == EQUAL)
    {
      nodes[0].key = nodes[1].key;
      nodes[0].value = nodes[1].value;
      nodes[1].key = nullptr;
      size--;
    }
    else if(size == 2 && compare_result_2 == EQUAL)
    {
      nodes[1].key = nullptr;
      size--;
    }
}
template<typename TKey, typename TValue>
void twothree_node<TKey, TValue>::become_node_2(key_and_value<TKey, TValue> &node, twothree_node<TKey, TValue> *first, twothree_node<TKey, TValue> *second)//преобразовать в 2-вершину
{
  nodes[0].key = node.key;
  nodes[1].key = nullptr;
  this->first = first;
  this->second = second;
  this->third = nullptr;
  this->parent = nullptr;
  size = 1;
}
template<typename TKey, typename TValue>
bool twothree_node<TKey, TValue>::is_leaf()//является ли вершина листом; проверка используется при вставке и удалении
{
  return (first == nullptr) && (second == nullptr) && (third == nullptr);
}
template<typename TKey, typename TValue>
TValue twothree_node<TKey, TValue>::search_value(TKey key, comparator<TKey> *key_comparator)
{
    compare_t compare_result;
    for (int i = 0; i < this->size; ++i)
    {
      compare_result = (*key_comparator)(*(this->nodes[i].key), key);
      if (compare_result == EQUAL)
      {
        return this->nodes[i].value;
      }
    }
}
#endif // NODE_H
