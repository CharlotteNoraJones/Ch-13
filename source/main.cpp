// Ch.12 Containers

//12.1 Introduction
// containers are classes whose job is to hold other objects
// std::string is and example

// 12.2 vector
// usual implementation of vector:
// ptr to first element
// ptr to one-past-the-last-element
// ptr to one-past-the-last-allocated-space
// allocator, can be used to acquire memory

// default allocator uses new and delete and acquire and release memory

// there's an optimization technique that makes it unneccsary to store data for simple allocators in the vector object.

#include<vector>
#include<string>
#include<iostream>

// example of a vector

struct Entry {
    std::string name;
    int phoneNum;
};

std::vector<Entry> phone_book = {
    {"David Hume", 123456},
    {"Karl Popper", 234567},
    {"Bertrand Arthur William Russel", 345678}
};

// this would work if we defined << for Entry

void print_book(const std::vector<Entry>& book)
{
    for(int i = 0; i!=book.size(); ++i)
        std::cout << book[i] << '\n';
}

// we could also use a range-for loop
void print_book(const std::vector<Entry>& book)
{
    for (const auto& x : book)
        std::cout << x << "\n";
}

// when define a std::vector, we give it an initial size
std::vector<int> v1 = {1, 2, 3, 4}; // size is 4
std::vector<std::string>> v2; //size is 0;
std::vector<Shape*> v3(23); // size is 23, initial element value: nullptr
std::<vector> v4(32, 9.9); // size is 32; initial element value: 9.9


// elements are initialized to the element's default value if not provided with a value

// size can be changed after initialization. 
// push_back can add for values

void input()
{
    for (Entry e: std::cin>>e;)
        phone_book.push_back(e);
}

// example of a vector class that can increase in size

template<typename T>
class Vector {
    allocator<T> alloc; // standard-library allocator of space for Ts
    T* elem; // pointer to first element
    T* space; // pointer to first unused(and uninitialized) slot
    T* last; // pointer to last slot
    public: 
        // ..
        int size() const {return space-elem;} // number of elements
        int capacity() const {return last-elem;} // number of slots available for elements
        // ..
        void reserve(int newsz); // increase capacity() to newsz
        // ..
        void push_back(const T& t); // copy t into Vector
        void push_back(T&& t); // move t into Vector
};

// when elements are moved to a new and larger allocation, pointers to it are broken. 

// implementation of push_back

template<typename T>
void Vector<T>::push_back(const T& t)
{
    if (capacity()<=size()) // make sure that we have space for t
        reserve(size()==0?8:2*size()); // double the capacity
    construct_at(space,t); // initialize *space to t ("place t at space")
    ++space;
}

// vector can be copied in assignments and initialization
std::vector<Entry> books = phone_book; // copies elements of phone_book to books
// use references, pointers, or move operations if you don't want to copy

//12.2.1 Elements
// if you have a class hiearachy that relies on virtual functions for polymorphic behaivor,
// store a pointer or smart pointer to the class in a vector
std::vector<Shape> vs; // No, don't - there is no room for a Circle or a Smiley
std::vector<Shape*> vps; // better, but risks memory leaks
std::vector<unique_ptr<Shape>> vups; // OK

//12.2.2 Range Checking

// std vector doesn't guareente range checking
void silly(std::Vector<Entry>& book)
{
    int i = book[book.size()].number; // book.size() is out of range
    // ..
}

// i will probably end up with a random value

// adaption of std::vector with range checking

template<typename T>
struct Vec : std::vector<T>
{
    using std::vector<T>::vector; // use the constructors from vector (under the name vec)

    T& operator[](int i) {return Vector<T>::at(i);} // range check
    const T& operator[](int i) const {return vector<T>::at(i);} // range check const objects

    auto begin() {return Checked_iter<vector<T>>}{*this};
    auto end() {return Checked_iter<vector<T>>{*this, vector<T> end()}};
}

// at() operation is a vector subscript operation that throws an exception of type out_of_range

// Vec inherits everything from vector, except for substricpting operations for range checking

// the above function would produce undefined behaivor with std::vector, and an out_of_range exception with vec

// you can use a main function with a try catch block to minimize surprises from uncaught exceptions

int main()
try {
    // your code
}
catch (out_of_range&) {
    std::cerr << "range error\n";
}
catch(...) {
    std::cerr << "unknown exception thrown\n";
}

// std::vector is doesn't guareente range checking because it would cause a roughly 10% increase in cost
// if we were to check all subscripting

// range-for avoids range errors at no cost. 

// vector::at() is safer

//12.3 list
// std::list is a doubley linked list
// list is good for when inserting and deleting entries are common

#include<list>

std::list<Entry> phone_book_list {
    {"David Hume",123456},
    {"Karl Popper",234567},
    {"Bertrand Arthur William Russell",345678}
};

//we search for elements by value, instead of doing subscripting

int get_number(const string& s)
{
    for (const auto& x:phone_book_list)
        if (x.name==s)
            return x.number;
    return 0; //use 0 to represent "number not found"
}

int get_number(const std::string& s)
{
    for (const auto& x : phone_book)
        if (x.name==s)
            return x.number;
    return 0; // use 0 to represent "number not found"
}

// the above function using iterators

int get_number(const string& s)
{
    for (auto p = phone_book.begin(); p!=phone_book.end(); ++p)
        if (p->name==s)
            return p->number;
    return 0;
}

