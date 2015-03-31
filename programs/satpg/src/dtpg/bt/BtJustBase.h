#ifndef BTJUSTBASE_H
#define BTJUSTBASE_H

/// @file BtJustBase.h
/// @brief BtJustBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtBase.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class BtJustBase BtJustBase.h "BtJustBase.h"
/// @brief 必要なノードのみ正当化する BackTracer の基底クラス
//////////////////////////////////////////////////////////////////////
class BtJustBase :
  public BtBase
{
public:

  /// @brief コンストラクタ
  BtJustBase();

  /// @brief デストラクタ
  virtual
  ~BtJustBase();


public:
  //////////////////////////////////////////////////////////////////////
  // Backtracer の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードID番号の最大値を設定する．
  /// @param[in] max_id ID番号の最大値
  virtual
  void
  set_max_id(ymuint max_id);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief justified マークをつけ，mJustifiedNodeList に加える．
  /// @param[in] node 対象のノード
  void
  set_justified(TpgNode* node);

  /// @brief justified マークを読む．
  /// @param[in] node 対象のノード
  bool
  justified_mark(TpgNode* node);

  /// @brief justified マークを消す．
  void
  clear_justified();

  /// @brief clear_justified() 中で呼ばれるフック関数
  /// @note デフォルトの実装はなにもしない．
  virtual
  void
  clear_justified_hook(TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 正当化されたノードのリスト
  vector<TpgNode*> mJustifiedNodeList;

  // 正当化マークの配列
  // インデックスは TpgNode::id()
  vector<bool> mJustifiedMarkArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief justified マークをつける．
// @param[in] node 対象のノード
inline
void
BtJustBase::set_justified(TpgNode* node)
{
  ASSERT_COND( node->id() < mJustifiedMarkArray.size() );
  mJustifiedMarkArray[node->id()] = true;
  mJustifiedNodeList.push_back(node);
}

// @brief justified マークを読む．
// @param[in] node 対象のノード
inline
bool
BtJustBase::justified_mark(TpgNode* node)
{
  ASSERT_COND( node->id() < mJustifiedMarkArray.size() );
  return mJustifiedMarkArray[node->id()];
}

END_NAMESPACE_YM_SATPG

#endif // BTJUST1_H
