// $Id: combigen_test.cc 700 2007-05-31 00:41:30Z matsunaga $

// nsMagus::CombiGen のテスト


#include "utils/CombiGen.h"


int
main(int argc, char** argv)
{
  using namespace nsYm;
  try {
    char buff[1024];
    while ( cin.getline(buff, 1024, '\n') ) {
      char* p = strtok(buff, " \t");
      if ( !p ) {
	cerr << "ERROR: n must be specified" << endl;
	return 2;
      }
      ymuint n = atoi(p);
      p = strtok(NULL, " \t");
      if ( !p ) {
	cerr << "ERROR: k must be specified" << endl;
	return 2;
      }
      ymuint k = atoi(p);
      if ( n < k ) {
	cerr << "ERROR: n(" << n << ") < k(" << k << ")" << endl;
	return 1;
      }
      for (CombiGen cg(n, k); !cg.is_end(); ++ cg) {
	for (ymuint i = 0; i < k; ++ i) {
	  cout << " " << cg(i);
	}
	cout << endl;
      }
      cout << "---" << endl;
    }
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }

  return 0;
}
