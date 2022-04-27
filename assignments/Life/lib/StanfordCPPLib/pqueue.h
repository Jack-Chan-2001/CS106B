/*
 * File: pqueue.h
 * --------------
 * This file exports the <code>PriorityQueue</code> class, a
 * collection in which values are processed in priority order.
 * 
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2015/06/22
 * - added optional compiler flag PQUEUE_PRINT_IN_HEAP_ORDER to indicate
 *   that PQ should be printed in heap-internal order rather than sorted order
 * - added optional compiler flag PQUEUE_ALLOW_HEAP_ACCESS and corresponding
 *   semi-private methods to access value/priority at a given index
 *   (wanted internally for some testing and for practice exam problems;
 *    not meant to be called explicitly by students or most clients)
 * @version 2014/12/04
 * - moved comparison operators <, >=, etc. behind a (default-disabled)
 *   compiler flag because implementation is inefficient
 * @version 2014/11/13
 * - added comparison operators <, >=, etc.
 * - added add() method as synonym for enqueue()
 * - added remove() method as synonym for dequeue()
 * - added template hashCode function
 * @version 2014/10/20
 * - added equals method, ==, != operators
 * @version 2014/10/10
 * - removed usage of __foreach macro
 * 2014/02/01
 * - added changePriority member to raise a given value's priority
 */

#ifndef _pqueue_h
#define _pqueue_h

#include "compare.h"
#include "error.h"
#include "hashcode.h"
#include "vector.h"

/*
 * Class: PriorityQueue<ValueType>
 * -------------------------------
 * This class models a structure called a <b><i>priority&nbsp;queue</i></b>
 * in which values are processed in order of priority.  As in conventional
 * English usage, lower priority numbers correspond to higher effective
 * priorities, so that a priority 1 item takes precedence over a
 * priority 2 item.
 */

template <typename ValueType>
class PriorityQueue {
public:
    /*
     * Constructor: PriorityQueue
     * Usage: PriorityQueue<ValueType> pq;
     * -----------------------------------
     * Initializes a new priority queue, which is initially empty.
     */
    PriorityQueue();

    /*
     * Destructor: ~PriorityQueue
     * --------------------------
     * Frees any heap storage associated with this priority queue.
     */
    virtual ~PriorityQueue();
    
    /*
     * Method: add
     * Usage: pq.add(value, priority);
     * -------------------------------
     * A synonym for the enqueue method.
     */
    void add(const ValueType& value, double priority);
    
    /*
     * Method: back
     * Usage: ValueType last = pq.back();
     * ----------------------------------
     * Returns the last value in the queue by reference.
     */
    ValueType& back();
    
    /*
     * Method: changePriority
     * Usage: pq.changePriority(value, newPriority);
     * ---------------------------------------------
     * Adjusts <code>value</code> in the queue to now have the specified new priority,
     * which must be at least as urgent (lower number) than that value's previous
     * priority in the queue.
     * Throws an error if the element value is not present in the queue, or if the
     * new priority passed is not at least as urgent as its current priority.
     */
    void changePriority(ValueType value, double newPriority);

    /*
     * Method: clear
     * Usage: pq.clear();
     * ------------------
     * Removes all elements from the priority queue.
     */
    void clear();
    
    /*
     * Method: dequeue
     * Usage: ValueType first = pq.dequeue();
     * --------------------------------------
     * Removes and returns the highest priority value.  If multiple
     * entries in the queue have the same priority, those values are
     * dequeued in the same order in which they were enqueued.
     */
    ValueType dequeue();

    /*
     * Method: enqueue
     * Usage: pq.enqueue(value, priority);
     * -----------------------------------
     * Adds <code>value</code> to the queue with the specified priority.
     * Lower priority numbers correspond to higher priorities, which
     * means that all priority 1 elements are dequeued before any
     * priority 2 elements.
     */
    void enqueue(const ValueType& value, double priority);
    
    /*
     * Method: equals
     * Usage: if (pq.equals(pq2)) ...
     * ------------------------------
     * Compares two priority queues for equality.
     * Returns <code>true</code> if this queue contains exactly the same
     * values and priorities as the given other queue.
     * Identical in behavior to the == operator.
     */
    bool equals(const PriorityQueue<ValueType>& pq2) const;
    
    /*
     * Method: front
     * Usage: ValueType first = pq.front();
     * ------------------------------------
     * Returns the first value in the queue by reference.
     */
    ValueType& front();

