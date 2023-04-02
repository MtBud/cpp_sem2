#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
#include <climits>
using namespace std;
#endif /* __PROGTEST__ */

class CPersonalAgenda {
    vector<string> Name;
    vector<string> Surname;
    vector<string> Email;
    vector<unsigned int> Salary;
    int size;

public:
    CPersonalAgenda() {
        size = 0;
    };

    /*~CPersonalAgenda ( void ){

    };*/


    bool add(const string &name,
             const string &surname,
             const string &email,
             unsigned int salary) {
        cout << "add" <<endl;
        if(Name.empty()){
            pushBack(name, surname, email, salary);
            return true;
        }
        if (findUN(name, surname) != -1)
            return false;

        if (findEmail(email) != -1)
            return false;

        int pos = sortedPos(name, surname);
        cout << "sortedPos: " << pos << endl;
        if(pos == -1) {
            pushBack(name, surname, email, salary);
            return true;
        }

        Name.insert(Name.begin() + pos, name);
        Surname.insert(Surname.begin() + pos, surname);
        Email.insert(Email.begin() + pos, email);
        Salary.insert(Salary.begin() + pos, salary);
        size ++;
        return true;
    };

    bool del(const string &name,
             const string &surname) {
        cout << "del" <<endl;
        int position = findUN(name, surname);
        if (position == -1)
            return false;
        Name.erase(Name.begin() + position);
        Surname.erase(Surname.begin() + position);
        Email.erase(Email.begin() + position);
        Salary.erase(Salary.begin() + position);
        size --;
        return true;
    };

    bool del(const string &email) {
        //cout << "del" <<endl;
        int i = findEmail(email);
        if (i == -1)
            return false;
        Name.erase(Name.begin() + i);
        Surname.erase(Surname.begin() + i);
        Email.erase(Email.begin() + i);
        Salary.erase(Salary.begin() + i);
        size --;
        return true;
    };

    bool changeName(const string &email,
                    const string &newName,
                    const string &newSurname) {
        if (findUN(newName, newSurname) != -1)
            return false;

        int i = findEmail(email);
        if (i == -1)
            return false;
        unsigned int salary = Salary.at(i);
        Name.erase(Name.begin() + i);
        Surname.erase(Surname.begin() + i);
        Email.erase(Email.begin() + i);
        Salary.erase(Salary.begin() + i);

        int pos = sortedPos(newName, newSurname);
        if (pos == -1) {
            pushBack(newName, newSurname, email, salary);
            return true;
        }

        Name.insert(Name.begin() + pos, newName);
        Surname.insert(Surname.begin() + pos, newSurname);
        Email.insert(Email.begin() + pos, email);
        Salary.insert(Salary.begin() + pos, salary);
        return true;
    };

    bool changeEmail(const string &name,
                     const string &surname,
                     const string &newEmail) {
        if (findEmail(newEmail) != -1)
            return false;

        int i = findUN(name, surname);
        if(i == -1)
            return false;
        Email.at(i) = newEmail;
        return true;
    };

    bool setSalary(const string &name,
                   const string &surname,
                   unsigned int salary) {
        int i = findUN(name, surname);
        if(i == -1)
            return false;
        Salary.at(i) = salary;
        return true;
    };

    bool setSalary(const string &email,
                   unsigned int salary){
        int i = findEmail(email);
        if(i == -1)
            return false;
        Salary.at(i) = salary;
        return true;
    };

    unsigned int getSalary(const string &name,
                           const string &surname) const{
        int i = findUN(name, surname);
        if(i == -1)
            return 0;
        return Salary.at(i);
    };

    unsigned int getSalary(const string &email) const{
        int i = findEmail(email);
        if(i == -1)
            return 0;
        return Salary.at(i);
    };

    bool getRank(const string &name,
                 const string &surname,
                 int &rankMin,
                 int &rankMax) const{
        int pos = findUN(name, surname);
        if(pos == -1)
            return false;
        unsigned int salary = Salary.at(pos);
        int less = 0;
        int equal = 0;
        for(int i = 0; i < size; i++){
            if(Salary.at(i) < salary)
                less ++;
            if(Salary.at(i) == salary)
                equal ++;
        }
        rankMin = less;
        rankMax = less + equal - 1;
        return true;
    };

