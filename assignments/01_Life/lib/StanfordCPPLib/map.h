/*
 * File: map.h
 * -----------
 * This file exports the template class <code>Map</code>, which
 * maintains a collection of <i>key</i>-<i>value</i> pairs.
 * 
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * - fixed bug where string quotes would not show when map was printed
 * @version 2014/11/13
 * - added comparison operators <, >=, etc.
 * - added add() method as synonym for put()
 * - added template hashCode function
 * @version 2014/10/10
 * - removed usage of __foreach macro
 */

#ifndef _map_h
#define _map_h

#include <cstdlib>
#include <map>
#include "compare.h"
#include "error.h"
#include "hashcode.h"
#include "stack.h"
#include "vector.h"

/*
 * Class: Map<KeyType,ValueType>
 * -----------------------------
 * This class maintains an association between <b><i>keys</i></b> and
 * <b><i>values</i></b>.  The types used for keys and values are
 * specified using templates, which makes it possible to use
 * this structure with any data type.
 */
template <typename KeyType, typename ValueType>
class Map {
public:
    /*
     * Constructor: Map
     * Usage: Map<KeyType,ValueType> map;
     * ----------------------------------
     * Initializes a new empty map that associates keys and values of the
     * specified types.
     */
    Map();

    /*
     * Destructor: ~Map
     * ----------------
     * Frees any heap storage associated with this map.
     */
    virtual ~Map();
    
    /*
     * Method: add
     * Usage: map.add(key, value);
     * ---------------------------
     * Associates <code>key</code> with <code>value</code> in this map.
     * A synonym for the put method.
     */
    void add(const KeyType& key, const ValueType& value);
    
    /*
     * Method: clear
     * Usage: map.clear();
     * -------------------
     * Removes all entries from this map.
     */
    void clear();
    
    /*
     * Method: containsKey
     * Usage: if (map.containsKey(key)) ...
     * ------------------------------------
     * Returns <code>true</code> if there is an entry for <code>key</code>
     * in this map.
     */
    bool containsKey(const KeyType& key) const;

    /*
     * Method: equals
     * Usage: if (map.equals(map2)) ...
     * --------------------------------
     * Returns <code>true</code> if the two maps contain exactly the same
     * key/value pairs, and <code>false</code> otherwise.
     */
    bool equals(const Map& map2) const;

    /*
     * Method: get
     * Usage: ValueType value = map.get(key);
     * --------------------------------------
     * Returns the value associated with <code>key</code> in this map.
     * If <code>key</code> is not found, <code>get</code> returns the
     * default value for <code>ValueType</code>.
     */
    ValueType get(const KeyType& key) const;

    /*
     * Method: isEmpty
     * Usage: if (map.isEmpty()) ...
     * -----------------------------
     * Returns <code>true</code> if this map contains no entries.
     */
    bool isEmpty() const;
    
    /*
     * Method: keys
     * Usage: Vector<KeyType> keys = map.keys();
     * -----------------------------------------
     * Returns a collection containing all keys in this map.
     * Note that this implementation makes a deep copy of the keys,
     * so it is inefficient to call on large maps.
     */
    Vector<KeyType> keys() const;
    
    /*
     * Method: mapAll
     * Usage: map.mapAll(fn);
     * ----------------------
     * Iterates through the map entries and calls <code>fn(key, value)</code>
     * for each one.  The keys are processed in ascending order, as defined
     * by the comparison function.
     */
    void mapAll(void (*fn)(KeyType, ValueType)) const;
    void mapAll(void (*fn)(const KeyType&, const ValueType&)) const;
    
    template <typename FunctorType>
    void mapAll(FunctorType fn) const;

    /*
     * Method: put
     * Usage: map.put(key, value);
     * ---------------------------
     * Associates <code>key</code> with <code>value</code> in this map.
     * Any previous value associated with <code>key</code> is replaced
     * by the new value.
     */
    void put(const KeyType& key, const ValueType& value);

    /*
     * Method: putAll
     * Usage: map.putAll(map2);
     * ---------------------------
     * Adds all key/value pairs from the given map to this map.
     * If both maps contain a pair for the same key, the one from map2 will
     * replace the one from this map.
     * Returns a reference to this map.
     */
    Map& putAll(const Map& map2);

