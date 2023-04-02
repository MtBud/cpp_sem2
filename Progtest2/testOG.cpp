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

struct userInfo {
    string name;
    string surname;
    string email;
    unsigned int salary;
    userInfo(const string& newName, const string& newSurname, const string& newEmail, const unsigned int& newSalary){
        name = newName;
        surname = newSurname;
        email = newEmail;
        salary = newSalary;
    }
};


class CPersonalAgenda {
    vector<userInfo> NameSort;
    vector<userInfo> EmailSort;
    int size;

public:
    CPersonalAgenda() {
        size = 0;
    };

    bool add(const string &name,
             const string &surname,
             const string &email,
             unsigned int salary) {
        cout << "add" <<endl;
        if(NameSort.empty()){
            pushBack(name, surname, email, salary, NameSort);
            pushBack(name, surname, email, salary, EmailSort);
            size ++;
            cout << "empty" << endl;

            return true;
        }
        if (findUN(name, surname) != -1)
            return false;

        if (findEmail(email) != -1)
            return false;

        int pos = sortedPos(name, surname);
        int posmail = sortedPosMail(email);
        cout << "pos, posmail: " << pos << " " << posmail << endl;

        userInfo entry(name, surname, email, salary);
        if(pos == -1)
            pushBack(name, surname, email, salary, NameSort);
        else
            NameSort.insert(NameSort.begin() + pos, entry);

        if(posmail == -1)
            pushBack(name, surname, email, salary, EmailSort);
        else
            EmailSort.insert(EmailSort.begin() + posmail, entry);

        size ++;
        return true;
    };

    bool del(const string &name,
             const string &surname) {
        cout << "del" <<endl;
        int pos = findUN(name, surname);
        if (pos == -1)
            return false;
        string email = NameSort.at(pos).email;
        int posmail = findEmail(email);

        NameSort.erase(NameSort.begin() + pos);
        EmailSort.erase(EmailSort.begin() + posmail);
        size --;
        return true;
    };

    bool del(const string &email) {
        cout << "del" <<endl;
        int posmail = findEmail(email);
        if (posmail == -1)
            return false;
        string name = EmailSort.at(posmail).name;
        string surname = EmailSort.at(posmail).surname;
        int pos = findUN(name, surname);

        NameSort.erase(NameSort.begin() + pos);
        EmailSort.erase(EmailSort.begin() + posmail);
        size --;
        return true;
    };

    bool changeName(const string &email,
                    const string &newName,
                    const string &newSurname) {
        cout << "changename" << endl;

        if (findUN(newName, newSurname) != -1)
            return false;

        int posmail = findEmail(email);
        if (posmail == -1)
            return false;
        string name = EmailSort.at(posmail).name;
        string surname = EmailSort.at(posmail).surname;
        unsigned int salary = EmailSort.at(posmail).salary;
        EmailSort.at(posmail).name = newName;
        EmailSort.at(posmail).surname = newSurname;

        int pos = findUN(name, surname);
        cout << "pos: " << pos << endl;

        NameSort.erase(NameSort.begin() + pos);
        size --;

        pos = sortedPos(newName, newSurname);
        if (pos == -1) {
            pushBack(newName, newSurname, email, salary, NameSort);
            size++;
            return true;
        }

        userInfo add(newName, newSurname, email, salary);
        NameSort.insert(NameSort.begin() + pos, add);
        size++;
        return true;
    };

    bool changeEmail(const string &name,
                     const string &surname,
                     const string &newEmail) {
        cout << "changeemail" << endl;

        if (findEmail(newEmail) != -1)
            return false;

        int pos = findUN(name, surname);
        if (pos == -1)
            return false;
        string email = NameSort.at(pos).email;
        unsigned int salary = NameSort.at(pos).salary;
        int posmail = findEmail(email);
        NameSort.at(pos).email = newEmail;

        EmailSort.erase(EmailSort.begin() + posmail);
        size --;

        posmail = sortedPosMail(newEmail);
        if (posmail == -1) {
            pushBack(name, surname, newEmail, salary, EmailSort);
            size++;
            cout << "change size: " << size << endl;
            return true;
        }

        userInfo add(name, surname, newEmail, salary);
        EmailSort.insert(EmailSort.begin() + posmail, add);
        size++;
        return true;
    };

