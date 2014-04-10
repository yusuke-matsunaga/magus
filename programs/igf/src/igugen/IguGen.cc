
/// @file IguGen.cc
/// @brief IguGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IguGen.h"
#include "PhfGen.h"
#include "InputFunc.h"
#include "RegVect.h"
#include "FuncVect.h"


BEGIN_NAMESPACE_YM_IGF

// @brief コンストラクタ
IguGen::IguGen()
{
}

// @brief デストラクタ
IguGen::~IguGen()
{
}

// @brief 登録ベクタを読み込む．
// @param[in] s 読み込み元のストリーム演算子
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
IguGen::read_data(istream& s)
{
  return mRvMgr.read_data(s);
}

// @brief ベクタのサイズを得る．
//
// ベクタのサイズとはベクタのビット長
ymuint
IguGen::vect_size() const
{
  return mRvMgr.vect_size();
}

// @brief ベクタのリストを得る．
const vector<const RegVect*>&
IguGen::vect_list() const
{
  return mRvMgr.vect_list();
}

// @brief インデックスのサイズを得る．
//
// インデックスのサイズとはインデックスを2進符号化するのに
// 必要なビット数 = ceil(log (k + 1)): k はベクタ数
ymuint
IguGen::index_size() const
{
  return mRvMgr.index_size();
}

// @brief conflict free 分割法で構成する．
// @param[in] h_funcs 入力ハッシュ関数のリスト
// @param[out] map_list IGUごとのベクタ番号のリスト
// @retval true 構成できた．
// @retval false 構成が失敗した．
bool
IguGen::cfp(const vector<InputFunc*>& h_funcs,
	    vector<vector<ymuint> >& map_list) const
{
  const vector<const RegVect*>& vlist = mRvMgr.vect_list();
  ymuint nv = vlist.size();
  ymuint m = h_funcs.size();
  ymuint p = h_funcs[0]->output_width();
  ymuint exp_p = 1U << p;

  vector<const FuncVect*> func_list(m);
  for (ymuint i = 0; i < m; ++ i) {
    InputFunc* f = h_funcs[i];
    FuncVect* fv = new FuncVect(nv, exp_p);
    for (ymuint v = 0; v < nv; ++ v) {
      const RegVect* rv = vlist[v];
      fv->set_val(v, f->eval(rv));
    }
    func_list[i] = fv;
  }

  vector<ymuint> block_map(nv);
  PhfGen pg;
  bool stat = pg.cf_partition(func_list, block_map);
  if ( stat ) {
    map_list.clear();
    for (ymuint i = 0; i < m; ++ i) {
      map_list.push_back(vector<ymuint>(exp_p));
    }
    for (ymuint i = 0; i < nv; ++ i) {
      const RegVect* rv = vlist[i];
      ymuint b = block_map[i];
      map_list[b][h_funcs[b]->eval(rv)] = i;
    }
    return true;
  }
  return false;
}

END_NAMESPACE_YM_IGF