    /*
     * Method: isEmpty
     * Usage: if (pq.isEmpty()) ...
     * ----------------------------
     * Returns <code>true</code> if the priority queue contains no elements.
     */
    bool isEmpty() const;
    
    /*
     * Method: peek
     * Usage: ValueType first = pq.peek();
     * -----------------------------------
     * Returns the value of highest priority in the queue, without
     * removing it.
     */
    ValueType peek() const;

    /*
     * Method: peekPriority
     * Usage: double priority = pq.peekPriority();
     * -------------------------------------------
     * Returns the priority of the first element in the queue, without
     * removing it.
     */
    double peekPriority() const;

    /*
     * Method: remove
     * Usage: ValueType first = pq.remove();
     * --------------------------------------
     * A synonym for the dequeue method.
     */
    ValueType remove();

    /*
     * Method: size
     * Usage: int n = pq.size();
     * -------------------------
     * Returns the number of values in the priority queue.
     */
    int size() const;

    /*
     * Method: toString
     * Usage: string str = pq.toString();
     * ----------------------------------
     * Converts the queue to a printable string representation.
     */
    std::string toString() const;
    
    /*
     * Operator: <<
     * Prints the priority queue to the given output stream.
     */
    template <typename T>
    friend std::ostream& operator <<(std::ostream& os, const PriorityQueue<T>& pq);
    
    /*
     * Operators: ==, !=
     * Usage: if (pq1 == pq2) ...
     * --------------------------
     * Relational operators to compare two queues to see if they have the same elements.
     * The ==, != operators require that the ValueType has a == operator
     * so that the elements can be tested for equality.
     */
    bool operator ==(const PriorityQueue& pq2) const;
    bool operator !=(const PriorityQueue& pq2) const;

#ifdef PQUEUE_COMPARISON_OPERATORS_ENABLED
    /*
     * Operators: <, <=, >, >=
     * Usage: if (pq1 < pq2) ...
     * -------------------------
     * Relational operators to compare two queues.
     * The <, >, <=, >= operators require that the ValueType has a < operator
     * so that the elements can be compared pairwise.
     */
    bool operator <(const PriorityQueue& pq2) const;
    bool operator <=(const PriorityQueue& pq2) const;
    bool operator >(const PriorityQueue& pq2) const;
    bool operator >=(const PriorityQueue& pq2) const;
#endif // PQUEUE_COMPARISON_OPERATORS_ENABLED

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

    /*
     * Implementation notes: PriorityQueue data structure
     * --------------------------------------------------
     * The PriorityQueue class is implemented using a data structure called
     * a heap.
     */
private:
    /* Type used for each heap entry */
    struct HeapEntry {
        ValueType value;
        double priority;
        long sequence;
    };

    /* Instance variables */
    Vector<HeapEntry> heap;
    long enqueueCount;
    int backIndex;
    int count;
    int capacity;

    /* Private function prototypes */
    const HeapEntry& heapGet(int index) const;
#ifdef PQUEUE_COMPARISON_OPERATORS_ENABLED
    int pqCompare(const PriorityQueue& other) const;
#endif // PQUEUE_COMPARISON_OPERATORS_ENABLED
    bool takesPriority(int i1, int i2);
    void swapHeapEntries(int i1, int i2);

    /*
     * Iterator support
     * ----------------
     * The classes in the StanfordCPPLib collection implement input
     * iterators so that they work symmetrically with respect to the
     * corresponding STL classes.
     */
    class pq_iterator : public std::iterator<std::input_iterator_tag, ValueType> {
    public:
        pq_iterator() {
            // empty
        }

        pq_iterator(const PriorityQueue& pq, bool end) {
            m_pq = &pq;
            m_index = end ? m_pq->count : 0;
        }

        pq_iterator(const pq_iterator& it)
                : m_pq(it.m_pq), m_index(it.m_index) {
            // empty
        }

        pq_iterator& operator ++() {
            // find 'next' element: one that is minimally higher pri/seq from last one
            // (this is O(N) and inefficient, but we want to avoid making a deep copy)
            if (m_index == m_pq->count) {
                error("PriorityQueue::iterator::operator ++: Cannot call on an end() iterator");
            }
            double pri = m_pq->heapGet(m_index).priority;
            int seq = m_pq->heapGet(m_index).sequence;
            
            // best new element/index we have seen so far (initially none)
            int newIndex = m_pq->count;
            double newPri;
            int newSeq;
            
            for (int i = 0; i < m_pq->count; i++) {
                if (i == m_index) {
                    continue;
                }
                double ipri = m_pq->heapGet(i).priority;
                int iseq = m_pq->heapGet(i).sequence;
                if (ipri < pri || (ipri == pri && iseq < seq)) {
                    continue;
                } else if (newIndex == m_pq->count
                           || ipri < newPri
                           || (ipri == newPri && iseq < newSeq)) {
                    newPri = ipri;
                    newSeq = iseq;
                    newIndex = i;
                }
            }
            
            // if no next element is found, newIndex will be count (end)
            m_index = newIndex;
            return *this;
        }

