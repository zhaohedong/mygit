#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
using namespace std;

using it = istream_iterator<char>;
using ot = ostream_iterator<char>;
using ans = ostream_iterator<char>;

char encrypt( char c ) { return c + 1; }         // I'm sure that you can do something more exciting
char decrypt( char c ) { return c - 1; }         // Ditto

int main ()
{
   ifstream in ( "infile.txt"  );   it initer ( in  );
   ofstream out( "outfile.txt" );   ot outiter( out );
   transform( initer, {}, outiter, encrypt );
}
