#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <utility>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>
using namespace std;


class CRect{
public:
    CRect ( double x, double y, double w, double h ):
            m_X ( x ), m_Y ( y ), m_W ( w ), m_H ( h ){}

    friend ostream & operator << ( ostream & os, const CRect& x ){
        return os << '(' << x . m_X << ',' << x . m_Y << ',' << x . m_W << ',' << x . m_H << ')';
    }
    double m_X;
    double m_Y;
    double m_W;
    double m_H;
};
#endif /* __PROGTEST__ */

/** CElement is a base abstract class.
 * All other classes are derived from this one */
class CElement{
protected:
    int m_id;
    CRect m_relPos;
    CRect m_absPos;
public:

    CElement ( int id, const CRect& pos ):
            m_id(id), m_relPos(pos), m_absPos(CRect(1,1,1,1)){};

    virtual ~CElement() = default;

    int id() const{
        return m_id;
    }

    void resize( CRect& winSize ){
        m_absPos = CRect( m_relPos.m_X * winSize.m_W + winSize.m_X,
                          m_relPos.m_Y * winSize.m_H + winSize.m_Y,
                          m_relPos.m_W * winSize.m_W,
                          m_relPos.m_H * winSize.m_H);
    }

    friend ostream& operator << ( ostream& os, const CElement& out);
    virtual void print ( ostream& os , const string& prefix ) const = 0;
    virtual CElement* clone() const = 0;
};

//------------------------------------------------------------------------------------------

class CComboBox : public CElement{
public:
    unsigned int m_idx;
    vector <string> m_options;
    CComboBox ( int id, const CRect& relPos )
            :CElement(id, relPos), m_idx(0){};

    CElement* clone() const override{ return new CComboBox(*this); }

    CComboBox& add ( const string& newOpt ) {
        m_options.push_back(newOpt);
        return *this;
    }

    void setSelected ( int idx){
        m_idx = idx;
    }

    unsigned int getSelected () const{
        return m_idx;
    }

    void print ( ostream& os, const string& prefix ) const override{
        os << "[" << m_id << "] ComboBox "
           << m_absPos << endl;
        for( unsigned int i = 0; i < m_options.size(); i++){
            if( m_idx == i)
                os << prefix << "+->" << m_options[i] << "<" << endl;
            else
                os << prefix << "+- " << m_options[i] << endl;
        }
    };
};

//-------------------------------------------------------------------------------------------

/** CContainer is an abstract base class for classes CPanel and CWindow.
 * It has all the methods shared by the two classes. */
class CContainer : public CElement{
protected:
    vector <CElement*> m_elements;
public:
    CContainer ( int id, CRect relPos): CElement(id, relPos){};

    ~CContainer() override{
        for( auto i : m_elements)
            delete i;
    }

    CContainer ( const CContainer& rhs): CContainer(rhs.m_id, CRect(1,1,1,1)){
        for ( auto& i : rhs.m_elements){
            m_elements.push_back(i->clone());
        }
        m_absPos = rhs.m_absPos;
    }

    // adds an element to the vector
    CContainer& add ( const CElement& newEl ) {
        m_elements.push_back(newEl.clone());
        auto* ptr = dynamic_cast<CContainer*>(m_elements.back());
        if(ptr)
            ptr->resize( m_absPos );
        else
            m_elements.back()->resize( m_absPos );
        return *this;
    };

    // changes the relative and absolute positions of itself and all contained elements
    void resize( CRect& winSize ){
        m_absPos = CRect( m_relPos.m_X * winSize.m_W + winSize.m_X,
                          m_relPos.m_Y * winSize.m_H + winSize.m_Y,
                          m_relPos.m_W * winSize.m_W,
                          m_relPos.m_H * winSize.m_H);
        for ( auto& i : m_elements){
            auto* ptr = dynamic_cast<CContainer*>(i);
            if(ptr)
                ptr->resize( m_absPos );
            else
                i->resize( m_absPos );
        }
    }

    // searches all elements for the given id
    CElement* search( int id) const {
        for(auto i : m_elements){
            if( i->id() == id )
                return i;
            auto* ptr = dynamic_cast<CContainer*>(i);
            if(ptr) return ptr->search(id);
        }
        return nullptr;
    }