    bool setSalary(const string &name,
                   const string &surname,
                   unsigned int salary) {
        int pos = findUN(name, surname);
        if(pos == -1)
            return false;
        string email = NameSort.at(pos).email;
        int posmail = findEmail(email);
        NameSort.at(pos).salary = salary;
        EmailSort.at(posmail).salary = salary;
        return true;
    };

    bool setSalary(const string &email,
                   unsigned int salary){
        int posmail = findEmail(email);
        if(posmail == -1)
            return false;
        string name = EmailSort.at(posmail).name;
        string surname = EmailSort.at(posmail).surname;
        int pos = findUN(name, surname);
        NameSort.at(pos).salary = salary;
        EmailSort.at(posmail).salary = salary;
        return true;
    };

    unsigned int getSalary(const string &name,
                           const string &surname) const{
        int pos = findUN(name, surname);
        if(pos == -1)
            return 0;
        return NameSort.at(pos).salary;
    };

    unsigned int getSalary(const string &email) const{
        int posmail = findEmail(email);
        if(posmail == -1){
            cout<< "email not found" << endl;
            return 0;}
        return EmailSort.at(posmail).salary;
    };

    bool getRank(const string &name,
                 const string &surname,
                 int &rankMin,
                 int &rankMax) const{
        int pos = findUN(name, surname);
        if(pos == -1)
            return false;
        unsigned int salary = NameSort.at(pos).salary;
        int less = 0;
        int equal = 0;
        for(int i = 0; i < size; i++){
            if(NameSort.at(i).salary < salary)
                less ++;
            if(NameSort.at(i).salary == salary)
                equal ++;
        }
        rankMin = less;
        rankMax = less + equal - 1;
        return true;
    };

    bool getRank(const string &email,
                 int &rankMin,
                 int &rankMax) const{
        int posmail = findEmail(email);
        if(posmail == -1)
            return false;
        unsigned int salary = EmailSort.at(posmail).salary;
        int less = 0;
        int equal = 0;
        for(int i = 0; i < size; i++){
            if(EmailSort.at(i).salary < salary)
                less ++;
            if(EmailSort.at(i).salary == salary)
                equal ++;
        }
        rankMin = less;
        rankMax = less + equal - 1;
        return true;
    };

    bool getFirst(string &outName,
                  string &outSurname) const{
        if(NameSort.empty())
            return false;
        outName = NameSort.at(0).name;
        outSurname = NameSort.at(0).surname;
        return true;
    };

    bool getNext(const string &name,
                 const string &surname,
                 string &outName,
                 string &outSurname) const{
        int pos = findUN(name, surname);
        if(pos == -1 || pos == size - 1)
            return false;
        outName = NameSort.at(pos+1).name;
        outSurname = NameSort.at(pos+1).surname;
        return true;
    };

    void printNameSort() const{
      for(int i = 0; i < size; i++){
          cout << NameSort.at(i).name << " " << NameSort.at(i).surname << " " <<
                    NameSort.at(i).email << " " << NameSort.at(i).salary << endl;
      }
      cout << "size: " << size << endl;
    };

