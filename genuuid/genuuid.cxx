
#include <iostream>
#include <libstud/uuid/uuid.hxx>

int main ()
{
  using namespace std;

  cout << stud::uuid::generate ().string () << endl;
}
