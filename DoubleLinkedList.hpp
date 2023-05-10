#pragma once

#include <iterator>
#include <ostream>
#include <functional>
#include <iostream>

template <typename T>
struct Node {
  Node* prev{nullptr};
  Node* next{nullptr};
  T data;
  Node(const T& element) : data{element}, next{nullptr}, prev{nullptr} {};
  Node(T&& element) : data{element}, next{nullptr}, prev{nullptr} {};
  Node(const Node& o) : data{o.data}, next{o.next}, prev{o.prev} {};
  Node(Node&& o){
    data = o.data;
    prev = o.prev;
    next = o.next;

    o.next = o.prev = nullptr;
    o.data = 0;
  };

  Node& operator=(const Node& o){
    
    data = o.data;
    prev = o.prev;
    next = o.next;

    return *this;
  };

  Node& operator=(Node&& o){
    
    data = o.data;
    prev = o.prev;
    next = o.next;
    
    o.next = o.prev = nullptr;
    o.data = 0;

    return *this;
  };

};

template <typename T>
class DoubleLinkedList {
  public: 
  class Iterator;
  using const_iterator = const Iterator;
  DoubleLinkedList();
  DoubleLinkedList(const DoubleLinkedList&);
  DoubleLinkedList(DoubleLinkedList&&);
  DoubleLinkedList& operator=(const DoubleLinkedList&);
  DoubleLinkedList& operator=(DoubleLinkedList&&);
  ~DoubleLinkedList();

  DoubleLinkedList& push_back(const T&);
  DoubleLinkedList& push_front(const T&);
  DoubleLinkedList& push_back(T&&);
  DoubleLinkedList& push_front(T&&);
  DoubleLinkedList& pop_front();
  DoubleLinkedList& pop_back();
  bool empty() const;
  size_t size() const;
  T& front();
  T& back();
  Iterator begin() const;
  Iterator end() const;
  const Iterator cbegin() const;
  const Iterator cend() const;
  Iterator rbegin() const;
  Iterator rend() const;
  const Iterator rcbegin() const;
  const Iterator rcend() const;
  void clear();
  Iterator insert(Iterator pos, const T& element);
  Iterator erase(Iterator pos);
  Iterator erase(Iterator beginIt, Iterator endIt);
  void remove_if(std::function<bool(const T&)>&& p);
  void reverse();
  Iterator find(const T& element) const;
  

  private:
  void dealoc();
  Node<T>* head_{nullptr};
  Node<T>* tail_{nullptr};
  size_t size_{0};
};

template<typename T>
void DoubleLinkedList<T>::clear(){
auto tmp = head_;
while ( head_ != nullptr){
tmp = head_;
head_ = head_->next;
delete tmp;
};
size_ = 0;

};

template<typename T>
DoubleLinkedList<T>::DoubleLinkedList(){};

template<typename T>  
DoubleLinkedList<T>::DoubleLinkedList(const DoubleLinkedList& o){

auto tmp = o.head_;
  while(tmp != nullptr){
  push_back(tmp->data);
  tmp= tmp->next;
  };
};

template<typename T>
DoubleLinkedList<T>::DoubleLinkedList(DoubleLinkedList&& o){
head_ = o.head_;
tail_ = o.tail_;
size_= o.size_;

o.head_=o.tail_= nullptr;
o.size_ = 0;
};

template<typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator=(const DoubleLinkedList& o){

if(this == &o) return *this;

clear();

auto tmp = o.head_;
  while(tmp != nullptr){
  push_back(tmp->data);
  tmp= tmp->next;
  };

return *this;  
};

template<typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator=(DoubleLinkedList&& o){

if (this == &o) return *this;  

clear();

head_ = o.head_;
tail_ = o.tail_;
size_= o.size_;

o.head_=o.tail_= nullptr;
o.size_ = 0;
return *this;
  };


template<typename T>
DoubleLinkedList<T>::~DoubleLinkedList(){
clear();
};

