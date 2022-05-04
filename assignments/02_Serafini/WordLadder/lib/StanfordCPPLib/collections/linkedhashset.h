/*
 * File: LinkedHashSet.h
 * ---------------------
 * This file exports the <code>LinkedHashSet</code> class, which
 * implements an efficient abstraction for storing sets of values.
 * 
 * @author Marty Stepp
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * @version 2016/09/22
 * - added containsAll, isSupersetOf methods
 * - added support for std initializer_list usage, such as {1, 2, 3}
 *   in constructor, addAll, containsAll, isSubsetOf, isSupersetOf, removeAll,
 *   retainAll, and operators +, +=, -, -=, *, *=
 * - bug fix in hashCode function
 * - added operators <, <=, >, >=
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2015/10/26
 * - initial version
 * @since 2015/10/26
 */

#ifndef _linkedhashset_h
#define _linkedhashset_h

#include <initializer_list>
#include <iostream>
#include<collections/collections.h>
#include <system/error.h>
#include <collections/hashcode.h>
#include "linkedhashmap.h"

/*
 * Class: LinkedHashSet<ValueType>
 * -------------------------------
 * Identical to a HashSet except that upon iteration using a for-each loop
 * or << / toString call, it will emit its elements in the order they were
 * originally inserted.  This is provided at a runtime and memory
 * cost due to needing to store an extra copy of the elements.
 */
template <typename ValueType>
class LinkedHashSet {
public:
    /*
     * Constructor: LinkedHashSet
     * Usage: LinkedHashSet<ValueType> set;
     * ------------------------------------
     * Initializes an empty set of the specified element type.
     */
    LinkedHashSet();

    /*
     * Constructor: LinkedHashSet
     * Usage: LinkedHashSet<ValueType> set {1, 2, 3};
     * ----------------------------------------------
     * Initializes a new set that stores the given elements.
     * Note that the elements are stored in unpredictable order internally and not
     * necessarily the order in which they are written in the initializer list.
     */
    LinkedHashSet(std::initializer_list<ValueType> list);

    /*
     * Destructor: ~LinkedHashSet
     * --------------------------
     * Frees any heap storage associated with this set.
     */
    virtual ~LinkedHashSet();

    /*
     * Method: add
     * Usage: set.add(value);
     * ----------------------
     * Adds an element to this set, if it was not already there.  For
     * compatibility with the STL <code>set</code> class, this method
     * is also exported as <code>insert</code>.
     */
    void add(const ValueType& value);
    
    /*
     * Method: addAll
     * Usage: set.addAll(set2);
     * ------------------------
     * Adds all elements of the given other set to this set.
     * Returns a reference to this set.
     * Identical in behavior to the += operator.
     */
    LinkedHashSet<ValueType>& addAll(const LinkedHashSet<ValueType>& set);
    LinkedHashSet<ValueType>& addAll(std::initializer_list<ValueType> list);

    /*
     * Method: clear
     * Usage: set.clear();
     * -------------------
     * Removes all elements from this set.
     */
    void clear();
    
    /*
     * Method: contains
     * Usage: if (set.contains(value)) ...
     * -----------------------------------
     * Returns <code>true</code> if the specified value is in this set.
     */
    bool contains(const ValueType& value) const;

    /*
     * Method: containsAll
     * Usage: if (set.containsAll(set2)) ...
     * -------------------------------------
     * Returns <code>true</code> if every value from the given other set
     * is also found in this set.
     * You can also pass an initializer list such as {1, 2, 3}.
     * Equivalent in behavior to isSupersetOf.
     */
    bool containsAll(const LinkedHashSet<ValueType>& set2) const;
    bool containsAll(std::initializer_list<ValueType> list) const;

    /*
     * Method: equals
     * Usage: if (set.equals(set2)) ...
     * --------------------------------
     * Returns <code>true</code> if this set contains exactly the same values
     * as the given other set.
     * Identical in behavior to the == operator.
     */
    bool equals(const LinkedHashSet<ValueType>& set2) const;
    
