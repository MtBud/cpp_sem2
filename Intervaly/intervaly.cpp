#ifndef __PROGTEST__
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <climits>
using namespace std;
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
// #define EXTENDED_SYNTAX

// max size of 2x long long
class CRange{
    long long lo;
    long long hi;
  public:
    CRange( const long long &newLo, const long long &newHi){
        if(newLo > newHi)
            throw logic_error("low is higher than high");
        lo = newLo;
        hi = newHi;
    };

    bool operator == (const CRange& rightRange) const{
        if(lo == rightRange.Lo() && hi == rightRange.Hi())
            return true;
        else
            return false;
    }

    bool operator != (const CRange& rightRange) const{
        if(lo != rightRange.Lo() || hi != rightRange.Hi())
            return true;
        else
            return false;
    }

    long long Lo() const{
        return lo;
    }

    long long Hi() const{
        return hi;
    }

    void print() const{
      cout << "lo: " << lo << " hi: " << hi << endl;
    };
  private:
    // todo
};


class CRangeList{
    vector<CRange> List;
  public:
    // constructor
    // includes long long / range
    bool includes(const long long& num) const{
        for (auto i: List)
            if (num >= i.Lo() && num <= i.Hi())
                return true;
        return false;
    };

    bool includes(const CRange& range) const{
        for (auto i: List)
            if (range.Lo() >= i.Lo() && range.Hi() <= i.Hi())
                return true;
        return false;
    }

    // += range / range list
    void operator += (const CRange& newInterval){
        unsigned int newpos = 0;
        if(List.empty()){
            List.push_back(newInterval);
            return;
        }
        for(unsigned int i = 0; i < List.size(); i++){
            if(newInterval.Lo() < List.at(i).Lo()){
                List.insert(List.begin() + i, newInterval);
                newpos = i;
                break;
            }
            if(i == List.size() - 1){
                List.push_back(newInterval);
                newpos = i;
                break;
            }
        }

        mergeRight(newpos);
        if(newpos != 0)
            mergeRight(newpos-1);
    }

    void operator += (const CRangeList& addList){
        for(auto i : addList.List){
            *this += i;
        }
    }

    friend CRangeList operator + (const CRange& leftInterval, const CRange& rightInterval);

    CRangeList operator + (const CRange& leftInterval){
        CRangeList tmp = *this;
        tmp += leftInterval;
        return tmp;
    };

    // -= range / range list
    void operator -= (const CRange delInterval){
        delInterval.print();
        if(List.empty())
            return;

        for(unsigned int i = 0; i < List.size(); i++){
            // whole interval
            if(delInterval.Lo() <= List.at(i).Lo() && delInterval.Hi() >= List.at(i).Hi()){
                List.erase(List.begin() + i);
                i --;
                continue;
            }
            // inside
            if(List.at(i).Lo() < delInterval.Lo() && List.at(i).Hi() > delInterval.Hi()){
                CRange replaceLow(List.at(i).Lo(), delInterval.Lo() - 1);
                CRange replaceHi(delInterval.Hi() + 1, List.at(i).Hi());
                List.erase(List.begin() + i);
                addToList( i ,replaceHi);
                addToList(i, replaceLow);
                break;
            }
            // higher edge
            if(delInterval.Lo() <= List.at(i).Hi() && delInterval.Lo() > List.at(i).Lo()){
                CRange replace(List.at(i).Lo(),delInterval.Lo() - 1);
                List.erase(List.begin() + i);
                addToList(i ,replace);
            }
            // lower edge
            if(delInterval.Hi() >= List.at(i).Lo() && delInterval.Hi() < List.at(i).Hi()){
                CRange replace(delInterval.Hi() + 1,List.at(i).Hi());
                List.erase(List.begin() + i);
                addToList( i ,replace);
            }

        }
    }

    void operator -= (const CRangeList& addList){
        for(auto i : addList.List){
            *this -= i;
        }
    }

    friend CRangeList operator - (const CRange& leftInterval, const CRange& rightInterval);

    CRangeList operator - (const CRange& leftInterval){
        CRangeList tmp = *this;
        tmp -= leftInterval;
        return tmp;
    };

    // = range / range list
    CRangeList& operator = ( const CRange newInterval ){
        List.clear();
        List.push_back(newInterval);
        return *this;
    }

    // operator ==
    bool operator == ( const CRangeList& rightList) const{
        for(unsigned int i = 0; i < List.size(); i++){
            if(List.at(i) == rightList.List.at(i))
                continue;
            else
                return false;
        }
        return true;
    }
    // operator !=
    bool operator != ( const CRangeList& rightList) const{
        for(unsigned int i = 0; i < List.size(); i++){
            if(List.at(i) != rightList.List.at(i))
                return true;
        }
        return false;
    }