        pq_iterator operator ++(int) {
            pq_iterator copy(*this);
            operator++();
            return copy;
        }

        bool operator ==(const pq_iterator& rhs) {
            return m_pq == rhs.m_pq && m_index == rhs.m_index;
        }

        bool operator !=(const pq_iterator& rhs) {
            return !(*this == rhs);
        }

        ValueType operator *() {
            if (m_index == m_pq->count) {
                error("PriorityQueue::iterator::operator *: Cannot call on an end() iterator");
            }
            return m_pq->heapGet(m_index).value;
        }

        ValueType* operator ->() {
            if (m_index == m_pq->count) {
                error("PriorityQueue::iterator::operator ->: Cannot call on an end() iterator");
            }
            return &m_pq->heapGet(m_index).value;
        }

        friend class PriorityQueue;
        // friend struct HeapEntry;
        
    private:
        const PriorityQueue* m_pq;
        int m_index;
    };
    
    pq_iterator begin() const {
        return pq_iterator(*this, /* end */ false);
    }

    pq_iterator end() const {
        return pq_iterator(*this, /* end */ true);
    }
    
public:
    /* private implentation section */
    
    template <typename Collection>
    friend int compare::compare(const Collection& pq1, const Collection& pq2);

#ifdef PQUEUE_ALLOW_HEAP_ACCESS
    const ValueType& __getValueFromHeap(int index) const;
    double __getPriorityFromHeap(int index) const;
#endif // PQUEUE_ALLOW_HEAP_ACCESS
};

template <typename ValueType>
PriorityQueue<ValueType>::PriorityQueue() {
    clear();
}

/*
 * Implementation notes: ~PriorityQueue destructor
 * -----------------------------------------------
 * All of the dynamic memory is allocated in the Vector class,
 * so no work is required at this level.
 */
template <typename ValueType>
PriorityQueue<ValueType>::~PriorityQueue() {
    /* Empty */
}

template <typename ValueType>
void PriorityQueue<ValueType>::add(const ValueType& value, double priority) {
    enqueue(value, priority);
}

template <typename ValueType>
ValueType & PriorityQueue<ValueType>::back() {
    if (count == 0) {
        error("PriorityQueue::back: Attempting to read back of an empty queue");
    }
    return heap.get(backIndex).value;
}

/*
 * changePriority function added by Marty Stepp.
 * Parts of this implementation are adapted from TrailblazerPQueue.h,
 * which was written by Keith Schwarz.
 */
template <typename ValueType>
void PriorityQueue<ValueType>::changePriority(ValueType value, double newPriority) {
    if (!(newPriority == newPriority)) {
        error("PriorityQueue::changePriority: Attempted to use NaN as a priority.");
    }
    if (newPriority == -0.0) {
        newPriority = 0.0;
    }

    // find the element in the pqueue; must use a simple iteration over elements
    for (int i = 0; i < count; i++) {
        if (heap[i].value == value) {
            if (heap[i].priority < newPriority) {
                error("PriorityQueue::changePriority: new priority cannot be less urgent than current priority.");
            }
            heap[i].priority = newPriority;

            // after changing the priority, must percolate up to proper level
            // to maintain heap ordering
            while (i > 0) {
                int parent = (i - 1) / 2;
                if (takesPriority(parent, i)) {
                    break;
                }
                swapHeapEntries(parent, i);
                i = parent;
            }

            return;
        }
    }

    // if we get here, the element was not ever found
    error("PriorityQueue::changePriority: Element value not found.");
}

template <typename ValueType>
void PriorityQueue<ValueType>::clear() {
    heap.clear();
    count = 0;
    enqueueCount = 0;   // BUGFIX 2014/10/10: was previously using garbage unassigned value
}

/*
 * Implementation notes: dequeue, peek, peekPriority
 * -------------------------------------------------
 * These methods must check for an empty queue and report an error
 * if there is no first element.
 */