    /*
     * Method: first
     * Usage: ValueType value = set.first();
     * -------------------------------------
     * Returns the first value in the set in the order established by the
     * <code>foreach</code> macro.  If the set is empty, <code>first</code>
     * generates an error.
     */
    ValueType first() const;

    /*
     * Method: insert
     * Usage: set.insert(value);
     * -------------------------
     * Adds an element to this set, if it was not already there.  This
     * method is exported for compatibility with the STL <code>set</code> class.
     */
    void insert(const ValueType& value);

    /*
     * Method: isEmpty
     * Usage: if (set.isEmpty()) ...
     * -----------------------------
     * Returns <code>true</code> if this set contains no elements.
     */
    bool isEmpty() const;
    
    /*
     * Method: isSubsetOf
     * Usage: if (set.isSubsetOf(set2)) ...
     * ------------------------------------
     * Implements the subset relation on sets.  It returns
     * <code>true</code> if every element of this set is
     * contained in <code>set2</code>.
     */
    bool isSubsetOf(const LinkedHashSet& set2) const;
    bool isSubsetOf(std::initializer_list<ValueType> list) const;

    /*
     * Method: isSupersetOf
     * Usage: if (set.isSupersetOf(set2)) ...
     * --------------------------------------
     * Implements the superset relation on sets.  It returns
     * <code>true</code> if every element of this set is
     * contained in <code>set2</code>.
     * Note that this will be true if the sets are equal.
     * You can also pass an initializer list such as {1, 2, 3}.
     * Equivalent in behavior to containsAll.
     */
    bool isSupersetOf(const LinkedHashSet& set2) const;
    bool isSupersetOf(std::initializer_list<ValueType> list) const;

    /*
     * Method: mapAll
     * Usage: set.mapAll(fn);
     * ----------------------
     * Iterates through the elements of the set and calls <code>fn(value)</code>
     * for each one.  The values are processed in ascending order, as defined
     * by the comparison function.
     */
    void mapAll(void (*fn)(ValueType)) const;
    void mapAll(void (*fn)(const ValueType&)) const;

    template <typename FunctorType>
    void mapAll(FunctorType fn) const;
    
    /*
     * Method: remove
     * Usage: set.remove(value);
     * -------------------------
     * Removes an element from this set.  If the value was not
     * contained in the set, no error is generated and the set
     * remains unchanged.
     */
    void remove(const ValueType& value);
    
    /*
     * Method: removeAll
     * Usage: set.removeAll(set2);
     * ---------------------------
     * Removes all elements of the given other set from this set.
     * Returns a reference to this set.
     * Identical in behavior to the -= operator.
     */
    LinkedHashSet<ValueType>& removeAll(const LinkedHashSet<ValueType>& set);
    LinkedHashSet<ValueType>& removeAll(std::initializer_list<ValueType> list);

    /*
     * Method: retainAll
     * Usage: set.retainAll(set2);
     * ----------------------
     * Removes all elements from this set that are not contained in the given
     * other set. Returns a reference to this set.
     * Identical in behavior to the *= operator.
     */
    LinkedHashSet<ValueType>& retainAll(const LinkedHashSet<ValueType>& set);
    LinkedHashSet<ValueType>& retainAll(std::initializer_list<ValueType> list);

    /*
     * Method: size
     * Usage: count = set.size();
     * --------------------------
     * Returns the number of elements in this set.
     */
    int size() const;
    
    /*
     * Method: toString
     * Usage: string str = set.toString();
     * -----------------------------------
     * Converts the set to a printable string representation.
     */
    std::string toString() const;

    /*
     * Operator: ==
     * Usage: set1 == set2
     * -------------------
     * Returns <code>true</code> if <code>set1</code> and <code>set2</code>
     * contain the same elements.
     */
    bool operator ==(const LinkedHashSet& set2) const;

