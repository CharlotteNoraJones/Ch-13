// Ch 11 Input and Output

// 11.1 Introduction
// ostream converts typed objects to a stream of characters (bytes)
// istream converts a string of char to typed objects
// they're type safe, type sensitive, extensible

// uses
// i/o for strings
// formatting into string buffers
// i/o for memory
// file i/o

// stream classes are RAII

// 11.2 Output
// cout is standard stream, cerr for reporting errors

#include<iostream>

int b = 'b';
char c = 'c';
std::cout << 'a' << b << c;
// this outputs a98c, b is converted to ascii value

// 11.3 Input
// istreams are for input
int i;
std::cin >> i; // read an integer into i

double d;
std::cin >> d; // read a double-precision floating point number into d

// this is equivalent to the statements above
int i;
double d;
std::cin >> i >> d; // read into i and d

// stringstreams do formatting to and from memory

//11.4 I/O state

// iostream has a state, check to see if operation succeeded
// iostream can be used to read a sequence of values
#include <vector>
std::vector<int> read_ints(std::istream& is)
{
    std::vector<int> res;
    for (int i; is>>i; )
        res.push_back(i);
    return res;
}
 // reads from is until something that's not an int is encountered

// version of read-ints that accepts a terminal string
std::vector<int> read_ints(istream& is, const string& terminator)
{
    std::vector<int> res;
    for (int i; is >> i; )
        res.push_back(i);
    
    if (is.eof()) // file:end of file
        return res;
    if (is.fail()) {    // we failed to read an int; was it the terminator?
        is.clear() // reset state to good()
        string s;
        if (is>>s && s==terminator)
            return res;
        is.setstate(ios_base::failbit); // add fail() to is's state
    }
    return res;
}

auto v = read_ints(cin,"stop");

// 11.5 I/O of User-Defined Types
// use iostream to define i/o for user-defined typed

struct Entry {
    string name;
    int number;
};

// defining an output operator
std::ostream& operator<<(std::ostream& os, const Entry&e)
{
    return os << "{\"" << e.name << "\"," << e.number << "}";
}

// example that checks for formatting and deals with errors
std::istream& operator>>(std::istream& is, Entry& e)
    // read {"name", number} pair. Note: formatted with { " ", and}
{
    char c, c2;
    if (is>>c && c=='{' && is>>c2 && c2=='"') { // start with a { followed by a "
        std::string name;  // the default value of a string is the empty string: ""
        while(is.get(c) && c!='"') // anything before a " is part of the name
            name +=c;
        
        if(is>>c && c==',') {
            int number = 0;
            if (is>>number>>c && c=='}') { // read the number and a }
                e = {name,number}; // assign to the entry
                return is;
            }
        }
    }

    is.setstate(std::ios_base::failbit); // register the failure in the stream
    return is;
}

// when used as a condition, is>>c checks if reading char from is into c was successful

// is>>c skips whitespace
// is.get(c) does not

// 11.6 Output Formatting
// output formatting only supports unicode
// iostream is old, format is new
// format is based around printf()

//11.6.1 Stream Formatting
// simplest formatting controls are called manipulators
#include<ios>
#include<istream>
#include<ostream>
#include<iomanip>

// example of manipulators to control the base of outputted numbers
std::cout << 1234 << ' ' << hex << 1234 << ' ' << oct << 1234 << dec << 1234 << '\n'; // 1234 4d2 2322 1234

//We can explicitly set the output format for floating-point numbers:
constexpr double d = 123.456;
    std::cout << d << "; " // use the default format for d
        << scientific << d << "; " // use 1.123e2 style format for d
        << hexfloat << d << "; " // use hexadecimal notation for d
        << fixed << d << "; " // use 123.456 style format for d
        << defaultfloat << d << '\n'; // use the default format for d
// This produces:
// 123.456; 1.234560e+002; 0x1.edd2f2p+6; 123.456000; 123.456

// floating-point manipulators are sticky
// sticky:: effects persist over subsequent operations


// use std::format
// it has useful format specifiers

//<filesystem> library is used to access files
// path class offers useful operations for messing with file systems