    /*
     * Method: remove
     * Usage: map.remove(key);
     * -----------------------
     * Removes any entry for <code>key</code> from this map.
     */
    void remove(const KeyType& key);

    /*
     * Method: removeAll
     * Usage: map.removeAll(map2);
     * ---------------------------
     * Removes all key/value pairs from this map that are contained in the given map.
     * If both maps contain the same key but it maps to different values, that
     * mapping will not be removed.
     * Returns a reference to this map.
     */
    Map& removeAll(const Map& map2);

    /*
     * Method: retainAll
     * Usage: map.retainAll(map2);
     * ---------------------------
     * Removes all key/value pairs from this map that are not contained in the given map.
     * If both maps contain the same key but it maps to different values, that
     * mapping will be removed.
     * Returns a reference to this map.
     */
    Map& retainAll(const Map& map2);

    /*
     * Method: size
     * Usage: int nEntries = map.size();
     * ---------------------------------
     * Returns the number of entries in this map.
     */
    int size() const;
    
    /*
     * Returns an STL map object with the same elements as this map.
     */
    std::map<KeyType, ValueType> toStlMap() const;
    
    /*
     * Method: toString
     * Usage: string str = map.toString();
     * -----------------------------------
     * Converts the map to a printable string representation.
     */
    std::string toString() const;

    /*
     * Method: values
     * Usage: Vector<ValueType> values = map.values();
     * -----------------------------------------------
     * Returns a collection containing all values in this map.
     * Note that this implementation makes a deep copy of the values,
     * so it is inefficient to call on large maps.
     */
    Vector<ValueType> values() const;
    
    /*
     * Operator: []
     * Usage: map[key]
     * ---------------
     * Selects the value associated with <code>key</code>.  This syntax
     * makes it easy to think of a map as an "associative array"
     * indexed by the key type.  If <code>key</code> is already present
     * in the map, this function returns a reference to its associated
     * value.  If key is not present in the map, a new entry is created
     * whose value is set to the default for the value type.
     */
    ValueType& operator [](const KeyType& key);
    ValueType operator [](const KeyType& key) const;

    /*
     * Operator: ==
     * Usage: if (map1 == map2) ...
     * ----------------------------
     * Compares two maps for equality.
     */
    bool operator ==(const Map& map2) const;

    /*
     * Operator: !=
     * Usage: if (map1 != map2) ...
     * ----------------------------
     * Compares two maps for inequality.
     */
    bool operator !=(const Map& map2) const;

    /*
     * Operators: <, <=, >, >=
     * Usage: if (map1 < map2) ...
     * ---------------------------
     * Relational operators to compare two maps.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     */
    bool operator <(const Map& map2) const;
    bool operator <=(const Map& map2) const;
    bool operator >(const Map& map2) const;
    bool operator >=(const Map& map2) const;

    /*
     * Operator: +
     * Usage: map1 + map2
     * ------------------
     * Returns the union of the two maps, equivalent to a copy of the first map
     * with addAll called on it passing the second map as a parameter.
     * If the two maps both contain a mapping for the same key, the mapping
     * from the second map is favored.
     */
    Map operator +(const Map& map2) const;

    /*
     * Operator: +=
     * Usage: map1 += map2;
     * --------------------
     * Adds all key/value pairs from the given map to this map.
     * Equivalent to calling addAll(map2).
     */
    Map& operator +=(const Map& map2);

    /*
     * Operator: -
     * Usage: map1 - map2
     * ------------------
     * Returns the difference of the two maps, equivalent to a copy of the first map
     * with removeAll called on it passing the second map as a parameter.
     */
    Map operator -(const Map& map2) const;

    /*
     * Operator: -=
     * Usage: map1 -= map2;
     * --------------------
     * Removes all key/value pairs from the given map to this map.
     * Equivalent to calling removeAll(map2).
     */
    Map& operator -=(const Map& map2);

    /*
     * Operator: *
     * Usage: map1 * map2
     * ------------------
     * Returns the intersection of the two maps, equivalent to a copy of the first map
     * with retainAll called on it passing the second map as a parameter.
     */
    Map operator *(const Map& map2) const;