    /*
     * Operator: !=
     * Usage: set1 != set2
     * -------------------
     * Returns <code>true</code> if <code>set1</code> and <code>set2</code>
     * are different.
     */
    bool operator !=(const LinkedHashSet& set2) const;

    /*
     * Operators: <, >, <=, >=
     * Usage: if (set1 <= set2) ...
     * ...
     * ----------------------------
     * Relational operators to compare two sets.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     */
    bool operator <(const LinkedHashSet& set2) const;
    bool operator <=(const LinkedHashSet& set2) const;
    bool operator >(const LinkedHashSet& set2) const;
    bool operator >=(const LinkedHashSet& set2) const;

    /*
     * Operator: +
     * Usage: set1 + set2
     *        set1 + element
     * ---------------------
     * Returns the union of sets <code>set1</code> and <code>set2</code>, which
     * is the set of elements that appear in at least one of the two sets.  The
     * right hand set can be replaced by an element of the value type, in which
     * case the operator returns a new set formed by adding that element.
     */
    LinkedHashSet operator +(const LinkedHashSet& set2) const;
    LinkedHashSet operator +(std::initializer_list<ValueType> list) const;
    LinkedHashSet operator +(const ValueType& element) const;

    /*
     * Operator: *
     * Usage: set1 * set2
     * ------------------
     * Returns the intersection of sets <code>set1</code> and <code>set2</code>,
     * which is the set of all elements that appear in both.
     */
    LinkedHashSet operator *(const LinkedHashSet& set2) const;
    LinkedHashSet operator *(std::initializer_list<ValueType> list) const;

    /*
     * Operator: -
     * Usage: set1 - set2
     *        set1 - element
     * ---------------------
     * Returns the difference of sets <code>set1</code> and <code>set2</code>,
     * which is all of the elements that appear in <code>set1</code> but
     * not <code>set2</code>.  The right hand set can be replaced by an
     * element of the value type, in which case the operator returns a new
     * set formed by removing that element.
     */
    LinkedHashSet operator -(const LinkedHashSet& set2) const;
    LinkedHashSet operator -(std::initializer_list<ValueType> list) const;
    LinkedHashSet operator -(const ValueType& element) const;

    /*
     * Operator: +=
     * Usage: set1 += set2;
     *        set1 += value;
     * ---------------------
     * Adds all of the elements from <code>set2</code> (or the single
     * specified value) to <code>set1</code>.  As a convenience, the
     * <code>LinkedHashSet</code> package also overloads the comma operator so
     * that it is possible to initialize a set like this:
     *
     *<pre>
     *    LinkedHashSet&lt;int&lt; digits;
     *    digits += 0, 1, 2, 3, 4, 5, 6, 7, 8, 9;
     *</pre>
     */
    LinkedHashSet& operator +=(const LinkedHashSet& set2);
    LinkedHashSet& operator +=(std::initializer_list<ValueType> list);
    LinkedHashSet& operator +=(const ValueType& value);

    /*
     * Operator: *=
     * Usage: set1 *= set2;
     * --------------------
     * Removes any elements from <code>set1</code> that are not present in
     * <code>set2</code>.
     */
    LinkedHashSet& operator *=(const LinkedHashSet& set2);
    LinkedHashSet& operator *=(std::initializer_list<ValueType> list);

    /*
     * Operator: -=
     * Usage: set1 -= set2;
     *        set1 -= value;
     * ---------------------
     * Removes the elements from <code>set2</code> (or the single
     * specified value) from <code>set1</code>.  As a convenience, the
     * <code>LinkedHashSet</code> package also overloads the comma operator so
     * that it is possible to remove multiple elements from a set
     * like this:
     *
     *<pre>
     *    digits -= 0, 2, 4, 6, 8;
     *</pre>
     *
     * which removes the values 0, 2, 4, 6, and 8 from the set
     * <code>digits</code>.
     */
    LinkedHashSet& operator -=(const LinkedHashSet& set2);
    LinkedHashSet& operator -=(std::initializer_list<ValueType> list);
    LinkedHashSet& operator -=(const ValueType& value);

