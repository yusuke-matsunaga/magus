#ifndef DTPGNETWORK_H
#define DTPGNETWORK_H

/// @file DtpgNetwork.h
/// @brief DtpgNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"
#include "SaFault.h"
#include "ym_YmNetworks/tgnet.h"
#include "ym_YmLogic/expr_nsdef.h"
#include "ym_YmUtils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgNetwork DtpgNetwork.h "DtpgNetwork.h"
/// @brief DtpgSat 用のネットワークを表すクラス
//////////////////////////////////////////////////////////////////////
class DtpgNetwork
{
public:

  /// @brief コンストラクタ
  /// @param[in] tgnetwork もとのネットワーク
  /// @param[in] fault_list 故障リスト
  DtpgNetwork(const TgNetwork& tgnetwork,
	      const vector<SaFault*>& fault_list);

  /// @brief デストラクタ
  ~DtpgNetwork();


public:
  //////////////////////////////////////////////////////////////////////
  // 通常の構造情報を得る関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を得る．
  ymuint
  node_num();

  /// @brief ノードを得る．
  /// @param[in] id ID番号 ( 0 <= id < node_num() )
  /// @note node->id() == id となるノードを返す．
  DtpgNode*
  node(ymuint id);

  /// @brief TgNode::gid() に対応したノードを得る．
  DtpgNode*
  node_from_gid(ymuint gid);

  /// @brief 外部入力数を得る．
  ymuint
  input_num();

  /// @brief 外部入力数 + FF数を得る．
  ymuint
  input_num2();

  /// @brief 外部入力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num2() )
  DtpgNode*
  input(ymuint pos);

  /// @brief 外部出力数を得る．
  ymuint
  output_num();

  /// @brief 外部出力数 + FF数を得る．
  ymuint
  output_num2();

  /// @brief 外部出力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < output_num2() )
  DtpgNode*
  output(ymuint pos);

  /// @brief サイズの降順で整列した順番で外部出力ノードを取り出す．
  DtpgNode*
  output2(ymuint pos);

  /// @brief SaFault に対応する DtpgFault を得る．
  DtpgFault*
  conv_fault(SaFault* src_fault);


public:
  //////////////////////////////////////////////////////////////////////
  // アクティブ領域に関するアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一つの外部出力に関係するノードのみをアクティブにする．
  /// @param[in] po_pos 出力番号
  void
  activate_po(ymuint po_pos);

  /// @brief 全てのノードをアクティブにする．
  void
  activate_all();

  /// @brief アクティブなノード数を得る．
  ymuint
  active_node_num();

