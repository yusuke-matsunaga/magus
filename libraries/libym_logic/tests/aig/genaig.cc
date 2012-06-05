
/// @file genaig.cc
/// @brief N 入力関数を実現する AIG  を生成するプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "GenAig.h"
#include "GenAigNaive.h"
#include "GenAigDj.h"


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  GenAig genaig;


  cout << "genaig(3)" << endl;
  genaig(3);

  cout << "genaig(4)" << endl;
  genaig(4);

  return 0;
}
