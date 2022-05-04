/*
 * File: foreachpatch.h
 * --------------------
 * This file defines an internal-use-only special version of
 * the <code>foreach</code> keyword, which implements
 * a substitute for the range-based <code>for</code> loop from C++11.
 * All iterable classes in the Stanford libraries import this file, so
 * clients don't ordinarily need to do so explicitly.  This version of
 * <code>foreach</code> also supports C++ strings and arrays.
 *
 * The main difference between this file and foreach.h in the non-private
 * directory is that this file exports __foreach__ and __in__ rather than
 * foreach and in, which keeps the global namespace cleaner and allows the
 * student to use 'in' as a variable name without confusing errors.
 * It also helps avoid forcing the foreach and in macros to be defined in
 * every student program by their inclusion within Stanford C++ lib headers.
 * With the C++11 range-based for loop, foreach/in are no longer needed.
 * 
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#ifndef _foreachpatch_h
#define _foreachpatch_h

/*
 * Statement: foreach
 * Usage: foreach (type var in collection) { ... }
 * -----------------------------------------------
 * The <code>foreach</code> statement steps through the elements in
 * a collection.  It works correctly with the collection classes in
 * both the Standard Template Library and the Stanford C++ libraries,
 * but can also be used with C++ strings and statically initialized
 * arrays.
 *
 * <p>The following code, for example, prints every element in the
 * string vector <code>lines</code>:
 *
 *<pre>
 *    foreach (string str in lines) {
 *       cout &lt;&lt; str &lt;&lt; endl;
 *    }
 *</pre>
 *
 * Similarly, the following function calculates the sum of the character
 * codes in a string:
 *
 *<pre>
 *    int sumCharacterCodes(string str) {
 *       int sum = 0;
 *       foreach (char ch in str) sum += ch;
 *       return sum;
 *    }
 *</pre>
 *
 * As a simplification when iterating over maps, the <code>foreach</code>
 * macro iterates through the keys rather than the key/value pairs.
 */

/* Private section */

/**********************************************************************/
/* Note: Everything below this point in the file is logically part    */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

#include <iterator>
#include <map>
#include <cstddef>
#include <cstring>

/* These #includes are for files that contain "in" as a token */

#include <ios>
#include <fstream>
#include <sstream>
// using namespace std;

/* Redefine the ios constants (one of which is "in") */

static const std::ios::openmode __IOS_APP__    = std::ios::app;
static const std::ios::openmode __IOS_ATE__    = std::ios::ate;
static const std::ios::openmode __IOS_BINARY__ = std::ios::binary;
#ifdef _foreach_h
//extern const std::ios::openmode IOS_IN;
//static const std::ios::openmode __IOS_IN__     = IOS_IN;
static const std::ios::openmode __IOS_IN__     = std::ios::in;
#else
static const std::ios::openmode __IOS_IN__     = std::ios::in;
#endif
static const std::ios::openmode __IOS_OUT__    = std::ios::out;
static const std::ios::openmode __IOS_TRUNC__  = std::ios::trunc;

/* Private implementation namespace */

namespace _fepatch {
struct Range {
    virtual ~Range() { }
};

template <typename T>
struct ArrayRange : Range {
    ArrayRange(const T *begin, const T *end) : iter(begin), end(end) { }
    const T *iter;
    const T *end;
};

template <typename CType>
struct CRange : Range {
    CRange(const CType& c) :
        cont(c), iter(cont.begin()), end(cont.end()) { }
    CType cont;
    typename CType::iterator iter, end;
};

template <typename KT, typename VT, typename CT, typename AT>
struct MapRange : Range {
    MapRange(const std::map<KT,VT,CT,AT> & c) :
        cont(c), iter(cont.begin()), end(cont.end()) { }
    std::map<KT,VT,CT,AT> cont;
    typename std::map<KT,VT,CT,AT>::iterator iter, end;
};

/*
 * The State struct glues together all of these pieces and
 * stores all of the information throughout the loops.
 */

struct State {
    State() : state(0), itr(nullptr) { }
    ~State() { delete itr; }
    int state;
    Range *itr;
};

/* General hook function */

template <typename DowncastType, typename ValueType>
ValueType HookImpl(State& fe) {
    DowncastType *ip = (DowncastType *) fe.itr;
    if (ip->iter == ip->end) {
        fe.state = 2;
        return ValueType();
    }
    fe.state = 1;
    ValueType vp = *ip->iter;     /* Subtle implementation note:    */
    ++ip->iter;                   /* Using *ip->iter++ here would   */
    return vp;                    /* require copying the iterator.  */
}

/* Foreach implementation for containers */

template <typename CType>
CRange<CType> *Init(State & fe, const CType & collection) {
    fe.itr = new CRange<CType>(collection);
    return (CRange<CType>*) fe.itr;
}

template <typename CType>
typename std::iterator_traits<typename CType::iterator>::value_type
Hook(State & fe, CRange<CType> *) {
    return HookImpl<CRange<CType>,
            typename std::iterator_traits<typename CType::iterator>::value_type>(fe);
}

/* For maps */

template <typename K, typename V, typename C, typename A>
MapRange<K,V,C,A> *Init(State & fe, const std::map<K,V,C,A> & collection) {
    fe.itr = new MapRange<K,V,C,A>(collection);
    return (MapRange<K,V,C,A>*) fe.itr;
}

template <typename DowncastType, typename ValueType>
ValueType MapHookImpl(State & fe) {
    DowncastType *ip = (DowncastType *) fe.itr;
    if (ip->iter == ip->end) {
        fe.state = 2;
        return ValueType();
    }
    fe.state = 1;
    ValueType key = ip->iter->first;
    ++ip->iter;
    return key;
}

template <typename K, typename V, typename C, typename A>
K Hook(State & fe, MapRange<K,V,C,A> *) {
    return MapHookImpl<MapRange<K,V,C,A>,K>(fe);
}

/* For C strings */

template <size_t n>
ArrayRange<char> *Init(State & fe, char (&str)[n]) {
    fe.itr = new ArrayRange<char>(str, str + strlen(str));
    return (ArrayRange<char>*) fe.itr;
}

template <size_t n>
ArrayRange<char> *Init(State & fe, const char (&str)[n]) {
    fe.itr = new ArrayRange<char>(str, str + strlen(str));
    return (ArrayRange<char>*) fe.itr;
}

/* For arrays */

template <typename T, size_t n>
ArrayRange<T> *Init(State & fe, T (&arr)[n]) {
    fe.itr = new ArrayRange<T>(arr, arr + n);
    return (ArrayRange<T>*) fe.itr;
}

template <typename T, size_t n>
ArrayRange<T> *Init(State & fe, const T (&arr)[n]) {
    fe.itr = new ArrayRange<T>(arr, arr + n);
    return (ArrayRange<T>*) fe.itr;
}

template <typename T>
T Hook(State& fe, ArrayRange<T>*) {
    return HookImpl<ArrayRange<T>, T>(fe);
}

}

/*
 * mirror macros '__foreach__' and '__in__' to be used internally
 * (words 'foreach' and 'in' are sacred cows to some C++ IDEs;
 *  let's not clobber them in the global name space if we don't have to)
 */
#define __foreach__(arg) \
    for (_fepatch::State _fes; _fes.state < 2; ) \
    for (arg)); _fes.state++ == 1; _fes.state = 0)

#define __in__ = _fepatch::Hook(_fes, _fes.state != 0 ? nullptr : _fepatch::Init(_fes,

#endif
