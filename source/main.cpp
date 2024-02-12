// Ch.13 Algorithms

// 13.1 Introduction
// standard library provides common algos
#include <vector>
#include <list>
#include <string>
#include <<algorithm>
#include <iostream>

struct Entry {
    std::string name;
    int number;
};


void f(std::vector<Entry>& vec, std::list<Entry>& lst)
{
    std::sort(vec.begin(),vec.end()); // use < for order
    std::unique_copy(vec.begin(),vec.end(),lst.begin()); // don't copy adjacent equal elements
}

bool operator<(const Entry& x, const Entry& y) // less than
{
    return x.name<y.name; // order Entries by their name
}

// std algo is expressed in terms of half open sequences of elements
// sequence: a pair of iterators specifing the first element and one-beyond the last element

// in the above example, lst has to have >= the amount of elements in vec to avoid errors
// there is no range checking by default for std::unique_copy, lst being bigger will result in
// undefined behaivor

// abtraction implementation for range-checking when writing into a container
template<typename C>
class Checked_iter {
    public: 
        using value_type = typename C::value_type;
        using difference_type = int;

        Checked_iter() { throw Missing_container{};} // concept forward_iterator requires a default constructor
        Checked_iter(C& cc) : pc {&cc} {}
        Checked_iter(C& cc, typename C::iterator pp) : pc { &cc}. p {pp} {}

        Checked_iter& operator++() {checked_end(); ++p; return *this;}
        Checked_iter operator++(int) { checked_end(); auto t{*this};++p;return t;}
        value_type& operator*() const { check_end(); return *p; }

        bool operator==(const Checked_iter& a) const { return p==a.p;}
        bool operator!=(const Checked_iter& a) const {return p!=a.p;}

    private:
        void check_end() const { if (p == pc -> end()) throw Overflow{}; }
        C* pc {}; // default initialize to nullptr
        typename C::iterator p = pc -> begin();
};

std::vector<int> v1 {1, 2, 3}; // three elements
std::vector<int> v2(2); // two elements

std::copy(v1,vs.begin()); // will overflow
std::copy(v1,Checked_iter{v2}); // will throw

// if we wanted to place the unique elements in a new list in the function above:

std::list<Entry> f(std::vector<Entry>& vec)
{
    std::list<Entry> res;
    std::sort(vec.begin(),vec.end());
    std::unique_copy(vec.begin().vec.end(),std::back_inserted(res)); // append to res
    return res;
}

// back_intserter constructs an iterator for res that adds elements to the end of a container, extending the container to make room
// otherwise, we would have to allocate a fixed amoung of space and then fill it
// this eliminates the need to use error-prone explicit C-style memory management using realloc()
// std has a move construct that makes returning res by value efficient(even if the list has thousands of elements)

// writing sort(vec) will use the range version of the algorithims

for (auto& x : v) std::cout << x;   //write out all the elements of v
for (auto p = v.begin(); p!=v.end();++p) std::cout << *p;   // write out all the elements of 
// range-for is more efficient and less error prone

// 13.2 Use of Iterators
// many algos in standard lib return iterators

bool has_c(const std::string& s, char c) // does s contain the character c?
{
    auto p = std::find(s.begin(),s.end(),c);
    if (p!=s.end())
        return true;
    else
        return false;
}

// find() returns end() to endicate not found
// this is common for many standard library algos

// equivalent shorter function
bool has_c_short(const std::string& s, char c) // does a contain the character c?
{
    return std::find(s,c)!=s.end();
}

// finding the location of all the occurences of a character in a string. 
// (returning a std vector is efficient because it has move semantics)

std::vector<std::string::iterator> find_all(std::string& s, char c) // find all occurences of c in s
{
    std::vector<char*> res;
    for (auto p = s.begin(); p!=s.end(); ++p)
        if(*p==c)
            res.push_back(&*p);
    return res;
}

// test of find_all()

void test()
{
    std::string m {"Mary had a little lamb"};
    for (auto p : find_all(m,'a'))
        if (*p='a')
            std::cerr << "a bug\n";
}

