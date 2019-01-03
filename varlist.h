#ifndef VARLIST_H
#define VARLIST_H

/*! \file VarList.h contains definition of a flexible array.
 * 
 *  VarList is a an array of variable length with most of the standard functions  
 *  you would expect from such a structure. 
 *
 *  The original code was created by my nephew, Aron. Thanks Aron!
*/

/*! \brief Definition for items in  varlist, containing a variable and a pointer to the next item in the varlist*/
template<typename T>
struct VarListItem {
   /*! \brief constructor for listitem */
   VarListItem(const T& value) : nextItem(nullptr), value(value) {};

   VarListItem<T>* nextItem;
   T value;
};

/*! \brief Defenition of varlist object */
template<typename T>
struct VarList {
   VarListItem<T>* firstItem;
   uint16_t size;

   /*! \brief default constructor with zero length */
   VarList();
   /*! \brief constructor with initialized length and default initialization for the items
    * 
    * \param length, integer for setting the initial lenght
    * \param defaultValue for the variables
    */
   VarList(uint16_t length, T defaultValue);
   /*! \brief Copy constructor, uses operator= to create a deep copy. 
    * 
    * \param old Varlist to be copied
    */
   VarList(const VarList<T>& old);
   /*! \brief destructor*/
   ~VarList();
   /* \brief the operator= performs a deep copy (the list items are copied as well)
    *  
    *  \param old, Varlist that is to be copied
    */
   VarList<T>& operator=(const VarList<T>& old);
   /* \brief Get varlistitem (read only) based on its index.
    *  
    *  \param index(integer), index of the requested item
    */
   VarListItem<T>* const getVarListItem(uint16_t index) const;
   /* \brief Get variable (read only) of varlistitem  at requested index.
    *  
    *  \param index(integer), index of the requested item
    */
   const T& get(uint16_t index) const;
   /* \brief Operator[], get variable (read only) of varlistitem  at requested index.
    *  
    *  \param index(integer), index of the requested item
    */
   const T& operator[](uint16_t index) const;
   /* \brief Get varlistitem based on its index.
    *  
    *  \param index(integer), index of the requested item
    */
   VarListItem<T>* getVarListItem(uint16_t index);
   /* \brief Get variable of varlistitem  at requested index.
    *  
    *  \param index(integer), index of the requested item
    */
   T& get(uint16_t index);
   /* \brief Operator[], get variable of varlistitem  at requested index.
    *  
    *  \param index(integer), index of the requested item
    */
   T& operator[](uint16_t index);
   /*! \brief pushes an item at the front of the list 
    * 
    * \param value, variable to be pushed at the front of the list
    */
   void push(T& value);
   /*! \brief pushes an item at the end of the list 
    * 
    * \param value, variable to be pushed at the end of the list
    */   
   void push_back(T& value);
   /* \brief Removes an item from the list at specified index.
    *  
    *  \param index (integer), the index of the item to be removed from the list. 
    */
   void removeItem(uint16_t index);   
};

template<typename T>
VarList<T>::VarList(uint16_t length, T defaultValue) : size(length) {
   VarListItem<T>* currentItem = firstItem = new VarListItem<T>(defaultValue);
   for(uint16_t i = 1; i < size; i++) {
      currentItem->nextItem = new VarListItem<T>(defaultValue);
      currentItem = currentItem->nextItem;
   }
}

template<typename T>
VarList<T>::VarList() : firstItem(nullptr), size(0) {}

template<typename T>
VarList<T>::VarList(const VarList<T>& old) {
   *this = old;
}

template<typename T>
VarList<T>::~VarList() {
   VarListItem<T>* currentItem = firstItem;
   VarListItem<T>* nextItem;
   while (currentItem != nullptr) {
      nextItem = currentItem->nextItem;
      delete currentItem;
      currentItem = nextItem;
   }
}



template<typename T>
VarList<T>& VarList<T>::operator=(const VarList<T>& old) {
   size=old.size;
   
   if (size == 0) {
      firstItem = nullptr;
      return *this;
   }
   VarListItem<T>* currentItem = firstItem = new VarListItem<T>(old[0]);
   for(uint16_t i = 1; i < size; i++) {
      currentItem->nextItem = new VarListItem<T>(old[i]);
      currentItem = currentItem->nextItem;
   }
   return *this;
}

template<typename T>
VarListItem<T>* const VarList<T>::getVarListItem(uint16_t index) const {
   VarListItem<T>* currentItem = firstItem;
   for(uint16_t j = 0; j < index && j < size-1; j++) {
      currentItem = currentItem->nextItem;
   }
   return currentItem;
}

template<typename T>
const T& VarList<T>::get(uint16_t index) const {
   /*if(size==0) {
    throw "invalid index";
   }*/
   return getVarListItem(index)->value;
}

template<typename T>
const T& VarList<T>::operator[](uint16_t index) const {
   return get(index);
}

template<typename T>
VarListItem<T>* VarList<T>::getVarListItem(uint16_t index) {
   VarListItem<T>* currentItem = firstItem;
   for(uint16_t j = 0; j < index && j < size-1; j++) {
      currentItem = currentItem->nextItem;
   }
   return currentItem;
}

template<typename T>
T& VarList<T>::get(uint16_t index) {
   return getVarListItem(index)->value;
}

template<typename T>
T& VarList<T>::operator[](uint16_t index) {
   return get(index);
}

template<typename T>
void VarList<T>::push(T& value) {
   VarListItem<T>* newItem = new VarListItem<T>(value);
   newItem->nextItem = firstItem;
   firstItem = newItem;
   size++;
}

template<typename T>
void VarList<T>::push_back(T& value) {
   if(size!=0) {
      getVarListItem(size-1)->nextItem = new VarListItem<T>(value);
   } else {
      firstItem = new VarListItem<T>(value);
   }
   size++;
}

template<typename T>
void VarList<T>::removeItem(uint16_t index) {
   if(index >= size)
      return;

   if(index!=0){
      VarListItem<T>* itemBefore = getVarListItem(index-1);
      VarListItem<T>* deleteItem = itemBefore->nextItem;
      itemBefore->nextItem = itemBefore->nextItem->nextItem;
      delete deleteItem;
   } else {
      VarListItem<T>* deleteItem = firstItem;
      firstItem = firstItem->nextItem;
      delete deleteItem;
   }
   size--;
}

#endif
