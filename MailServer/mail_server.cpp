#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
//#include <cstdio>
#include <cassert>
//#include <cctype>
//#include <cmath>
#include <iostream>
//#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */

/*email adresses
 * v úloze mohou být libovolné řetězce, při jejich porovnávání rozlišujeme malá a velká písmena (case sensitive)
 * - v tomto se úloha liší od reálného světa, kde e-mailové adresy mají předepsaný formální tvar a kde se malá
 * a velká písmena zpravidla nerozlišují.*/

class CustomStr{
    char*  m_String;
    size_t m_length;
    size_t m_capacity;
public:
    CustomStr():m_String(nullptr), m_length(0), m_capacity(0){};

    explicit CustomStr(const char* str){
        m_length = strlen(str);
        m_capacity = m_length + 1;
        m_String = new char [m_capacity];
        memcpy( m_String, str, m_capacity );
    };

    ~CustomStr(){
        //cout << m_String << endl;
        delete [] m_String;
    };

    CustomStr(const CustomStr& src ): m_String(new char [src.m_length + 1]),
                                      m_length(src.m_length),
                                      m_capacity(src.m_capacity){
        memcpy( m_String, src . m_String, m_length + 1 );
    }

    CustomStr& operator = ( const CustomStr& rhs){
        if ( &rhs != this )
        {
            delete [] m_String;
            m_capacity   = rhs.m_capacity;
            m_length   = rhs.m_length;
            m_String  = new char [m_capacity];
            memcpy( m_String, rhs.m_String, m_length + 1 );
        }
        return *this;
    };

    operator const char* () const{
        return m_String;
    };

    bool operator == (const CustomStr& rhs) const{
        if(strcmp(m_String, rhs.m_String) == 0)
            return true;
        else
            return false;
    };

private:
};

//------------------------------------------------------------------------------------------------------------

class CMail{
    CustomStr m_from;
    CustomStr m_to;
    CustomStr m_body;
  public:
    /*konstruktor:
      vytvoří instanci e-mailu se složkami from/to/body vyplněnými podle parametrů. Můžete předpokládat,
      že e-mailové adresy jsou relativně krátké (desítky až stovky znaků) a že tělo zprávy může být
      relativně dlouhé (i několik megabyte)*/
    CMail()= default;

    CMail( const char * from, const char * to, const char * body ):
           m_from(from), m_to(to), m_body(body){
    };

    CMail& operator = (const CMail& rhs){
        if ( &rhs != this )
        {
            m_from = rhs.m_from;
            m_to = rhs.m_to;
            m_body = rhs.m_body;
        }
        return *this;
    };
    /*operator ==
      porovná obsah dvou instancí CMail, metoda vrací true, pokud jsou instance
      identické (shodují se všechny složky from, to i obsah e-mailu).*/
    bool operator == ( const CMail& rhs ) const{
        if( strcmp(m_from, rhs.m_from) != 0 )
            return false;
        if( strcmp(m_to, rhs.m_to) != 0 )
            return false;
        if( strcmp(m_to, rhs.m_to) != 0 )
            return false;
        return true;
    };

    CustomStr From() const{
        return m_from;
    }

    CustomStr To() const{
        return m_to;
    }

    /*operator <<
     *     zobrazí informace o mailu do zadaného streamu. Formát je zřejmý z ukázky.*/
    friend ostream& operator << ( ostream& out, const CMail& outMail );

  private:
    // todo
};

// format: "From: alice, To: john, Body: deadline confirmation"
ostream& operator << ( ostream& out, const CMail& outMail ){
    out << "From: " << outMail.m_from
        << ", To: " << outMail.m_to
        << ", Body: " << outMail.m_body;
    return out;
}

//-----------------------------------------------------------------------------------------------------------------

class CUser{
    size_t sentCap;
    size_t recievedCap;
public:
    CustomStr m_name;
    int* sent;
    int* recieved;
    size_t sentLen;
    size_t recievedLen;

    CUser():sentCap(0), recievedCap(0), m_name(), sent(nullptr), recieved(nullptr), sentLen(0), recievedLen(0){};

    explicit CUser(const CustomStr& name):sentCap(5), recievedCap(5), sentLen(0), recievedLen(0){
        sent = new int [sentCap];
        recieved = new int [recievedCap];
        m_name = name;
    };

