#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-pass-by-value"
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

    virtual void print ( ostream& os , string prefix ) const = 0;

    virtual CElement* clone() const = 0;
};

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

    int getSelected () const{
        return m_idx;
    }

    void print ( ostream& os, string prefix ) const override{
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

class CWindow : public CElement{
    string m_title;
    vector <CElement*> m_elements;
public:
    CWindow ( int id, const string& title, const CRect& absPos )
        : CElement(id, CRect(1,1,1,1)), m_title(title){
        m_absPos = absPos;
    };

    ~CWindow () override{
        for( auto i : m_elements)
            delete i;
    }

    CElement* clone() const override{ return new CWindow(*this); }

    CWindow& add ( const CElement& newEl) {
        m_elements.push_back(newEl.clone());
        m_elements.back()->resize( m_absPos );
        return *this;
    };

    CElement* search( int id) const {
        for(auto i : m_elements){
            if( i->id() == id )
                return i;
        }
        return nullptr;
    }

    void setPosition( const CRect& absPos ){
        m_absPos = absPos;
        for( auto& i: m_elements)
            i->resize(m_absPos);
    }

    void print ( ostream& os, string prefix) const override{
        os << "[" << m_id << "] Window "
           << "\"" << m_title << "\" "
           << m_absPos << endl;
        for ( unsigned int i = 0; i < m_elements.size(); i++){
            CComboBox* ptr = dynamic_cast<CComboBox*>(m_elements[i]);
            if( ptr != nullptr){
                os << "+- ";
                if( i != m_elements.size() - 1)
                    ptr->print(os, string("|  ") );
                else
                    ptr->print(os, string("   ") );
                continue;
            }
            os << "+- " << *m_elements[i];
        }
    };
};


class CButton : public CElement{
    string m_name;
public:
    CButton ( int id, const CRect& relPos, const string& name ):
        CElement(id, relPos), m_name(name){};

    CElement* clone() const override{ return new CButton(*this); }

    void print ( ostream& os, string prefix) const override{
        os << "[" << m_id << "] Button "
           << "\"" << m_name << "\" "
           << m_absPos << endl;
    };
};


class CInput : public CElement{
    string m_value;
public:
    CInput ( int id, const CRect& relPos, const string& value )
        :CElement(id, relPos), m_value(value){};

    CElement* clone() const override{ return new CInput(*this); }

    void setValue ( const string& input ){
        m_value = input;
    }

    string getValue () const{
        return m_value;
    }

    void print ( ostream& os, string prefix) const override{
        os << "[" << m_id << "] Input "
           << "\"" << m_value << "\" "
           << m_absPos << endl;
    };
};


class CLabel : public CElement{
    string m_label;
public:
    CLabel ( int id, const CRect& relPos, const string& label )
        :CElement(id, relPos), m_label(label){};

    CElement* clone() const override{ return new CLabel(*this); }

    void print ( ostream& os, string prefix) const override{
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
template <typename T>
string toString ( const T & x ){
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int main ()
{
    cout << "sizeof: " <<  sizeof ( CButton ) - sizeof ( string ) << endl;
    cout << "sizeof: " << sizeof(CLabel) - sizeof ( string ) << endl;
    cout << "sizeof: " << sizeof(CInput) - sizeof ( string ) << endl;
    cout << "sizeof: " << sizeof ( CComboBox ) - sizeof ( vector<string> ) << endl;

    assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );

  CWindow aa ( 0, "Okno", CRect ( 10, 10, 600, 480 ));
  aa.add(CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ));
  cout << "TO STRING" << endl;
  cout << toString(aa) << endl;

  cout << "STRAIGHT TO COUT" << endl;
  cout << aa << endl;

  cout << "ONLY COMBOBOX" << endl;
  cout << toString( *aa. search(20)) << endl;
    assert ( toString ( *aa . search ( 20 ) ) ==
             "[20] ComboBox (70,154,480,48)\n"
             "+->Karate<\n"
             "+- Judo\n"
             "+- Box\n"
             "+- Progtest\n" );


  CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) );
  cout << toString(a);
  cout << "[0] Window \"Sample window\" (10,10,600,480)\n"
          "+- [1] Button \"Ok\" (70,394,180,48)\n"
          "+- [2] Button \"Cancel\" (370,394,180,48)\n"
          "+- [10] Label \"Username:\" (70,58,120,48)\n"
          "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
          "+- [20] ComboBox (70,154,480,48)\n"
          "   +->Karate<\n"
          "   +- Judo\n"
          "   +- Box\n"
          "   +- Progtest\n";
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  CWindow b = a;
  assert ( toString ( *b . search ( 20 ) ) ==
    "[20] ComboBox (70,154,480,48)\n"
    "+->Karate<\n"
    "+- Judo\n"
    "+- Box\n"
    "+- Progtest\n" );
  assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
  dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
  assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
  dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
  b . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (70,250,480,48)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  b . setPosition ( CRect ( 20, 30, 640, 520 ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [20] ComboBox (84,186,512,52)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (84,290,512,52)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

#pragma clang diagnostic pop