// iterators and std algo work the same on every container that their use makes sense on. 

// generalization of find all

template<typename C, typename V>
std::vector<typename C::iterator> find_all(C& c, V c) // find all occurences of v in c
{
    std::vector<typename C::iterator> res;
    for (auto p = c.begin(); p!=c.end(); ++p)
        if (*p==v)
            res.push_back(p);
    return res;
}

// typename informs the compiler that C iterator is a type and not a value of some type

// returning a vector of ordinary pointers to the elements
template<typename C, typename V>
auto find_all(C& c, V v) // find all occurences of v in c
{
    std::vector<range_value_t<C>*> res;
    for (auto& x:c)
        res.push_back(&x);
    return res;
}

// definition for a simplified version of range_value_t

template<typename T>
using range_value_type_t = T::value_type;

// code using find_all. Any of the above versions will work in this code. 
void test()
{
    std::string m {"Mary had a little lamb."};

    for (auto p : find_all(m,'a')) // p is a string::iterator
        if(*p!='a')
            std::cerr << "string bug!\n";
    
    std::list<int> Id {1, 2, 3, 1, -11, 2};
    for (auto p : find_all(Id,1)) // p is a list<int>::iterator
        if (*p!=1)
            std::cerr << "list bug!\n";

    std::vector<string> vs {"red", "blue", "green", "green", "orange", "green"};
    for (auto p : find_all(vs,"red")) // p is a vector<string>::iterator
        if (*p!="red")
            std::cerr <, "vector bug!\n";
    
    for (auto p : find_all(vc,"green"))
        *p = "vert";
}

// iterators seperate algorithims and containers
// algos can just operator on the iterators, they don't need to know what kind of container stores the data. 
// all the container has to do is provide iterators on request

//13.3 Iterator Types
// an particular iterator is a value of some type
// in a vector, an iterator is probably a pointer

// a vector iterator could also implemented as a pointer to a the vector plus an index
// this kind of iterator allows range checking

// a list iterator is more complicated because each element of a list doesn't know where the next element of list is
// a list iterator could be a pointer to a link


// all iterators have common names and sematics
// ++ applied to an iterator yields an iterator referring to the next element
// * yields the element the iterator refers to
// std makes different kinds of iterators available as concepts

// if the iterator is not a member type, so std offers iterator_t<X> that works where X's iterator is defined

//13.3.1 Stream Iterators
// iterators can be applied to i/o streams

// ostream iterator
ostream_iterator<string> oo {cout}; // write strings to cout

// effect of assiging *oo is t write the assigned value to cout

int main()
{
    *oo = "Hello, "; // meaning cout<<"Hello,"
    ++oo;
    *oo = "world!\n"; // meaning cout<<"world\n"
}

// this allows using algos on streams
std::vector<std::string>> v { "Hello", ", ", "World!\n" };
copy(v,oo);

// an istream_iterator is something that allows us to treat an input stream as a read-only container

istream_iterator<string> ii {cin};

// input iterators are used in pairs representing a sequence, so we must provide an istream_iterator to indicate the end of input. This is the defaul istream_iterator
istream_iterator<string> eos {};

// istream _itatoras and ostream_iterators are usually provided as args instead of used directly

// a simple program to read a file, sort word read, eleminate duplicates, and write the results to anothe file

int main()
{
    std::string from, to;
    std::cin >> from >> to; // get source and target file names

    ifstream is {from}; // input stream for file "from"
    istream_iterator<string> oo {os, "\n"}; // output iterator for stream plus a separator

    std::vector<std::string> b {ii,eos}; // b is a vector initialized from input
    sort(b); // sort the buffer

    unique_copy(b,oo); // copy the buffer to output, discard replicated values

    return !is.eof() || !os; // return error state
}

// in the above code, we used range versions of sort() and unique copy()
// using the iterators directly, like sort(b.begin(),b.end()) is common in older code

