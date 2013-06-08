
/// @file var_analyzer.cc
/// @brief var_analyzer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "ym_utils/PoptMainApp.h"
#include "RvMgr.h"
#include "RegVect.h"
#include "Variable.h"

#include "ym_utils/CombiGen.h"
#include "ym_utils/RandGen.h"
#include "ym_utils/RandPermGen.h"


BEGIN_NAMESPACE_YM_IGF

int
calc_ndiff(const Variable* var,
	   const vector<const RegVect*>& vect_list)
{
  ymuint n0 = 0;
  ymuint n1 = 0;
  for (vector<const RegVect*>::const_iterator p = vect_list.begin();
       p != vect_list.end(); ++ p) {
    const RegVect* vect = *p;
    if ( var->classify(vect) == 0 ) {
      ++ n0;
    }
    else {
      ++ n1;
    }
  }
  return n0 - n1;
}

void
var_analyze(const vector<const Variable*>& var_list,
	    const vector<const RegVect*>& vect_list)
{
  vector<int> ndiff_list;
  ndiff_list.reserve(var_list.size());
  for (vector<const Variable*>::const_iterator p = var_list.begin();
       p != var_list.end(); ++ p) {
    const Variable* var = *p;
    int ndiff = calc_ndiff(var, vect_list);
    ndiff_list.push_back(ndiff);
  }

  int total_ndiff = 0;
  for (vector<int>::iterator p = ndiff_list.begin();
       p != ndiff_list.end(); ++ p) {
    total_ndiff += *p;
  }

  double n = var_list.size();
  double ave_ndiff = static_cast<double>(total_ndiff) / n;
  double total_vsq = 0.0;
  for (vector<int>::iterator p = ndiff_list.begin();
       p != ndiff_list.end(); ++ p) {
    int ndiff = *p;
    double v = ndiff - ave_ndiff;
    double vsq = v * v;
    total_vsq += vsq;
  }
  double div = sqrt(total_vsq) / n;

  cout << "Mean:     " << ave_ndiff << endl
       << "St. Div.: " << div << endl
       << endl;
}

int
igf(int argc,
    const char** argv)
{
  PoptMainApp app;

  // xor オプション
  PoptUint popt_xor("xor", 'x',
		    "specify XOR complexity", "<INT>");

  app.add_option(&popt_xor);

  app.set_other_option_help("<filename>");

  // オプション解析を行う．
  tPoptStat stat = app.parse_options(argc, argv, 0);
  if ( stat == kPoptAbort ) {
    return -1;
  }

  ymuint32 comp = 1;

  if ( popt_xor.is_specified() ) {
    comp = popt_xor.val();
  }

  vector<string> args;
  ymuint n_args = app.get_args(args);

  if ( n_args != 1 ) {
    app.usage(2);
  }

  const char* f_str = args[0].c_str();
  ifstream ifs(f_str);
  if ( !ifs ) {
    cerr << f_str << ": No such file" << endl;
    return 2;
  }

  RvMgr rvmgr;

  bool rstat = rvmgr.read_data(ifs);
  if ( !rstat ) {
    cerr << "read error" << endl;
    return 3;
  }

  const vector<const RegVect*>& vect_list = rvmgr.vect_list();

  ymuint n = rvmgr.vect_size();

  // Variable を生成
  vector<const Variable*> var_list;
  for (ymuint i = 0; i < n; ++ i) {
    const Variable* var = new Variable(i);
    var_list.push_back(var);
  }
  var_analyze(var_list, vect_list);

  for (ymuint comp_i = 2; comp_i <= comp; ++ comp_i) {
    var_list.clear();
    for (CombiGen cg(n, comp_i); !cg.is_end(); ++ cg) {
      vector<ymuint> vid_list(comp_i);
      for (ymuint j = 0; j < comp_i; ++ j) {
	vid_list[j] = cg(j);
      }
      const Variable* var = new Variable(vid_list);
      var_list.push_back(var);
    }
    var_analyze(var_list, vect_list);
  }

  return 0;
}

END_NAMESPACE_YM_IGF


int
main(int argc,
     const char** argv)
{
  using nsYm::nsIgf::igf;

  return igf(argc, argv);
}
