
/// @file npngen.cc
/// @brief npngen のソースファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/PermGen.h"
#include "NpnXform.h"


BEGIN_NAMESPACE_YM

void
npngen()
{
  PermGen pg(4, 4);

  const char* comma0 = "";
  for (PermGen::iterator p = pg.begin(); !p.is_end(); ++ p) {
    ymuint perm[4];
    for (ymuint i = 0; i < 4; ++ i) {
      perm[i] = p(i);
    }
    ymuint pid = NpnXform::perm_id(perm);
    for (ymuint pols = 0; pols < 32; ++ pols) {
      NpnXform xf(pid, pols);

      if ( comma0[0] != '\0' ) {
	cout << comma0 << endl;
      }
      cout << "0x" << hex << setw(4) << setfill('0')
	   <<  xf.data() << dec;
      comma0 = ",";
    }
  }
  cout << endl;
}

void
usage(char* progname)
{
  cerr << "USAGE: " << progname << endl;
}

END_NAMESPACE_YM

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  if ( argc != 1 ) {
    usage(argv[0]);
    exit(1);
  }

  npngen();

  return 0;
}
