#ifndef IMPMGR_H
#define IMPMGR_H

/// @file ImpMgr.h
/// @brief ImpMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BNetwork.h"
#include "ym_networks/BdnMgr.h"
#include "ImpNode.h"
#include "ImpVal.h"
#include "BNodeMap.h"
#include "ym_utils/RandGen.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpRec;

//////////////////////////////////////////////////////////////////////
/// @class ImpMgr ImpMgr.h "ImpMgr.h"
/// @brief ImpNode の含意操作を行うクラス
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

  /// @brief ノード数 (ノード番号の最大値+1) を得る
  ymuint
  node_num() const;

  /// @brief ノードを取り出す．
  /// @param[in] id ノード番号
  ImpNode*
  node(ymuint id) const;

  /// @brief 入力数を得る．
  ymuint
  input_num() const;

  /// @brief 入力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  ImpNode*
  input_node(ymuint pos) const;

  /// @brief id 番めのノードが定数かどうか調べる．
  bool
  is_const(ymuint id) const;

  /// @brief id 番めのノードが定数かどうか調べる．
  bool
  is_const0(ymuint id) const;

  /// @brief id 番めのノードが定数かどうか調べる．
  bool
  is_const1(ymuint id) const;

  /// @brief トポロジカル順のノードリストを得る．
  /// @param[out] node_list 結果を格納する変数
  void
  get_node_list(vector<ImpNode*>& node_list) const;

  /// @brief BNode との対応付けの情報を返す．
  const BNodeMap&
  bnodemap() const;

  /// @brief 内容を書き出す．
  void
  print_network(ostream& s) const;


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
  set(const BNetwork& src_network);

  /// @brief ネットワークを設定する．
  /// @param[in] src_network 元となるネットワーク
  void
  set(const BdnMgr& src_network);

  /// @brief 論理式に対応したノードの木を作る．
  ImpNodeHandle
  make_tree(const LogExpr& expr,
	    const vector<ImpNodeHandle>& fanins);

  /// @brief AND ノードの木を作る．
  /// @param[in] fanins 葉のノードの配列
  /// @param[in] begin 開始位置
  /// @param[in] end 終了位置
  ImpNodeHandle
  make_and(const vector<ImpNodeHandle>& fanins,
	   ymuint begin,
	   ymuint end);

  /// @brief XOR ノードの木を作る．
  /// @param[in] fanins 葉のノードの配列
  /// @param[in] begin 開始位置
  /// @param[in] end 終了位置
  ImpNodeHandle
  make_xor(const vector<ImpNodeHandle>& fanins,
	   ymuint begin,
	   ymuint end);

  /// @brief 入力ノードを作る．
  ImpNode*
  new_input();

  /// @brief ANDノードを作る．
  /// @param[in] handle0 ファンイン0のハンドル
  /// @param[in] handle1 ファンイン1のハンドル
  ImpNode*
  new_and(ImpNodeHandle handle0,
	  ImpNodeHandle handle1);

  /// @brief 定数の印をつける．
  /// @param[in] id ノード番号
  /// @param[in] val 値 ( 0 or 1 )
  void
  set_const(ymuint id,
	    ymuint val);


public:
  //////////////////////////////////////////////////////////////////////
  // ImpRec を用いた含意関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードに値を設定し含意操作を行う．
  /// @param[in] node ノード
  /// @param[in] val 設定する値 ( 0 or 1 )
  /// @param[in] rec 含意を記録するオブジェクト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  assert(ImpNode* node,
	 ymuint val,
	 ImpRec& rec);

  /// @brief ノードのファンアウト先に0を伝搬する．
  /// @param[in] node ノード
  /// @param[in] from 後方含意の場合の含意元のノード
  /// @param[in] rec 含意を記録するオブジェクト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanout_prop0(ImpNode* node,
	       ImpNode* from,
	       ImpRec& rec);

  /// @brief ノードのファンアウト先に1を伝搬する．
  /// @param[in] node ノード
  /// @param[in] from 後方含意の場合の含意元のノード
  /// @param[in] rec 含意を記録するオブジェクト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanout_prop1(ImpNode* node,
	       ImpNode* from,
	       ImpRec& rec);

  /// @brief ノードのファンイン0に0を伝搬する．
  /// @param[in] node ノード
  /// @param[in] rec 含意を記録するオブジェクト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanin0_prop0(ImpNode* node,
	       ImpRec& rec);

  /// @brief ノードのファンイン0に1を伝搬する．
  /// @param[in] node ノード
  /// @param[in] rec 含意を記録するオブジェクト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanin0_prop1(ImpNode* node,
	       ImpRec& rec);

  /// @brief ノードのファンイン1に0を伝搬する．
  /// @param[in] node ノード
  /// @param[in] rec 含意を記録するオブジェクト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanin1_prop0(ImpNode* node,
	       ImpRec& rec);

  /// @brief ノードのファンイン1に1を伝搬する．
  /// @param[in] node ノード
  /// @param[in] rec 含意を記録するオブジェクト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanin1_prop1(ImpNode* node,
	       ImpRec& rec);

  /// @brief ノードに後方含意で0を割り当てる．
  /// @param[in] node ノード
  /// @param[in] from_node 含意元のノード
  /// @param[in] rec 含意を記録するオブジェクト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  bwd_prop0(ImpNode* node,
	    ImpNode* from_node,
	    ImpRec& rec);

  /// @brief ノードに後方含意で1を割り当てる．
  /// @param[in] node ノード
  /// @param[in] from_node 含意元のノード
  /// @param[in] rec 含意を記録するオブジェクト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  bwd_prop1(ImpNode* node,
	    ImpNode* from_node,
	    ImpRec& rec);

  /// @brief unjustified ノードを得る．
  void
  get_unodelist(vector<ImpNode*>& unode_list);

  /// @brief ノードが unjustified になったときの処理を行なう．
  void
  set_unjustified(ImpNode* ndoe);

  /// @brief ノードが unjustified でなくなったときの処理を行なう．
  void
  reset_unjustified(ImpNode* node);

  /// @brief ラーニング結果を各ノードに設定する．
  void
  set_ind_imp(ImpNode* node,
	      ymuint val,
	      const vector<ImpVal>& imp_list);


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
  assert(ImpNode* node,
	 ymuint val);

  /// @brief 指定されたところまで値を戻す．
  void
  backtrack();

  /// @brief ノードの値をスタックに積む．
  /// @param[in] node ノード
  /// @param[in] old_state 変更前の値
  void
  save_value(ImpNode* node,
	     ymuint32 old_state);

  /// @brief ノードのファンアウト先に0を伝搬する．
  /// @param[in] node ノード
  /// @param[in] from 後方含意の場合の含意元のノード
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanout_prop0(ImpNode* node,
	       ImpNode* from);

  /// @brief ノードのファンアウト先に1を伝搬する．
  /// @param[in] node ノード
  /// @param[in] from 後方含意の場合の含意元のノード
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanout_prop1(ImpNode* node,
	       ImpNode* from);

  /// @brief ノードのファンイン0に0を伝搬する．
  /// @param[in] node ノード
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanin0_prop0(ImpNode* node);

  /// @brief ノードのファンイン0に1を伝搬する．
  /// @param[in] node ノード
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanin0_prop1(ImpNode* node);

  /// @brief ノードのファンイン1に0を伝搬する．
  /// @param[in] node ノード
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanin1_prop0(ImpNode* node);

  /// @brief ノードのファンイン1に1を伝搬する．
  /// @param[in] node ノード
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanin1_prop1(ImpNode* node);

  /// @brief ノードに後方含意で0を割り当てる．
  /// @param[in] node ノード
  /// @param[in] from_node 含意元のノード
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  bwd_prop0(ImpNode* node,
	    ImpNode* from_node);

  /// @brief ノードに後方含意で1を割り当てる．
  /// @param[in] node ノード
  /// @param[in] from_node 含意元のノード
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  bwd_prop1(ImpNode* node,
	    ImpNode* from_node);


