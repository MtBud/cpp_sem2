#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

struct CNode{
    bool visited = false;
    set<string> relations;
};

template <typename M_>
class CContest{
    map < pair<string, string>, pair<M_,bool> > m_matches;
    // possibly add graph cache
    map < string, CNode > m_graph;
    set<string> m_start;
public:
    // default constructor
    CContest() = default;
    // destructor
    // addMatch ( contestant1, contestant2, result )
    CContest& addMatch (  string contestant1,  string contestant2, const M_& result){
        bool flipped = false;
        if( contestant1 == contestant2)
            throw logic_error("Contestant 1 and contestant 2 are equal");
        if( contestant1 > contestant2){
            swap(contestant1, contestant2);
            flipped = true;
        }
        if( m_matches.insert(make_pair( make_pair(contestant1, contestant2), make_pair(result, flipped)) ).second == false )
            throw logic_error("Adding duplicate entry");

        // add nodes to graph
        if( m_graph.insert( make_pair(contestant1, CNode() ) ).second)
            m_start.insert(contestant1);
        if( m_graph.insert( make_pair(contestant2, CNode() ) ).second)
            m_start.insert(contestant1);


        return *this;
    }
    // isOrdered ( comparator )
    bool isOrdered ( int (*comparator)(M_) ){
        makeGraph( comparator );
        if(m_start.size() != 1)
            return false;
        list<string> outList =  findPath(*m_start.begin(), 1, list<string>());
        if(outList.empty())
            return false;
        else
            return true;
    }

    // results ( comparator )
    list<string> results ( int (*comparator)(M_)  ){
        makeGraph( comparator );
        if(m_start.size() != 1)
            throw logic_error("Multiple starting points");
        list<string> outList =  findPath(*m_start.begin(), 1, list<string>());
        if(outList.empty())
            throw logic_error("List couldn't be completed");
        else
            return outList;
    }

    void printMatches(){
        cout << "MATCHES" << endl;
        for( auto& i : m_matches){
            cout << i.first.first << " " << i.first.second << endl;
        }
    }

    void printGraph(){
        cout << "GRAPH" << endl;
        for( auto& i : m_graph){
            cout << i.first << " -- ";
            for( auto& j : i.second.relations )
                cout << j << " ";
            cout << endl;
        }
    }
private:
    void makeGraph( int (*comparator)(M_) ){

        for ( auto& i : m_matches ){
            const string& cont1 = m_matches.first.first, cont2 = m_matches.first.second;
            if(m_matches.second.second)
                cont1 = m_matches.first.second, cont2 = m_matches.first.first;

            int result = comparator( m_matches.second.first );
            if(result < 0){
                m_graph.at(cont2).relations.insert(cont1);
                m_start.erase(cont1);
                continue;
            }
            if(result > 0){
                m_graph.at(cont1).relations.insert(cont2);
                m_start.erase(cont2);
            }
        }
    }

    list<string> findPath( const string& currNode , int depth, list<string> outList){
        m_graph.at( currNode).visited = true;
        depth ++;
        // terminate reccursion when the graph reaches final node
        if( m_graph.at(currNode).relations.empty()){
            if( depth == m_graph.size()){
                m_graph.at( currNode).visited = false;
                outList.push_back(currNode);
                return outList;
            }
            else{
                m_graph.at( currNode).visited = false;
                return {};
            }
        }

        for( auto& i : m_graph.at( currNode).relations ){
            if( m_graph.at(i).visited ){
                m_graph.at( currNode).visited = false;
                return {};
            }
            list<string> result = findPath(i, depth, outList);
            if( ! result.empty() ){
                m_graph.at( currNode).visited = false;
                return result;
            }
        }

        m_graph.at( currNode).visited = false;
        return {};

    }

};

#ifndef __PROGTEST__
struct CMatch{
public:
      CMatch ( int a, int b ): m_A ( a ), m_B ( b ){}
    
    int m_A;
    int m_B;
};

class HigherScoreThreshold{
public:
    HigherScoreThreshold( int diffAtLeast ): m_DiffAtLeast ( diffAtLeast ){}
    int operator () ( const CMatch & x ) const{
      return ( x . m_A > x . m_B + m_DiffAtLeast ) - ( x . m_B > x . m_A + m_DiffAtLeast );
    }

private:
    int m_DiffAtLeast;
};

// 0 if equal
// - if m_A is smaller
// + if m_A is higher
int HigherScore( const CMatch& x ){
  return ( x . m_A > x . m_B ) - ( x . m_B > x . m_A );
}

int main(){
  CContest<CMatch>  x;
  
  x . addMatch ( "C++", "Pascal", CMatch ( 10, 3 ) )
    . addMatch ( "C++", "Java", CMatch ( 8, 1 ) )
    . addMatch ( "Pascal", "Basic", CMatch ( 40, 0 ) )
    . addMatch ( "Java", "PHP", CMatch ( 6, 2 ) )
    . addMatch ( "Java", "Pascal", CMatch ( 7, 3 ) )
    . addMatch ( "PHP", "Basic", CMatch ( 10, 0 ) );

    try{
        x . addMatch ( "PHP", "Basic", CMatch ( 10, 0 ) );
        assert ( "Exception missing!" == nullptr );
    }
    catch ( const logic_error & e ){}
    try{
        x . addMatch ( "Basic", "PHP", CMatch ( 10, 0 ) );
        assert ( "Exception missing!" == nullptr );
    }
    catch ( const logic_error & e ){}
    try{
        x . addMatch ( "C++", "C++", CMatch ( 10, 0 ) );
        assert ( "Exception missing!" == nullptr );
    }
    catch ( const logic_error & e ){}

    x.printMatches();
    

  assert ( ! x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }

  /*
  x . addMatch ( "PHP", "Pascal", CMatch ( 3, 6 ) ); 

  assert ( x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( ( res == list<string>{ "C++", "Java", "Pascal", "PHP", "Basic" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }

  
  assert ( ! x . isOrdered ( HigherScoreThreshold ( 3 ) ) );
  try
  {
    list<string> res = x . results ( HigherScoreThreshold ( 3 ) );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  assert ( x . isOrdered ( [] ( const CMatch & x )
  {
    return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
  } ) );
  try
  {
    list<string> res = x . results ( [] ( const CMatch & x )
    {
      return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
    } );
    assert ( ( res == list<string>{ "Basic", "PHP", "Pascal", "Java", "C++" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
  
  CContest<bool>  y;
  
  y . addMatch ( "Python", "PHP", true )
    . addMatch ( "PHP", "Perl", true )
    . addMatch ( "Perl", "Bash", true )
    . addMatch ( "Bash", "JavaScript", true )
    . addMatch ( "JavaScript", "VBScript", true );
  
  assert ( y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    });
    assert ( ( res == list<string>{ "Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
    
  y . addMatch ( "PHP", "JavaScript", false );
  assert ( !y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    } );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "PHP", "JavaScript", false );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "JavaScript", "PHP", true );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  return EXIT_SUCCESS;

   */
}

#endif /* __PROGTEST__ */
