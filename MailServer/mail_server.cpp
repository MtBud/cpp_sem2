#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
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
        m_String = new char [strlen(str) + 1];
        strcpy(m_String, str);
        m_length = strlen(m_String);
        m_capacity = m_length + 1;
    };

    ~CustomStr(){
        delete [] m_String;
    };

    CustomStr(const CustomStr& src ): m_String(new char [src.m_length + 1]),
                                      m_length(src.m_length),
                                      m_capacity(src.m_capacity){
        memcpy( m_String, src . m_String, m_length + 1 );
    }

    operator const char* () const{
        return m_String;
    }

    size_t len()const{
        return m_length;
    }
private:
};

//------------------------------------------------------------------------------------------------------------

class CMail
{
    CustomStr m_from;
    CustomStr m_to;
    CustomStr m_body;
  public:
    /*konstruktor:
      vytvoří instanci e-mailu se složkami from/to/body vyplněnými podle parametrů. Můžete předpokládat,
      že e-mailové adresy jsou relativně krátké (desítky až stovky znaků) a že tělo zprávy může být
      relativně dlouhé (i několik megabyte)*/
    CMail( const char * from, const char * to, const char * body ):
        m_from(from), m_to(to), m_body(body){};
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

class CMailIterator
        /*copy constructor, operator =, destructor
         *     podle způsobu implementace možná nebude postačovat automaticky generovaná varianta.
         *     Testovací prostředí iterátory nikde explicitně nekopíruje, ale ke kopírování dochází
         *     v okamžiku předávání návratové hodnoty metodami inbox a outbox.*/
{
  public:
    /*operator bool
     *     operátor zjistí, zda iterátor odkazuje na platný e-mail (vrací true), nebo zda dosáhl za poslední e-mail
     *     v seznamu (tedy e-mail už nelze číst, vrátí false),*/
    explicit operator bool ( void ) const;
    /*operator !
     *     funguje stejně jako předešlý operátor, pouze vrací opačnou návratovou hodnotu */
    bool operator ! ( void ) const;
    /*operator *
     * unární operátor * zpřístupní e-mail na aktuální pozici. Návratovou hodnotou je instance CMail
     * (případně konstantní reference na CMail). Nemusíte řešit situaci, že by se zpřístupnil e-mail za koncem
     * seznamu - testovací prostředí vždy nejprve kontroluje platnost iterátoru a teprve pak případně
     * zpřístupní odkazovaný e-mail.*/
    const CMail& operator * ( void ) const;
    /*prefixový operátor ++ zajistí přesun iterátoru na další e-mail v seznamu. E-maily jsou iterátorem procházené
     * v pořadí, ve kterém byly odeslané/přijaté. Opakovaným voláním tohoto iterátoru se lze přesunout od prvního
     * e-mailu přijatého/odeslaného zadanou e-mailovou adresou až k poslednímu
     * (pak musí operátor přetypování na bool vracet false).*/
    CMailIterator& operator ++ ( void );
  private:
    // todo
};

//------------------------------------------------------------------------------------------------------------------

class CMailServer 
{
  public:
    /*implicit constructor
     *     vytvoří prázdnou instanci */
    CMailServer();
    /*copy constructor / operator =
     *     vytvoří identické kopie instance podle standardních pravidel,*/
    CMailServer ( const CMailServer & src );
    CMailServer& operator =                    ( const CMailServer & src );
    /*destructor
     *     uvolní prostředky alokované instancí,*/
    ~CMailServer ( void );
    /*sendMail
     * zašle e-mail předaný v parametrech, efektivně jej zařadí do odpovídajících schránek odesílatele a příjemce.
     * E-mail je vždy zařazen na konec existujícího seznamu. Příjemce ani odesílatele není potřeba zakládat, schránka
     * se automaticky vytvoří po zpracování prvního e-mailu, který obsahuje danou e-mailovou adresu,*/
    void                     sendMail                      ( const CMail     & m );
    /*outbox
     * zpřístupní poštu odeslanou ze zadané adresy. Návratovou hodnotou je instance CMailIterator, která umožní
     * procházet emaily odeslané z adresy email. Pokud je zadaná neznámá e-mailová adresa, je výsledkem iterátor
     * pro prázdný seznam e-mailů. Vrácený iterátor musí zachycovat stav mailové schránky v okamžiku, kdy byl vytvořen.
     * Tedy pokud během používání iterátoru dojde ke změně obsahu procházené schránky, tato změna se do hodnot vracených
     * iterátorem nijak nepromítne. Toto chování je demonstrované v ukázkovém běhu např. pro iterátor i5.*/
    CMailIterator            outbox                        ( const char      * email ) const;
    /*inbox
     *     zpřístupní poštu přijatou na zadanou adresu. Jinak metoda pracuje stejně jako metoda outbox.*/
    CMailIterator            inbox                         ( const char      * email ) const;
  
  private:
    // todo
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
  //char from[100], to[100], body[1024];

  assert ( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "peter", "progtest deadline" ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "progtest deadline", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "john", "progtest deadline" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "progtest deadline", "john" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "john", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "peter", "john" ) ) );
  /*
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
  CMailIterator i0 = s0 . inbox ( "alice" );
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
  */
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