    /*
     * Operator: *=
     * Usage: map1 *= map2;
     * ---------------------
     * Removes all key/value pairs that are not found in the given map from this map.
     * Equivalent to calling retainAll(map2).
     */
    Map& operator *=(const Map& map2);

    /*
     * Additional Map operations
     * -------------------------
     * In addition to the methods listed in this interface, the Map
     * class supports the following operations:
     *
     *   - Stream I/O using the << and >> operators
     *   - Deep copying for the copy constructor and assignment operator
     *   - Iteration using the range-based for statement and STL iterators
     *
     * All iteration is guaranteed to proceed in the order established by
     * the comparison function passed to the constructor, which ordinarily
     * matches the order of the key type.
     */

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

    /*
     * Implementation notes:
     * ---------------------
     * The map class is represented using a binary search tree.  The
     * specific implementation used here is the classic AVL algorithm
     * developed by Georgii Adel'son-Vel'skii and Evgenii Landis in 1962.
     */

private:
    /* Constant definitions */
    static const int BST_LEFT_HEAVY = -1;
    static const int BST_IN_BALANCE = 0;
    static const int BST_RIGHT_HEAVY = +1;

    /* Type definition for nodes in the binary search tree */
    struct BSTNode {
        KeyType key;             /* The key stored in this node         */
        ValueType value;         /* The corresponding value             */
        BSTNode *left;           /* Subtree containing all smaller keys */
        BSTNode *right;          /* Subtree containing all larger keys  */
        int bf;                  /* AVL balance factor                  */
    };

    /*
     * Implementation notes: Comparator
     * --------------------------------
     * The Comparator class encapsulates a functor that compares two values
     * of KeyType.  In contrast to the classes in the STL, all of which embed
     * the comparator in the type, the Map class and its derivatives pass an
     * optional Comparator value.  While this strategy results in a more
     * complex implementation, it has the advantage of allowing maps and sets
     * to carry their own comparators without forcing the client to include
     * the comparator in the template declaration.  This simplification is
     * particularly important for the Graph class.
     *
     * The allocation is required in the TemplateComparator class because
     * the type std::binary_function has subclasses but does not define a
     * virtual destructor.
     */
    class Comparator {
    public:
        virtual ~Comparator() { /* empty */ }
        virtual bool lessThan(const KeyType& k1, const KeyType& k2) = 0;
        virtual Comparator* clone() = 0;
    };

    template <typename CompareType>
    class TemplateComparator : public Comparator {
    public:
        TemplateComparator(CompareType cmp) {
            this->cmp = new CompareType(cmp);
        }

        virtual bool lessThan(const KeyType& k1, const KeyType& k2) {
            return (*cmp)(k1, k2);
        }

        virtual Comparator* clone() {
            return new TemplateComparator<CompareType>(*cmp);
        }

    private:
        CompareType* cmp;
    };

    Comparator& getComparator() const {
        return *cmpp;
    }

    /* Instance variables */
    BSTNode*root;                   /* Pointer to the root of the tree */
    int nodeCount;                  /* Number of entries in the map    */
    Comparator* cmpp;               /* Pointer to the comparator       */

    /* Private methods */

    /*
     * Implementation notes: findNode(t, key)
     * --------------------------------------
     * Searches the tree rooted at t to find the specified key, searching
     * in the left or right subtree, as approriate.  If a matching node
     * is found, findNode returns a pointer to the value cell in that node.
     * If no matching node exists in the tree, findNode returns NULL.
     */
    ValueType* findNode(BSTNode* t, const KeyType& key) const {
        if (t == NULL) {
            return NULL;
        }
        int sign = compareKeys(key, t->key);
        if (sign == 0) {
            return &t->value;
        }
        if (sign < 0) {
            return findNode(t->left, key);
        } else {
            return findNode(t->right, key);
        }
    }