    void printEmailSort() const{
        for(int i = 0; i < size; i++){
            cout << EmailSort.at(i).name << " " << EmailSort.at(i).surname << " " <<
                 EmailSort.at(i).email << " " << EmailSort.at(i).salary << endl;
        }
        cout << "size: " << size << endl;
    };

private:
    int findUN(const string &name,
               const string &surname) const {
        int hi = size - 1;
        int lo = 0;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (NameSort.at(mid).surname == surname && NameSort.at(mid).name == name)
                return mid;

            if (NameSort.at(mid).surname < surname)
                lo = mid + 1;
            if (NameSort.at(mid).surname > surname)
                hi = mid - 1;
            if(NameSort.at(mid).surname == surname && NameSort.at(mid).name < name)
                lo = mid + 1;
            if(NameSort.at(mid).surname == surname && NameSort.at(mid).name > name)
                hi = mid - 1;
        }
        return -1;
    };


    int findEmail(const string &email) const {
        int hi = size - 1;
        int lo = 0;
        int mid = lo + (hi - lo) / 2;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (EmailSort.at(mid).email == email)
                return mid;

            if (EmailSort.at(mid).email < email)
                lo = mid + 1;
            if (EmailSort.at(mid).email > email)
                hi = mid - 1;
        }
        cout << "mid: " << mid << endl;
        return -1;
    };

    // sus switch binary search for finding the spot to put the name
    int sortedPos(const string &name, const string &surname){
        int hi = size - 1;
        int lo = 0;
        int mid = lo + (hi - lo) / 2;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            cout << "mid: " << mid << endl;

            // if the position is at the beginning
            if(mid == 0 && NameSort.at(mid).surname > surname)
                return mid;
            if(mid == 0 && NameSort.at(mid).surname == surname && NameSort.at(mid).name > name)
                return mid;

            // if the position is at the end
            if(mid == size - 1 && NameSort.at(mid).surname < surname)
                return -1;
            if(mid == size - 1 && NameSort.at(mid).surname == surname && NameSort.at(mid).name < name)
                return -1;

            // moving further in the binary search
            if(NameSort.at(mid).surname < surname)
                lo = mid + 1;
            if(NameSort.at(mid).surname > surname)
                hi = mid - 1;
            if(NameSort.at(mid).surname == surname && NameSort.at(mid).name < name)
                lo = mid + 1;
            if(NameSort.at(mid).surname == surname && NameSort.at(mid).name > name)
                hi = mid - 1;
        }
        if(mid == size - 1)
            return mid;
        if(NameSort.at(mid).surname < surname)
            return mid + 1;
        if(NameSort.at(mid).surname == surname && NameSort.at(mid).name < name)
            return mid + 1;
        return mid;
    };

    int sortedPosMail(const string &email){
        int hi = size - 1;
        int lo = 0;
        int mid = lo + (hi - lo) / 2;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            cout << "mid: " << mid << endl;

            // if the position is at the beginning
            if(mid == 0 && EmailSort.at(mid).email > email)
                return mid;

            // if the position is at the end
            if(mid == size - 1 && EmailSort.at(mid).email < email)
                return -1;

            // moving further in the binary search
            if(EmailSort.at(mid).email < email)
                lo = mid + 1;
            if(EmailSort.at(mid).email > email)
                hi = mid - 1;

        }

        if(mid == size - 1)
            return mid;
        if(EmailSort.at(mid).email < email){
            return mid + 1;
        }
        return mid;
    };

    static void pushBack(const string &name,
                 const string &surname,
                 const string &email,
                 unsigned int salary,
                 vector<userInfo>& Database){
        userInfo add(name, surname, email, salary);
        cout << "pushback" << endl;
        Database.push_back(add);
    };

};

