#ifndef FUNCMGR_H
#define FUNCMGR_H

/// @file FuncMgr.h
/// @brief FuncMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/TvFunc.h"
#include "YmUtils/IDO.h"
#include "YmUtils/ODO.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class FuncMgr FuncMgr.h "FuncMgr.h"
/// @brief 論理関数を管理するためのクラス
//////////////////////////////////////////////////////////////////////
class FuncMgr
{
public:

  /// @brief コンストラクタ
  FuncMgr();

  /// @brief デストラクタ
  ~FuncMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 関数を登録する．
  /// @note すでに登録されていたらなにもしない．
  void
  reg_func(const TvFunc& f);

  /// @brief マージする．
  /// @param[in] src マージする他のマネージャ
  void
  merge(const FuncMgr& src);

  /// @brief 代表関数のリストを取り出す．
  void
  func_list(vector<TvFunc>& func_list) const;

  /// @brief 指定された入力数の代表関数のリストを取り出す．
  void
  func_list(ymuint ni,
	    vector<TvFunc>& func_list) const;


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリダンプ
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

  typedef unordered_set<TvFunc> FuncSet;

  // 代表関数のハッシュ
  FuncSet mRepHash;

};

END_NAMESPACE_YM

#endif // FUNCMGR_H