  /// @brief アクティブなノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < active_node_num() )
  DtpgNode*
  active_node(ymuint pos);


public:
  //////////////////////////////////////////////////////////////////////
  // 必要割り当てに関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief f の検出に必要な割り当てを求める．
  /// @param[in] f 対象の故障
  /// @param[in] ma_list 割り当て結果を格納するリスト
  /// @return 矛盾が生じたら(fが冗長故障の場合) false を返す．
  /// @note DtpgNetwork のメンバにはアクセスしないので static メンバになっている．
  /// @note ma_list の内容は DtpgNode::id() * 2 + val (0 / 1)
  static
  bool
  get_mandatory_assignment(DtpgFault* f,
			   vector<ymuint32>& ma_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief DtpgNode の内容を設定する．
  /// @param[in] id ID番号
  /// @param[in] tgnode もととなる TgNode
  DtpgNode*
  make_node(ymuint id,
	    const TgNode* tgnode);

  /// @brief 複雑な論理式に対応するプリミティブを作る．
  /// @param[in] expr 論理式
  /// @param[in] tgnode もととなる TgNode
  /// @param[in] primitive_list プリミティブを設定する領域
  /// @param[inout] id プリミティブID
  /// @note id は内部でインクリメントされる．
  DtpgPrimitive*
  make_primitive(const Expr& expr,
		 const TgNode* tgnode,
		 DtpgPrimitive* primitive_list,
		 ymuint& id);

  /// @brief 入力プリミティブの設定を行なう．
  /// @param[in] prim プリミティブ
  /// @param[in] id 入力番号
  void
  set_input_primitive(DtpgPrimitive* prim,
		      ymuint id);

  /// @brief 論理プリミティブの設定を行なう．
  /// @param[in] prim プリミティブ
  /// @param[in] gate_type ゲートタイプ
  /// @param[in] ni 入力数
  void
  set_logic_primitive(DtpgPrimitive* prim,
		      tTgGateType gate_type,
		      ymuint ni);

  /// @brief ノードの TFI にマークをつける．
  /// @note 結果は mTmpMark[node->id()] に格納される．
  void
  dfs_mark(DtpgNode* node);

  /// @brief ノードの TFI のマークを消す．
  /// @note 結果は mTmpMark[node->id()] に格納される．
  void
  dfs_unmark(DtpgNode* node);

  /// @brief activate_po(), activate_all() の下請け関数
  void
  activate_sub();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // DtpgNetwork 関係のメモリ確保を行なうオブジェクト
  SimpleAlloc mAlloc;

  // 全ノード数
  ymuint32 mNodeNum;

  // 外部入力数
  ymuint32 mInputNum;

  // 外部出力数
  ymuint32 mOutputNum;

  // FF数
  ymuint32 mFFNum;

  // ノードの本体の配列
  DtpgNode* mNodeArray;

  // TgNode->gid() をキーにしたノードの配列
  DtpgNode** mNodeMap;

  // 外部入力ノードの配列
  DtpgNode** mInputArray;

  // 外部出力ノードの配列
  DtpgNode** mOutputArray;

  // TFI サイズの降順に整列した外部出力ノードの配列
  DtpgNode** mOutputArray2;

  // アクティブなノード数
  ymuint32 mActNodeNum;

  // アクティブなノードの配列
  DtpgNode** mActNodeArray;

  // activate_sub() で用いられるマーク
  // サイズは mNodeNum
  bool* mTmpMark;

#if 0
  // 関数の個数
  ymuint32 mFuncNum;
#endif

  // 故障の本体の配列
  DtpgFault* mFaultChunk;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード数を得る．
inline
ymuint
DtpgNetwork::node_num()
{
  return mNodeNum;
}

// @brief TgNode::gid() に対応したノードを得る．
inline
DtpgNode*
DtpgNetwork::node_from_gid(ymuint gid)
{
  return mNodeMap[gid];
}

// @brief 外部入力数を得る．
inline
ymuint
DtpgNetwork::input_num()
{
  return mInputNum;
}

// @brief 外部入力数 + FF数を得る．
inline
ymuint
DtpgNetwork::input_num2()
{
  return mInputNum + mFFNum;
}

// @brief 外部入力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < input_num2() )
inline
DtpgNode*
DtpgNetwork::input(ymuint pos)
{
  assert_cond( pos < input_num2(), __FILE__, __LINE__);
  return mInputArray[pos];
}

// @brief 外部出力数を得る．
inline
ymuint
DtpgNetwork::output_num()
{
  return mOutputNum;
}

// @brief 外部出力数 + FF数を得る．
inline
ymuint
DtpgNetwork::output_num2()
{
  return mOutputNum + mFFNum;
}

// @brief 外部出力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < output_num2() )
inline
DtpgNode*
DtpgNetwork::output(ymuint pos)
{
  assert_cond( pos < output_num2(), __FILE__, __LINE__);
  return mOutputArray[pos];
}

// @brief サイズの降順で整列した順番で外部出力ノードを取り出す．
inline
DtpgNode*
DtpgNetwork::output2(ymuint pos)
{
  assert_cond( pos < output_num2(), __FILE__, __LINE__);
  return mOutputArray2[pos];
}

// @brief アクティブなノード数を得る．
inline
ymuint
DtpgNetwork::active_node_num()
{
  return mActNodeNum;
}

// @brief アクティブなノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < active_node_num() )
inline
DtpgNode*
DtpgNetwork::active_node(ymuint pos)
{
  assert_cond( pos < mActNodeNum, __FILE__, __LINE__);
  return mActNodeArray[pos];
}

END_NAMESPACE_YM_SATPG_DTPG

#endif // DTPGNETWORK_H