template<typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::push_back(const T& n){

auto tmp = new Node<T>{n};

if(size_ == 0){
head_ = tail_ = tmp;
size_ ++;
return *this;
};

tail_->next = tmp;
tmp->prev = tail_;
tail_ = tmp;
size_ ++;

return *this;
};

template<typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::push_front(const T& n){

auto tmp = new Node<T>{n};

if(size_ == 0){
head_ = tail_ = tmp;
size_ ++;
return *this;
};

tmp->next = head_;
head_->prev = tmp;
head_ = tmp;
size_++;

return *this;
};

template<typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::push_back(T&& n){

auto tmp = new Node<T>{std::move(n)};

if(size_ == 0){
head_ = tail_ = tmp;
size_ ++;
return *this;
};

tail_->next = tmp;
tmp->prev = tail_;
tail_ = tmp;
size_ ++;

return *this;

};

template<typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::push_front(T&& n){

auto tmp = new Node<T>{std::move(n)};

if(size_ == 0){
head_ = tail_ = tmp;
size_ ++;
return *this;
};

tmp->next = head_;
head_->prev = tmp;
head_ = tmp;
size_++;

return *this;
};


template<typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::pop_front(){

if (empty()) throw std::out_of_range{"Prazna lista!"};

if(size_ == 1){
delete head_;
head_ = tail_ = nullptr;
size_--;
return *this;
};

auto tmp = head_;
//head_->next->prev = nullptr;
head_ = head_->next;
head_->prev = nullptr;
delete tmp;
size_--;

return *this;
};

template<typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::pop_back(){

if (empty()) throw std::out_of_range{"Prazna lista!"};

if(size_ == 1){
delete head_;
size_--;
return *this;
};

auto tmp = tail_;
tail_->prev->next=nullptr;
tail_ = tail_->prev;
delete tmp;
size_--;

return *this;
};

template<typename T>
bool DoubleLinkedList<T>::empty() const{
return size_ == 0;
};

template<typename T>
size_t DoubleLinkedList<T>::size() const{
return size_;
};

template<typename T>
T& DoubleLinkedList<T>::front(){
if (empty()) throw std::out_of_range{"Prazna lista!"};
return head_->data;
};

template<typename T>
T& DoubleLinkedList<T>::back(){
if (empty()) throw std::out_of_range{"Prazna lista!"};
return tail_->data;
};

template<typename T>
class DoubleLinkedList<T>::Iterator : public
std::iterator<std::bidirectional_iterator_tag, T>{ 
  public:
    friend class DoubleLinkedList<T>;
    Iterator() = default;
    Iterator(Node<T>* p) : p_{p} {};
    Iterator(const Iterator& o) : p_{o.p_} {};
    Iterator(Iterator&& o){
    p_=o.p_;
    o.p_=nullptr;
    };
    Iterator& operator=(const Iterator& o){
    p_=o.p_;
    return *this;
    };
    Iterator& operator=(Iterator&& o){
    p_=o.p_;
    o.p_=nullptr;
    return *this;
    };
    
   

    Iterator& operator++(){
      p_ = p_->next;
    return *this;
    };

    Iterator operator++(int){
    auto tmp = p_;
    
      p_ = p_->next;
    return Iterator{tmp};
    };

    Iterator& operator--(){
//    std::cout<< size() << std::endl;
    // if(p_ == nullptr){
    //   p_ = DoubleLinkedList<T>::tail_;
    //   return *this;
    // };
    p_ = p_->prev;
    return *this;
    };

    Iterator operator--(int){
       auto tmp = p_;
 //std::cout<< size() << std::endl;
    //  if(p_ == nullptr){
    //  p_ = DoubleLinkedList<T>::tail_;
    //  return Iterator{tmp};
    // };
    //
        p_ = p_->prev;
     return Iterator{tmp};
    };

    bool operator==(const Iterator o){
    return p_ == o.p_;
    };

    bool operator!=(const Iterator o){
    return p_!= o.p_;
    };

    T& operator*(){
    return p_->data;
    };
    
    const T& operator*() const{
    return p_->data;
    };

    T* operator->(){
    return &p_->data;
    };
    
    Node<T>* node(){
    return p_;
    };

  private:
    Node<T>* p_{nullptr};
};