    bool getRank(const string &email,
                 int &rankMin,
                 int &rankMax) const{
        int pos = findEmail(email);
        if(pos == -1)
            return false;
        unsigned int salary = Salary.at(pos);
        int less = 0;
        int equal = 0;
        for(int i = 0; i < size; i++){
            if(Salary.at(i) < salary)
                less ++;
            if(Salary.at(i) == salary)
                equal ++;
        }
        rankMin = less;
        rankMax = less + equal - 1;
        return true;
    };

    bool getFirst(string &outName,
                  string &outSurname) const{
        if(Name.empty())
            return false;
        outName = Name.at(0);
        outSurname = Surname.at(0);
        return true;
    };

    bool getNext(const string &name,
                 const string &surname,
                 string &outName,
                 string &outSurname) const{
        int pos = findUN(name, surname);
        if(pos == -1 || pos == size - 1)
            return false;
        outName = Name.at(pos+1);
        outSurname = Surname.at(pos+1);
        return true;
    };

    void print() const{
      for(int i = 0; i < size; i++){
          cout << Name.at(i) << " " << Surname.at(i) << " " << Email.at(i) << " " << Salary.at(i) << endl;
      }
    };

private:
    int findUN(const string &name,
               const string &surname) const {
        string userName = surname + name;
        int hi = size - 1;
        int lo = 0;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            string currUN = Surname.at(mid) + Name.at(mid);
            if (currUN == userName)
                return mid;

            if (userName.compare(currUN) > 0)
                lo = mid + 1;
            else
                hi = mid - 1;
        }
        return -1;
    };


    int findEmail(const string &email) const {
        for (int i = 0; i < size; i++) {
            if (Email.at(i) == email) {
                return i;
            }
        }
        return -1;
    };

    int sortedPos(const string &name, const string &surname){
        string userName = surname + name;
        int hi = size - 1;
        int lo = 0;
        int mid = lo + (hi - lo) / 2;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            string currUN = Surname.at(mid) + Name.at(mid);

            if(mid == 0 && userName.compare(currUN) < 0)
                return mid;

            if(mid == size - 1 && userName.compare(currUN) > 0)
                return -1;

            if (userName.compare(currUN) < 0 && userName.compare(Surname.at(mid-1) + Name.at(mid-1)) > 0)
                return mid;

            if (userName.compare(currUN) > 0)
                lo = mid + 1;
            else
                hi = mid - 1;
        }
        return mid;
    };

    void pushBack(const string &name,
                 const string &surname,
                 const string &email,
                 unsigned int salary){
        Name.push_back(name);
        Surname.push_back(surname);
        Email.push_back(email);
        Salary.push_back(salary);
        size ++;
    };

};