    /*
     * Implementation notes: addNode(t, key, heightFlag)
     * -------------------------------------------------
     * Searches the tree rooted at t to find the specified key, searching
     * in the left or right subtree, as approriate.  If a matching node
     * is found, addNode returns a pointer to the value cell in that node,
     * just like findNode.  If no matching node exists in the tree, addNode
     * creates a new node with a default value.  The heightFlag reference
     * parameter returns a bool indicating whether the height of the tree
     * was changed by this operation.
     */
    ValueType* addNode(BSTNode*& t, const KeyType& key, bool& heightFlag) {
        heightFlag = false;
        if (t == NULL)  {
            t = new BSTNode();
            t->key = key;
            t->value = ValueType();
            t->bf = BST_IN_BALANCE;
            t->left = t->right = NULL;
            heightFlag = true;
            nodeCount++;
            return &t->value;
        }
        
        int sign = compareKeys(key, t->key);
        if (sign == 0) {
            return &t->value;
        }
        ValueType* vp = NULL;
        int bfDelta = BST_IN_BALANCE;
        if (sign < 0) {
            vp = addNode(t->left, key, heightFlag);
            if (heightFlag) {
                bfDelta = BST_LEFT_HEAVY;
            }
        } else {
            vp = addNode(t->right, key, heightFlag);
            if (heightFlag) {
                bfDelta = BST_RIGHT_HEAVY;
            }
        }
        updateBF(t, bfDelta);
        heightFlag = (bfDelta != 0 && t->bf != BST_IN_BALANCE);
        return vp;
    }

    /*
     * Implementation notes: removeNode(t, key)
     * ----------------------------------------
     * Removes the node containing the specified key from the tree rooted
     * at t.  The return value is true if the height of this subtree
     * changes.  The removeTargetNode method does the actual deletion.
     */
    bool removeNode(BSTNode*& t, const KeyType& key) {
        if (t == NULL) {
            return false;
        }
        int sign = compareKeys(key, t->key);
        if (sign == 0) {
            return removeTargetNode(t);
        }
        int bfDelta = BST_IN_BALANCE;
        if (sign < 0) {
            if (removeNode(t->left, key)) {
                bfDelta = BST_RIGHT_HEAVY;
            }
        } else {
            if (removeNode(t->right, key)) {
                bfDelta = BST_LEFT_HEAVY;
            }
        }
        updateBF(t, bfDelta);
        return bfDelta != 0 && t->bf == BST_IN_BALANCE;
    }

    /*
     * Implementation notes: removeTargetNode(t)
     * -----------------------------------------
     * Removes the node which is passed by reference as t.  The easy case
     * occurs when either (or both) of the children is NULL; all you need
     * to do is replace the node with its non-NULL child, if any.  If both
     * children are non-NULL, this code finds the rightmost descendent of
     * the left child; this node may not be a leaf, but will have no right
     * child.  Its left child replaces it in the tree, after which the
     * replacement data is moved to the position occupied by the target node.
     */
    bool removeTargetNode(BSTNode*& t) {
        BSTNode* toDelete = t;
        if (t->left == NULL) {
            t = t->right;
            delete toDelete;
            nodeCount--;
            return true;
        } else if (t->right == NULL) {
            t = t->left;
            delete toDelete;
            nodeCount--;
            return true;
        } else {
            BSTNode* successor = t->left;
            while (successor->right != NULL) {
                successor = successor->right;
            }
            t->key = successor->key;
            t->value = successor->value;
            if (removeNode(t->left, successor->key)) {
                updateBF(t, BST_RIGHT_HEAVY);
                return (t->bf == BST_IN_BALANCE);
            }
            return false;
        }
    }

    /*
     * Implementation notes: updateBF(t, bfDelta)
     * ------------------------------------------
     * Updates the balance factor in the node and rebalances the tree
     * if necessary.
     */
    void updateBF(BSTNode*& t, int bfDelta) {
        t->bf += bfDelta;
        if (t->bf < BST_LEFT_HEAVY) {
            fixLeftImbalance(t);
        } else if (t->bf > BST_RIGHT_HEAVY) {
            fixRightImbalance(t);
        }
    }