    /*
     * Additional LinkedHashSet operations
     * -----------------------------------
     * In addition to the methods listed in this interface, the LinkedHashSet
     * class supports the following operations:
     *
     *   - Stream I/O using the << and >> operators
     *   - Deep copying for the copy constructor and assignment operator
     *   - Iteration using the range-based for statement and STL iterators
     *
     * The iteration forms process the LinkedHashSet in the order of insertion.
     */

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

private:
    LinkedHashMap<ValueType, bool> map;  /* Map used to store the element     */
    bool removeFlag;                     /* Flag to differentiate += and -=   */

public:
    /*
     * Hidden features
     * ---------------
     * The remainder of this file consists of the code required to
     * support the comma operator, deep copying, and iteration.
     * Including these methods in the public interface would make
     * that interface more difficult to understand for the average client.
     */
    LinkedHashSet& operator ,(const ValueType& value) {
        if (this->removeFlag) {
            this->remove(value);
        } else {
            this->add(value);
        }
        return *this;
    }

    /*
     * Iterator support
     * ----------------
     * The classes in the StanfordCPPLib collection implement input
     * iterators so that they work symmetrically with respect to the
     * corresponding STL classes.
     */
    class iterator : public std::iterator<std::input_iterator_tag,ValueType> {
    private:
        typename LinkedHashMap<ValueType,bool>::iterator mapit;

    public:
        iterator() {
            /* Empty */
        }

        iterator(typename LinkedHashMap<ValueType, bool>::iterator it) : mapit(it) {
            /* Empty */
        }

        iterator(const iterator& it) {
            mapit = it.mapit;
        }

        iterator& operator ++() {
            ++mapit;
            return *this;
        }

        iterator operator ++(int) {
            iterator copy(*this);
            operator++();
            return copy;
        }

        bool operator ==(const iterator& rhs) {
            return mapit == rhs.mapit;
        }

        bool operator !=(const iterator& rhs) {
            return !(*this == rhs);
        }

        ValueType& operator *() {
            return *mapit;
        }

        ValueType* operator ->() {
            return mapit;
        }
    };

    iterator begin() const {
        return iterator(map.begin());
    }

    iterator end() const {
        return iterator(map.end());
    }
};

template <typename ValueType>
LinkedHashSet<ValueType>::LinkedHashSet() : removeFlag(false) {
    /* Empty */
}

template <typename ValueType>
LinkedHashSet<ValueType>::LinkedHashSet(std::initializer_list<ValueType> list)
        : removeFlag(false) {
    addAll(list);
}

template <typename ValueType>
LinkedHashSet<ValueType>::~LinkedHashSet() {
    /* Empty */
}

template <typename ValueType>
void LinkedHashSet<ValueType>::add(const ValueType& value) {
    map.put(value, true);
}

template <typename ValueType>
LinkedHashSet<ValueType>& LinkedHashSet<ValueType>::addAll(const LinkedHashSet& set2) {
    for (ValueType value : set2) {
        this->add(value);
    }
    return *this;
}

template <typename ValueType>
LinkedHashSet<ValueType>& LinkedHashSet<ValueType>::addAll(std::initializer_list<ValueType> list) {
    for (const ValueType& value : list) {
        this->add(value);
    }
    return *this;
}

template <typename ValueType>
void LinkedHashSet<ValueType>::clear() {
    map.clear();
}

template <typename ValueType>
bool LinkedHashSet<ValueType>::contains(const ValueType& value) const {
    return map.containsKey(value);
}

