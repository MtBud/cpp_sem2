#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
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
    CDate( int y, int m, int d ): m_Year (y), m_Month (m), m_Day (d){}

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
    /*    inicializace fakturu datem, jménem prodávající a kupující firmy, fakturovanou částkou a DPH.*/
    CInvoice ( const CDate& date, string  seller, string  buyer,
               unsigned int amount, double vat ):m_date(date), m_seller(std::move(seller)), m_buyer(std::move(buyer)), m_amount(amount), m_vat(vat), m_ID(0){
        m_sellerF = formatName(m_seller);
        m_buyerF = formatName(m_buyer);
    };

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
    void buyerSet ( string newBuyer){
        m_buyer = std::move(newBuyer);
    }
    void sellerSet ( string newSeller){
        m_seller = std::move(newSeller);
    }

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


    void print() const{
        cout    << "ID: " << m_ID
                << "\tDate: " << m_date
                << "\tSeller: " << m_seller
                << "\tBuyer: " << m_buyer
                << "\tAmount: " << m_amount
                << "\tVAT: " << m_vat<< endl;
    }

    bool operator == ( const CInvoice& rhs ) const{
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


struct CSortKey{
    int m_key;
    bool m_ascending;
    CSortKey( int key, bool ascending ): m_key(key), m_ascending(ascending){};
};

class CSortOpt{
  public:
    static const int BY_DATE = 0;
    static const int BY_BUYER = 1;
    static const int BY_SELLER = 2;
    static const int BY_AMOUNT = 3;
    static const int BY_VAT = 4;
    vector <CSortKey> m_keys;
    CSortOpt () = default;
    CSortOpt& addKey ( int key, bool ascending = true ){
        CSortKey newKey(key, ascending);
        if ( m_keys.size() == 5)
            return *this;
        for (auto & i : m_keys){
            if(i.m_key == newKey.m_key)
                return *this;
        }
        m_keys.push_back(newKey);
        return *this;
    };
};

struct CSortCompare{
    CSortOpt m_rules;
    explicit CSortCompare( CSortOpt  rules ):m_rules(std::move(rules)){};

    bool operator()(const CInvoice& lhs, const CInvoice& rhs) const{
        return compare(lhs, rhs);
    }

    bool compare(const CInvoice& lhs, const CInvoice& rhs) const{
        static unsigned int level = 0;
        if(level == m_rules.m_keys.size()){
            level = 0;
            return lhs.m_ID < rhs.m_ID;
        }
        switch (m_rules.m_keys[level].m_key){
            case 0:
                if (lhs.date().compare(rhs.date()) == 0){
                    level ++;
                    return compare(lhs, rhs);
                }
                else{
                    bool result = checkAsc(lhs.date().compare(rhs.date()) < 0, m_rules.m_keys[level].m_ascending);
                    level = 0;
                    return result;
                }

            case 1:
                if (lhs.buyerF() == rhs.buyerF()){
                    level ++;
                    return compare(lhs, rhs);
                }
                else{
                    bool result = checkAsc(lhs.buyerF() < rhs.buyerF(), m_rules.m_keys[level].m_ascending);
                    level = 0;
                    return result;
                }

            case 2:
                if (lhs.sellerF() == rhs.sellerF()){
                    level ++;
                    return compare(lhs, rhs);
                }
                else{
                    bool result = checkAsc(lhs.sellerF() < rhs.sellerF(), m_rules.m_keys[level].m_ascending);
                    level = 0;
                    return result;
                }

            case 3:
                if (lhs.amount() == rhs.amount()){
                    level ++;
                    return compare(lhs, rhs);
                }
                else{
                    bool result = checkAsc(lhs.amount() < rhs.amount(), m_rules.m_keys[level].m_ascending);
                    level = 0;
                    return result;
                }

            case 4:
                if (lhs.vat() == rhs.vat()){
                    level ++;
                    return compare(lhs, rhs);
                }
                else{
                    bool result = checkAsc(lhs.vat() < rhs.vat(), m_rules.m_keys[level].m_ascending);
                    level = 0;
                    return result;
                }
            default:
                return true;
        }
    }


    // function exists for easier reading and debugging
    static bool checkAsc(const bool result, const bool asc) {
        if ( result == asc)
            return true;
        else
            return false;
    }


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


class CVATRegister{
    CCounter m_IDmaker;
    unordered_map <string, string> m_companyNames;          // original names of companies mapped to the foormated versions
    unordered_map <string, set<CInvoice, CInvoiceCompare>> m_companies;  // name of company mapped to set of IDs of solo Invoices
    set <CInvoice, CInvoiceCompare> m_accepted;             // set of all accepted invoices
    set <CInvoice, CInvoiceCompare> m_issued;               // set of all issued invoices

  public:
    CVATRegister () = default;


    bool registerCompany ( const string& name ){
        string tmp;
        tmp = formatName(name);
        set<CInvoice, CInvoiceCompare> keys;
        return m_companies.insert(make_pair(tmp, keys)).second && m_companyNames.insert(make_pair(tmp, name)).second;
    };


    bool addIssued ( const CInvoice& newInv ){
        return addInvoice(m_issued, m_accepted, newInv);
    };


    bool addAccepted ( const CInvoice& newInv ){
        return addInvoice(m_accepted, m_issued, newInv);
    };


    bool delIssued ( const CInvoice& newInv ){
        return removeInvoice(m_issued, m_accepted, newInv);
    };


    bool delAccepted ( const CInvoice& newInv ){
        return removeInvoice(m_accepted, m_issued, newInv);
    };


    list<CInvoice> unmatched ( const string& company, const CSortOpt& sortBy ) const{
        string name = formatName(company);
        list<CInvoice> outList;
        // copy invoices into the list
        if (m_companies.find(name) == m_companies.end())
            return outList;

        CSortCompare comparator(sortBy);

        for( const auto& i : m_companies.at(name) ){
            outList.push_back(i);
        }

        outList.sort(comparator);

        /*
        for(auto const& i: outList)
            i.print();
        cout << endl;*/
        return outList;
    };

    void printCompanies() const{
        cout << "PRINT COMPANIES:" << endl;
        for(const auto& i : m_companies)
            cout << i.first << endl;
        cout << endl;
    }

    /*
    void printSolo() const{
        cout << "PRINT SOLO:" << endl;
        for(const auto& i : m_solos)
            i.second.print();
        cout << endl;
    }*/

  private:
    bool checkInvoiceValidity( const CInvoice& newInv) const{
        if ( newInv.buyerF() == newInv.sellerF() )
            return false;
        if ( m_companies.find(newInv.buyerF()) == m_companies.end() || m_companies.find(newInv.sellerF()) == m_companies.end() )
            return false;
        return true;
    }

    void addNewSolo( CInvoice& newInv){
        set<CInvoice, CInvoiceCompare>& keys = m_companies[newInv.buyerF()];
        keys.insert(newInv);
        set<CInvoice, CInvoiceCompare>& keys2 = m_companies[newInv.sellerF()];
        keys2.insert(newInv);
    }

    void removeSolo( CInvoice& remInv){
        set<CInvoice, CInvoiceCompare>& keys = m_companies[remInv.buyerF()];
        keys.erase(remInv);
        set<CInvoice, CInvoiceCompare>& keys2 = m_companies[remInv.sellerF()];
        keys2.erase(remInv);
    }

    bool addInvoice( set <CInvoice, CInvoiceCompare>& primary, set <CInvoice, CInvoiceCompare>& secondary, const CInvoice& newInv ){
        if ( ! checkInvoiceValidity(newInv) )
            return false;

        CInvoice newCopy = newInv;
        newCopy.buyerSet(m_companyNames[newCopy.buyerF()]);
        newCopy.sellerSet(m_companyNames[newCopy.sellerF()]);
        newCopy.m_ID = m_IDmaker();
        // check if secondary has the complementary invoice
        if( ! primary.insert(newCopy).second){
            return false;
        }
        auto compInvoice = secondary.find(newCopy);
        if ( compInvoice == secondary.end() ){
            // if not, add solo invoice to m_solos and add key to m_companies
            addNewSolo( newCopy);
        }
        else{
            removeSolo(newCopy);
        }
        return true;
    }

    bool removeInvoice( set <CInvoice, CInvoiceCompare>& primary, set <CInvoice, CInvoiceCompare>& secondary, const CInvoice& newInv ){
        CInvoice newCopy = newInv;
        newCopy.buyerSet(m_companyNames[newCopy.buyerF()]);
        newCopy.sellerSet(m_companyNames[newCopy.sellerF()]);

        // find the corresponding invoice
        auto iter = primary.find(newInv);
        if ( iter == primary.end() )
            return false;

        // check if secondary has the complementary invoice
        auto compInvoice = secondary.find(newCopy);
        if ( compInvoice == secondary.end() ){
            // if not, add solo invoice to m_solos and add key to m_companies
            removeSolo(newCopy);
        }
        else{
            newCopy.m_ID = iter->m_ID;
            addNewSolo( newCopy);
        }
        primary.erase(iter);
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
    if ( a.size() != b.size()){
        cout << "ERROR: DIFFERENT SIZE" << endl
            << "A SIZE: " << a.size() << endl
                << "B SIZE: " << b.size() <<endl;
        return false;
    }
    for( size_t i = 0; i < a.size(); i ++){
        //cout << "iteration " << i << endl;
        //iterA->print();
        //iterB->print();
        //cout << endl;
        if(iterA->buyer() != iterB->buyer())
            return false;
        if(iterA->seller() != iterB->seller())
            return false;
        if(iterA->amount() != iterB->amount())
            return false;
        if(iterA->vat() != iterB->vat())
            return false;
        if(iterA->date().compare(iterB->date()) != 0)
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
    assert ( r . addAccepted( CInvoice ( CDate ( 2000, 1, 1 ), " ", "Second Company ", 100, 20 ) ) );

    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A", "Second Company ", 100, 20 ) ) );
    assert ( r . addAccepted( CInvoice ( CDate ( 2000, 1, 1 ), "A", "Second Company ", 100, 20 ) ) );
    assert ( r . delIssued( CInvoice ( CDate ( 2000, 1, 1 ), "A", "Second Company ", 100, 20 ) ) );
    assert ( r . delAccepted( CInvoice ( CDate ( 2000, 1, 1 ), "A", "Second Company ", 100, 20 ) ) );


    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", "First Company", 300, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34 ) ) );

    assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
    assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "First Company", "First   Company", 200, 30 ) ) );
    assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "Another Company", "First   Company", 200, 30 ) ) );
    r.printCompanies();
    cout << endl;
    //r.printSolo();
    cout << endl;

    assert ( CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ) == CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ));

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

    r.printCompanies();
    //r.printSolo();
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

    r.printCompanies();
    //r.printSolo();
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
    //r.printSolo();

    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ). addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
             list<CInvoice>
             {
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
               CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
             } ) );
    assert ( equalLists ( r . unmatched ( " ", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) ),
                          list<CInvoice>
                                  {
                                  } ) );
    assert ( equalLists ( r . unmatched ( "First company", CSortOpt ()),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
                                  } ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
