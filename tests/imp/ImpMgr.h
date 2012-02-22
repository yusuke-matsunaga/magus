#ifndef IMPMGR_H
#define IMPMGR_H

/// @file ImpMgr.h
/// @brief ImpMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"
#include "ImpInfo.h"


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
  max_node_id() const;

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
  /// @param[out] imp_list 含意の結果を格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  assert(StrNode* node,
	 ymuint val,
	 vector<ImpCell>& imp_list);

  /// @brief 現在のスタックポインタの値を得る．
  ymuint
  cur_sp() const;

  /// @brief 指定されたところまで値を戻す．
  void
  backtrack();

  /// @brief ノードのファンアウト先に0を伝搬する．
  /// @param[in] node ノード
  /// @param[out] imp_list 含意の結果を格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fwd_prop0(StrNode* node,
	    vector<ImpCell>& imp_list);

  /// @brief ノードのファンアウト先に1を伝搬する．
  /// @param[in] node ノード
  /// @param[out] imp_list 含意の結果を格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fwd_prop1(StrNode* node,
	    vector<ImpCell>& imp_list);

  /// @brief ノードのファンイン0に0を伝搬する．
  /// @param[in] node ノード
  /// @param[out] imp_list 含意の結果を格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanin0_prop0(StrNode* node,
	       vector<ImpCell>& imp_list);

  /// @brief ノードのファンイン0に1を伝搬する．
  /// @param[in] node ノード
  /// @param[out] imp_list 含意の結果を格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanin0_prop1(StrNode* node,
	       vector<ImpCell>& imp_list);

  /// @brief ノードのファンイン1に0を伝搬する．
  /// @param[in] node ノード
  /// @param[out] imp_list 含意の結果を格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanin1_prop0(StrNode* node,
	       vector<ImpCell>& imp_list);

  /// @brief ノードのファンイン1に1を伝搬する．
  /// @param[in] node ノード
  /// @param[out] imp_list 含意の結果を格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanin1_prop1(StrNode* node,
	       vector<ImpCell>& imp_list);

  /// @brief ノードに後方含意で0を割り当てる．
  /// @param[in] node ノード
  /// @param[in] from_node 含意元のノード
  /// @param[out] imp_list 含意の結果を格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  bwd_prop0(StrNode* node,
	    StrNode* from_node,
	    vector<ImpCell>& imp_list);

  /// @brief ノードに後方含意で1を割り当てる．
  /// @param[in] node ノード
  /// @param[in] from_node 含意元のノード
  /// @param[out] imp_list 含意の結果を格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  bwd_prop1(StrNode* node,
	    StrNode* from_node,
	    vector<ImpCell>& imp_list);

  /// @brief ノードの値をスタックに積む．
  /// @param[in] node ノード
  /// @param[in] old_state 変更前の値
  void
  save_value(StrNode* node,
	     ymuint32 old_state);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの値の変更履歴
  struct NodeChg
  {
    /// @brief コンストラクタ
    NodeChg(StrNode* node = NULL,
	    ymuint32 state = 0U);

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

  // マーカーのスタック
  vector<ymuint32> mMarkerStack;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード番号の最大値+1を得る．
inline
ymuint
ImpMgr::max_node_id() const
{
  return mNodeArray.size();
}

// @brief ノードを得る．
// @param[in] id ノード番号 ( 0 <= id < max_node_id() )
inline
StrNode*
ImpMgr::node(ymuint id) const
{
  assert_cond( id < max_node_id(), __FILE__, __LINE__);
  return mNodeArray[id];
}

// @brief コンストラクタ
inline
ImpMgr::NodeChg::NodeChg(StrNode* node,
			 ymuint32 state) :
  mNode(node),
  mState(state)
{
}

END_NAMESPACE_YM_NETWORKS

#endif // IMPMGR_H