template <typename ValueType>
bool LinkedHashSet<ValueType>::containsAll(const LinkedHashSet<ValueType>& set2) const {
    for (const ValueType& value : set2) {
        if (!contains(value)) {
            return false;
        }
    }
    return true;
}

template <typename ValueType>
bool LinkedHashSet<ValueType>::containsAll(std::initializer_list<ValueType> list) const {
    for (const ValueType& value : list) {
        if (!contains(value)) {
            return false;
        }
    }
    return true;
}

template <typename ValueType>
bool LinkedHashSet<ValueType>::equals(const LinkedHashSet<ValueType>& set2) const {
    // optimization: if literally same set, stop
    if (this == &set2) {
        return true;
    }

    if (size() != set2.size()) {
        return false;
    }

    return isSubsetOf(set2) && set2.isSubsetOf(*this);
}

template <typename ValueType>
ValueType LinkedHashSet<ValueType>::first() const {
    if (isEmpty()) {
        error("LinkedHashSet::first: set is empty");
    }
    return *begin();
}

template <typename ValueType>
void LinkedHashSet<ValueType>::insert(const ValueType& value) {
    map.put(value, true);
}

template <typename ValueType>
bool LinkedHashSet<ValueType>::isEmpty() const {
    return map.isEmpty();
}

template <typename ValueType>
bool LinkedHashSet<ValueType>::isSubsetOf(const LinkedHashSet& set2) const {
    iterator it = begin();
    iterator end = this->end();
    while (it != end) {
        if (!set2.map.containsKey(*it)) {
            return false;
        }
        ++it;
    }
    return true;
}

template <typename ValueType>
bool LinkedHashSet<ValueType>::isSubsetOf(std::initializer_list<ValueType> list) const {
    LinkedHashSet<ValueType> set2(list);
    return isSubsetOf(set2);
}

template <typename ValueType>
bool LinkedHashSet<ValueType>::isSupersetOf(const LinkedHashSet& set2) const {
    return containsAll(set2);
}

template <typename ValueType>
bool LinkedHashSet<ValueType>::isSupersetOf(std::initializer_list<ValueType> list) const {
    return containsAll(list);
}

template <typename ValueType>
void LinkedHashSet<ValueType>::mapAll(void (*fn)(ValueType)) const {
    map.mapAll(fn);
}

template <typename ValueType>
void LinkedHashSet<ValueType>::mapAll(void (*fn)(const ValueType&)) const {
    map.mapAll(fn);
}

template <typename ValueType>
template <typename FunctorType>
void LinkedHashSet<ValueType>::mapAll(FunctorType fn) const {
    map.mapAll(fn);
}

template <typename ValueType>
void LinkedHashSet<ValueType>::remove(const ValueType& value) {
    map.remove(value);
}

template <typename ValueType>
LinkedHashSet<ValueType>& LinkedHashSet<ValueType>::removeAll(const LinkedHashSet& set2) {
    Vector<ValueType> toRemove;
    for (const ValueType& value : *this) {
        if (set2.map.containsKey(value)) {
            toRemove.add(value);
        }
    }
    for (const ValueType& value : toRemove) {
        remove(value);
    }
    return *this;
}

template <typename ValueType>
LinkedHashSet<ValueType>& LinkedHashSet<ValueType>::removeAll(std::initializer_list<ValueType> list) {
    for (const ValueType& value : list) {
        remove(value);
    }
    return *this;
}

template <typename ValueType>
LinkedHashSet<ValueType>& LinkedHashSet<ValueType>::retainAll(const LinkedHashSet& set2) {
    Vector<ValueType> toRemove;
    for (const ValueType& value : *this) {
        if (!set2.map.containsKey(value)) {
            toRemove.add(value);
        }
    }
    for (const ValueType& value : toRemove) {
        remove(value);
    }
    return *this;
}

template <typename ValueType>
LinkedHashSet<ValueType>& LinkedHashSet<ValueType>::retainAll(std::initializer_list<ValueType> list) {
    LinkedHashSet<ValueType> set2(list);
    return retainAll(set2);
}