    // operator <<
    friend ostream& operator << (ostream& out, const CRangeList& outList);


private:
    void mergeRight(const unsigned int& newpos){
        if(List.at(newpos).Hi() == LLONG_MIN && List.at(newpos + 1).Lo() == LLONG_MIN){
            long long newHi = List.at(newpos + 1).Hi();
            if(List.at(newpos).Hi() > List.at(newpos + 1).Hi())
                newHi = List.at(newpos).Hi();

            CRange replace(List.at(newpos).Lo(), newHi);
            List.erase(List.begin() + newpos + 1);
            List.erase(List.begin() + newpos);
            addToList(newpos, replace);
            return;
        }
        while(List.at(newpos).Hi()  >= List.at(newpos + 1).Lo() - 1){
            long long newHi = List.at(newpos + 1).Hi();
            if(List.at(newpos).Hi() > List.at(newpos + 1).Hi())
                newHi = List.at(newpos).Hi();

            CRange replace(List.at(newpos).Lo(), newHi);
            List.erase(List.begin() + newpos + 1);
            List.erase(List.begin() + newpos);
            addToList(newpos, replace);
            if(newpos == List.size()-1){
                break;
            }
        }
    }

    void addToList(const unsigned int& index, const CRange& newInterval){
        if(index == List.size())
            List.push_back(newInterval);
        else
            List.insert(List.begin() + index, newInterval);
    }
};



CRangeList operator + (const CRange& leftInterval, const CRange& rightInterval){
    CRangeList addList;
    addList = leftInterval;
    addList += rightInterval;
    return addList;
}

CRangeList operator - (const CRange& leftInterval, const CRange& rightInterval){
    CRangeList delList;
    delList = leftInterval;
    delList -= rightInterval;
    return delList;
}


ostream& operator << (ostream& out, const CRangeList& outList) {
    out << "{";
    if(outList.List.empty())
        return out << "}";

    long long lowest = outList.List.at(0).Lo();
    for(auto i : outList.List){
        if(i.Lo() != lowest)
            out << ",";
        if(i.Lo() == i.Hi())
            out << i.Lo();
        else
            out << "<" << i.Lo() << ".." << i.Hi() << ">";
    }
    return out << "}";
}


#ifndef __PROGTEST__


string             toString                                ( const CRangeList& x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}


