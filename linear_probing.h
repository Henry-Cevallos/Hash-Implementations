//Henry Cevallos
//This is a hash table with linear probing implemented
#ifndef LINEAR_PROBING_H
#define LINEAR_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>

// Linear probing implementation.
template <typename HashedObj>
class HashTableLinear {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  //Constructor
  explicit HashTableLinear(size_t size = 101) : array_(NextPrime(size))
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

  //Inserts passed obj into table and keeps track of collisions
  bool Insert(const HashedObj & x) 
  {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))  //prevents duplicates
      return false;
    
    array_[current_pos].element_ = x; 
    array_[current_pos].info_ = ACTIVE; //places x in pos and sets it to active
    
    if (++current_size_ > array_.size() / 2)  //if our load factor is greater than 50% then rehash 
      Rehash();    
    return true;
  }

  //Inserts passed R value obj 
  bool Insert(HashedObj && x) 
  {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))  //prevents duplicates
      return false;
    
    array_[current_pos] = std::move(x);
    array_[current_pos].info_ = ACTIVE; //places x in pos and sets it to active

    if (++current_size_ > array_.size() / 2)  //if our load factor is greater than 50% then rehash 
      Rehash();

    return true;
  }

  //Removes passed obj from the table
  bool Remove(const HashedObj & x) 
  {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos)) //if the pos is already not active
      return false;

    array_[current_pos].info_ = DELETED;  //set it to deleted
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

  //checks if position has an item in it
  bool IsActive(size_t current_pos) const
  { 
    return array_[current_pos].info_ == ACTIVE; 
  }

  //Finds the position of an obj 
  size_t FindPos(const HashedObj & x) const 
  {
    size_t current_pos = InternalHash(x);
      
    while (array_[current_pos].info_ != EMPTY &&
	   array_[current_pos].element_ != x) {
      current_pos += 1;  //adds 1 to current position if there is a collison (linear)
      current_pos %= array_.size(); //mod array_ size to wrap values around the table size
    }
    return current_pos;
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
  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size( );
  }
};
#endif  // LINEAR_PROBING_H
