#ifndef BTJUST1_H
#define BTJUST1_H

/// @file BtJust1.h
/// @brief BtJust1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "BtJustBase.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class BtJust1 BtJust1.h "BtJust1.h"
/// @brief 必要なノードのみ正当化する BackTracer
//////////////////////////////////////////////////////////////////////
class BtJust1 :
  public BtJustBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_id ノードの最大 ID + 1 ( = TpgNetwork::node_num() )
  BtJust1(ymuint max_id);

  /// @brief デストラクタ
  virtual
  ~BtJust1();


public:
  //////////////////////////////////////////////////////////////////////
  // BackTracer の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バックトレースを行なう．
  /// @param[in] fnode 故障のあるノード
  /// @param[in] input_list テストパタンに関係のある入力のリスト
  /// @param[in] output_list 故障伝搬の可能性のある出力のリスト
  void
  operator()(TpgNode* fnode,
	     const vector<TpgNode*>& input_list,
	     const vector<TpgNode*>& output_list);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief solve 中で変数割り当ての正当化を行なう．
  /// @param[in] node 対象のノード
  /// @note node の値割り当てを正当化する．
  /// @note 正当化に用いられているノードには mark3 がつく．
  /// @note mark3 がついたノードは mBwdNodeList に格納される．
  void
  justify(TpgNode* node);

  /// @brief すべてのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  void
  just_sub1(TpgNode* node);

  /// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] val 値
  void
  just_sub2(TpgNode* node,
	    Bool3 val);

  /// @brief justify の下請け関数
  /// @param[in] prim 対象のプリミティブ
  /// @param[in] node 対象のノード
  /// @note node の値割り当てを正当化する．
  /// @note 正当化に用いられているノードには mark3 がつく．
  /// @note mark3 がついたノードは mBwdNodeList に格納される．
  void
  justify_primitive(TpgPrimitive* prim,
		    TpgNode* node);

  /// @brief すべてのファンインに対して justify_primitive() を呼ぶ．
  /// @param[in] prim 対象のプリミティブ
  /// @param[in] node 対象のノード
  void
  jp_sub1(TpgPrimitive* prim,
	  TpgNode* node);

  /// @brief 指定した値を持つファンインに対して justify_primitive() を呼ぶ．
  /// @param[in] prim 対象のプリミティブ
  /// @param[in] node 対象のノード
  /// @param[in] val 値
  void
  jp_sub2(TpgPrimitive* prim,
	  TpgNode* node,
	  Bool3 val);

};

END_NAMESPACE_YM_SATPG

#endif // BTJUST1_H