template <typename ValueType>
int LinkedHashSet<ValueType>::size() const {
    return map.size();
}

template <typename ValueType>
std::string LinkedHashSet<ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

/*
 * Implementation notes: set operators
 * -----------------------------------
 * The implementations for the set operators use iteration to walk
 * over the elements in one or both sets.
 */
template <typename ValueType>
bool LinkedHashSet<ValueType>::operator ==(const LinkedHashSet& set2) const {
    return equals(set2);
}

template <typename ValueType>
bool LinkedHashSet<ValueType>::operator !=(const LinkedHashSet& set2) const {
    return !equals(set2);
}

// Implementation note:
// The definitions of <, <=, >, and >= are a bit unintuitive here.
// Because Sets are considered to be "equal" if they have the same elements,
// regardless of order, the equals() method and == / != operators ignore order.
//
// Example: {1, 2, 3, 4} == {4, 3, 2, 1}  true
//
// Similarly, if you ask whether a set is <= or >= to another, this includes
// the notion of equality, so it should return true if the sets contain the same
// elements, regardless of order.
//
// Example: {1, 2, 3, 4} == {4, 3, 2, 1}  true
// Example: {1, 2, 3, 4} <= {4, 3, 2, 1}  true
// Example: {1, 2, 3, 4} >= {4, 3, 2, 1}  true
//
// If you ask whether a set is < or > to another, it is assumed that you don't
// want this to return true if they are "equal", so we must check for non-equality
// before checking the elements pairwise.
//
// Example: {1, 2, 3, 4} <  {4, 3, 2, 1}       false   (because equal)
// Example: {4, 2, 3, 1} >  {1, 3, 2, 4}       false   (because equal)
// Example: {1, 2, 3, 4} <  {4, 3, 2}          true
// Example: {1, 2, 3, 4} <= {4, 3, 2}          true
// Example: {1, 2, 3, 4} <  {4, 3, 2, 1, 0}    true
// Example: {1, 2, 3, 4} <= {4, 3, 2, 1, 0}    true
//
// This issue is unique to LinkedHashSet because Set sorts into a predictable sorted order
// and HashSet opts not to implement <, <=, >, or >= due to its unpredictable hash order.

template <typename ValueType>
bool LinkedHashSet<ValueType>::operator <(const LinkedHashSet& set2) const {
    return !equals(set2) && stanfordcpplib::collections::compare(*this, set2) < 0;
}

template <typename ValueType>
bool LinkedHashSet<ValueType>::operator <=(const LinkedHashSet& set2) const {
    return equals(set2) || stanfordcpplib::collections::compare(*this, set2) <= 0;
}

template <typename ValueType>
bool LinkedHashSet<ValueType>::operator >(const LinkedHashSet& set2) const {
    return !equals(set2) && stanfordcpplib::collections::compare(*this, set2) > 0;
}

template <typename ValueType>
bool LinkedHashSet<ValueType>::operator >=(const LinkedHashSet& set2) const {
    return equals(set2) || stanfordcpplib::collections::compare(*this, set2) >= 0;
}

template <typename ValueType>
LinkedHashSet<ValueType> LinkedHashSet<ValueType>::operator +(const LinkedHashSet& set2) const {
    LinkedHashSet<ValueType> set = *this;
    set.addAll(set2);
    return set;
}

template <typename ValueType>
LinkedHashSet<ValueType> LinkedHashSet<ValueType>::operator +(std::initializer_list<ValueType> list) const {
    LinkedHashSet<ValueType> set = *this;
    set.addAll(list);
    return set;
}

template <typename ValueType>
LinkedHashSet<ValueType> LinkedHashSet<ValueType>::operator +(const ValueType& element) const {
    LinkedHashSet<ValueType> set = *this;
    set.add(element);
    return set;
}