template <typename ValueType>
ValueType PriorityQueue<ValueType>::dequeue() {
    if (count == 0) {
        error("PriorityQueue::dequeue: Attempting to dequeue an empty queue");
    }
    count--;
    bool wasBack = (backIndex == count);
    ValueType value = heap[0].value;
    swapHeapEntries(0, count);
    int index = 0;
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        if (left >= count) {
            break;
        }
        int child = left;
        if (right < count && takesPriority(right, left)) {
            child = right;
        }
        if (takesPriority(index, child)) {
            break;
        }
        swapHeapEntries(index, child);
        index = child;
    }
    if (wasBack) {
        backIndex = index;
    }
    return value;
}

template <typename ValueType>
void PriorityQueue<ValueType>::enqueue(const ValueType& value, double priority) {
    if (!(priority == priority)) {
        error("PriorityQueue::enqueue: Attempted to use NaN as a priority.");
    }
    if (priority == -0.0) {
        priority = 0.0;
    }

    if (count == heap.size()) {
        heap.add(HeapEntry());
    }
    int index = count++;
    heap[index].value = value;
    heap[index].priority = priority;
    heap[index].sequence = enqueueCount++;
    if (index == 0 || takesPriority(backIndex, index)) {
        backIndex = index;
    }
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (takesPriority(parent, index)) {
            break;
        }
        swapHeapEntries(parent, index);
        index = parent;
    }
}

template <typename ValueType>
bool PriorityQueue<ValueType>::equals(const PriorityQueue<ValueType>& pq2) const {
    // optimization: if literally same pq, stop
    if (this == &pq2) {
        return true;
    }
    if (size() != pq2.size()) {
        return false;
    }
    PriorityQueue<ValueType> backup1 = *this;
    PriorityQueue<ValueType> backup2 = pq2;
    while (!backup1.isEmpty() && !backup2.isEmpty()) {
        if (backup1.peekPriority() != backup2.peekPriority()) {
            return false;
        }
        if (backup1.dequeue() != backup2.dequeue()) {
            return false;
        }
    }
    return backup1.isEmpty() == backup2.isEmpty();
}