// the compiler implements range-for in a similar manner

// adding and removing list elements
void f(const Entry& ee, list<Entry>::iterator p, list<Entry>::iterator q)
{
    phone_book.insert(p,ee); // add ee before the element referred to by p
    phone_book.erase(q); // remove the element referred to by q
}

// the above examples would run with a vector. vector are usually faster. Unless there is a reason not to, favor vector

//12.4 Forward list
// std provides a single-linked list called forward_list

// it only allows forward iteration
// it takes up less space
// it doesn't keep track of the count of elements - count them if you need a count
// if you can't afford to count, don't use a forward list

// 12.5 Map
// a balanced binary search tree

// map might be a know as an associate array or a dictionary in other contexts

// std map is a container of pairs and values

#include <map>
std::map<std::string,int> phone_book_map {
    {"David Hume",123456},
    {"Karl Popper",234567},
    {"Bertrand Arthur William Russell",345678}
}
// the map returns the second value when indexed by the first

int get_number(const std::string& s)
{
    return phone_book[s];
}

// if a key isn't found, it's entered into the map with a default value
// find() and insert() won't enter a value if it isn't found

//12.6 unordered_map
// cost of a map lookup is O(log(n)) where n is the number of elements in the map
// for a map with 1,000,000 elements, we perform about 20 comparisons and indirections to find an element
// a hashed lookup can be better

#include <unordered_map>

// phone book implementation
std::unordered_map<string,int> phone_book_unordered_map {
    {"David Hume",123456},
    {"Karl Popper",234567},
    {"Bertrand Arthur William Russell",345678}
};

// subscripting an unordered map
int get_number(const std::string& s)
{
    return phone_book_unordered_map[s];
}

// a common need for a custom has function is when we need an unordered container of one of our own types
// implementation of a hash function is a function object

#include  <functional>

struct Record {
    std::string name;
    int product_code;
    // ...
};

struct Rhash { // a hash function for Record
    size_t operator()(const Record& r) const
    {
        return std::hash<std::string>()(r.name) ^ std::hash<int>()(r.product_code);
    }
};

std::unordered_map<Record,Rhash> my_set; // set of Records using Rhash for lookup

// designing good hash functions is complicated
// it's usually best to make a new hash function by combing two with exclusive or ^
// make sure that every value that takes part in the hash actually helps distinguish teh values
// combing two hashes isn't beneficial in the above case unless multiple names exist for each product code. 

// we can avoid explicitly passing the hash operation by defining it as a specialization of the standard library hash

namespace std { // make a hash function for Record
    template<> struct hash<Record> {
        using argument_type = Record;
        using result_type = size_t;

        result_type operator()(const Record& r) const{
            return hash<string>()(r.name) ^ hash<int>()(r.product_code);
        }
    };
}

// differences between a map and un unordered_map
// map requires an ordering function (< is default) and yields an ordered sequence
// an unordeded_map requires an equality function (the default is ==); and does not maintain an order amoung its elements

// with a good hash function, an unordered_map is much faster than a map for big containers
// worse case of an unordered_map with a bad hash function is worse than a map

//12.7 Allocators
// std containers allocate space with new by default
// new and delete provide a general free store (called dynamic memory or heap) that can hold objects of arbitrary size
// and user-controlled lifetime

// the std lib can install allocators with specific semantics if needed

// example of use of a pool allocator
// an event queue that was using vectors as events that were passed as shared_pts

struct Event {
    std::vector<int> data = std::vector<int>(512);
};

std::list<std::shared_ptr<Event>> q;

void producer()
{
    for (int n = 0; n!=LOTS; ++n) {
        lock_guard lk {m}; // m is a mutex
        q.push_back(std::make_shared<Event>());
        cv.notify_one(); // cv is a condition_variable
    }
}

// the code above led to massive fragmentation
// passing 100,000 events amoung 16 producers and 4 consumers consumed 6gb of memory

// the praditional solution to fragmentation problems involves rewriting the code to use a pool allocator
// a pool allocator manages objects of a fixed size and allocates space for many objects at a time

// defined in pmr sub-namespace of std

pmr::synchronized_pool_resource pool; // make a pool
struct Event {
    std::vector<int> data = std::vector<int>{512,&pool}; // let Events use the pool
};

std::list<std::shared_ptr<Event>> q {$pool}; // let q use the pool

// the above solution consumed less than 3mb where the other consumed 6gb
// the amount of memory actually in use is unchanged

// std containers now take optional allocator arguments
// they use new and delete by default

// other polymorphic resources include
// unsynchronized_polymorphic_resource, like polymorphic resource but can only be used on one thread
// monotonic_polymorphic_resource, a fast allocator that releases its memory only upon its destruction and can only be used by one thread

// a polymorphic resource must be derived from memory_resource and define members allocate(), deallocate(), and is_equal()

//12.8 Container Overview
// unorderd containers are optimized for lookup with a key (often a string)
// they're hash tables

// std containers have a consistent set of operations
// a vector is usually more efficient than a list for short sequences with small elements
// list is the opposite

// use vector unless you have a reason

// an emplace operation (like emplace_back()) takes arguements for an element's constructor and builds the object in a newly allocated space in the container, rather than copying
// an object into the container

v.push_back(pair{1, "copy or move"}); // make a pair and move it into v
v.emplace_back(1,"build in place"); // build a pair in v

// for simple cases like these, optimization can result in performance being identical to both calls