    ~CUser(){
        delete [] sent;
        delete [] recieved;
    }

    CUser& operator = (const CUser& rhs){
        if ( &rhs != this )
        {
            delete [] sent;
            delete [] recieved;
            sentLen = rhs.sentLen;
            sentCap = rhs.sentCap;
            recievedLen = rhs.recievedLen;
            recievedCap = rhs.recievedCap;
            m_name = rhs.m_name;
            sent = new int[sentCap];
            recieved = new int[recievedCap];
            memcpy(sent, rhs.sent, sizeof(int) * sentLen);
            memcpy(recieved, rhs.recieved, sizeof(int) * recievedLen);
            //for(size_t i = 0; i < rhs.sentLen; i++)
            //    sent[i] = rhs.sent[i];
            //for(size_t i = 0; i < rhs.recievedLen; i++)
            //    recieved[i] = rhs.recieved[i];
        }
        return *this;
    };

    void sentAdd(const int idx){
        if(sentLen == sentCap)
            realocate(sentCap, sentLen, sent);
        sent[sentLen] = idx;
        sentLen ++;
    }

    void recievedAdd(const int idx){
        if(recievedLen == recievedCap)
            realocate(recievedCap, recievedLen, recieved);
        recieved[recievedLen] = idx;
        recievedLen ++;
    }

    static void realocate ( size_t& cap, const size_t& len, int*& arr){
        int* tmp;
        cap *= 1.5;
        tmp = new int[cap];
        for(size_t i = 0; i < len; i++)
            tmp[i] = arr[i];
        delete [] arr;
        arr = tmp;
    }

    void printUserSentIndexes() const{
        cout << "PRINT USER SENT INDEXES" << endl;
        for(size_t i = 0; i < sentLen; i++)
            cout << '\t' << sent[i] << endl;
        cout << endl;
    };

    void printUserRecievedIndexes() const{
        cout << "PRINT USER RECIEVED INDEXES" << endl;
        for(size_t i = 0; i < recievedLen; i++)
            cout << '\t' << recieved[i] << endl;
        cout << endl;
    };
};

//-----------------------------------------------------------------------------------------------------------------

class CMailIterator{
    CMail* m_mailSnap;
    size_t m_mailLen;
    int* m_idxArr;
    size_t m_idx;
    size_t m_len;
  public:
    CMailIterator():m_mailSnap(nullptr), m_mailLen(0), m_idxArr(nullptr), m_idx(0), m_len(0){};

    explicit CMailIterator( const int idxArr[], const size_t len, const CMail mailList[], const size_t mailLen)
                           :m_mailLen(mailLen), m_idx(0), m_len(len){
        m_idxArr = new int[m_len + 1];
        for(size_t i = 0; i < m_len; i++)
            m_idxArr[i] = idxArr[i];
        m_mailSnap = new CMail[mailLen];
        for(size_t i = 0; i < mailLen; i++){
            m_mailSnap[i] = mailList[i];
        }
    };

    CMailIterator( const CMailIterator& src):m_mailLen(src.m_mailLen), m_idx(src.m_idx), m_len(src.m_len){
        m_mailSnap = new CMail[m_mailLen];
        m_idxArr = new int[m_len];
        for(size_t i = 0; i < m_mailLen; i ++)
            m_mailSnap[i] = src.m_mailSnap[i];
        for(size_t i = 0; i < m_len; i ++)
            m_idxArr[i] = src.m_idxArr[i];
    }

    ~CMailIterator(){
        delete [] m_mailSnap;
        delete [] m_idxArr;
    };

    CMailIterator& operator = ( const CMailIterator& src){
        if (&src != this){
            delete [] m_mailSnap;
            delete [] m_idxArr;
            m_mailLen = src.m_mailLen;
            m_idx = src.m_idx;
            m_len = src.m_len;
            m_mailSnap = new CMail[m_mailLen];
            m_idxArr = new int[m_len];
            for(size_t i = 0; i < m_mailLen; i ++)
                m_mailSnap[i] = src.m_mailSnap[i];
            for(size_t i = 0; i < m_len; i ++)
                m_idxArr[i] = src.m_idxArr[i];
        }
        return *this;
    }

    explicit operator bool () const{
        if(m_idx < m_len)
            return true;
        else
            return false;
    };