    void setPosition( const CRect& absPos ){
        m_absPos = absPos;
        for( auto& i: m_elements){
            auto* ptr = dynamic_cast<CContainer*>(i);
            if(ptr)
                ptr->resize( m_absPos );
            else
            i->resize(m_absPos);
        }
    }

    // prints out all the elements with correct formating
    void printElements( ostream& os, const string& prefix ) const{
        for ( unsigned int i = 0; i < m_elements.size(); i++){
            auto* ptr = dynamic_cast<CComboBox*>(m_elements[i]);
            if( ptr ){
                os << prefix << "+- ";
                if( i != m_elements.size() - 1)
                    ptr->print(os, prefix + "|  ");
                else
                    ptr->print(os, prefix + "   ");
                continue;
            }
            auto* ptr2 = dynamic_cast<CContainer*>(m_elements[i]);
            if( ptr2 ){
                os << prefix << "+- ";
                if( i != m_elements.size() - 1)
                    ptr2->print(os, prefix + "|  " );
                else
                    ptr2->print(os, prefix + "   " );
                continue;
            }
            os << prefix << "+- " << *m_elements[i];
        }
    }
};

//-----------------------------------------------------------------------------------------

class CPanel : public CContainer {
public:
    CPanel(int id, const CRect &relPos) : CContainer(id, relPos) {};

    CElement *clone() const override {
        CElement *cloned = new CPanel(*this);
        auto *ptr = dynamic_cast<CPanel *>(cloned);
        ptr->m_relPos = m_relPos;
        return cloned;
    }

    void print(ostream &os, const string &prefix) const override {
        os << "[" << m_id << "] Panel "
           << m_absPos << endl;
        printElements(os, prefix);
    };
};

//-------------------------------------------------------------------------------------------

class CWindow : public CContainer{
    string m_title;
public:
    CWindow ( int id, string  title, const CRect& absPos )
            : CContainer(id, CRect(1,1,1,1)), m_title(std::move(title)){
        m_absPos = absPos;
    };

    CWindow& operator = ( const CWindow& rhs){
        if ( &rhs != this )
        {
            for( auto i : m_elements)
                delete i;
            m_elements.clear();
            m_id = rhs.m_id;
            m_title = rhs.m_title;
            m_absPos = rhs.m_absPos;
            for ( auto& i : rhs.m_elements){
                m_elements.push_back(i->clone());
            }
        }
        return *this;
    }

    CElement* clone() const override{ return new CWindow(*this); }

    void print ( ostream& os, const string& prefix) const override{
        os << "[" << m_id << "] Window "
           << "\"" << m_title << "\" "
           << m_absPos << endl;
        printElements(os, prefix);
    };
};

//-------------------------------------------------------------------------------------------

class CButton : public CElement{
    string m_name;
public:
    CButton ( int id, const CRect& relPos, string  name ):
            CElement(id, relPos), m_name(std::move(name)){};

    CElement* clone() const override{ return new CButton(*this); }

    void print ( ostream& os, const string& prefix) const override{
        os << "[" << m_id << "] Button "
           << "\"" << m_name << "\" "
           << m_absPos << endl;
    };
};

//-------------------------------------------------------------------------------------------

class CInput : public CElement {
    string m_value;
public:
    CInput(int id, const CRect &relPos, string value)
            : CElement(id, relPos), m_value(std::move(value)) {};

    CElement *clone() const override { return new CInput(*this); }

    void setValue(const string &input) {
        m_value = input;
    }

    string getValue() const {
        return m_value;
    }

    void print(ostream &os, const string &prefix) const override {
        os << "[" << m_id << "] Input "
           << "\"" << m_value << "\" "
           << m_absPos << endl;
    };
};

//-------------------------------------------------------------------------------------------

class CLabel : public CElement{
    string m_label;
public:
    CLabel ( int id, const CRect& relPos, string  label )
            :CElement(id, relPos), m_label(std::move(label)){};

    CElement* clone() const override{ return new CLabel(*this); }