template <typename ValueType>
ValueType & PriorityQueue<ValueType>::front() {
    if (count == 0) {
        error("PriorityQueue::front: Attempting to read front of an empty queue");
    }
    return heap.get(0).value;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::isEmpty() const {
    return count == 0;
}

template <typename ValueType>
ValueType PriorityQueue<ValueType>::peek() const {
    if (count == 0) {
        error("PriorityQueue::peek: Attempting to peek at an empty queue");
    }
    return heap.get(0).value;
}

template <typename ValueType>
double PriorityQueue<ValueType>::peekPriority() const {
    if (count == 0) {
        error("PriorityQueue::peekPriority: Attempting to peek at an empty queue");
    }
    return heap.get(0).priority;
}

template <typename ValueType>
ValueType PriorityQueue<ValueType>::remove() {
    return dequeue();
}

template <typename ValueType>
int PriorityQueue<ValueType>::size() const {
    return count;
}

template <typename ValueType>
std::string PriorityQueue<ValueType>::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

template <typename ValueType>
const typename PriorityQueue<ValueType>::HeapEntry&
PriorityQueue<ValueType>::heapGet(int index) const {
    return heap[index];
}

#ifdef PQUEUE_COMPARISON_OPERATORS_ENABLED
/*
 * Implementation note: Due to the complexity and unpredictable heap ordering of the elements,
 * this function sadly makes a deep copy of both PQs for comparing.
 * Therefore it is recommended not to use PQs in a context where <, <=, etc. are being
 * called on them frequently.
 */
template <typename ValueType>
int PriorityQueue<ValueType>::pqCompare(const PriorityQueue& pq2) const {
    if (this == &pq2) {
        return true;
    }
    PriorityQueue<ValueType> backup1 = *this;
    PriorityQueue<ValueType> backup2 = pq2;
    while (!backup1.isEmpty() && !backup2.isEmpty()) {
        if (backup1.peek() < backup2.peek()) {
            return -1;
        } else if (backup2.peek() < backup1.peek()) {
            return 1;
        }
        
        double pri1 = backup1.peekPriority();
        double pri2 = backup2.peekPriority();
        if (pri1 < pri2) {
            return -1;
        } else if (pri2 < pri1) {
            return 1;
        }
        
        backup1.dequeue();
        backup2.dequeue();
    }
    if (backup1.size() < backup2.size()) {
        return -1;
    } else if (backup2.size() < backup1.size()) {
        return 1;
    } else {
        return 0;
    }
}
#endif // PQUEUE_COMPARISON_OPERATORS_ENABLED

template <typename ValueType>
void PriorityQueue<ValueType>::swapHeapEntries(int i1, int i2) {
    HeapEntry entry = heap[i1];
    heap[i1] = heap[i2];
    heap[i2] = entry;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::takesPriority(int i1, int i2) {
    if (heap[i1].priority < heap[i2].priority) {
        return true;
    }
    if (heap[i1].priority > heap[i2].priority) {
        return false;
    }
    return (heap[i1].sequence < heap[i2].sequence);
}

template <typename ValueType>
bool PriorityQueue<ValueType>::operator ==(const PriorityQueue& pq2) const {
    return equals(pq2);
}

template <typename ValueType>
bool PriorityQueue<ValueType>::operator !=(const PriorityQueue& pq2) const {
    return !equals(pq2);
}

#ifdef PQUEUE_COMPARISON_OPERATORS_ENABLED
template <typename ValueType>
bool PriorityQueue<ValueType>::operator <(const PriorityQueue& pq2) const {
    return pqCompare(pq2) < 0;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::operator <=(const PriorityQueue& pq2) const {
    return pqCompare(pq2) <= 0;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::operator >(const PriorityQueue& pq2) const {
    return pqCompare(pq2) > 0;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::operator >=(const PriorityQueue& pq2) const {
    return pqCompare(pq2) >= 0;
}
#endif // PQUEUE_COMPARISON_OPERATORS_ENABLED

/*
 * Template hash function for hash sets.
 * Requires the element type in the HashSet to have a hashCode function.
 */
template <typename T>
int hashCode(const PriorityQueue<T>& pq) {
    // (slow, memory-inefficient) implementation: copy pq, dequeue all, and hash together
    PriorityQueue<T> backup = pq;
    int code = hashSeed();
    while (!backup.isEmpty()) {
        code = hashMultiplier() * code + hashCode(backup.peek());
        code = hashMultiplier() * code + hashCode(backup.peekPriority());
        backup.dequeue();
    }
    return int(code & hashMask());
}

#ifdef PQUEUE_ALLOW_HEAP_ACCESS
template <typename ValueType>
const ValueType& PriorityQueue<ValueType>::__getValueFromHeap(int index) const {
    return heap[index].value;
}

template <typename ValueType>
double PriorityQueue<ValueType>::__getPriorityFromHeap(int index) const {
    return heap[index].priority;
}
#endif // PQUEUE_ALLOW_HEAP_ACCESS

template <typename ValueType>
std::ostream& operator <<(std::ostream& os,
                          const PriorityQueue<ValueType>& pq) {
    os << "{";

#ifdef PQUEUE_PRINT_IN_HEAP_ORDER
    // faster implementation: print in heap order
    // (only downside: doesn't print in 'sorted' priority order,
    //  which might confuse student client)
    for (int i = 0, len = pq.size(); i < len; i++) {
        if (i > 0) {
            os << ", ";
        }
        os << pq.heap[i].priority << ":";
        writeGenericValue(os, pq.heap[i].value, true);
    }
#else
    // (default) slow, memory-inefficient implementation: copy pq and print
    PriorityQueue<ValueType> copy = pq;
    for (int i = 0, len = pq.size(); i < len; i++) {
        if (i > 0) {
            os << ", ";
        }
        os << copy.peekPriority() << ":";
        writeGenericValue(os, copy.dequeue(), true);
    }
#endif
    return os << "}";
}

template <typename ValueType>
std::istream& operator >>(std::istream& is, PriorityQueue<ValueType>& pq) {
    char ch = '\0';
    is >> ch;
    if (ch != '{') {
        error("PriorityQueue::operator >>: Missing {");
    }
    pq.clear();
    is >> ch;
    if (ch != '}') {
        is.unget();
        while (true) {
            double priority = 0.0;
            is >> priority >> ch;
            if (ch != ':') {
                error("PriorityQueue::operator >>: Missing colon after priority");
            }
            ValueType value;
            readGenericValue(is, value);
            pq.enqueue(value, priority);
            is >> ch;
            if (ch == '}') {
                break;
            }
            if (ch != ',') {
                error(std::string("PriorityQueue::operator >>: Unexpected character ") + ch);
            }
        }
    }
    return is;
}

#endif