    /*
     * Implementation notes: fixLeftImbalance(t)
     * -----------------------------------------
     * This function is called when a node has been found that is out
     * of balance with the longer subtree on the left.  Depending on
     * the balance factor of the left child, the code performs a
     * single or double rotation.
     */
    void fixLeftImbalance(BSTNode*& t) {
        BSTNode *child = t->left;
        if (child->bf == BST_RIGHT_HEAVY) {
            int oldBF = child->right->bf;
            rotateLeft(t->left);
            rotateRight(t);
            t->bf = BST_IN_BALANCE;
            switch (oldBF) {
            case BST_LEFT_HEAVY:
                t->left->bf = BST_IN_BALANCE;
                t->right->bf = BST_RIGHT_HEAVY;
                break;
            case BST_IN_BALANCE:
                t->left->bf = t->right->bf = BST_IN_BALANCE;
                break;
            case BST_RIGHT_HEAVY:
                t->left->bf = BST_LEFT_HEAVY;
                t->right->bf = BST_IN_BALANCE;
                break;
            }
        } else if (child->bf == BST_IN_BALANCE) {
            rotateRight(t);
            t->bf = BST_RIGHT_HEAVY;
            t->right->bf = BST_LEFT_HEAVY;
        } else {
            rotateRight(t);
            t->right->bf = t->bf = BST_IN_BALANCE;
        }
    }

    /*
     * Implementation notes: rotateLeft(t)
     * -----------------------------------
     * This function performs a single left rotation of the tree
     * that is passed by reference.  The balance factors
     * are unchanged by this function and must be corrected at a
     * higher level of the algorithm.
     */
    void rotateLeft(BSTNode*& t) {
        BSTNode* child = t->right;
        t->right = child->left;
        child->left = t;
        t = child;
    }

    /*
     * Implementation notes: fixRightImbalance(t)
     * ------------------------------------------
     * This function is called when a node has been found that
     * is out of balance with the longer subtree on the right.
     * Depending on the balance factor of the right child, the
     * code performs a single or double rotation.
     */
    void fixRightImbalance(BSTNode*& t) {
        BSTNode* child = t->right;
        if (child->bf == BST_LEFT_HEAVY) {
            int oldBF = child->left->bf;
            rotateRight(t->right);
            rotateLeft(t);
            t->bf = BST_IN_BALANCE;
            switch (oldBF) {
            case BST_LEFT_HEAVY:
                t->left->bf = BST_IN_BALANCE;
                t->right->bf = BST_RIGHT_HEAVY;
                break;
            case BST_IN_BALANCE:
                t->left->bf = t->right->bf = BST_IN_BALANCE;
                break;
            case BST_RIGHT_HEAVY:
                t->left->bf = BST_LEFT_HEAVY;
                t->right->bf = BST_IN_BALANCE;
                break;
            }
        } else if (child->bf == BST_IN_BALANCE) {
            rotateLeft(t);
            t->bf = BST_LEFT_HEAVY;
            t->left->bf = BST_RIGHT_HEAVY;
        } else {
            rotateLeft(t);
            t->left->bf = t->bf = BST_IN_BALANCE;
        }
    }

    /*
     * Implementation notes: rotateRight(t)
     * ------------------------------------
     * This function performs a single right rotation of the tree
     * that is passed by reference.  The balance factors
     * are unchanged by this function and must be corrected at a
     * higher level of the algorithm.
     */
    void rotateRight(BSTNode*& t) {

        BSTNode* child = t->left;
        t->left = child->right;
        child->right = t;
        t = child;
    }

    /*
     * Implementation notes: deleteTree(t)
     * -----------------------------------
     * Deletes all the nodes in the tree.
     */
    void deleteTree(BSTNode* t) {
        if (t != NULL) {
            deleteTree(t->left);
            deleteTree(t->right);
            delete t;
        }
    }

    /*
     * Implementation notes: mapAll
     * ----------------------------
     * Calls fn(key, value) for every key-value pair in the tree.
     */
    void mapAll(BSTNode* t, void (*fn)(KeyType, ValueType)) const {
        if (t != NULL) {
            mapAll(t->left, fn);
            fn(t->key, t->value);
            mapAll(t->right, fn);
        }
    }

    void mapAll(BSTNode* t,
                void (*fn)(const KeyType&, const ValueType&)) const {
        if (t != NULL) {
            mapAll(t->left, fn);
            fn(t->key, t->value);
            mapAll(t->right, fn);
        }
    }

    template <typename FunctorType>
    void mapAll(BSTNode* t, FunctorType fn) const {
        if (t != NULL) {
            mapAll(t->left, fn);
            fn(t->key, t->value);
            mapAll(t->right, fn);
        }
    }

