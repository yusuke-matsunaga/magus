#ifndef TPGCNF0_H
#define TPGCNF0_H

/// @file TpgCnf0.h
/// @brief TpgCnf0 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "SatEngine.h"
#include "GenVidMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgCnf0 TpgCnf0.h "TpgCnf0.h"
/// @brief 正常回路のCNFを表すクラス
//////////////////////////////////////////////////////////////////////
class TpgCnf0
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  TpgCnf0(const string& sat_type,
	  const string& sat_option,
	  ostream* sat_outp);

  /// @brief デストラクタ
  ~TpgCnf0();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 割当を満たすパタンを作る．
  /// @param[in] network ネットワーク
  /// @param[in] list 割当リスト
  /// @param[in] testvector パタン
  bool
  get_testvector(TpgNetwork& network,
		 const NodeValList& list,
		 TestVector* testvector);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 仮定を追加する．
  /// @param[in] assign_list 割当リスト
  void
  add_assumptions(const NodeValList& assign_list);

  /// @brief node とその TFI の部分の CNF を作る．
  void
  mark_dfs(const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SATエンジン
  SatEngine mEngine;

  // 正常値
  GenVidMap mGvarMap;

  // mark_dfs() で用いるマーク配列
  vector<bool> mMark;

};

END_NAMESPACE_YM_SATPG

#endif // TPGCNF0_H
