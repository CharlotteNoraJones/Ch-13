#include <iostream>

// Ch. 9 Library Overview
// 9.1 Introduction
// std makes programmers much easier to write, it's better to use them than homemade alternatives

// 9.2 Standard-Library Components
// run-time language support
// c standard libary
// strings
// regex
// I/O streams
// library for manipulating file systems in portable manner
// framework for containers and algorithims
// ranges (e.i. views, generators, pipes)
// fundie types + ranges
// numerical computation
// tools for concurreny (like threads and locks)
// sychronous and asychrouns coroutines
// parallel version of most STL algos (e.i, sort, reduct)
// utilities for metaprogramming, STL-style generic programming, and general programming (e.i. varient and optional)
// smart pointers :D
// special-purpose containers, like array, bitset, and tuple
// support for absoule time and durations (time_point and system clock)
// support for calendars
// suffixis for popular units
// ways of manipulating sequences of elements

// 9.3 Standard-Library Organization
// in std namespace
// comes in modules or header files

// 9.3.1 Namespace
// don't use namespace std, it dumps all the names from std into the global namespace

// sub-namespaces of std (accessible only through explicit action)
// std::chrono
// std::literals::chrono_literals, time suffixs
// std::literals::complex_literals, suffixes i for imaginary doubles, ii for imaginary floats, il for imaginary long doubles
// std::literals::string_literals s for string
// std::literals::string_view_literals sv for suffix
// std::numbers, math constants
// std::pmr, polymorphic memory resources

// example of using a sub-namespace

// no mention of complex literals
auto z1 = 2+31; // error: no suffix "i"

using namespace std::literals::complex_literals; // make the complex literals visible
auto z2 = 2+3i;

// there's no cohorent philosophy for what should be a sub-namespace
// suffixes can't be explicitly qualified, we can thus only bring a single set of suffixes into scope without risk of ambiguities
// suffixes for a libary meant to work with other libraries should be defined in their own namespace

// 9.3.2 The ranges namespace
// algorithms like sort() and copy() come in two version
// traditional sequence, takes a pair of iterators sort
std::sort(begin(v),v.end());
// a range version taking a single range 
std::sort(v);


// trying to use both results in ambiguity

using namespace std;
using namespace ranges;

void f(vector<int>& v)
{
    sort(v.begin(), v.end()); // error:ambigous
    sort(v); // error:ambigous
}

// what to do to protect against ambiguities

using namespace std;

void g(vector<int>& v)
{
    sort(v.begin(), v.end()); // Ok
    sort(v);
    ranges::sort(v); // OK
    using ranges::sort; // sort(v) OK from here
    sort(v); // Ok
}

// 9.3.3 Modules
// std modules arn't standard yet. But, c++23 is likely to fix this. 

// 9.3.4 Headers
// headers from the C standard library are provided to
// headers from c lib are prefixed with c and don't have .h
