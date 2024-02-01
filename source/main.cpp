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