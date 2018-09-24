#include <string>
#include <cstdint>

// RFC4122 UUID
//
struct uuid
{
  std::uint32_t time_low = 0;
  std::uint16_t time_mid = 0;
  std::uint16_t time_hiv = 0;
  std::uint8_t  clock_seq_hir = 0;
  std::uint8_t  clock_seq_low = 0;
  std::uint8_t  node[6] = {0, 0, 0, 0, 0, 0};

  static uuid
  generate ();

  std::string
  string () const;
};

#include <iostream>

int main ()
{
  using namespace std;

  cout << uuid::generate ().string () << endl;
}

#include <cstdio>  // sprintf()
#include <cstring> // memcpy()

#ifdef _WIN32
#  include <rpc.h>
#else
#  include <uuid/uuid.h>
#endif

using namespace std;

uuid uuid::
generate ()
{
  uuid r;

#ifdef _WIN32

  UUID d;
  if (UuidCreate (&d) == RPC_S_OK)
  {
    r.time_low  = d.Data1;
    r.time_mid  = d.Data2;
    r.time_hiv  = d.Data3;
    r.clock_seq_hir = d.Data4[0];
    r.clock_seq_low = d.Data4[1];
    memcpy (r.node, &d.Data4[2], 6);
  }

#else

  uuid_t d;
  uuid_generate (d);

  const uint8_t* p (d);

  {
    uint32_t t;

    t = *p++;
    t = (t << 8) | *p++;
    t = (t << 8) | *p++;
    t = (t << 8) | *p++;
    r.time_low = t;
  }

  uint16_t t;

  t = *p++;
  t = (t << 8) | *p++;
  r.time_mid = t;

  t = *p++;
  t = (t << 8) | *p++;
  r.time_hiv = t;

  r.clock_seq_hir = *p++;
  r.clock_seq_low = *p++;

  memcpy (r.node, p, 6);

#endif

  return r;
}

string uuid::
string () const
{
  char r[37];
  sprintf (r,
           "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
           time_low,
           time_mid,
           time_hiv,
           clock_seq_hir,
           clock_seq_low,
           node[0], node[1], node[2], node[3], node[4], node[5]);
  return r;
}