#ifndef __PROGTEST__
int main ( )
{
  string outName, outSurname;
  int lo, hi;

  CPersonalAgenda b1;
  assert ( b1 . add ( "John", "Smith", "john", 30000 ) );
  b1.print();
  assert ( b1 . add ( "John", "Miller", "johnm", 35000 ) );
  b1.print();
  assert ( b1 . add ( "Peter", "Smith", "peter", 23000 ) );
  b1.print();
  assert ( b1 . add ( "Peter", "Smith", "petera", 23000 ) == false);
  assert ( b1 . add ( "Peter", "Dickens", "peter", 23000 ) == false);

  cout << endl;
  assert ( b1 . add ( "Peter", "Dickens", "petera", 23000 ) );
  b1.print();
  assert ( b1 . del ( "Peter", "Dickens") );

  cout << endl;
  assert ( b1 . add ( "Peter", "Dickens", "petera", 23000 ) );
  assert ( b1 . del ( "petera") );

  b1.print();

    assert ( b1 . getFirst ( outName, outSurname )
             && outName == "John"
             && outSurname == "Miller" );
    assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( b1 . getNext ( "John", "Smith", outName, outSurname )
             && outName == "Peter"
             && outSurname == "Smith" );
    assert ( ! b1 . getNext ( "Peter", "Smith", outName, outSurname ) );
    assert ( b1 . setSalary ( "john", 32000 ) );
    assert ( b1 . getSalary ( "john" ) ==  32000 );
    assert ( b1 . getSalary ( "John", "Smith" ) ==  32000 );
    assert ( b1 . getRank ( "John", "Smith", lo, hi )
             && lo == 1
             && hi == 1 );
    assert ( b1 . getRank ( "john", lo, hi )
             && lo == 1
             && hi == 1 );

    cout << endl;
    b1.print();
    cout << "lo: " << lo << "  hi: " << hi << endl;
    assert ( b1 . getRank ( "peter", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . getRank ( "johnm", lo, hi )
             && lo == 2
             && hi == 2 );
    assert ( b1 . setSalary ( "John", "Smith", 35000 ) );
    assert ( b1 . getSalary ( "John", "Smith" ) ==  35000 );
    assert ( b1 . getSalary ( "john" ) ==  35000 );
    assert ( b1 . getRank ( "John", "Smith", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . getRank ( "john", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . getRank ( "peter", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . getRank ( "johnm", lo, hi )
             && lo == 1
             && hi == 2 );
    assert ( b1 . changeName ( "peter", "James", "Bond" ) );
    assert ( b1 . getSalary ( "peter" ) ==  23000 );
    assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
    assert ( b1 . getSalary ( "Peter", "Smith" ) ==  0 );
    b1.print();
    assert ( b1 . getFirst ( outName, outSurname )
             && outName == "James"
             && outSurname == "Bond" );
    assert ( b1 . getNext ( "James", "Bond", outName, outSurname )
             && outName == "John"
             && outSurname == "Miller" );
    assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
    assert ( b1 . changeEmail ( "James", "Bond", "james" ) );
    assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
    assert ( b1 . getSalary ( "james" ) ==  23000 );
    assert ( b1 . getSalary ( "peter" ) ==  0 );
    assert ( b1 . del ( "james" ) );
    assert ( b1 . getRank ( "john", lo, hi )
             && lo == 0
             && hi == 1 );
    assert ( b1 . del ( "John", "Miller" ) );
    assert ( b1 . getRank ( "john", lo, hi )
             && lo == 0
             && hi == 0 );
    assert ( b1 . getFirst ( outName, outSurname )
             && outName == "John"
             && outSurname == "Smith" );
    assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
    assert ( b1 . del ( "john" ) );
    assert ( ! b1 . getFirst ( outName, outSurname ) );
    assert ( b1 . add ( "John", "Smith", "john", 31000 ) );
    assert ( b1 . add ( "john", "Smith", "joHn", 31000 ) );
    assert ( b1 . add ( "John", "smith", "jOhn", 31000 ) );

    CPersonalAgenda b2;
    assert ( ! b2 . getFirst ( outName, outSurname ) );
    assert ( b2 . add ( "James", "Bond", "james", 70000 ) );
    assert ( b2 . add ( "James", "Smith", "james2", 30000 ) );
    assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
    assert ( ! b2 . add ( "James", "Bond", "james3", 60000 ) );
    assert ( ! b2 . add ( "Peter", "Bond", "peter", 50000 ) );
    assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
    assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
    assert ( ! b2 . setSalary ( "Joe", "Black", 90000 ) );
    assert ( ! b2 . setSalary ( "joe", 90000 ) );
    assert ( b2 . getSalary ( "Joe", "Black" ) ==  0 );
    assert ( b2 . getSalary ( "joe" ) ==  0 );
    assert ( ! b2 . getRank ( "Joe", "Black", lo, hi ) );
    assert ( ! b2 . getRank ( "joe", lo, hi ) );
    assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
    assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
    assert ( ! b2 . del ( "Joe", "Black" ) );
    assert ( ! b2 . del ( "joe" ) );
    assert ( ! b2 . changeName ( "james2", "James", "Bond" ) );
    assert ( ! b2 . changeEmail ( "Peter", "Smith", "james" ) );
    assert ( ! b2 . changeName ( "peter", "Peter", "Smith" ) );
    assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter" ) );
    assert ( b2 . del ( "Peter", "Smith" ) );
    assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter2" ) );
    assert ( ! b2 . setSalary ( "Peter", "Smith", 35000 ) );
    assert ( b2 . getSalary ( "Peter", "Smith" ) ==  0 );
    assert ( ! b2 . getRank ( "Peter", "Smith", lo, hi ) );
    assert ( ! b2 . changeName ( "peter", "Peter", "Falcon" ) );
    assert ( ! b2 . setSalary ( "peter", 37000 ) );
    assert ( b2 . getSalary ( "peter" ) ==  0 );
    assert ( ! b2 . getRank ( "peter", lo, hi ) );
    assert ( ! b2 . del ( "Peter", "Smith" ) );
    assert ( ! b2 . del ( "peter" ) );
    assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
    assert ( b2 . getSalary ( "peter" ) ==  40000 );
    assert ( b2 . setSalary ( "peter", UINT_MAX ) );
    assert ( b2 . getSalary ( "peter" ) ==  UINT_MAX );
    assert ( b2 . setSalary ( "James", "Bond", UINT_MAX ) );
    assert ( b2 . getSalary ( "James", "Bond" ) ==  UINT_MAX );

    CPersonalAgenda b3;
    string sus, amogus;
    assert ( ! b3 . getSalary ("bruh"));
    assert ( ! b3 . getSalary ("bruh", "brother"));
    assert ( ! b3 . setSalary ("bruh", 15));
    assert ( ! b3 . setSalary ("bruh", "brother", 15));
    assert ( ! b3 . changeName ( "peter", "Peter", "Falcon" ) );
    assert ( ! b3 . del ( "peter" ) );
    assert ( ! b3 . del ( "peter", "beter" ) );
    assert ( ! b3 . getFirst(amogus, sus));
    assert ( ! b3 . getNext("bruh", "brother", amogus, sus));

    CPersonalAgenda b4;
    assert (  b4 . add ("AAA", "AAA", "aaa", 121));    b4.print();
    assert (  b4 . add ("AAA", "BBB", "bbb", 122));    b4.print();
    assert (  b4 . add ("BBB", "AAA", "ccc", 123));    b4.print();
    assert (  b4 . add ("CCC", "AAA", "ddd", 124));    b4.print();
    assert (  b4 . add ("DDD", "AAA", "eee", 125));    b4.print();
    assert (  b4 . add ("EEE", "AAA", "fff", 126));    b4.print();
    assert (  b4 . add ("AAA", "CCC", "ggg", 127));    b4.print();
    assert (  b4 . add ("AAA", "DDD", "hhh", 128));    b4.print();
    assert (  b4 . add ("AAA", "EEE", "iii", 129));    b4.print();
    assert (  b4 . add ("AAA", "FFF", "jjj", 120));    b4.print();
    assert ( !b4 . add ("AAA", "AAA", "kkk", 130));    b4.print();
    assert ( !b4 . add ("AAA", "PPP", "jjj", 131));    b4.print();

    cout << endl;
    CPersonalAgenda b5;
    assert (  b5 . add ("BBB", "AAA", "ccc", 123));    b5.print();
    assert (  b5 . add ("AAA", "AAA", "aaa", 121));    b5.print();
    assert ( !b5 . add ("AAA", "AAA", "kkk", 130));    b5.print();
    assert (  b5 . add ("AAA", "BBB", "bbb", 122));    b5.print();
    assert (  b5 . add ("AAA", "DDD", "hhh", 128));    b5.print();
    assert (  b5 . add ("CCC", "AAA", "ddd", 124));    b5.print();
    assert (  b5 . add ("AAA", "CCC", "ggg", 127));    b5.print();
    assert (  b5 . add ("DDD", "AAA", "eee", 125));    b5.print();
    assert (  b5 . add ("EEE", "AAA", "fff", 126));    b5.print();
    assert (  b5 . add ("AAA", "EEE", "iii", 129));    b5.print();
    assert (  b5 . add ("AAA", "FFF", "jjj", 120));    b5.print();
    assert ( !b5 . add ("AAA", "PPP", "jjj", 131));    b5.print();
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