template<typename T>
typename DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::begin() const{
if(size_ == 0) return Iterator{nullptr};
return Iterator{head_};
};
 template<typename T>
 typename DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::end() const{
   if(size_ == 0) return Iterator{nullptr};
   return Iterator{tail_->next};
 };
  template<typename T>
 typename DoubleLinkedList<T>::const_iterator DoubleLinkedList<T>::cbegin() const{
  if(size_ == 0) return const_iterator{nullptr};
   return const_iterator{head_};
 };
  template<typename T>
const typename DoubleLinkedList<T>::const_iterator DoubleLinkedList<T>::cend() const{
  if(size_ == 0) return const_iterator{nullptr};
  return const_iterator{tail_->next};
};
  template<typename T>
typename DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::rbegin() const{
  if(size_==0) return Iterator{nullptr};
  return Iterator{tail_}; 
}
  template<typename T>
typename DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::rend() const{
  if(size_==0) return Iterator{nullptr};
  return Iterator{head_->prev};
};
  template<typename T>
const typename DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::rcbegin() const{
 if(size_ == 0) return const_iterator{nullptr};
   return const_iterator{tail_};
};
  template<typename T>
const typename DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::rcend() const{
  if(size_ == 0) return const_iterator{nullptr};
  return const_iterator{head_->prev};
}
  template<typename T>
typename DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::insert(Iterator pos, const T& element){
auto tmp = new Node<T>(element);

if(size_==0){
head_=tail_=tmp;
//head_->next= head_->prev=tail_->next=tail_->prev = nullptr;
size_++;
return Iterator{head_};
}


// ubacivanje na pocetak
if(pos.node()==head_){
head_->prev= tmp;
tmp->next = head_;
head_ = tmp;
size_++;
return Iterator{tmp};
}  


tmp->prev = pos.node()->prev;
tmp->next = pos.node();
tmp->prev->next= tmp;
pos.node()->prev = tmp;
size_++;
return Iterator{tmp};
};


  template<typename T>
typename DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::erase(Iterator pos){

  if(pos == Iterator(tail_)){
    pop_back();
    return Iterator(tail_);
  };
  pos.node()->next->prev = pos.node()->prev;
  pos.node()->prev->next = pos.node()->next;
  auto tmp = pos.node()->next;
  delete pos.node();
  size_--;
  return Iterator{tmp};

};

template<typename T>
typename DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::erase(Iterator beginIt, Iterator endIt){

beginIt.node()->prev->next = endIt.node();
endIt.node()->prev = beginIt.node()->prev;
auto tmp = beginIt;
  while(beginIt != endIt){
  delete tmp.node();
  size_--;
  beginIt++;
  tmp = beginIt;
  };
  return endIt;
};
  template<typename T>
void DoubleLinkedList<T>::remove_if(std::function<bool(const T& e)>&& p){
Iterator start,del;
start = del = begin();
 while(start != nullptr){
  if(p(*start)){
    erase(del);
  };
  start++;
  del = start;
 };  
};

  template<typename T>
void DoubleLinkedList<T>::reverse(){
int iter = size()/2;
auto h = head_;
auto t = tail_;

for(int i=0; i<iter; i++){
  std::swap(head_->data,tail_->data);
  head_= head_->next;
  tail_ = tail_->prev;
};
  head_=h;
  tail_=t;
}
  template<typename T>
typename DoubleLinkedList<T>::Iterator DoubleLinkedList<T>::find(const T& element) const{

  auto it=begin();

  while(it!=end()){
   if(*it == element){
   return it;
   };
  it++;
  };
  return end();
};