    void print ( ostream& os, const string& prefix) const override{
        os << "[" << m_id << "] Label "
           << "\"" << m_label << "\" "
           << m_absPos << endl;
    };
};


ostream& operator << (ostream& os, const CElement& out){
    out.print( os , string());
    return os;
}

// output operators

#ifndef __PROGTEST__
template <typename T_>
string toString ( const T_ & x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int main ()
{
  assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CButton ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
  CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . add ( CPanel ( 12, CRect ( 0.1, 0.3, 0.8, 0.7 ) ) . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) ) );


  cout << "TO STRING" << endl;
  cout << toString(a) << endl;
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [12] Panel (70,154,480,336)\n"
    "   +- [20] ComboBox (118,254.8,384,33.6)\n"
    "      +->Karate<\n"
    "      +- Judo\n"
    "      +- Box\n"
    "      +- Progtest\n" );
  CWindow b = a;
  assert ( toString ( *b . search ( 20 ) ) ==
    "[20] ComboBox (118,254.8,384,33.6)\n"
    "+->Karate<\n"
    "+- Judo\n"
    "+- Box\n"
    "+- Progtest\n" );
  assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
  dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
  assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
  dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
  CPanel & p = dynamic_cast<CPanel &> ( *b . search ( 12 ) );
  p . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );

    cout << "TO STRING A" << endl;
    cout << toString(a) << endl;
    cout << "TO STRING B" << endl;
    cout << toString(b) << endl;
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
    "+- [12] Panel (70,154,480,336)\n"
    "   +- [20] ComboBox (118,254.8,384,33.6)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (118,322,384,33.6)\n"
    "      +->PA2<\n"
    "      +- OSY\n"
    "      +- Both\n" );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [12] Panel (70,154,480,336)\n"
    "   +- [20] ComboBox (118,254.8,384,33.6)\n"
    "      +->Karate<\n"
    "      +- Judo\n"
    "      +- Box\n"
    "      +- Progtest\n" );
  assert ( toString ( p ) ==
    "[12] Panel (70,154,480,336)\n"
    "+- [20] ComboBox (118,254.8,384,33.6)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (118,322,384,33.6)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  b . setPosition ( CRect ( 20, 30, 640, 520 ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [12] Panel (84,186,512,364)\n"
    "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "      +->PA2<\n"
    "      +- OSY\n"
    "      +- Both\n" );
  p . add ( p );
    cout << "TO STRING P" << endl;
    cout << toString(p) << endl;
  assert ( toString ( p ) ==
    "[12] Panel (84,186,512,364)\n"
    "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "|  +->PA2<\n"
    "|  +- OSY\n"
    "|  +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "      +->PA2<\n"
    "      +- OSY\n"
    "      +- Both\n" );
  p . add ( p );

  cout << "MAD SUS SHIT" << endl;
  cout << toString(p) << endl;
  assert ( toString ( p ) ==
    "[12] Panel (84,186,512,364)\n"
    "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "|  +->PA2<\n"
    "|  +- OSY\n"
    "|  +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "|  |  +- Karate\n"
    "|  |  +- Judo\n"
    "|  |  +- Box\n"
    "|  |  +->Progtest<\n"
    "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "|     +->PA2<\n"
    "|     +- OSY\n"
    "|     +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "   |  +->PA2<\n"
    "   |  +- OSY\n"
    "   |  +- Both\n"
    "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "      |  +- Karate\n"
    "      |  +- Judo\n"
    "      |  +- Box\n"
    "      |  +->Progtest<\n"
    "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "         +->PA2<\n"
    "         +- OSY\n"
    "         +- Both\n" );
  p . add ( p );
  assert ( toString ( p ) ==
    "[12] Panel (84,186,512,364)\n"
    "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "|  +->PA2<\n"
    "|  +- OSY\n"
    "|  +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "|  |  +- Karate\n"
    "|  |  +- Judo\n"
    "|  |  +- Box\n"
    "|  |  +->Progtest<\n"
    "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "|     +->PA2<\n"
    "|     +- OSY\n"
    "|     +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "|  |  +- Karate\n"
    "|  |  +- Judo\n"
    "|  |  +- Box\n"
    "|  |  +->Progtest<\n"
    "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "|  |  +->PA2<\n"
    "|  |  +- OSY\n"
    "|  |  +- Both\n"
    "|  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "|     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "|     |  +- Karate\n"
    "|     |  +- Judo\n"
    "|     |  +- Box\n"
    "|     |  +->Progtest<\n"
    "|     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "|        +->PA2<\n"
    "|        +- OSY\n"
    "|        +- Both\n"
    "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "   |  +->PA2<\n"
    "   |  +- OSY\n"
    "   |  +- Both\n"
    "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "   |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "   |  |  +- Karate\n"
    "   |  |  +- Judo\n"
    "   |  |  +- Box\n"
    "   |  |  +->Progtest<\n"
    "   |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "   |     +->PA2<\n"
    "   |     +- OSY\n"
    "   |     +- Both\n"
    "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "      |  +- Karate\n"
    "      |  +- Judo\n"
    "      |  +- Box\n"
    "      |  +->Progtest<\n"
    "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "      |  +->PA2<\n"
    "      |  +- OSY\n"
    "      |  +- Both\n"
    "      +- [12] Panel (208.928,425.148,262.144,124.852)\n"
    "         +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
    "         |  +- Karate\n"
    "         |  +- Judo\n"
    "         |  +- Box\n"
    "         |  +->Progtest<\n"
    "         +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
    "            +->PA2<\n"
    "            +- OSY\n"
    "            +- Both\n" );

    cout << "SUSSIEST OF BAKAS" << endl;
    cout << toString(b) << endl;
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [12] Panel (84,186,512,364)\n"
    "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
    "   |  +- Karate\n"
    "   |  +- Judo\n"
    "   |  +- Box\n"
    "   |  +->Progtest<\n"
    "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
    "   |  +->PA2<\n"
    "   |  +- OSY\n"
    "   |  +- Both\n"
    "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  |  +- Karate\n"
    "   |  |  +- Judo\n"
    "   |  |  +- Box\n"
    "   |  |  +->Progtest<\n"
    "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "   |     +->PA2<\n"
    "   |     +- OSY\n"
    "   |     +- Both\n"
    "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "   |  |  +- Karate\n"
    "   |  |  +- Judo\n"
    "   |  |  +- Box\n"
    "   |  |  +->Progtest<\n"
    "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "   |  |  +->PA2<\n"
    "   |  |  +- OSY\n"
    "   |  |  +- Both\n"
    "   |  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "   |     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "   |     |  +- Karate\n"
    "   |     |  +- Judo\n"
    "   |     |  +- Box\n"
    "   |     |  +->Progtest<\n"
    "   |     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "   |        +->PA2<\n"
    "   |        +- OSY\n"
    "   |        +- Both\n"
    "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
    "      +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
    "      |  +- Karate\n"
    "      |  +- Judo\n"
    "      |  +- Box\n"
    "      |  +->Progtest<\n"
    "      +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
    "      |  +->PA2<\n"
    "      |  +- OSY\n"
    "      |  +- Both\n"
    "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "      |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "      |  |  +- Karate\n"
    "      |  |  +- Judo\n"
    "      |  |  +- Box\n"
    "      |  |  +->Progtest<\n"
    "      |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "      |     +->PA2<\n"
    "      |     +- OSY\n"
    "      |     +- Both\n"
    "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
    "         +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
    "         |  +- Karate\n"
    "         |  +- Judo\n"
    "         |  +- Box\n"
    "         |  +->Progtest<\n"
    "         +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
    "         |  +->PA2<\n"
    "         |  +- OSY\n"
    "         |  +- Both\n"
    "         +- [12] Panel (208.928,425.148,262.144,124.852)\n"
    "            +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
    "            |  +- Karate\n"
    "            |  +- Judo\n"
    "            |  +- Box\n"
    "            |  +->Progtest<\n"
    "            +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
    "               +->PA2<\n"
    "               +- OSY\n"
    "               +- Both\n" );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [12] Panel (70,154,480,336)\n"
    "   +- [20] ComboBox (118,254.8,384,33.6)\n"
    "      +->Karate<\n"
    "      +- Judo\n"
    "      +- Box\n"
    "      +- Progtest\n" );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
