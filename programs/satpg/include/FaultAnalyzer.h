#ifndef FAULTANALYZER_H
#define FAULTANALYZER_H

/// @file FaultAnalyzer.h
/// @brief FaultAnalyzer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "SatEngine.h"
#include "GenVidMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FaultAnalyzer FaultAnalyzer.h "FaultAnalyzer.h"
/// @brief 故障解析器
//////////////////////////////////////////////////////////////////////
class FaultAnalyzer
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  FaultAnalyzer(const string& sat_type,
		const string& sat_option,
		ostream* sat_outp);

  /// @brief デストラクタ
  ~FaultAnalyzer();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  init(ymuint max_id);

  /// @brief 正常回路のCNFを作る．
  /// @param[in] network 対象のネットワーク
  void
  make_gval_cnf(TpgNetwork& network);

  /// @brief 正常回路のCNFのもとで2つの割当が両立するか調べる．
  /// @param[in] list1, list2 割当リスト
  bool
  check_intersect(const NodeValList& list1,
		  const NodeValList& list2);

  /// @brief 正常回路のCNFのもとで2つの割当が矛盾するか調べる．
  /// @param[in] list1, list2 割当リスト
  bool
  check_conflict(const NodeValList& list1,
		 const NodeValList& list2);

  /// @brief 正常回路と故障回路のCNFを作る．
  /// @param[in] network 対象のネットワーク
  /// @param[in] fault 故障
  void
  make_fval_cnf(TpgNetwork& network,
		TpgFault* fault);

  /// @brief 故障回路のCNFのもとで割当が両立するか調べる．
  /// @param[in] list 割当リスト
  bool
  check_intersect(const NodeValList& list);

  /// @brief 故障回路のCNFのもとで割当が矛盾するか調べる．
  /// @param[in] list 割当リスト
  bool
  check_conflict(const NodeValList& list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 仮定を追加する．
  /// @param[in] assign_list 割当リスト
  void
  add_assumptions(const NodeValList& assign_list);

  /// @brief ノードのCNFを作る．
  void
  make_gval_cnf(const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SATエンジン
  SatEngine mEngine;

  // 正常値
  GenVidMap mGvarMap;

  // 故障値
  GenVidMap mFvarMap;

  // 故障伝搬値
  GenVidMap mDvarMap;

  // 正常回路のCNFを作ってあるノードのマーク
  vector<bool> mNodeMark;

};

END_NAMESPACE_YM_SATPG

#endif // FAULTANALYZER_H