public:
  //////////////////////////////////////////////////////////////////////
  // シミュレーション関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ランダムシミュレーションを行なう．
  void
  random_sim();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードに BNode-ID を割り当てる．
  void
  set_bnode_id(ImpNode* node,
	       bool inv,
	       ymuint bnode_id);

  /// @brief ノードを登録する．
  void
  reg_node(ImpNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの値の変更履歴
  struct NodeChg
  {
    /// @brief コンストラクタ
    NodeChg(ImpNode* node = NULL,
	    ymuint32 state = 0U);

    /// @brief ノード
    ImpNode* mNode;
    /// @brief 変更前の状態
    ymuint32 mState;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力の配列
  vector<ImpNode*> mInputArray;

  // トポロジカル順に並べたANDノードのリスト
  vector<ImpNode*> mNodeList;

  // ImpNode のID番号をキーにしたノードの配列
  vector<ImpNode*> mNodeArray;

  // BNode と ImpNode の対応付けの情報を持つオブジェクト
  BNodeMap mBNodeMap;

  // 値の変更履歴を記憶するスタック
  vector<NodeChg> mChgStack;

  // マーカーのスタック
  vector<ymuint32> mMarkerStack;

  // unjustified ノードのリスト
  list<ImpNode*> mUnodeList;

  // ランダムシミュレーション用の乱数発生器
  RandGen mRandGen;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 入力数を得る．
inline
ymuint
ImpMgr::input_num() const
{
  return mInputArray.size();
}

// @brief 入力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
inline
ImpNode*
ImpMgr::input_node(ymuint pos) const
{
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return mInputArray[pos];
}

// @brief ノード数 (ノード番号の最大値+1) を得る
inline
ymuint
ImpMgr::node_num() const
{
  return mNodeArray.size();
}

// @brief ノードを取り出す．
// @param[in] id ノード番号
inline
ImpNode*
ImpMgr::node(ymuint id) const
{
  assert_cond( id < mNodeArray.size(), __FILE__, __LINE__);
  assert_cond( mNodeArray[id]->id() == id, __FILE__, __LINE__);
  return mNodeArray[id];
}

// @brief id 番めのノードが定数かどうか調べる．
inline
bool
ImpMgr::is_const(ymuint id) const
{
  return node(id)->is_const();
}

// @brief id 番めのノードが定数かどうか調べる．
inline
bool
ImpMgr::is_const0(ymuint id) const
{
  return node(id)->is_const0();
}

// @brief id 番めのノードが定数かどうか調べる．
inline
bool
ImpMgr::is_const1(ymuint id) const
{
  return node(id)->is_const1();
}

// @brief トポロジカル順のノードリストを得る．
// @param[out] node_list 結果を格納する変数
inline
void
ImpMgr::get_node_list(vector<ImpNode*>& node_list) const
{
  node_list = mNodeList;
}

// @brief BNode との対応付けの情報を返す．
inline
const BNodeMap&
ImpMgr::bnodemap() const
{
  return mBNodeMap;
}

// @brief コンストラクタ
inline
ImpMgr::NodeChg::NodeChg(ImpNode* node,
			 ymuint32 state) :
  mNode(node),
  mState(state)
{
}

END_NAMESPACE_YM_NETWORKS

#endif // IMPMGR_H
