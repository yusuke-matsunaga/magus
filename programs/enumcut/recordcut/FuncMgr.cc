
/// @file FuncMgr.cc
/// @brief FuncMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "FuncMgr.h"
#include "YmLogic/TvFunc.h"
#include "YmLogic/NpnMgr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス FuncMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncMgr::FuncMgr()
{
}

// @brief デストラクタ
FuncMgr::~FuncMgr()
{
}

// @brief 内容をクリアする．
void
FuncMgr::clear()
{
  mRepHash.clear();
}

// @brief 関数を登録する．
// @note すでに登録されていたらなにもしない．
void
FuncMgr::reg_func(const TvFunc& f)
{
  NpnMgr npn_mgr;
  NpnMap cmap;
  npn_mgr.cannonical(f, cmap);
  TvFunc rep = f.xform(cmap);
  mRepHash.insert(rep);
}

// @brief マージする．
// @param[in] src マージする他のマネージャ
void
FuncMgr::merge(const FuncMgr& src)
{
  for (FuncSet::const_iterator p = src.mRepHash.begin();
       p != src.mRepHash.end(); ++ p) {
    mRepHash.insert(*p);
  }
}

// @brief 代表関数のリストを取り出す．
void
FuncMgr::func_list(vector<TvFunc>& func_list) const
{
  func_list.clear();
  func_list.reserve(mRepHash.size());
  for (FuncSet::const_iterator p = mRepHash.begin();
       p != mRepHash.end(); ++ p) {
    func_list.push_back(*p);
  }
}

// @brief 指定された入力数の代表関数のリストを取り出す．
void
FuncMgr::func_list(ymuint ni,
		   vector<TvFunc>& func_list) const
{
  // 数を数える．
  ymuint n = 0;
  for (FuncSet::const_iterator p = mRepHash.begin();
       p != mRepHash.end(); ++ p) {
    const TvFunc& f = *p;
    if ( f.input_num() == ni ) {
      ++ n;
    }
  }
  func_list.clear();
  func_list.reserve(n);
  for (FuncSet::const_iterator p = mRepHash.begin();
       p != mRepHash.end(); ++ p) {
    const TvFunc& f = *p;
    if ( f.input_num() == ni ) {
      func_list.push_back(f);
    }
  }
}

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
void
FuncMgr::dump(ODO& s) const
{
  ymuint32 n = mRepHash.size();
  s << n;
  for (FuncSet::const_iterator p = mRepHash.begin();
       p != mRepHash.end(); ++ p) {
    const TvFunc& f = *p;
    s << f;
  }
}

// @brief バイナリダンプされたファイルを読み込む．
void
FuncMgr::restore(IDO& s)
{
  clear();
  ymuint32 n;
  s >> n;

  for (ymuint i = 0; i < n; ++ i) {
    TvFunc f;
    s >> f;
    mRepHash.insert(f);
  }
}

END_NAMESPACE_YM