    bool operator ! () const{
        if(m_idx < m_len)
            return false;
        else
            return true;
    };

    const CMail& operator * () const{
        return m_mailSnap[m_idxArr[m_idx]];
    };

    CMailIterator& operator ++ (){
        m_idx ++;
        return *this;
    };

    void printIteratorIndexes() const{
        cout << "PRINT ITERATOR INDEXES" << endl;
        for(size_t i = 0; i < m_len; i++)
            cout << m_idxArr[i] << endl;
        cout << endl;
    };

    void printCurrentSpot() const{
        cout << "PRINT CURRENT ITERATION" << endl;
        cout << m_idx << endl;
        cout << m_mailSnap[m_idxArr[0]] << endl;
        cout << endl;
    }
  private:
    // todo
};

//------------------------------------------------------------------------------------------------------------------

class CMailServer{
    CMail* m_MailList;
    size_t m_length;
    size_t m_capacity;
    CUser* m_Users;
    size_t m_Ulength;
    size_t m_Ucapacity;

  public:

    CMailServer():m_length(0),m_capacity(2), m_Ulength(0), m_Ucapacity(2){
        m_MailList = new CMail[2];
        m_Users = new CUser[2];
    };

    CMailServer ( const CMailServer& src ):m_length(src.m_length), m_capacity(src.m_capacity),
                                           m_Ulength(src.m_Ulength), m_Ucapacity(src.m_Ucapacity){
        m_MailList = new CMail[m_capacity];
        m_Users = new CUser[m_Ucapacity];
        for(size_t i = 0; i < m_length; i++)
            m_MailList[i] = src.m_MailList[i];
        for(size_t i = 0; i < m_Ulength; i++)
            m_Users[i] = src.m_Users[i];
    };

    CMailServer& operator = ( const CMailServer& src ){
        if ( &src != this ) {
            delete[] m_MailList;
            delete [] m_Users;
            m_length = src.m_length;
            m_capacity = src.m_capacity;
            m_Ulength = src.m_Ulength;
            m_Ucapacity = src.m_Ucapacity;
            m_MailList = new CMail[m_capacity];
            m_Users = new CUser[m_Ucapacity];
            for (size_t i = 0; i < m_length; i++)
                m_MailList[i] = src.m_MailList[i];
            for (size_t i = 0; i < m_Ulength; i++)
                m_Users[i] = src.m_Users[i];
        }
        return *this;
    };

    ~CMailServer (){
        delete [] m_MailList;
        delete [] m_Users;
    };

    void sendMail ( const CMail& m ){
        if(m_length == m_capacity)
            realocateMail();
        m_MailList[m_length] = m;
        m_length ++;

        int fromPos = findUsersPos(m.From());
        if(fromPos == -1) {
            addUser(m.From());
            fromPos = (int)m_Ulength - 1;
        }
        int toPos = findUsersPos(m.To());
        if(toPos == -1) {
            addUser(m.To());
            toPos = (int)m_Ulength - 1;
        }
        m_Users[fromPos].sentAdd((int)m_length-1);
        m_Users[toPos].recievedAdd((int)m_length - 1);
    };

    CMailIterator outbox ( const char* email ) const{
        CustomStr emailStr(email);
        int pos = findUsersPos(emailStr);
        CUser tmp;
        if(pos == -1) {
            CMailIterator iterOut;
            return iterOut;
        }
        tmp = m_Users[pos];
        CMailIterator iterOut(tmp.sent, tmp.sentLen, m_MailList, m_length);
        return iterOut;
    };

    CMailIterator inbox ( const char* email ) const{
        CustomStr emailStr(email);
        int pos = findUsersPos(emailStr);
        CUser tmp;
        if(pos == -1) {
            CMailIterator iterOut;
            return iterOut;
        }
        tmp = m_Users[pos];
        CMailIterator iterOut(tmp.recieved, tmp.recievedLen, m_MailList, m_length);
        return iterOut;
    };

    void print() const{
        cout << "PRINT MAIL LIST" << endl;
        for(size_t i = 0; i < m_length; i++)
            cout << i <<"  " << m_MailList[i] << endl;
        cout << endl;
    }

