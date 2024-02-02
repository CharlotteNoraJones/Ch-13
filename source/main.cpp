#include <iostream>
#include <concepts>
#include <ranges>

using namespace std;

// Ch. 8 Concepts and Generic Programming

// 8.1 Introductions
// advantages of templates: 
// ability to pass types without loss of information
// weaving information form different context as instantiaiton time (good optimization)
// passing values as template arguements (compile time computation, good optimization)

// generic programming: emphasis on general algorithims
// general: algorithims that accept any type that meets its invarients

// also called parametric polymorphism

// 8.2 Concepts

template<typename Seq, typename Value>
Value sum(Seq s, Value v)
{
    for (const auto& x : s)
        v += x;
    return v;
}

// requirements: seq is a sequence of elements, value is a number

// it can be invoked if: 
// seq has a begin() and end()
// Value is an arithmatic type that supports +=
// these requirements are called concepts

// 8.2.1 Use of Concepts
// most templates should be constrained templates
// constrained template: throws compiler error if type doens't meet requirements
// typename is least constraining, only requires arg be a type

// example from above improved using concepts

template<Sequence Seq, Number Num>
    requires Arithmetic<range_value_t<Seq>,Num>
Num sum(Seq s, Num v)
{
    for (const auto& x : s)
        v += x;
    return v;
}

// range_value_t: type of elements in a sequence
// Arithmetic<X,Y> concept specifiing we can do arithmetic with X and Y
// if an algorithim requires arguements of differing types, it's good to make
// the relationship between the types explicit

// if a specification is not complete, some errors won't be fpound until instantiation time. 
// partial specifications are better than nothing and part of the development process

// requires Arithmetic<range_value_t<Seq><Num> is a requirement
// templace <Sequence Seq> is a shorthand for requires Sequence<Seq>
// this is the verbose equivalent
template<typename Seq, typename Num>
    requires Sequence<Seq> && Number<Num> && Arithmetic<range_value_t<Seq>Num>
Num sum(Seq s, Num n);

// equivalence between the notations lets us write
template<Sequence Seq, Arithmetic<range_value_t<Seq>> Num>
Num sum(Seq s, Num n);

// if you can't use concepts, naming conventions and comments can work
template<typename Sequence, typename Number>
    // requires Arithmetic<range_value_t<Sequence>,Number>
Number sum(Sequence s, Number n);

// whatetever you do, give the template semantically meaningful constraints on arguements. 

// 8.2.2 Concept-based Overloading
// templates can be overloaded as long as their properties are respected

// example:
template<forward_iterator Iter>
void advance(Iter p, int n) // move p elements forward
{
    while (n--)
        ++p; // a forward iterator has ++, but not + or +=
}

template<random_access_iterator Iter>
void advance(Iter p, int n)
{
    p+=n; // a random-access iterator has +=
}

// compiler seletcts the template with the strongest requirements met by the arguements

void user(vector<int>::iterator vip, list<string>::iterator lsp){
    advance(vip, 10); // uses the fast advance()
    advance(lsp, 10); // uses the slow advance()
}

// compiler gives an ambiguity error if there's no best choice. 

// rules for chosing concept overloaded functions based on one arguement
// if the alternative doens't match the concept, it can't be chosen
// if the arguement matches the concept for just on alternative, that alternative is chosen
// if arguements form two alternative match a concept and one is stricter than the other, the stricter is chosen
// ambigtuity: when two are equally good matches

// requirements for an alternative to be chosen
// a match for all the arguements
// at least as good a match for all arguements as other alternatives
// a better match for at least one arguement

// 8.2.3 Valid Code
// expressions are used to check the validity of a template paramenter

// using a requires expression to check if a template is valid

template<forward_iterator Iter>
    requires requires(Iter p, int i) {p[i]; p+1;} // Iter has subscripting and integer addition
void advance(Iter p, int n) // move p n elements forward
{
    p+=n;
}

// first requires stars the requirements clause and the second starts the expression

// requires expression: predicate, returns true if statements are true
// requires requires shouldn't be soon in regular code, its too low level

// proper version of the above code

template<random_access_iterator Iter>
void advance(Iter p, int n) // move p n elements forward
{
    p+=n; // a random access iterator as +=
}

