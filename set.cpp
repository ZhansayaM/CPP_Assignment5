#include "set.h"

void print( std::ostream& out, const treenode* n, size_t indent )
{

   for( size_t i = 0; i != indent; ++ i )
      out << "|  "; 
   if(n)
   {
      out << ( n -> val ) << "\n";
      print( out, n -> left, indent + 1 );
      print( out, n -> right, indent + 1 ); 
   }
   else
      out << "#\n"; 
}


void checksorted( 
          treenode* n, 
          const std::string* lowerbound, 
          const std::string* upperbound )
{
   while(n) 
   {
      if( lowerbound && !before( *lowerbound, n -> val ))
      {
         std::cout << "value " << ( n -> val );
         std::cout << " is not above lower bound " << *lowerbound << "\n";
         std::abort( );
      }

      if( upperbound && !before( n -> val, *upperbound ))
      {
         std::cout << "value " << ( n -> val );
         std::cout << "is not below upperbound " << *upperbound << "\n";
         std::abort( );
      }

      checksorted( n -> left, lowerbound, &( n -> val ));
      lowerbound = & ( n -> val );
      n = n -> right;
   }
} 


// Used by copy constructor and assignment:

void writecopy( treenode** to, treenode* from )
{
   while( from )
   {
      *to = new treenode{ from -> val };

      writecopy( & (*to) -> left, from -> left );
      to = & (*to) -> right;
      from = from -> right;
   }
}

// Used by destructor:

void deallocate( treenode* n )
{
   while(n)
   {
      deallocate( n -> left );
      treenode* bye = n;
      n = n -> right;
      delete bye;
   }
}

size_t log_base2( size_t s )
{
   return (s>1)?1+log_base2(s/2):0;
}

// From previous task:

bool equal( const std::string& s1, const std::string& s2 )
{
	if (s1.size() != s2.size())
		return false;
	
	for (size_t i = 0; i < s1.size(); i++) {
		if ((char)  tolower(s1[i]) != (char) tolower(s2[i]))
			return false;
	}

	return true;
}

// Must be written for task 5(part 2) :

bool before( const std::string& s1, const std::string& s2 )
{
  std::string s11;
  std::string s22;
  s11 = s1;
  s22 = s2;

  for (size_t i=0; i < s1.size(); i++){
      s11[i] = tolower(s1[i]);
  }
  
  for (size_t i=0; i < s2.size(); i++){
      s22[i] = tolower(s2[i]);
  }

  if (s22 > s11)
    return true;
  else 
    return false;
}


const treenode* find( const treenode* n, const std::string& el )
{
    if (n == nullptr)
        return nullptr;
    if (equal(n->val, el) == 1)
        return n;
    else if (n->left && before(n->val, el) == 0)
        return find(n->left, el);
    else if (n->right && before(n->val, el) == 1)
        return find(n->right, el);
    else
        return nullptr;
}
   // Write this one first.

treenode** find( treenode** n, const std::string& el )
{
    if (*n && (*n)->left && before((*n)->val, el) == 0)
        return find(&(*n)->left, el);
    else if (*n && (*n)->right && before((*n)->val, el) == 1)
        return find(&(*n)->right, el);
    return (n);
}
   // Modify the other find into this one, also have a look at
   // stackscan4.pdf


// Insert n at the right most position in into: 
void rightinsert( treenode** into, treenode* n )
{
    treenode *tmp = *into;
    if (tmp == nullptr)
    {
        *into = n;
        return ;
    }
    while (tmp->right)
        tmp = tmp->right;
    tmp->right = n;
}


size_t size( const treenode* n )
{ 
    if (n == nullptr)
        return (0);
    if (n->right == nullptr && n->left == nullptr)
        return (1);
    return (1 + size(n->left) + size(n->right));
}


size_t height( const treenode* n )
{
    if (n == nullptr)  
        return 0;
    else
    {  
        // depth of each subtree
        int leftDepth = height(n->left);  
        int rightDepth = height(n->right);  
      
        // find larger one
        if (leftDepth > rightDepth)  
            return(leftDepth + 1);  
        else
            return(rightDepth + 1);  
    } 
}
 
bool set::insert( const std::string& el )
{
    const treenode *check = find(tr, el);
    if (check != nullptr)
        return (false);
    treenode **found = find(&tr, el);
    treenode *n = new treenode(el);
    if (*found == nullptr)
      *found = n;
    else
    {
        if (before((*found)->val, el) == 0)
            (*found)->left = n;
        else
            (*found)->right = n;
    }
    return (true);
}

bool set::contains( const std::string& el ) const 
{
   return find( tr, el ) != nullptr; 
}


bool set::remove( const std::string& el )
{
    const treenode *check = find(tr, el);
    if (check == nullptr)
        return (false);
    treenode **nn = find(&tr, el);
    if ((*nn)->left == nullptr && (*nn)->right == nullptr)
    {
        delete *nn;
        *nn = nullptr;
        return (true);
    }
    treenode *tmp = *nn;
    treenode *right = tmp->right;
    *nn = tmp->left;
    rightinsert(nn, right);
    delete tmp;
    tmp = nullptr;
    return (true);
}



void set::checksorted( ) const
{
   ::checksorted( tr, nullptr, nullptr );
      // :: are needed because the other checksorted is not in the class. 
}


std::ostream& set::print( size_t indent, std::ostream& out ) const
{
   ::print( out, tr, indent );
   return out;
}


