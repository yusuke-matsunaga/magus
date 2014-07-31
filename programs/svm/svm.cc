
/// @file svm.cc
/// @brief svm のメインファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "YmLogic/Bdd.h"
#include "YmLogic/BddMgr.h"
#include "ThBddGen.h"


BEGIN_NAMESPACE_YM

// 係数ファイルの読み込み
bool
read_biasfile(const string& filename)
{
  ifstream ifs;
  ifs.open(filename.c_str());

  if ( !ifs ) {
    cerr << filename << " : No such file" << endl;
    return false;
  }

  vector<double> val_list;
  double threshold = 0.0;;
  for (ymuint pos = 0; ifs; ++ pos) {
    double val;
    ifs >> val;
    if ( pos == 0 ) {
      threshold = val;
    }
    else {
      val_list.push_back(val);
    }
  }

  BddMgr bddmgr("bmc", "Bdd Manager");

  ThBddGen thbddgen(bddmgr);
  Bdd f = thbddgen(val_list, - threshold);

  return true;
}

END_NAMESPACE_YM


int
main(int argc,
     const char** argv)
{
  using namespace std;

  if ( argc != 2 ) {
    cerr << "Usage: " << argv[0] << " filename" << endl;
    return 255;
  }

  nsYm::read_biasfile(argv[1]);

  return 0;
}
