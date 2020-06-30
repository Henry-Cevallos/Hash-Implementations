//Henry Cevallos
//This file is a hash table with double hashing implemented

#ifndef DOUBLE_HASHING_H
#define DOUBLE_HASHING_H

#include <vector>
#include <algorithm>
#include <functional>
template <typename HashedObj>
class HashTableDouble {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  //Constructor
  explicit HashTableDouble(size_t rValue = 97, size_t size = 101) : array_(NextPrime(size)), r_value_(rValue)
    { MakeEmpty(); }

  //Checks if passed obj is in table and keeps track of probes
  bool Contains(const HashedObj & x) const 
  {
    return IsActive(FindPos(x);
  }
  
  //Makes hash empty
  void MakeEmpty() 
  {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  //Inserts passed obj into table 
  bool Insert(const HashedObj & x) 
  {
    
    size_t current_pos = FindPos(x); 
    if (IsActive(current_pos))  //does not allow for duplicates
      return false;
    
    array_[current_pos].element_ = x; //set element to x
    array_[current_pos].info_ = ACTIVE; //set to active
    
    if (++current_size_ > array_.size() / 2)  //if our load factor is greater than 50% then rehash
      Rehash();

    return true;
  }

  //Inserts passed R value obj 
  bool Insert(HashedObj && x) 
  {
    
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))   //does not allow for duplicates
      return false;
    
    array_[current_pos] = std::move(x); //set element to x
    array_[current_pos].info_ = ACTIVE; //set to active

    if (++current_size_ > array_.size() / 2)  //if our load factor is greater than 50% then rehash
      Rehash();

    return true;
  }

  //Removes passed obj from the table
  bool Remove(const HashedObj & x) 
  {
    size_t current_pos = FindPos(x);  //gets pos of x
    if (!IsActive(current_pos)) //if it is not found
      return false;

    array_[current_pos].info_ = DELETED;  //set to deleted
    return true;
  }

  //gets total items in array_
  int getItemCount()
  {
    return current_size_;
  }

  //gets size of array_
  int getTableSize()
  {
    return array_.size();
  }

 private:        
  struct HashEntry 
  {
    HashedObj element_;
    EntryType info_;
    
    HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
    :element_{e}, info_{i} { }
    
    HashEntry(HashedObj && e, EntryType i = EMPTY)
    :element_{std::move(e)}, info_{ i } {}
  };
    
  //Private variables
  std::vector<HashEntry> array_;
  size_t current_size_;
  size_t r_value_;  //R value passed

  //checks if position has an item in it
  bool IsActive(size_t current_pos) const
  { 
    return array_[current_pos].info_ == ACTIVE; 
  }

  //Finds the position of an obj 
  size_t FindPos(const HashedObj & x) const 
  {
    size_t current_pos = InternalHash(x); //the position where it should be 
    size_t attempts = 1;
    while (array_[current_pos].info_ != EMPTY &&  //if pos not euqal to x then we double hash to next possible pos
	   array_[current_pos].element_ != x) {
       current_pos = (current_pos + attempts * DoubleHash(current_pos)) % array_.size();  //double hash
       attempts++;
    }
    return current_pos; //item found
  }

  //Increases size of table by double, rehashes objects within it
  void Rehash() 
  {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(NextPrime(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    
    // Copy table over.
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	      Insert(std::move(entry.element_));  //rehash it so its well hashed according to new size
  }
  
  //hashes the passed object
  size_t InternalHash(const HashedObj & x) const 
  {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size();
  }

  //Double hash function of the passed key
  size_t DoubleHash(const HashedObj& key) const
  {
    std::hash<HashedObj> hf;
    return (r_value_ - (hf(key) % r_value_)); //uses R value to double hash
  }

};
#endif  // DOUBLE_HASHING_H_