// 8.2.4 Definition of conepts
// concept: a compile time predicate specifying how one or more types can be used
// example:
template<typename T>
concept Equality_comparable =
    requires(T a, T b) {
        {a == b} -> Boolean; // compare Ts with ==
        {a != b} -> Boolean; // compare Ts with !=
    };

// the concept above ensures we can compare values of type equal and non-equal

static_asset(Equality_comparable<int>); // succeeds

struct S {int a; };
static_assert(equality_comparable<S>); //fails because structs don't automatically get == and !=

// value of a concept is always a bool

// result o {..} after -> is a concept

// handling nonhomogeneous comparions
template<typename T, typename T2 = T>
concept equality_comparable = 
    requires (T a, T2 b) {
        {a == b} -> Boolean; //compare a T to a T2 with ==
        {a != b} -> Boolean; // compare a t to a T2 with !=
        { b == a} -> Boolean; // compare a T2 to a T with ==
        {b != a} -> Boolean; // compare a T2 to a T with !=
    };

// typename T2 = t says that i we don't specify a second template argument, T2 will be the same as T; T is a defualt template arguement

// testing Equality_comparable
static_assert(equality_comparable<int, double>); // succeeds
static_assert(equality_comparable<int>); // succeeds (T2 is defaulted to int)
static_assert(equality_comparable<int,string>); // fails

// defining a concept the requires arithmetic to be valid between numbers

template<typename T, typename U = T>
concept Number = 
    requires (T x, U y) { // something with arithmetic operations and a zero
        x+y; x-y; x*y; x/y;
        x+=y; x-=y; x*=y; x/=y;
        x=x; // copy
        x=0;
    };

// with one arg: checks if the arg has desired properties of a Number
// with two args: checks if the args can be used with the required properties

// now we can define Arithmetic
template<typename T, typename U = T>
concept Arithmetic = Number<T,U> && Number<U,T>;

// sequence concept, more complex than arithmatic

template<typename S>
concept Sequence = requires(S a) {
    typename range_value_t<S>; // S must have a value type
    typename iterator_t<S>; // S must have an interator type

    { a.begin() } -> same_as<iterator_t<S>>; // S must have a begin() that returns an iterator
    { a.end() } => same_as<iterator_t<S>>;

    requires input_iterator<iterator_t<S>>; // S's iterator must be input_iterator
    requires same_as<range_value_t<S>, iter_value_t<S>>;
}

// hardest concepts to define deal with fundamental language concepts
// use standard library templates for these

// example:
    template<typename S>
    concept Sequence = input_range<S>; // simple to write and general

// restrict S's value type to S::value_type

template<class S>
using Value_type = typename S::value_type;

// 8.2.4.1 Definition Checking
// concepts check args at the point of use of the template.
// they don't check the use of parameters in the definition of the template

template<equality_comparable T>
bool cmp(T a, T b)
{
    return a<b;
}

// the concept guarantees the presence of == but not <:
bool b0 = cmp(cout,cerr); //error: ostream doesn't support ==
bool b1 = cmp(2, 3); // Ok: returns true
bool b2 = cmp(2+3i,3+4i); // error: complex<double> doesn't support <

// benefits of delaying the final check of the template to instantiation time:
// allows use of incomplete concepts during development. 
// can insert, debug, tracing, telemetry, etc. code into a template without affecting the interface. (which causes large recompilation)

// the price of these is that some errors are caught very late in compilation

// 8.5.2 Concepts and auto
// auto can indicate the type should have the type of its initializer
auto x = 1; // x is an int
auto z = complex<double>{1,2}; // z is a complex<double>

// initialization doesn't just take place in the simple variable definitions

auto g() { return 99; } // g() returns an int
int f(auto x) {/* ... */} // take an arguement of any type
int x = f(1); // this f() takes an int
int z = f(complex<double>{1,2}); // this f() takes a complex<double>

// auto denotes least constrained concept for a value
// auto as a concept: value must be of some type
// taking an auto parameter makes a function into a function template

// requirements can be strengthened by all such initializations by preceding auto with a concept

auto twice(Arithmetic auto x) {return x+x;} // just for numbers
auto thrice(auto x) {return x + x + x; } // for anything with a +

