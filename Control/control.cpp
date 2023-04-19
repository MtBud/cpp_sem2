#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iterator>
#include <functional>
using namespace std;

class CDate{
  public:
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
    CDate( int y, int m, int d ): m_Year (y), m_Month (m), m_Day (d){}
#pragma clang diagnostic pop

    int compare ( const CDate& x ) const{
      if ( m_Year != x . m_Year )
        return m_Year - x . m_Year;
      if ( m_Month != x . m_Month )
        return m_Month - x . m_Month;
      return m_Day - x . m_Day;
    }

    int year () const{
      return m_Year;
    }

    int month () const{
      return m_Month;
    }

    int day () const{
      return m_Day;
    }

    friend ostream& operator << ( ostream& os, const CDate& x ){
      char oldFill = os . fill ();
      return os << setfill ( '0' ) << setw ( 4 ) << x . m_Year << "-"
                                     << setw ( 2 ) << static_cast<int> ( x . m_Month ) << "-"
                                     << setw ( 2 ) << static_cast<int> ( x . m_Day )
                << setfill ( oldFill );
    }

  private:
    int16_t m_Year;
    int8_t m_Month;
    int8_t m_Day;
};
#endif /* __PROGTEST__ */

string trimWhitespace(const string& str){
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first){
        string empty;
        return empty;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

string removeWhitespace(const string &input, string &output){
    unique_copy (input.begin(), input.end(), back_insert_iterator<string>(output),
                 [](char a,char b){ return isspace(a) && isspace(b);});
    return output;
}

string formatName ( string name ){
    // capitalize all letters
    transform( name.begin(), name.end(), name.begin(), [](unsigned char c) { return toupper(c); });
    // remove whitespace
    name = trimWhitespace(name);
    string tmp;
    name = removeWhitespace(name, tmp );
    return name;
}


// reprezentuje jednu fakturu
class CInvoice{
    CDate m_date;
    string m_seller;
    string m_buyer;
    string m_sellerF;
    string m_buyerF;
    unsigned int m_amount;
    double m_vat;
public:
    unsigned long long int m_ID;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-pass-by-value"
    /*    inicializace fakturu datem, jménem prodávající a kupující firmy, fakturovanou částkou a DPH.*/
    CInvoice ( const CDate& date, const string& seller, const string& buyer,
               unsigned int amount, double vat ):m_date(date), m_seller(seller), m_buyer(buyer), m_amount(amount), m_vat(vat), m_ID(0){
        m_sellerF = formatName(m_seller);
        m_buyerF = formatName(m_buyer);
    };
#pragma clang diagnostic pop

    /*date, seller, buyer, amount, vat -- přístupové metody ke čtení jednotlivých složek faktury.*/
    CDate date () const{
        return m_date;
    };
    string buyer () const{
        return m_buyer;
    };
    string seller () const{
        return m_seller;
    };
    unsigned int amount () const{
        return m_amount;
    };
    double vat () const{
        return m_vat;
    };
    string buyerF () const{
        return m_buyerF;
    };
    string sellerF () const{
        return m_sellerF;
    };

    void flip(){
        string tmp;
        tmp = m_buyer;
        m_buyer = m_seller;
        m_seller = tmp;
        tmp = m_buyerF;
        m_buyerF = m_sellerF;
        m_sellerF = tmp;
    }

    void print() const{
        cout    << "ID: " << m_ID
                << " Date: " << m_date
                << " Buyer: " << m_buyer
                << " Seller: " << m_seller
                << " Amount: " << m_amount
                << " VAT: " << m_vat<< endl;
    }

    bool operator == ( const CInvoice& rhs ){
        if (m_date.compare(rhs.m_date) != 0 ||
            m_buyerF != rhs.m_buyerF ||
            m_sellerF != rhs.m_sellerF ||
            m_amount != rhs.m_amount ||
            m_vat != rhs.m_vat)
            return false;
        else
            return true;
    };
private:
    friend string trimWhitespace(const string& str);
    friend string removeWhitespace(const string &input, string &output);
    friend string formatName ( string name );
};

struct CInvoiceCompare{
    bool operator()(const CInvoice& lhs, const CInvoice& rhs) const{
        if (lhs.buyerF() != rhs.buyerF())
            return lhs.buyerF() < rhs.buyerF();
        if (lhs.sellerF() != rhs.sellerF())
            return lhs.sellerF() < rhs.sellerF();
        if (lhs.date().compare(rhs.date()) != 0){
            if(lhs.date().compare(rhs.date()) > 0)
                return false;
            else
                return true;
        }
        if (lhs.amount() != rhs.amount())
            return lhs.amount() < rhs.amount();
        if (lhs.vat() != rhs.vat())
            return lhs.vat() < rhs.vat();
        return false;
    }
};

/*Třída CSortOpt určuje kritéria pro řazení. Pro řazení lze použít všechny složky faktury. Pokud například vytvoříme instanci:
 * CSortOpt () . addKey ( CSortOpt::BY_AMOUNT, true ) . addKey ( CSortOpt::BY_SELLER, false )
 * pak se řadí podle fakturované částky vzestupně (první řadicí kritérium) a pro stejné hodnoty fakturované částky
 * se použije řazení podle jména prodávajícího sestupně (druhé řadicí kritérium). Pokud by ani takto nebylo pořadí
 * jednoznačně určené, použije se jako řadicí kritérium pořadí zavedení faktury do registru. Rozhraní třídy CSortOpt je:*/
struct CSortKey{
    int m_key;
    bool m_ascending;
    CSortKey( int key, bool ascending ): m_key(key), m_ascending(ascending){};
};

class CSortOpt{
    vector <CSortKey> m_keys;
  public:
    static const int BY_DATE = 0;
    static const int BY_BUYER = 1;
    static const int BY_SELLER = 2;
    static const int BY_AMOUNT = 3;
    static const int BY_VAT = 4;
    CSortOpt () = default;
    CSortOpt& addKey ( int key, bool ascending = true ){
        CSortKey newKey(key, ascending);
        m_keys.push_back(newKey);
        return *this;
    };
};


class CCounter{
    unsigned long long int m_counter;
public:
    CCounter ():m_counter(0){};
    unsigned long long int operator ()(){
        m_counter ++;
        return m_counter;
    }
};

// company class, maybe useful later
/*
class CCompany{
    string m_name;
public:
    set <unsigned long long int> m_keys;

    explicit CCompany(string name):m_name(std::move(name)){};

    string name () const{
        return m_name;
    }
};

struct CCompanyCompare{
    bool operator()(const CCompany& lhs, const CCompany& rhs) const{
        if (lhs.name() != rhs.name())
            return lhs.name() < rhs.name();
        return false;
    }
};
*/

/* containers:
 * set - all accepted invoices -> sorted for easy serach
 * set - all issued   invoices -> sorted for easy serach
 * map - all solo invoices with IDs
 * set - companies with keys to the solo invoices
*/
class CVATRegister{
    CCounter m_IDmaker;
    map <string, set<unsigned long long int>> m_companies;  // name of company mapped to set of IDs of solo Invoices
    set <CInvoice, CInvoiceCompare> m_accepted;             // set of all accepted invoices
    set <CInvoice, CInvoiceCompare> m_issued;               // set of all issued invoices
    map <unsigned long long int, CInvoice> m_solos;         // ID mapped to its corresponding solo invoice

  public:
    //inicializuje prázdnou instanci registru,
    CVATRegister () = default;

    /*metoda zavede zadanou firmu do registru. Předané jméno je oficiální název firmy, toto jméno bude používané v
     * exportech z registru. Návratovou hodnotou je indikátor úspěchu (true)/neúspěchu (false). Za neúspěch považujte,
     * pokud v registru již existuje firma stejného jména. Při porovnávání jména firmy je registr docela tolerantní:

    při porovnávání nerozlišuje malá a velká písmena,
    při porovnávání neuvažuje nadbytečné mezery.

    Tato pravidla jsou používána při zakládání nové firmy i vkládání / mazání faktur. Například názvy "My Company",
     "mY CoMpAnY", " My Company " a " mY CoMPAnY " jsou považované za jednu firmu, ale názvy "My Company" a "MyCompany" označují dvě rozdílné firmy.*/
    // edge cases: empty name, only whitespace name
    bool registerCompany ( const string& name ){
        string tmp;
        tmp = formatName(name);
        set<unsigned long long int> keys;
        return m_companies.insert(make_pair(tmp, keys)).second;
    };

    /*metoda přidá fakturu do registru. Tuto metodu volá firma, která fakturu vydala. Návratovou hodnotou je příznak
     * úspěch (true)/neúspěch (false). Za chybu je považováno, pokud prodávající / kupující ve faktuře nejsou
     * registrované, prodávající a kupující je ta samá firma nebo pokud stejná faktura již byla pomocí addIssued
     * zadaná (dvě faktury se musí lišit alespoň v jednom z: prodávající/kupující/datum/částka/DPH).*/
    /*
    * add invoice to the list
    * check if it has complementary invoice
    *      if yes, delete complementary solo invoice from map and key from company class
    *      if not, add solo invoice to the map and add key to the class*/
    bool addIssued ( const CInvoice& newInv ){
        return addInvoice(m_issued, m_accepted, newInv);
    };


    /*    metoda přidá fakturu do registru, tuto metodu volá firma, která fakturu přijala (kupující).
     * Jinak se metoda chová stejně jako addIssued.*/
    bool addAccepted ( const CInvoice& newInv ){
        return addInvoice(m_accepted, m_issued, newInv);
    };

    /*metoda odebere fakturu z registru. Tuto metodu volá firma, která fakturu vydala a dříve zaregistrovala.
     * Návratovou hodnotou je příznak úspěch (true)/neúspěch (false). Za chybu je považováno, pokud identická
     * faktura nebyla dříve registrovaná metodou addIssued.*/
    bool delIssued ( const CInvoice& newInv ){
        return removeInvoice(m_issued, m_accepted, newInv);
    };

    /*metoda odebere fakturu z registru. Tuto metodu volá firma, která fakturu přijala a dříve zaregistrovala.
     * Návratovou hodnotou je příznak úspěch (true)/neúspěch (false). Za chybu je považováno, pokud identická
     * faktura nebyla dříve registrovaná metodou addAccepted.*/
    bool delAccepted ( const CInvoice& newInv ){
        return removeInvoice(m_accepted, m_issued, newInv);
    };

    /*metoda nalezne všechny faktury, které se týkají zadané firmy company a nebyly spárované (tedy byly registrované
     * pouze pomocí addIssued nebo pouze pomocí addAccepted). Metoda vrátí seznam těchto faktur, faktury budou
     * seřazené podle kritérií udaných sortOpt. Faktury vrácené touto metodou budou mít na místě názvu firmy
     * "oficiální" název, tedy ten název, který byl zadán při registraci firmy metodou registerCompany.
     * Tento oficiální název bude rovněž použit při řazení.*/
    list<CInvoice> unmatched ( const string& company, const CSortOpt& sortBy ) const{
        string name = formatName(company);
        list<CInvoice>
    };

    void printCompanies() const{
        for(const auto& i : m_companies)
            cout << i.first << endl;
    }

    void printSolo() const{
        for(const auto& i : m_solos)
            i.second.print();
    }

  private:
    bool checkInvoiceValidity( const CInvoice& newInv, set<CInvoice, CInvoiceCompare>& invRegister) {
        if ( newInv.buyerF() == newInv.sellerF() )
            return false;
        if ( m_companies.find(newInv.buyerF()) == m_companies.end() || m_companies.find(newInv.sellerF()) == m_companies.end() )
            return false;
        if ( invRegister.find(newInv) != invRegister.end() )
            return false;
        return true;
    }

    void addNewSolo( CInvoice& newInv){
        m_solos.insert( make_pair(newInv.m_ID, newInv));
        auto keys = m_companies[newInv.buyerF()];
        keys.insert(newInv.m_ID);
        keys = m_companies[newInv.sellerF()];
        keys.insert(newInv.m_ID);
    }

    void removeSolo( CInvoice& remInv ){
        m_solos.erase(remInv.m_ID);
        auto keys = m_companies[remInv.buyerF()];
        keys.erase(remInv.m_ID);
        keys = m_companies[remInv.sellerF()];
        keys.erase(remInv.m_ID);
    }

    bool addInvoice( set <CInvoice, CInvoiceCompare>& primary, set <CInvoice, CInvoiceCompare>& secondary, const CInvoice& newInv ){
        if ( ! checkInvoiceValidity(newInv, primary) )
            return false;

        CInvoice newCopy = newInv;
        newCopy.flip();
        // check if secondary has the complementary invoice
        auto compInvoice = secondary.find(newCopy);
        if ( compInvoice == secondary.end() ){
            // if not, add solo invoice to m_solos and add key to m_companies
            newCopy.flip();
            newCopy.m_ID = m_IDmaker();
            addNewSolo( newCopy );
        }
        else{
            newCopy.m_ID = compInvoice->m_ID;
            removeSolo(newCopy);
            newCopy.flip();
        }
        primary.insert(newCopy);
        return true;
    }

    bool removeInvoice( set <CInvoice, CInvoiceCompare>& primary, set <CInvoice, CInvoiceCompare>& secondary, const CInvoice& newInv ){
        if ( primary.find(newInv) == primary.end() )
            return false;

        CInvoice newCopy = newInv;
        newCopy.flip();
        // check if secondary has the complementary invoice
        if ( secondary.find(newCopy) == secondary.end() ){
            // if not, add solo invoice to m_solos and add key to m_companies
            newCopy.flip();
            removeSolo(newCopy);
        }
        else{
            addNewSolo( newCopy );
            newCopy.flip();
        }
        primary.erase(newCopy);
        return true;
    }

    friend string trimWhitespace(const string& str);
    friend string removeWhitespace(const string &input, string &output);
    friend string formatName ( string name );
};



#ifndef __PROGTEST__
bool equalLists ( const list<CInvoice> & a, const list<CInvoice> & b ){
    auto iterA = a.begin();
    auto iterB = b.begin();
    for( int i = 0; i < a.size(); i ++){
        if( ! (iterA == iterB) )
            return false;
        iterA ++;
        iterB ++;
    }
    return true;
}

int main ()
{
    CVATRegister r;
    assert ( r . registerCompany ( "first Company" ) );
    assert ( r . registerCompany ( "Second     Company" ) );
    assert ( r . registerCompany ( "ThirdCompany, Ltd." ) );
    assert ( r . registerCompany ( "Third Company, Ltd." ) );
    assert ( r . registerCompany ( "" ) );
    assert ( r . registerCompany ( "A" ) );
    assert ( !r . registerCompany ( "           " ) );
    assert ( !r . registerCompany ( "" ) );
    assert ( !r . registerCompany ( " " ) );
    assert ( !r . registerCompany ( "Third Company, Ltd." ) );
    assert ( !r . registerCompany ( " Third  Company,  Ltd.  " ) );

    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 20 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "FirSt Company", "Second Company ", 200, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );

    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "", "Second Company ", 100, 20 ) ) );
    assert ( r . addAccepted( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", " ", 100, 20 ) ) );

    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A", "Second Company ", 100, 20 ) ) );
    assert ( r . addAccepted( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", "A", 100, 20 ) ) );

    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", "First Company", 300, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34 ) ) );

    assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
    assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "First Company", "First   Company", 200, 30 ) ) );
    assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "Another Company", "First   Company", 200, 30 ) ) );
    r.printCompanies();
    r.printSolo();
    /*
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, false ) . addKey ( CSortOpt::BY_DATE, false ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
             } ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 )
             } ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) . addKey ( CSortOpt::BY_AMOUNT, true ) . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
             } ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
             } ) );
    assert ( equalLists ( r . unmatched ( "second company", CSortOpt () . addKey ( CSortOpt::BY_DATE, false ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Third Company, Ltd.", "Second     Company", 400, 34.000000 )
             } ) );
    assert ( equalLists ( r . unmatched ( "last company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) ),
             list<CInvoice>
             {
             } ) );
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company ", 200, 30 ) ) );
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "Second company ", "First Company", 300, 32 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
             } ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2001, 1, 1 ), "First Company", "Second Company ", 200, 30 ) ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A First Company", "Second Company ", 200, 30 ) ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Hand", 200, 30 ) ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 1200, 30 ) ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 200, 130 ) ) );
    assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
             } ) );
    assert ( r . delAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
             } ) );
    assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
             } ) );
             */
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
