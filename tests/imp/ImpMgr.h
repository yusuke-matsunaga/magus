#ifndef IMPMGR_H
#define IMPMGR_H

/// @file ImpMgr.h
/// @brief ImpMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

class StrNode;

//////////////////////////////////////////////////////////////////////
/// @class ImpMgr ImpMgr.h "ImpMgr.h"
/// @brief StrNode の含意操作を行うクラス
//////////////////////////////////////////////////////////////////////
class ImpMgr
{
public:

  /// @brief コンストラクタ
  ImpMgr();

  /// @brief デストラクタ
  ~ImpMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード番号の最大値+1を得る．
  ymuint
  max_node_id();

  /// @brief ノードを得る．
  /// @param[in] id ノード番号 ( 0 <= id < max_node_id() )
  /// @note 場合によっては NULL が返ることもある
  StrNode*
  node(ymuint id) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を空にする．
  void
  clear();

  /// @brief ネットワークを設定する．
  /// @param[in] src_network 元となるネットワーク
  void
  set(const BdnMgr& src_network);


public:
  //////////////////////////////////////////////////////////////////////
  // 含意操作を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードに値を設定し含意操作を行う．
  /// @param[in] node ノード
  /// @param[in] val 設定する値 ( 0 or 1 )
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  assert(StrNode* node,
	 ymuint val);

  /// @brief 現在のスタックポインタの値を得る．
  ymuint
  cur_sp() const;

  /// @brief 指定されたところまで値を戻す．
  /// @param[in] pos スタックポインタの値
  void
  back(ymuint pos);

  /// @brief ノードのファンアウト先に0を伝搬する．
  /// @param[in] node ノード
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fwd_prop0(StrNode* node);

  /// @brief ノードのファンアウト先に1を伝搬する．
  /// @param[in] node ノード
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fwd_prop1(StrNode* node);

  /// @brief ノードのファンイン0に0を伝搬する．
  /// @param[in] node ノード
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanin0_prop0(StrNode* node);

  /// @brief ノードのファンイン0に1を伝搬する．
  /// @param[in] node ノード
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanin0_prop1(StrNode* node);

  /// @brief ノードのファンイン1に0を伝搬する．
  /// @param[in] node ノード
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanin1_prop0(StrNode* node);

  /// @brief ノードのファンイン1に1を伝搬する．
  /// @param[in] node ノード
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanin1_prop1(StrNode* node);

  /// @brief ノードに後方含意で0を割り当てる．
  /// @param[in] node ノード
  /// @param[in] from_node 含意元のノード
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  bwd_prop0(StrNode* node,
	    StrNode* from_node);

  /// @brief ノードに後方含意で1を割り当てる．
  /// @param[in] node ノード
  /// @param[in] from_node 含意元のノード
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  bwd_prop1(StrNode* node,
	    StrNode* from_node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの値の変更履歴
  struct NodeChg
  {
    /// @brief ノード
    StrNode* mNode;
    /// @brief 変更前の状態
    ymuint32 mState;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードの配列
  vector<StrNode*> mNodeArray;

  // 値の変更履歴を記憶するスタック
  vector<NodeChg> mChgStack;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード番号の最大値+1を得る．
inline
ymuint
ImpMgr::max_node_id()
{
  return mNodeArray.size();
}

// @brief ノードを得る．
// @param[in] id ノード番号 ( 0 <= id < max_node_id() )
inline
StrNode*
ImpMgr::node(ymuint id) const
{
  assert( id < max_node_id(), __FILE__, __LINE__);
  return mNodeArray[id];
}

END_NAMESPACE_YM_NETWORKS

#endif // IMPMGR_H