auto x1 = twice(7); // Ok: x1==14
string s "Hello";
auto x2 = twice(s); // error: a string is not Arithmetic
auto x3 = thrice(s); // Ok x3=="Hello Hello Hello "

// concepts can also constrain the initialization of variables
auto ch1 = open_channel("foo"); // works with whatever open_channel() returns
Arithmetic auto ch2 = open_channel("foo"); // error: a channel is not Arithmetic
Channel auto ch3 = open_channel("foo"); // Ok: assuming Channel is an appropriate concept
                                        // and that open_channel() returns one

// this documents requirements of code and helps prevent overuse of auto

// catch a type error as close to its origin as possible
// contraining a return type can help with this
Number auto some_function(int x)
{
    // ..
    return fct(x); // an error unless fct(x) returns a Number
    // ..
}

// we can acheive the same thing by introducing a variable
// but its a little verbose + not all types are cheap to copy
auto some_function(int x)
{
    // ..
    Number auto y = fct(x); // an error unless fct(x) returns a Number
    return y;
    // ..
}

// 8.2.6 Concepts and Types
// a type
    // specifies the set of operations that can be applied to an object, implicitly and explicitly
    // relies on function delcarations and language rules
    // specifies how an object is laid out in memory
// a single-arguement concept
    // specifies the set of operations that can be applied to an object, implicitly and explicitly
    // relies on use patterns reflecting function declarations and language rules
    // says nothing about the layout of the object
    // enables the use of a set of types

// constraining with concepts gives more flexibility than constraining with types. 
// concepts define the relationship amoung several arguements
// ideally, most functions are template functions with types constrained by concepts. 
// for now, we must use templates as adjectives instead of nouns

void sort(Sortable auto&); // 'auto' required
void sort(Sortable&); // error: 'auto' required after concept name

// 8.3 Generic Programming
// generic programming: abstracting from conrete efficient algorithms to generic algorithims
// concepts: abstractions that represent fundamental operations and data structures

// 8.3.1 Use of Concepts
// it can be difficult to identify and formalizing concepts

// regular: a type that behaves a lot like int or vector
    // can be default constructed
    // can be copied (with the usual semantics of copy, yielding two objects that are indpenedant and compare equal) using a constructor or assignment
    // can be compared using == and !=
    // doesn't suffer technical problems with overly clever programming tricks

// string is also regular.
// int and string are both totally_ordered: can be compared with <, <=, >, >=, and <=>

// concept is not just syntactic notion, also about semantics

// 8.3.2 Abstraction Using Templates
// good abstractions grow from concrete examples
// start with an example from real life and try to eliminate inessentail details
// don't try to come up with an abstraction for every possible scenario

//example:
double sum(const vector<int>& v)
{
    double res = 0;
    for (auto x : v)
        res += x;
    return res;
}

// this is one of many ways to compute a sum of a sequence of numbers. 
// what makes this code less general than it needs to be:
    // why just ints?
    // why just vectors?
    // why accumuate in a double?
    // why start at 0?
    // why add?

// generalize the sum algorithim
// this is a simplified version of stl accumulate
template<forward_iterator Iter, Arithmetic<iter_value_t<Iter>> Val>
Val accumulate(Iter first, Iter last, Val res)
{
    for (auto p = first; p!=last; ++p)
        res += *p;
    return res;
}

// the data structure to be traversed has been abstracted into a pair of iterators representing a sequence
// the type of the accumulator has been made into a parameter
// the type of the accumulator must be arithmetic
// the type of the accumulator must work with the iterator's value type (the element type of the sequence)
// the initial value is now an input; the type of the accumulator is the type of this initial value

// generic functions have identical performance to non-generic functions

// example:
void use (const vector<int>& vevc, const list<double>* lst)
{
    auto sum = accumulate(begin(vec),end(vec),0.0); // accumulate in a double
    auto sum2 = accumuate(begin(lst), end(lst),sum);
}
//lifting: generalizing from a concrete piece of code (or several) while perserving performcing

// often best way to develop a template is
// first, write a concrete version
// then, debug, test, and measure it
// finally, replace the concrete types with template arguments

// the user interface can be simpliced like so
template<forward_range R, Arithmetic<value_type_t<R>> Val>
Val accumulate(const R& r, Val res = 0)
{
    for (auto x : r)
        res += x;
    return res;
}

