#ifndef CUTMGR_H
#define CUTMGR_H

/// @file CutMgr.h
/// @brief CutMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "networks/bdn.h"
#include "logic/AigMgr.h"
#include "logic/TvFunc.h"
#include "logic/NpnMgr.h"
#include "utils/IDO.h"
#include "utils/ODO.h"
#include "CutHash.h"


BEGIN_NAMESPACE_YM

class Cut;

//////////////////////////////////////////////////////////////////////
/// @class CutMgr CutMgr.h "CutMgr.h"
/// @brief Cut を管理するクラス
//////////////////////////////////////////////////////////////////////
class CutMgr
{
public:

  /// @brief コンストラクタ
  CutMgr();

  /// @brief デストラクタ
  ~CutMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  clear();

  /// @brief カットを登録する．
  /// @note すでの同じ関数のカットがあればそれを返す．
  Cut*
  new_cut(const BdnNode* root,
	  ymuint ni,
	  const BdnNode** inputs);

  /// @brief 全カットのリストを返す．
  const list<Cut*>&
  cut_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリファイルIO
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  void
  dump(ODO& s) const;

  /// @brief バイナリダンプされたファイルを読み込む．
  void
  restore(IDO& s);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // NpnMgr
  NpnMgr mNpnMgr;

  // AigMgr
  AigMgr mAigMgr;

  CutHash mCutHash;

  typedef unordered_set<TvFunc> FuncSet;

  // 関数のハッシュ表
  FuncSet mAllFunc;

  // 代表関数のハッシュ表
  FuncSet mRepFunc;

  // 全カットのリスト
  list<Cut*> mCutList;

};

END_NAMESPACE_YM

#endif // CUTMGR_H