    void deepCopy(const Map& other) {
        root = copyTree(other.root);
        nodeCount = other.nodeCount;
        cmpp = (other.cmpp == NULL) ? NULL : other.cmpp->clone();
    }

    BSTNode* copyTree(BSTNode* const t) {
        if (t == NULL) {
            return NULL;
        }
        BSTNode* np = new BSTNode;
        np->key = t->key;
        np->value = t->value;
        np->bf = t->bf;
        np->left = copyTree(t->left);
        np->right = copyTree(t->right);
        return np;
    }

public:
    /*
     * Hidden features
     * ---------------
     * The remainder of this file consists of the code required to
     * support deep copying and iteration.  Including these methods in
     * the public portion of the interface would make that interface more
     * difficult to understand for the average client.
     */

    /* Extended constructors */
    template <typename CompareType>
    explicit Map(CompareType cmp) {
        root = NULL;
        nodeCount = 0;
        cmpp = new TemplateComparator<CompareType>(cmp);
    }

    /*
     * Implementation notes: compareKeys(k1, k2)
     * -----------------------------------------
     * Compares the keys k1 and k2 and returns an integer (-1, 0, or +1)
     * depending on whether k1 < k2, k1 == k2, or k1 > k2, respectively.
     */
    int compareKeys(const KeyType& k1, const KeyType& k2) const {
        if (cmpp->lessThan(k1, k2)) {
            return -1;
        } else if (cmpp->lessThan(k2, k1)) {
            return +1;
        } else {
            return 0;
        }
    }

    /*
     * Deep copying support
     * --------------------
     * This copy constructor and operator= are defined to make a
     * deep copy, making it possible to pass/return maps by value
     * and assign from one map to another.
     */
    Map& operator =(const Map& src) {
        if (this != &src) {
            clear();
            deepCopy(src);
        }
        return *this;
    }

    Map(const Map& src) {
        deepCopy(src);
    }

    /*
     * Iterator support
     * ----------------
     * The classes in the StanfordCPPLib collection implement input
     * iterators so that they work symmetrically with respect to the
     * corresponding STL classes.
     */
    class iterator : public std::iterator<std::input_iterator_tag, KeyType> {
    private:
        struct NodeMarker {
            BSTNode* np;
            bool processed;
        };

        const Map* mp;               /* Pointer to the map         */
        int index;                   /* Index of current element   */
        Stack<NodeMarker> stack;     /* Stack of unprocessed nodes */

        void findLeftmostChild() {
            BSTNode *np = stack.peek().np;
            if (np == NULL) {
                return;
            }
            while (np->left != NULL) {
                NodeMarker marker = { np->left,  false };
                stack.push(marker);
                np = np->left;
            }
        }

    public:
        iterator() : mp(NULL), index(0) {
            /* Empty */
        }

        iterator(const Map* mp, bool end) {
            this->mp = mp;
            if (end || mp->nodeCount == 0) {
                index = mp->nodeCount;
            } else {
                index = 0;
                NodeMarker marker = { mp->root, false };
                stack.push(marker);
                findLeftmostChild();
            }
        }

        iterator(const iterator& it) {
            mp = it.mp;
            index = it.index;
            stack = it.stack;
        }

        iterator& operator ++() {
            NodeMarker marker = stack.pop();
            BSTNode* np = marker.np;
            if (np->right == NULL) {
                while (!stack.isEmpty() && stack.peek().processed) {
                    stack.pop();
                }
            } else {
                marker.processed = true;
                stack.push(marker);
                marker.np = np->right;
                marker.processed = false;
                stack.push(marker);
                findLeftmostChild();
            }
            index++;
            return *this;
        }

        iterator operator ++(int) {
            iterator copy(*this);
            operator++();
            return copy;
        }

        bool operator ==(const iterator& rhs) {
            return mp == rhs.mp && index == rhs.index;
        }

        bool operator !=(const iterator& rhs) {
            return !(*this == rhs);
        }

        KeyType& operator *() {
            return stack.peek().np->key;
        }

        KeyType* operator ->() {
            return &stack.peek().np->key;
        }

        friend class Map;
    };

    iterator begin() const {
        return iterator(this, /* end */ false);
    }