// range: a stl concept representing a sequence with begin() and end()
// the += operation can be abstacted
// pair-of-iterators version is useful for generality, the range version for simplicity of common uses.

// 8.4 Variadic Templates
// varaidic template: accepts an arbitrary number of arbitrary types

// a function that writes out values of any type that has a << operator
void user()
{
    print("first: ", 1, 2.2, "hello\n"s); // first 1 2.2 hello
    print("\nsecond: ", 0.2, 'c', "yuck!"s, 0, 1, 2, '\n'); // second: 0.2 c yuck! 0 1 2
}

// traditional way to implement a variadic template
// seperater the first arguement from the rest and then recursively call the variadic templace for the tail of the arguments

template<typename T>
concept Printable = requires(T t) {std::cout<<t;} // just one operation!

void print()
{
    // what we do for no arguments: nothing
}

template<Printable T, Printable... Tail>
void print(T head, Tail... tail)
{
    cout << head << << ''; // first, what we do for the head
    print(tail...); // then, what we do for the tail
}

// Printable.. indicates that Tail is a sequence of types. Tail.. indicicates that tail is is a sequence
// of values of the types in Tail

// parameter pack: parameter declared with ...

// print() can take any number of arguements of any types

// print() seperates the arguments into a head (the first) and a tail (the rest)

// if we don't want to allow the zero arguement case, we can do 
template<Printable T, Printable... Tail>
void print(T head, Tail... tail)
{
    cout << head << '';
    if constexpr(sizeof...(tail)>0)
        print(tail...);
}

// using a compile-time if avoids a final call print() from being generated. 
// thus, "empty" print() doesn't need to be defined

// weaknessed of variadic templates
// recursive implementations are tricky
// type check of the interface is a possibly elaborate template program
// type checking code is ad hoc, rather than defined in the standard
// recursive implementations can be surprisingly expensive in compile time and the compiler memory requirements. 

// variadic templates are widely used in stl, and sometimes overused

// 8.4.1 Fold Expressions
// simplied form of iteration over elements or a parameter pack
template<Number... T>
int sum(T...v)
{
    return (v + ... + 0); // add all elements to v starting with 0
}

// the sum above can take any number of elements of any type
int x = sum(1, 2, 3, 4, 5); // x becomes 15
int y = sum('a', 2.4, x); // y becomes 114 (2.4 is truncated and the value of 'a' is 97)

// the body of sum uses a fold expression
// (v+...+0) == (v[0]+(v[1]+(v[2]+(v[3]+(v[4]+0))))
// that is a right fold
// example with a left fold

template<Number... T>
int sum2(T...v)
{
    return (0+...+v);// add all elements of v to 0
}

// (0+...+v) == (((((0+v[0])+v[1])+v[2])+v[3])+v[4])

// a fold expression in c++ is currently restricted to simplifying the implementation of variadic templates
// fold's don't have to perform numeric computations
template<Printable ...T>
void print(T&&... args)
{
    (std::cout << .. << args) << '\n'; // print all arguments
}

print("Hello!"s, '', "World ", 2017) // (((((std::cout << "Hello!"s) << ’ ’) << "World ") << 2017) << ’\n’);

// 8.4.2 Forwarding arguments
// variadic templates let us pass arguements unchanged through an interface. 
// example: network channel, where the actual method of moving values is a parameter
// different transport mechanisms have different sets of constructor parameters:
template<concepts::InputTransport Transport>
class InputChannel {
    public:
        // ..
        InputChannel(Transport::Args&&... transportArgs)
            :_transport(std::forward<TransportArgs>(transportArgs)...)
    {}
        //..
        Transport _transport;
}

// std::forward() moves args unchanged from InputChannel construct to th TRansport constructor

// the writer of InputChannel can construct an object of type Transport without having to know what arguements are required to construct a particular transport.
// the implementer of InputChannel needs only to know the common user interface for all Transport objects. 

// 8.5 Template Compilation Model
// at the point of use, args for a template are checked against its concepts. 
// some code is also checked at template instantiation time
// one example is args for unconstrained template parameters

// this can result in an error being detected uncomfortable late
// this also gives bad error messages

// instantation time-type checking provided for templates checks the use of arguements in the template definition. 
// this provides compile time duck typing