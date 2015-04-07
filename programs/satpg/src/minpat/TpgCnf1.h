#ifndef TPGCNF1_H
#define TPGCNF1_H

/// @file TpgCnf1.h
/// @brief TpgCnf1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "SatEngine.h"
#include "NodeSet.h"
#include "GenVidMap.h"
#include "BackTracer.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgCnf1 TpgCnf1.h "TpgCnf1.h"
/// @brief 単一の故障解析器
//////////////////////////////////////////////////////////////////////
class TpgCnf1
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  TpgCnf1(const string& sat_type,
	  const string& sat_option,
	  ostream* sat_outp);

  /// @brief デストラクタ
  ~TpgCnf1();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常回路と故障回路のCNFを作る．
  /// @param[in] fault 故障
  /// @param[in] max_id ノードの最大番号
  void
  make_fval_cnf(TpgFault* fault,
		ymuint max_id);

  /// @brief 故障回路のCNFのもとで割当が両立するか調べる．
  /// @param[in] list 割当リスト
  bool
  check_intersect(const NodeValList& list);

  /// @brief 故障回路のCNFのもとで割当が矛盾するか調べる．
  /// @param[in] list 割当リスト
  bool
  check_conflict(const NodeValList& list);

  /// @brief 故障回路のCNFのもとで割当が両立するか調べる．
  /// @param[in] src_list もとの割当リスト
  /// @param[in] new_list 新しい割当リスト
  bool
  get_suf_list(const NodeValList& src_list,
	       NodeValList& new_list);

  /// @brief 故障を検出する十分割当を求める．
  /// @param[out] suf_list 十分割当
  /// @param[out] pi_suf_list 外部入力のみの十分割当
  bool
  get_suf_list2(NodeValList& suf_list,
		NodeValList& pi_suf_list);


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

  // ノード番号の最大値
  ymuint mMaxId;

  // 正常値
  GenVidMap mGvarMap;

  // 故障値
  GenVidMap mFvarMap;

  // 故障伝搬値
  GenVidMap mDvarMap;

  // 関係のあるノード集合
  NodeSet mNodeSet;

  // 正常回路のCNFを作ってあるノードのマーク
  vector<bool> mNodeMark;

  // 故障
  TpgFault* mFault;

  // バックトレーサー
  BackTracer* mBackTracer;

};

END_NAMESPACE_YM_SATPG

#endif // TPGCNF1_H