#ifndef __PROGTEST__
int main ( )
{
  string outName, outSurname;
  int lo, hi;

  CPersonalAgenda b1;
  assert ( b1 . add ( "John", "Smith", "john", 30000 ) );
  b1.printNameSort();
  assert ( b1 . add ( "John", "Miller", "johnm", 35000 ) );
  b1.printNameSort();
  assert ( b1 . add ( "Peter", "Smith", "peter", 23000 ) );
  b1.printNameSort();
  assert ( b1 . add ( "Peter", "Smith", "petera", 23000 ) == false);
  assert ( b1 . add ( "Peter", "Dickens", "peter", 23000 ) == false);

  cout << endl;
  assert ( b1 . add ( "Peter", "Dickens", "petera", 23000 ) );
  b1.printNameSort();
  assert ( b1 . del ( "Peter", "Dickens") );

  cout << endl;
  assert ( b1 . add ( "Peter", "Dickens", "petera", 23000 ) );
  assert ( b1 . del ( "petera") );

  b1.printNameSort();

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
    b1.printNameSort();
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
    b1.printNameSort();
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
    string sus = "deez", amogus = "nuts";
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
    assert (  b4 . add ("AAA", "AAA", "aaa", 121));    b4.printNameSort();
    assert (  b4 . getFirst(amogus, sus) && amogus == "AAA" && sus == "AAA");   b4.printNameSort();
    assert (  b4 . add ("AAA", "BBB", "bbb", 122));    b4.printNameSort();
    assert (  b4 . add ("BBB", "AAA", "ccc", 123));    b4.printNameSort();
    assert (  b4 . add ("CCC", "AAA", "ddd", 124));    b4.printNameSort();
    assert (  b4 . add ("DDD", "AAA", "eee", 125));    b4.printNameSort();
    assert (  b4 . add ("EEE", "AAA", "fff", 126));    b4.printNameSort();
    assert (  b4 . add ("AAA", "CCC", "ggg", 127));    b4.printNameSort();
    assert (  b4 . add ("AAA", "DDD", "hhh", 128));    b4.printNameSort();
    assert (  b4 . add ("BBB", "EEE", "iii", 129));    b4.printNameSort();
    assert (  b4 . add ("AAA", "FFF", "jjj", 120));    b4.printNameSort();
    assert ( !b4 . add ("AAA", "AAA", "kkk", 130));    b4.printNameSort();
    assert ( !b4 . add ("AAA", "PPP", "jjj", 131));    b4.printNameSort();
    assert (  b4 . add ("", "PPP", "lll", 131));    b4.printNameSort();
    assert (  b4 . add ("AAA", "", "mmm", 131));    b4.printNameSort();
    assert ( !b4 . add ("AAA", "", "nnn", 131));    b4.printNameSort();
    assert (  b4 . add ("AAA", "OOO", "", 131));    b4.printNameSort();
    assert (  b4 . add ("AAA", "QQQ", "ooo", -1));    b4.printNameSort();
    assert (  b4 . getSalary("AAA", "AAA") == 121);
    assert (  b4 . getSalary("AAA", "BBB") == 122);
    assert (  b4 . getSalary("", "PPP") == 131);
    assert (  b4 . getSalary("") == 131);
    assert (  b4 .setSalary("", "PPP", 1420));
    assert (  b4 .setSalary("", 1420));
    assert (  b4 . getSalary("", "PPP") == 1420);
    assert (  b4 . getSalary("") == 1420); cout << "salary" << endl; b4.printNameSort();
    assert ( !b4 .changeName("hhh", "AAA", "DDD")); b4.printNameSort();
    assert (  b4 .changeName("hhh", "BBB", "DDD")); b4.printNameSort();
    assert (  b4 .changeName("hhh", "", "DDD")); b4.printNameSort();
    assert (  b4 .changeName("hhh", "BBB", "")); b4.printNameSort();
    assert ( !b4 .changeEmail("DDD", "AAA", "ddd")); b4.printEmailSort();
    assert (  b4 .changeEmail("DDD", "AAA", "zzz")); b4.printEmailSort();
    assert (  b4 .changeEmail("AAA", "OOO", "yyy")); b4.printEmailSort();
    assert (  b4 .changeEmail("DDD", "AAA", "")); b4.printEmailSort();
    assert (  b4 . add ("CCC", "EEE", "ppp", 200));    b4.printNameSort();
    assert (  b4 . add ("AAA", "EEE", "sss", 200));    b4.printNameSort();
    b4.printEmailSort();
    assert (  b4 . getSalary("DDD", "AAA") == 125);
    assert (  b4 . getSalary("") == 125);
    assert (  b4 . getSalary("BBB", "") == 128);
    assert (  b4 . getSalary("hhh") == 128); b4.printNameSort();
    assert (  b4 . getFirst(amogus, sus) && amogus == "AAA" && sus.empty());
    assert (  b4 . getNext("BBB", "", amogus, sus) && amogus == "AAA" && sus == "AAA");
    assert (  b4 . getNext("DDD", "AAA", amogus, sus) && amogus == "EEE" && sus == "AAA");


    assert (  b4 . add ("AAA", "SSS", "qqq", 220));    b4.printNameSort();
    assert (  b4 . changeName ("hhh", "AAA", "TTT"));    b4.printNameSort();
    assert (  b4 . changeName ("hhh", "", ""));    b4.printNameSort();
    assert (  b4 . getSalary("AAA", "AAA") == 121);
    assert (  b4 . getSalary("AAA", "SSS") == 220);
    assert (  b4 . getSalary("", "PPP") == 1420); b4.printEmailSort();
    assert (  b4 . getSalary("") == 125);
    assert (  b4 .setSalary("", "PPP", 220));
    assert (  b4 .setSalary("", 220));
    assert (  b4 . getSalary("", "PPP") == 220);
    b4.printNameSort();
    assert (  b4 . getFirst(amogus, sus) && amogus.empty() && sus.empty());
    assert ( !b4 . getNext("AAA", "SSS", amogus, sus));
    assert (  b4 . getNext("AAA", "BBB", amogus, sus) && amogus == "AAA" && sus == "CCC");
    assert (  b4 . getNext("", "", amogus, sus) && amogus == "AAA" && sus.empty());
    b4.printNameSort();

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