    iterator end() const {
        return iterator(this, /* end */ true);
    }
};

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::Map() {
    root = NULL;
    nodeCount = 0;
    cmpp = new TemplateComparator<std::less<KeyType> >(std::less<KeyType>());
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>::~Map() {
    if (cmpp != NULL) {
        delete cmpp;
    }
    deleteTree(root);
}

template <typename KeyType, typename ValueType>
void Map<KeyType, ValueType>::add(const KeyType& key,
                                  const ValueType& value) {
    put(key, value);
}

template <typename KeyType, typename ValueType>
void Map<KeyType, ValueType>::clear() {
    deleteTree(root);
    root = NULL;
    nodeCount = 0;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::containsKey(const KeyType& key) const {
    return findNode(root, key) != NULL;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::equals(const Map<KeyType, ValueType>& map2) const {
    if (this == &map2) {
        return true;
    }
    if (size() != map2.size()) {
        return false;
    }

    // compare both ways; each must be subset of the other
    for (KeyType key : *this) {
        if (!map2.containsKey(key) || map2.get(key) != get(key)) {
            return false;
        }
    }
    for (KeyType key : map2) {
        if (!containsKey(key) || get(key) != map2.get(key)) {
            return false;
        }
    }
    return true;
}

template <typename KeyType, typename ValueType>
ValueType Map<KeyType, ValueType>::get(const KeyType& key) const {
    ValueType* vp = findNode(root, key);
    if (vp == NULL) {
        return ValueType();
    }
    return *vp;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::isEmpty() const {
    return nodeCount == 0;
}

template <typename KeyType,typename ValueType>
Vector<KeyType> Map<KeyType, ValueType>::keys() const {
    Vector<KeyType> keyset;
    for (KeyType key : *this) {
        keyset.add(key);
    }
    return keyset;
}

template <typename KeyType, typename ValueType>
void Map<KeyType, ValueType>::mapAll(void (*fn)(KeyType, ValueType)) const {
    mapAll(root, fn);
}

template <typename KeyType, typename ValueType>
void Map<KeyType, ValueType>::mapAll(void (*fn)(const KeyType &,
                                                const ValueType &)) const {
    mapAll(root, fn);
}

template <typename KeyType, typename ValueType>
template <typename FunctorType>
void Map<KeyType, ValueType>::mapAll(FunctorType fn) const {
    mapAll(root, fn);
}

template <typename KeyType, typename ValueType>
void Map<KeyType, ValueType>::put(const KeyType& key,
                                  const ValueType& value) {
    bool dummy;
    *addNode(root, key, dummy) = value;
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>& Map<KeyType, ValueType>::putAll(const Map& map2) {
    for (KeyType key : map2) {
        put(key, map2.get(key));
    }
    return *this;
}

template <typename KeyType, typename ValueType>
void Map<KeyType, ValueType>::remove(const KeyType& key) {
    removeNode(root, key);
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>& Map<KeyType, ValueType>::removeAll(const Map& map2) {
    for (KeyType key : map2) {
        if (containsKey(key) && get(key) == map2.get(key)) {
            remove(key);
        }
    }
    return *this;
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>& Map<KeyType, ValueType>::retainAll(const Map& map2) {
    Vector<KeyType> toRemove;
    for (KeyType key : *this) {
        if (!map2.containsKey(key) || get(key) != map2.get(key)) {
            toRemove.add(key);
        }
    }
    for (KeyType key : toRemove) {
        remove(key);
    }
    return *this;
}

template <typename KeyType, typename ValueType>
int Map<KeyType, ValueType>::size() const {
    return nodeCount;
}

template <typename KeyType, typename ValueType>
std::map<KeyType, ValueType> Map<KeyType, ValueType>::toStlMap() const {
    std::map<KeyType, ValueType> result;
    for (KeyType key : *this) {
        result[key] = this->get(key);
    }
    return result;
}

template <typename KeyType, typename ValueType>
std::string Map<KeyType, ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

template <typename KeyType,typename ValueType>
Vector<ValueType> Map<KeyType, ValueType>::values() const {
    Vector<ValueType> values;
    for (KeyType key : *this) {
        values.add(this->get(key));
    }
    return values;
}

template <typename KeyType, typename ValueType>
ValueType & Map<KeyType, ValueType>::operator [](const KeyType& key) {
    bool dummy;
    return *addNode(root, key, dummy);
}

template <typename KeyType, typename ValueType>
ValueType Map<KeyType, ValueType>::operator [](const KeyType& key) const {
    return get(key);
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType> Map<KeyType, ValueType>::operator +(const Map& map2) const {
    Map<KeyType, ValueType> result = *this;
    return result.putAll(map2);
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>& Map<KeyType, ValueType>::operator +=(const Map& map2) {
    return putAll(map2);
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType> Map<KeyType, ValueType>::operator -(const Map& map2) const {
    Map<KeyType, ValueType> result = *this;
    return result.removeAll(map2);
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>& Map<KeyType, ValueType>::operator -=(const Map& map2) {
    return removeAll(map2);
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType> Map<KeyType, ValueType>::operator *(const Map& map2) const {
    Map<KeyType, ValueType> result = *this;
    return result.retainAll(map2);
}

template <typename KeyType, typename ValueType>
Map<KeyType, ValueType>& Map<KeyType, ValueType>::operator *=(const Map& map2) {
    return retainAll(map2);
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator ==(const Map& map2) const {
    return equals(map2);
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator !=(const Map& map2) const {
    return equals(map2);
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator <(const Map& map2) const {
    return compare::compare(*this, map2) < 0;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator <=(const Map& map2) const {
    return compare::compare(*this, map2) <= 0;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator >(const Map& map2) const {
    return compare::compare(*this, map2) > 0;
}

template <typename KeyType, typename ValueType>
bool Map<KeyType, ValueType>::operator >=(const Map& map2) const {
    return compare::compare(*this, map2) >= 0;
}

/*
 * Implementation notes: << and >>
 * -------------------------------
 * The insertion and extraction operators use the template facilities in
 * strlib.h to read and write generic values in a way that treats strings
 * specially.
 */
template <typename KeyType, typename ValueType>
std::ostream& operator <<(std::ostream& os,
                          const Map<KeyType, ValueType>& map) {
    os << "{";
    typename Map<KeyType, ValueType>::iterator begin = map.begin();
    typename Map<KeyType, ValueType>::iterator end = map.end();
    typename Map<KeyType, ValueType>::iterator it = begin;
    while (it != end) {
        if (it != begin) {
            os << ", ";
        }
        writeGenericValue(os, *it, true);
        os << ":";
        writeGenericValue(os, map[*it], true);
        ++it;
    }
    return os << "}";
}

template <typename KeyType, typename ValueType>
std::istream& operator >>(std::istream& is, Map<KeyType,ValueType>& map) {
    char ch = '\0';
    is >> ch;
    if (ch != '{') {
        error("Map::operator >>: Missing {");
    }
    map.clear();
    is >> ch;
    if (ch != '}') {
        is.unget();
        while (true) {
            KeyType key;
            readGenericValue(is, key);
            is >> ch;
            if (ch != ':') {
                error("Map::operator >>: Missing colon after key");
            }
            ValueType value;
            readGenericValue(is, value);
            map[key] = value;
            is >> ch;
            if (ch == '}') {
                break;
            }
            if (ch != ',') {
                error(std::string("Map::operator >>: Unexpected character ") + ch);
            }
        }
    }
    return is;
}

/*
 * Template hash function for maps.
 * Requires the key and value types in the Map to have a hashCode function.
 */
template <typename K, typename V>
int hashCode(const Map<K, V>& map) {
    int code = hashSeed();
    for (K k : map) {
        code = hashMultiplier() * code + hashCode(k);
        V v = map[k];
        code = hashMultiplier() * code + hashCode(v);
    }
    return int(code & hashMask());
}

/*
 * Function: randomKey
 * Usage: element = randomKey(map);
 * --------------------------------
 * Returns a randomly chosen key of the given map.
 * Throws an error if the map is empty.
 */
template <typename K, typename V>
const K& randomKey(const Map<K, V>& map) {
    if (map.isEmpty()) {
        error("randomElement: empty map was passed");
    }
    int index = randomInteger(0, map.size() - 1);
    int i = 0;
    for (const K& key : map) {
        if (i == index) {
            return key;
        }
        i++;
    }
    
    // this code will never be reached
    static Vector<K> v = map.keys();
    return v[0];
}

#endif
