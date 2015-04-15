#ifndef GVALCNF_H
#define GVALCNF_H

/// @file GvalCnf.h
/// @brief GvalCnf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "GenVidMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class GvalCnf GvalCnf.h "GvalCnf.h"
/// @brief 正常回路のCNF式を作るためのクラス
//////////////////////////////////////////////////////////////////////
class GvalCnf
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_node_id ノード番号の最大値
  GvalCnf(ymuint max_node_id);

  /// @brief デストラクタ
  ~GvalCnf();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @param[in] max_node_id ノード番号の最大値
  void
  init(ymuint max_node_id);

  /// @brief NodeSet の内容に応じてCNFを作る．
  /// @param[in] engine SATエンジン
  /// @param[in] node_set 対象のノード集合
  ///
  /// 具体的には tfi_tfo_node() を対象にする．
  void
  make_cnf(SatEngine& engine,
	   const NodeSet& node_set);

  /// @brief ノードのTFI全体のCNFを作る．
  /// @param[in] engine SATエンジン
  /// @param[in] node ノード
  void
  make_cnf(SatEngine& engine,
	   const TpgNode* node);

  /// @brief 割当リストに対応する仮定を追加する．
  /// @param[in] engine SATエンジン
  /// @param[in] assign_list 割当リスト
  void
  add_assumption(SatEngine& engine,
		 const NodeValList& assign_list);

  /// @brief 変数マップを得る．
  const VidMap&
  var_map() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号の最大値
  ymuint mMaxId;

  // 処理済みのノードの印
  vector<bool> mMark;

  // 変数マップ
  GenVidMap mVarMap;

};

END_NAMESPACE_YM_SATPG

#endif // GVALCNF_H
