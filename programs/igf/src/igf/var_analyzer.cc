
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

  vector<const Variable*>* var_list_array = new vector<const Variable*>[comp - 1];

  // Variable を生成
  for (ymuint i = 0; i < n; ++ i) {
    const Variable* var = new Variable(i);
    var_list_array[0].push_back(var);
    assert_cond( var->vid_list().size() == 1, __FILE__, __LINE__);
  }

  if ( comp >= 2 ) {
    VarHeap var_heap(n + slack);
    for (ymuint i1 = 0; i1 < n; ++ i1) {
      vector<ymuint> vid_list(2);
      vid_list[0] = i1;
      for (ymuint i2 = i1 + 1; i2 < n; ++ i2) {
	vid_list[1] = i2;
	const Variable* var = new Variable(vid_list);
	ymuint ndiff = calc_ndiff(var, vect_list);
	var_heap.put(var, ndiff);
      }
    }
    ymuint nv = var_heap.elem_num();
    for (ymuint i = 0; i < nv; ++ i) {
      const Variable* var = var_heap.elem(i);
      var_list_array[1].push_back(var);
      assert_cond( var->vid_list().size() == 2, __FILE__, __LINE__);
    }
  }

  for (ymuint comp_i = 3; comp_i <= comp; ++ comp_i) {
    VarHeap var_heap(n + slack);
    const vector<const Variable*>& var_list1 = var_list_array[comp_i - 2];
    ymuint nv1 = var_list1.size();
    for (ymuint i = 0; i < nv1; ++ i) {
      const Variable* var0 = var_list1[i];
      const vector<ymuint>& vid_list0 = var0->vid_list();
      assert_cond( vid_list0.size() == comp_i - 1, __FILE__, __LINE__);
      vector<ymuint> vid_list(comp_i);
      for (ymuint i = 0; i < comp_i - 1; ++ i) {
	vid_list[i] = vid_list0[i];
      }
      for (ymuint j = 0; j < n; ++ j) {
	vid_list[comp_i - 1] = j;
      }
      const Variable* var = new Variable(vid_list);
      ymuint ndiff = calc_ndiff(var, vect_list);
      var_heap.put(var, ndiff);
    }
    ymuint nv = var_heap.elem_num();
    for (ymuint i = 0; i < nv; ++ i) {
      const Variable* var = var_heap.elem(i);
      var_list_array[comp_i - 1].push_back(var);
      assert_cond( var->vid_list().size() == comp_i, __FILE__, __LINE__);
    }
  }

  vector<const Variable*> var_all_list;
  for (ymuint i = 0; i < comp; ++ i) {
    ymuint nv = var_list_array[i].size();
    for (ymuint j = 0; j < nv; ++ j) {
      var_all_list.push_back(var_list_array[i][j]);
    }
  }
  ymuint nall = var_all_list.size();

  ymuint nlimit = 2000;
  ymuint p = rvmgr.index_size();
  RandGen rg;
  RandCombiGen rcg(nall, p);
  for (ymuint c = 0; c < nlimit; ++ c) {
    rcg.generate(rg);
    vector<vector<ymuint32> > vars_list(p);
    for (ymuint i = 0; i < p; ++ i) {
      ymuint pos = rcg.elem(i);
      const Variable* var = var_all_list[pos];
      const vector<ymuint>& vid_list = var->vid_list();
      vars_list[i] = vid_list;
    }
    XorFunc xf(vars_list);

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