template <typename ValueType>
LinkedHashSet<ValueType> LinkedHashSet<ValueType>::operator *(const LinkedHashSet& set2) const {
    LinkedHashSet<ValueType> set = *this;
    return set.retainAll(set2);
}

template <typename ValueType>
LinkedHashSet<ValueType> LinkedHashSet<ValueType>::operator *(std::initializer_list<ValueType> list) const {
    LinkedHashSet<ValueType> set = *this;
    return set.retainAll(list);
}

template <typename ValueType>
LinkedHashSet<ValueType> LinkedHashSet<ValueType>::operator -(const LinkedHashSet& set2) const {
    LinkedHashSet<ValueType> set = *this;
    return set.removeAll(set2);
}

template <typename ValueType>
LinkedHashSet<ValueType> LinkedHashSet<ValueType>::operator -(std::initializer_list<ValueType> list) const {
    LinkedHashSet<ValueType> set = *this;
    return set.removeAll(list);
}

template <typename ValueType>
LinkedHashSet<ValueType> LinkedHashSet<ValueType>::operator -(const ValueType& element) const {
    LinkedHashSet<ValueType> set = *this;
    set.remove(element);
    return set;
}

template <typename ValueType>
LinkedHashSet<ValueType>& LinkedHashSet<ValueType>::operator +=(const LinkedHashSet& set2) {
    return addAll(set2);
}

template <typename ValueType>
LinkedHashSet<ValueType>& LinkedHashSet<ValueType>::operator +=(std::initializer_list<ValueType> list) {
    return addAll(list);
}

template <typename ValueType>
LinkedHashSet<ValueType>& LinkedHashSet<ValueType>::operator +=(const ValueType& value) {
    add(value);
    removeFlag = false;
    return *this;
}

template <typename ValueType>
LinkedHashSet<ValueType>& LinkedHashSet<ValueType>::operator *=(const LinkedHashSet& set2) {
    return retainAll(set2);
}

template <typename ValueType>
LinkedHashSet<ValueType>& LinkedHashSet<ValueType>::operator *=(std::initializer_list<ValueType> list) {
    return retainAll(list);
}

template <typename ValueType>
LinkedHashSet<ValueType>& LinkedHashSet<ValueType>::operator -=(const LinkedHashSet& set2) {
    return removeAll(set2);
}

template <typename ValueType>
LinkedHashSet<ValueType>& LinkedHashSet<ValueType>::operator -=(std::initializer_list<ValueType> list) {
    return removeAll(list);
}

template <typename ValueType>
LinkedHashSet<ValueType>& LinkedHashSet<ValueType>::operator -=(const ValueType& value) {
    remove(value);
    removeFlag = true;
    return *this;
}

template <typename ValueType>
std::ostream& operator <<(std::ostream& os, const LinkedHashSet<ValueType>& set) {
    return stanfordcpplib::collections::writeCollection(os, set);
}

template <typename ValueType>
std::istream& operator >>(std::istream& is, LinkedHashSet<ValueType>& set) {
    ValueType element;
    return stanfordcpplib::collections::readCollection(is, set, element, /* descriptor */ "LinkedHashSet::operator >>");
}

/*
 * Template hash function for hash sets.
 * Requires the element type in the LinkedHashSet to have a hashCode function.
 */
template <typename T>
int hashCode(const LinkedHashSet<T>& set) {
    return stanfordcpplib::collections::hashCodeCollection(set, /* orderMatters */ false);
}

/*
 * Function: randomElement
 * Usage: element = randomElement(set);
 * ------------------------------------
 * Returns a randomly chosen element of the given set.
 * Throws an error if the set is empty.
 */
template <typename T>
const T& randomElement(const LinkedHashSet<T>& set) {
    return stanfordcpplib::collections::randomElement(set);
}

#include <private/init.h>   // ensure that Stanford C++ lib is initialized

#endif // _linkedhashset_h