    void printUsers() const{
        cout << "PRINT USERS" << endl;
        for(size_t i = 0; i < m_Ulength; i++){
            cout << m_Users[i].m_name <<endl;
            //m_Users[i].printUserSentIndexes();
            //m_Users[i].printUserRecievedIndexes();
        }
        cout << endl;
    }

  private:
    void realocateMail(){
        CMail* tmp;
        m_capacity *= 1.5;
        tmp = new CMail[m_capacity];
        for(size_t i = 0; i < m_length; i++)
            tmp[i] = m_MailList[i];
        delete [] m_MailList;
        m_MailList = tmp;
    }

    void realocateUsers(){
        CUser* tmp;
        m_Ucapacity *= 1.5;
        tmp = new CUser[m_Ucapacity];
        for(size_t i = 0; i < m_Ulength; i++)
            tmp[i] = m_Users[i];
        delete [] m_Users;
        m_Users = tmp;
    }

    int findUsersPos(const CustomStr& name) const{
        for(size_t i = 0; i < m_Ulength; i ++){
            if(m_Users[i].m_name == name){
                return (int) i;
            }
        }
        return -1;
    }

    void addUser(const CustomStr& name){
        CUser newUser(name);
        if(m_Ulength == m_Ucapacity)
            realocateUsers();
        m_Users[m_Ulength] = newUser;
        m_Ulength ++;
    }
};

//--------------------------------------------------------------------------------------------------------------

#ifndef __PROGTEST__

bool matchOutput ( const CMail& m, const char* str ){
    ostringstream oss;
    oss << m;
    return oss . str () == str;
}

int main ()
{
  char from[100], to[100], body[1024];

  assert ( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "peter", "progtest deadline" ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "progtest deadline", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "john", "progtest deadline" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "progtest deadline", "john" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "john", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "peter", "john" ) ) );
  CMailServer s0;

  s0 . sendMail ( CMail ( "john", "peter", "some important mail" ) );
  strncpy ( from, "john", sizeof ( from ) );
  strncpy ( to, "thomas", sizeof ( to ) );
  strncpy ( body, "another important mail", sizeof ( body ) );
  s0 . sendMail ( CMail ( from, to, body ) );
  strncpy ( from, "john", sizeof ( from ) );
  strncpy ( to, "alice", sizeof ( to ) );
  strncpy ( body, "deadline notice", sizeof ( body ) );
  s0 . sendMail ( CMail ( from, to, body ) );
  s0 . sendMail ( CMail ( "alice", "john", "deadline confirmation" ) );
  s0 . sendMail ( CMail ( "peter", "alice", "PR bullshit" ) );
  s0.print();
  s0.printUsers();
  cout << "deez" << endl;
  CMail nuts("from", "to", "body");

