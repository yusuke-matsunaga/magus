
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
#include "XorFunc.h"
#include "FuncVect.h"

#include "ym_utils/CombiGen.h"
#include "ym_utils/RandGen.h"
#include "ym_utils/RandCombiGen.h"


BEGIN_NAMESPACE_YM_IGF

struct PairLt
{
  PairLt() { }

  bool
  operator()(const pair<ymuint, const Variable*>& left,
	     const pair<ymuint, const Variable*>& right)
  {
    return left.first < right.first;
  }
};

struct VarInfo
{
  const Variable* mVar;

  ymuint mNdiff;

};

class VarHeap
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_size 最大サイズ
  /// @note max_size を越えた要素は切り捨てらる．
  VarHeap(ymuint max_size);

  /// @brief デストラクタ
  ~VarHeap();


public:

  /// @brief 空にする．
  void
  clear();

  /// @brief 要素を追加する．
  /// @note max_size を越えたら最大の要素を捨てる．
  void
  put(const Variable* var,
      ymuint ndiff);

  /// @brief 要素数を返す．
  ymuint
  elem_num() const;

  /// @brief 要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
  const Variable*
  elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の位置の要素を適当な位置まで沈めていく
  void
  push_down(ymuint first);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 配列のサイズ
  ymuint32 mMaxSize;

  // 要素数
  ymuint32 mNum;

  // 配列本体
  VarInfo* mArray;

};

// @brief コンストラクタ
// @param[in] max_size 最大サイズ
// @note max_size を越えた要素は切り捨てらる．
VarHeap::VarHeap(ymuint max_size) :
  mMaxSize(max_size),
  mNum(0),
  mArray(new VarInfo[max_size + 1])
{
}

// @brief デストラクタ
VarHeap::~VarHeap()
{
  delete [] mArray;
}

// @brief 空にする．
void
VarHeap::clear()
{
  mNum = 0;
}

// @brief 要素を追加する．
// @note max_size を越えたら最大の要素を捨てる．
void
VarHeap::put(const Variable* var,
	     ymuint ndiff)
{
  mArray[mNum].mVar = var;
  mArray[mNum].mNdiff = ndiff;

  ymuint pos = mNum;
  while ( pos > 0 ) {
    VarInfo& node = mArray[pos];
    ymuint parent = (pos - 1) >> 1;
    VarInfo& node_p = mArray[parent];
    if ( node_p.mNdiff > node.mNdiff ) {
      VarInfo tmp = node_p;
      node_p = node;
      node = tmp;
      pos = parent;
    }
    else {
      break;
    }
  }
  if ( mNum < mMaxSize ) {
    ++ mNum;
  }
}

// @brief 要素数を返す．
ymuint
VarHeap::elem_num() const
{
  return mNum;
}

// @brief 要素を返す．
// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
const Variable*
VarHeap::elem(ymuint pos) const
{
  return mArray[pos].mVar;
}

// @brief 引数の位置の要素を適当な位置まで沈めていく
void
VarHeap::push_down(ymuint first)
{
  ymuint parent = first;
  for ( ; ; ) {
    ymuint left = parent + parent + 1;
    ymuint right = left + 1;
    if ( right > mNum ) {
      break;
    }
    VarInfo& node_p = mArray[parent];
    VarInfo& node_l = mArray[left];
    if ( right == mNum ) {
      if ( node_p.mNdiff > node_l.mNdiff ) {
	VarInfo tmp = node_p;
	node_p = node_l;
	node_l = tmp;
      }
      break;
    }
    else {
      VarInfo& node_r = mArray[right];
      if ( node_p.mNdiff > node_l.mNdiff &&
	   node_l.mNdiff <= node_r.mNdiff ) {
	VarInfo tmp = node_p;
	node_p = node_r;
	node_r = tmp;
	parent = right;
      }
      else {
	break;
      }
    }
  }
}


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
  if ( n0 < n1 ) {
    return n1 - n0;
  }
  else {
    return n0 - n1;
  }
}

ymuint
calc_ds(XorFunc& xf,
	ymuint exp_p,
	const vector<const RegVect*>& vect_list)
{
  vector<ymuint> hist(exp_p, 0);
  ymuint nv = vect_list.size();
  for (ymuint i = 0; i < nv; ++ i) {
    const RegVect* rv = vect_list[i];
    ymuint pos = xf.eval(rv);
    ++ hist[pos];
  }
  ymuint ds = 0;
  for (ymuint i = 0; i < exp_p; ++ i) {
    ymuint h = hist[i];
    ds += h * h;
  }
  return ds;
}

int
igf(int argc,
    const char** argv)
{
  PoptMainApp app;

  // slack オプション
  PoptUint popt_slack("slack", 's',
		      "specify SLACK parameter", "<INT>");
  app.add_option(&popt_slack);

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

  ymuint32 slack = 0;
  if ( popt_slack.is_specified() ) {
    slack = popt_slack.val();
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

  vector<const Variable*> var_list;

  // Primary Variable を生成
  ymuint max_ndiff = 0;
  for (ymuint i = 0; i < n; ++ i) {
    const Variable* var = new Variable(i);
    var_list.push_back(var);
    ymuint ndiff = calc_ndiff(var, vect_list);
    if ( max_ndiff < ndiff ) {
      max_ndiff = ndiff;
    }
  }

  // Composed Variable を生成
  for (ymuint comp_i = 2; comp_i <= comp; ++ comp_i) {
    for (CombiGen cg(n, comp_i); !cg.is_end(); ++ cg) {
      vector<ymuint> vid_list(comp_i);
      for (ymuint i = 0; i < comp_i; ++ i) {
	ymuint pos = cg(i);
	vid_list[i] = pos;
      }
      const Variable* var = new Variable(vid_list);
      ymuint ndiff = calc_ndiff(var, vect_list);
      if ( ndiff < max_ndiff ) {
	var_list.push_back(var);
      }
    }
  }

  ymuint nall = var_list.size();

  vector<pair<ymuint, const Variable*> > pair_list;
  pair_list.reserve(nall);
  for (ymuint i = 0; i < nall; ++ i) {
    const Variable* var = var_list[i];
    ymuint ndiff = calc_ndiff(var, vect_list);
    pair_list.push_back(make_pair(ndiff, var));
  }
  sort(pair_list.begin(), pair_list.end(), PairLt());
  ymuint ncut = n * n;
  if ( ncut > nall ) {
    ncut = nall;
  }

  ymuint nlimit = 2000;
  ymuint p = rvmgr.index_size() - 1;
  ymuint exp_p = 1U << p;
  RandGen rg;
  RandCombiGen rcg(ncut, p);
  double ds_all = 0.0;
  ymuint ds_best = 0;
  for (ymuint c = 0; c < nlimit; ++ c) {
    rcg.generate(rg);
    vector<vector<ymuint32> > vars_list(p);

    for (ymuint i = 0; i < p; ++ i) {
      ymuint pos = rcg.elem(i);
      const Variable* var = var_list[pos];
      const vector<ymuint>& vid_list = var->vid_list();
      vars_list[i] = vid_list;
    }
    XorFunc xf(vars_list);

    ymuint ds = calc_ds(xf, exp_p, vect_list);
    if ( ds_all == 0 ) {
      ds_best = ds;
    }
    else {
      if ( ds_best > ds ) {
	ds_best = ds;
      }
    }
    ds_all += ds;
  }
  double ds_ave = ds_all / nlimit;
  ymuint nv = vect_list.size();
  double base = nv * (nv - 1);
  cout << (ds_ave / base) << endl;
  cout << (ds_best / base) << endl;

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