// to use a traditional iterator of a std algo and its ranges version, we need to explcitly qualify the call of the range version
// or use a using declaration

copy(v, oo); // potentially ambigous
ranges::copy(v,oo); // OK
using ranges::copy(v,oo); // copy(v,oo) Ok from here on
copy(v,oo); // OK

// ifstream is an istream file that can attached to a file
// ofstream is an ostream that can be attached to a file

// ostream_iterator's second arg is used to delimit output values

// the code above could be made easier by putting strings in a set, which keeps its elements in order and doesn't store duplicates
// we could thus replace the two lines using a vector with one using a set and replace unique_copy() with the simpler copy()

set<string> b {ii, eos}; // collect strings from input
copy(b,oo); // copy buffer to output

// reduced program
int main()
{
    string from, to;
    cin >> from >>to; // get source and target file names

    ifstream is {from}; // input stream for file "from"
    ofstream os {to}; // output stream for file "to"

    set<string> b {istream_iterator<string>{is},istream_iterator<string>{}}; // read input
    copy(b,ostream_iteartor<string>){os,"\n"}; // copy to output

    return !is.eof() || !osl // return error state
}

// 13.4 Use of Predicates

// predicates allow passing the action we want the algo to perform as an arg to the algo

// searching a map for an int > 42

void f(const map<string,int>& m)
{
    auto p = find_if(m,Greater_than{42});
    //..
}

// Greater_than is a function object
struct Greater_than {
    int val;
    Greater_than(int v) : val{v} {}
    bool operator()(const pair<string,int>& r) const {return r.second>val;}
}

// we can also use a lambda

auto p = find_if(m, [](const auto& r) {return r.second>42;});
// predicate should not modify the element its applied to

// 13.5 Algorithm Overview
// algo: a finite set of rules which gives a sequence of operations for solving a specific set of problems
// features of an algo
// finiteness
// definiteness
// input
// output
// effectiveness

// in std lib, an algo is a function template operating on sequences of elements

// a half-open sequence from b to e is [b:e)

// for each algo taking a [b:e), <ranges> offers a version that takes a range
// if using both, explicitly qualify or use using

// some algs modify element values, but none add or remove elements
// a sequence does not identify it's container

// you can add or delete elements with a function that knows about the container (back_inserter) or directly refers to the container itself (push_back() or erase())

// lambdas are common as operations passed as arguments

std::vector<int> v = {0,1,2,3,4,5};
for_each(v,[](int& x){x=x*x}); // v={0,1,4,9,16,25}
for_each(v.begin(),v.begin()+3,[](int& x){x=sqrt(x);}) // v={0,1,4,9,16,25}

// it's best to stick to std lib code

//13.6 Parallel Algorithms
// if the computations on different data algorithms are independent, we cn execute the same task in parallel

// parallel execution: tasks are done on multiple threads (often running on several processor cores)
// vectorized execution: tasks are done in a single thread using vectorization, also known as SIMD ("Single Instruction, Multiple Data")

// std lib supports both
// in <execution> in namespace execution
// seq: sequential execution
// par: parallel execution(if feasible)
// unseq: unsequenced (vectorized) execution (if feasible)
// par_unseq: parallel and/or unsequenced (vectorized) execution (if feasible)/

// example with std::sort()

#include <execution>

sort(v.begin(), v.end()); // sequential
sort(std::seq, v.begin(), v.end()); // sequential (same as the defaul, just being specific)
sort(std::par,v.begin(),v.end()); // parallel
sort(std::par_unseq,v.begin(),v.end()); // parallel and/or vectorized

// execution policy indiciators are just hints. a compiler or run-time scheduler will decide how much concurrency to use. 
// don't make statements about efficiency without measurement

// range versions of parallel algos are not yet in the standard, but they're easy to implement

void sort(auto pol, random_access_range auto& r)
{
    sort(pol,r.begin(),r.end());
}

// many parallel algo are used primarily for numeric data
// avoid data races and deadlock when requesting parallel execution