// here
  CMailIterator i0 = s0 . inbox ( "alice" );
  i0.printIteratorIndexes();
  i0.printCurrentSpot();
  assert ( *i0 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( i0 && *i0 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i0,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i0 && *i0 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i0,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ! ++i0 );

  CMailIterator i1 = s0 . inbox ( "john" );
  assert ( i1 && *i1 == CMail ( "alice", "john", "deadline confirmation" ) );
  assert ( matchOutput ( *i1,  "From: alice, To: john, Body: deadline confirmation" ) );
  assert ( ! ++i1 );

  CMailIterator i2 = s0 . outbox ( "john" );
  assert ( i2 && *i2 == CMail ( "john", "peter", "some important mail" ) );
  assert ( matchOutput ( *i2,  "From: john, To: peter, Body: some important mail" ) );
  assert ( ++i2 && *i2 == CMail ( "john", "thomas", "another important mail" ) );
  assert ( matchOutput ( *i2,  "From: john, To: thomas, Body: another important mail" ) );
  assert ( ++i2 && *i2 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i2,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ! ++i2 );

  CMailIterator i3 = s0 . outbox ( "thomas" );
  assert ( ! i3 );

  CMailIterator i4 = s0 . outbox ( "steve" );
  assert ( ! i4 );

  CMailIterator i5 = s0 . outbox ( "thomas" );
  s0 . sendMail ( CMail ( "thomas", "boss", "daily report" ) );
  assert ( ! i5 );

  CMailIterator i6 = s0 . outbox ( "thomas" );
  assert ( i6 && *i6 == CMail ( "thomas", "boss", "daily report" ) );
  assert ( matchOutput ( *i6,  "From: thomas, To: boss, Body: daily report" ) );
  assert ( ! ++i6 );

  CMailIterator i7 = s0 . inbox ( "alice" );
  s0 . sendMail ( CMail ( "thomas", "alice", "meeting details" ) );
  assert ( i7 && *i7 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i7,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i7 && *i7 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i7,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ! ++i7 );

  CMailIterator i8 = s0 . inbox ( "alice" );
  assert ( i8 && *i8 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i8,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i8 && *i8 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i8,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i8 && *i8 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i8,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ! ++i8 );

  CMailServer s1 ( s0 );
  s0 . sendMail ( CMail ( "joe", "alice", "delivery details" ) );
  s1 . sendMail ( CMail ( "sam", "alice", "order confirmation" ) );
  CMailIterator i9 = s0 . inbox ( "alice" );
  assert ( i9 && *i9 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i9,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i9 && *i9 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i9,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i9 && *i9 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i9,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i9 && *i9 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( matchOutput ( *i9,  "From: joe, To: alice, Body: delivery details" ) );
  assert ( ! ++i9 );

  CMailIterator i10 = s1 . inbox ( "alice" );
  assert ( i10 && *i10 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i10,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i10 && *i10 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i10,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i10 && *i10 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i10,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i10 && *i10 == CMail ( "sam", "alice", "order confirmation" ) );
  assert ( matchOutput ( *i10,  "From: sam, To: alice, Body: order confirmation" ) );
  assert ( ! ++i10 );

  CMailServer s2;
  s2 . sendMail ( CMail ( "alice", "alice", "mailbox test" ) );
  CMailIterator i11 = s2 . inbox ( "alice" );
  assert ( i11 && *i11 == CMail ( "alice", "alice", "mailbox test" ) );
  assert ( matchOutput ( *i11,  "From: alice, To: alice, Body: mailbox test" ) );
  assert ( ! ++i11 );

  s2 = s0;
  s0 . sendMail ( CMail ( "steve", "alice", "newsletter" ) );
  s2 . sendMail ( CMail ( "paul", "alice", "invalid invoice" ) );
  CMailIterator i12 = s0 . inbox ( "alice" );
  assert ( i12 && *i12 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i12,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i12 && *i12 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i12,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i12 && *i12 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i12,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i12 && *i12 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( matchOutput ( *i12,  "From: joe, To: alice, Body: delivery details" ) );
  assert ( ++i12 && *i12 == CMail ( "steve", "alice", "newsletter" ) );
  assert ( matchOutput ( *i12,  "From: steve, To: alice, Body: newsletter" ) );
  assert ( ! ++i12 );

  CMailIterator i13 = s2 . inbox ( "alice" );
  assert ( i13 && *i13 == CMail ( "john", "alice", "deadline notice" ) );
  assert ( matchOutput ( *i13,  "From: john, To: alice, Body: deadline notice" ) );
  assert ( ++i13 && *i13 == CMail ( "peter", "alice", "PR bullshit" ) );
  assert ( matchOutput ( *i13,  "From: peter, To: alice, Body: PR bullshit" ) );
  assert ( ++i13 && *i13 == CMail ( "thomas", "alice", "meeting details" ) );
  assert ( matchOutput ( *i13,  "From: thomas, To: alice, Body: meeting details" ) );
  assert ( ++i13 && *i13 == CMail ( "joe", "alice", "delivery details" ) );
  assert ( matchOutput ( *i13,  "From: joe, To: alice, Body: delivery details" ) );
  assert ( ++i13 && *i13 == CMail ( "paul", "alice", "invalid invoice" ) );
  assert ( matchOutput ( *i13,  "From: paul, To: alice, Body: invalid invoice" ) );
  assert ( ! ++i13 );


  char* emailText = new char[200000];
  cin >> emailText;
  CMail longassmail("deez", "nuts", emailText);
  CMailServer original;
  original.sendMail(longassmail);
  original.sendMail(longassmail);
  original.sendMail(longassmail);
  original.sendMail(longassmail);
  original.sendMail(longassmail);
  original.sendMail(longassmail);

  CMailServer copy1 = original;
  copy1.sendMail(longassmail);

  CMailServer copy2 = copy1;
  copy2.sendMail(longassmail);

  CMailServer copy3 = copy2;
  //copy3.print();


    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