int                main                                   ()
{
  CRangeList a, b;

  assert ( sizeof ( CRange ) <= 2 * sizeof ( long long ) );
  a = CRange ( 5, 10 );
  cout << a << endl;
  a += CRange ( 25, 100 );
  assert ( toString ( a ) == "{<5..10>,<25..100>}" );
  a += CRange ( -5, 0 );
  a += CRange ( 8, 50 );
  assert ( toString ( a ) == "{<-5..0>,<5..100>}" );
  a += CRange ( 101, 105 ) + CRange ( 120, 150 ) + CRange ( 160, 180 ) + CRange ( 190, 210 );
  cout << a << endl;
  assert ( toString ( a ) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}" );
  a += CRange ( 106, 119 ) + CRange ( 152, 158 );
  assert ( toString ( a ) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}" );
  a += CRange ( -3, 170 );
  a += CRange ( -30, 1000 );
  cout << a << endl;
  assert ( toString ( a ) == "{<-30..1000>}" );
  b = CRange ( -500, -300 ) + CRange ( 2000, 3000 ) + CRange ( 700, 1001 );
  a += b;
  assert ( toString ( a ) == "{<-500..-300>,<-30..1001>,<2000..3000>}" );
  cout << a << endl;
  a -= CRange ( -400, -400 );
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}" );
  cout << a << endl;
  CRangeList aa;
  aa += CRange ( 10, 20 ) + CRange ( 900, 2500 ) + CRange ( 30, 40 ) + CRange ( 10000, 20000 );
  cout << "aa: " << aa << endl;
  a -= CRange ( 10, 20 ) + CRange ( 900, 2500 ) + CRange ( 30, 40 ) + CRange ( 10000, 20000 );
  cout << a << endl;
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  try
  {
    a += CRange ( 15, 18 ) + CRange ( 10, 0 ) + CRange ( 35, 38 );
    assert ( "Exception not thrown" == nullptr );
  }
  catch ( const std::logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown" == nullptr );
  }
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  b = a;
  assert ( a == b );
  cout << "a == b" << endl;
  assert ( !( a != b ) );
  b += CRange ( 2600, 2700 );
  assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  assert ( a == b );
  assert ( !( a != b ) );
  b += CRange ( 15, 15 );
  assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}" );
  assert ( !( a == b ) );
  assert ( a != b );
  assert ( b . includes ( 15 ) );
  assert ( b . includes ( 2900 ) );
  assert ( b . includes ( CRange ( 15, 15 ) ) );
  assert ( b . includes ( CRange ( -350, -350 ) ) );
  assert ( b . includes ( CRange ( 100, 200 ) ) );
  assert ( !b . includes ( CRange ( 800, 900 ) ) );
  assert ( !b . includes ( CRange ( -1000, -450 ) ) );
  assert ( !b . includes ( CRange ( 0, 500 ) ) );
  a += CRange ( -10000, 10000 ) + CRange ( 10000000, 1000000000 );
  assert ( toString ( a ) == "{<-10000..10000>,<10000000..1000000000>}" );
  b += a;
  assert ( toString ( b ) == "{<-10000..10000>,<10000000..1000000000>}" );
  b -= a;
  assert ( toString ( b ) == "{}" );
  b += CRange ( 0, 100 ) + CRange ( 200, 300 ) - CRange ( 150, 250 ) + CRange ( 160, 180 ) - CRange ( 170, 170 );
  assert ( toString ( b ) == "{<0..100>,<160..169>,<171..180>,<251..300>}" );
  b -= CRange ( 10, 90 ) - CRange ( 20, 30 ) - CRange ( 40, 50 ) - CRange ( 60, 90 ) + CRange ( 70, 80 );
  assert ( toString ( b ) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}" );

  CRangeList c;
  c += CRange ( LLONG_MIN, -100) + CRange( 100, LLONG_MAX);
  cout << c << endl;
  assert( toString ( c ) == "{<-9223372036854775808..-100>,<100..9223372036854775807>}" );
  c = CRange ( LLONG_MIN, -100) + CRange( 100, LLONG_MAX);
  assert( toString ( c ) == "{<-9223372036854775808..-100>,<100..9223372036854775807>}" );
  c -= CRange ( LLONG_MIN, -2000) + CRange(2000, LLONG_MAX);
  assert( toString ( c ) == "{<-1999..-100>,<100..1999>}");
  c = CRange ( LLONG_MIN, -100) + CRange( 100, LLONG_MAX);
  assert ( c .includes( LLONG_MAX) );
  assert ( c .includes( LLONG_MIN) );
  CRangeList d = c;
  assert ( c == d);
  assert ( !( c != d) );
  c = CRange ( 101, 105 ) + CRange ( 120, 150 ) + CRange ( 160, 180 ) + CRange ( 190, 210 );
  c -= CRange(LLONG_MIN,LLONG_MAX);
  assert (toString( c ) == "{}");
  cout << c << endl;
  c = CRange ( LLONG_MIN, LLONG_MAX);
  c -= CRange(LLONG_MIN,LLONG_MAX);
  assert (toString( c ) == "{}");
  c = CRange ( LLONG_MIN, LLONG_MAX);
  c += CRange (LLONG_MAX, LLONG_MAX);
  cout << c << endl;
  assert (toString( c ) == "{<-9223372036854775808..9223372036854775807>}");
  c += CRange (LLONG_MIN, LLONG_MIN);
  assert (toString( c ) == "{<-9223372036854775808..9223372036854775807>}");
  c -= CRange(LLONG_MIN, LLONG_MIN);
  assert (toString( c ) == "{<-9223372036854775807..9223372036854775807>}");
  c -= CRange(LLONG_MAX, LLONG_MAX);
  assert (toString( c ) == "{<-9223372036854775807..9223372036854775806>}");
  c = CRange(LLONG_MIN, LLONG_MIN);
  assert (toString( c ) == "{-9223372036854775808}");
  c = CRange(5, 5) + CRange(5, 5);
  cout << c << endl;
  assert (toString( c ) == "{5}");
  c = CRange(LLONG_MIN, LLONG_MIN) + CRange(LLONG_MIN, LLONG_MIN);
  cout << c << endl;
  assert (toString( c ) == "{-9223372036854775808}");










#ifdef EXTENDED_SYNTAX
  CRangeList x { { 5, 20 }, { 150, 200 }, { -9, 12 }, { 48, 93 } };
  assert ( toString ( x ) == "{<-9..20>,<48..93>,<150..200>}" );
  ostringstream oss;
  oss << setfill ( '=' ) << hex << left;
  for ( const auto & v : x + CRange ( -100, -100 ) )
    oss << v << endl;
  oss << setw ( 10 ) << 1024;
  assert ( oss . str () == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======" );
#endif /* EXTENDED_SYNTAX */
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
