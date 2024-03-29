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

        return *this;
    }

    // isOrdered ( comparator )
    template <typename Comp>
    bool isOrdered ( const Comp& comparator ) const{
        map < string, CNode > graph;
        set<string> start;

        makeGraph( comparator, graph, start);
       // printGraph(graph);
        if(start.size() != 1)
            return false;
        list<string> outList;
        try{
            findPath(*start.begin(), 0, graph, outList);
        }
        catch( const logic_error& i){
            return false;
        }

        if(outList.empty())
            return false;
        else
            return true;
    }

    // results ( comparator )
    template <typename Comp>
    list<string> results ( const Comp& comparator ) const{
        map < string, CNode > graph;
        set<string> start;

        makeGraph( comparator, graph, start );
        if(start.size() != 1)
            throw logic_error("Multiple starting points");
        list<string> outList;
        findPath(*start.begin(), 0, graph, outList);

        //printList(outList);

        if(outList.empty())
            throw logic_error("List couldn't be completed");
        else
            return outList;
    }

    void printMatches() const{
        cout << "MATCHES" << endl;
        for( auto& i : m_matches){
            cout << i.first.first << " " << i.first.second << endl;
        }
    }

    void printGraph( const map < string, CNode >& graph) const{
        cout << "GRAPH" << endl;
        for( auto& i : graph){
            cout << i.first << " -- ";
            for( auto& j : i.second.relations )
                cout << j << ", ";
            cout << endl;
        }
    }

    void printList( const list < string >& outList) const{
        cout << "LIST" << endl;
        for( auto& i : outList){
            cout << i << endl;
        }
    }
private:
    template <typename Comp>
    void makeGraph( const Comp& comparator, map < string, CNode >& graph, set<string>& start) const{

        for ( auto& i : m_matches ){
            string cont1 = i.first.first, cont2 = i.first.second;
            if(i.second.second)
                cont1 = i.first.second, cont2 = i.first.first;

            if( graph.insert( make_pair(cont1, CNode() ) ).second)
                start.insert(cont1);
            if( graph.insert( make_pair(cont2, CNode() ) ).second)
                start.insert(cont2);

            int result = comparator( i.second.first );
            if(result < 0){
                graph.at(cont2).relations.insert(cont1);
                start.erase(cont1);
                continue;
            }
            if(result > 0){
                graph.at(cont1).relations.insert(cont2);
                start.erase(cont2);
            }
        }
    }

    bool findPath( const string& currNode , unsigned int depth, map < string, CNode >& graph, list<string>& outList) const{
        graph.at( currNode).visited = true;
        depth ++;
        // terminate reccursion when the graph reaches final node
        if( graph.at(currNode).relations.empty()){
            if( depth == graph.size()){
                graph.at( currNode).visited = false;
                outList.push_back(currNode);
                return true;
            }
            else{
                graph.at( currNode).visited = false;
                return false;
            }
        }

        // going through the node relations
        bool flag = true;
        for( auto& i : graph.at( currNode).relations ){
            if( graph.at(i).visited ){
                throw logic_error("Graph has a cycle");
            }
            if( flag && findPath(i, depth, graph, outList) ){
                graph.at( currNode).visited = false;
                outList.push_front(currNode);
                flag = false;
            }
        }
        if(! flag)
            return true;

        graph.at( currNode).visited = false;
        return false;
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
    cout << endl;
    

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


}

#endif /* __PROGTEST__ */
