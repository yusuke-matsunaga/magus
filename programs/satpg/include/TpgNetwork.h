#ifndef TPGNETWORK_H
#define TPGNETWORK_H

/// @file TpgNetwork.h
/// @brief TpgNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "ym_networks/tgnet.h"
#include "ym_cell/cell_nsdef.h"
#include "ym_logic/lexp_nsdef.h"
#include "ym_utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgNetwork TpgNetwork.h "TpgNetwork.h"
/// @brief SATPG 用のネットワークを表すクラス
//////////////////////////////////////////////////////////////////////
class TpgNetwork
{
public:
  //////////////////////////////////////////////////////////////////////
  // ファイルを読み込んでインスタンスを作るクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief blif ファイルを読み込んでインスタンスを作る．
  /// @param[in] filename ファイル名
  /// @param[in] cell_library セルライブラリ
  /// @note エラーが起こったら NULL を返す．
  static
  TpgNetwork*
  read_blif(const string& filename,
	    const CellLibrary* cell_library);

  /// @brief iscas89 形式のファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @note エラーが起こったら NULL を返す．
  static
  TpgNetwork*
  read_iscas89(const string& filename);

  /// @brief デストラクタ
  ~TpgNetwork();


public:
  //////////////////////////////////////////////////////////////////////
  // 通常の構造情報を得る関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief ノードを得る．
  /// @param[in] id ID番号 ( 0 <= id < node_num() )
  /// @note node->id() == id となるノードを返す．
  TpgNode*
  node(ymuint id);

  /// @brief ノードを得る．
  /// @param[in] id ID番号 ( 0 <= id < node_num() )
  /// @note node->id() == id となるノードを返す．
  const TpgNode*
  node(ymuint id) const;

  /// @brief 外部入力数を得る．
  ymuint
  input_num() const;

  /// @brief 外部入力数 + FF数を得る．
  ymuint
  input_num2() const;

  /// @brief 外部入力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num2() )
  TpgNode*
  input(ymuint pos) const;

  /// @brief 外部出力数を得る．
  ymuint
  output_num() const;

  /// @brief 外部出力数 + FF数を得る．
  ymuint
  output_num2() const;

  /// @brief 外部出力ノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < output_num2() )
  TpgNode*
  output(ymuint pos) const;

  /// @brief サイズの降順で整列した順番で外部出力ノードを取り出す．
  TpgNode*
  output2(ymuint pos) const;


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
  active_node_num() const;

  /// @brief アクティブなノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < active_node_num() )
  TpgNode*
  active_node(ymuint pos);

  /// @brief アクティブなノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < active_node_num() )
  const TpgNode*
  active_node(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 故障に関するアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 全故障数を返す．
  ymuint
  all_fault_num() const;

  /// @brief 故障を取り出す．
  /// @param[in] id 故障番号 ( 0 <= id < all_fault_num() )
  TpgFault*
  fault(ymuint id);

  /// @brief 故障を取り出す．
  /// @param[in] id 故障番号 ( 0 <= id < all_fault_num() )
  const TpgFault*
  fault(ymuint id) const;

  /// @brief 代表故障数を得る．
  ymuint
  rep_fault_num() const;

  /// @brief 代表故障を取り出す．
  /// @param[in] pos 位置番号 ( 0 <= pos < rep_fault_num() )
  TpgFault*
  rep_fault(ymuint pos);

  /// @brief 代表故障を取り出す．
  /// @param[in] pos 位置番号 ( 0 <= pos < rep_fault_num() )
  const TpgFault*
  rep_fault(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 必要割り当てに関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief f の検出に必要な割り当てを求める．
  /// @param[in] f 対象の故障
  /// @param[in] ma_list 割り当て結果を格納するリスト
  /// @return 矛盾が生じたら(fが冗長故障の場合) false を返す．
  /// @note TpgNetwork のメンバにはアクセスしないので static メンバになっている．
  /// @note ma_list の内容は TpgNode::id() * 2 + val (0 / 1)
  static
  bool
  get_mandatory_assignment(TpgFault* f,
			   vector<ymuint32>& ma_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief TpgNode の内容を設定する．
  /// @param[in] id ID番号
  /// @param[in] tgnode もととなる TgNode
  TpgNode*
  make_node(ymuint id,
	    const TgNode* tgnode);

  /// @brief 複雑な論理式に対応するプリミティブを作る．
  /// @param[in] expr 論理式
  /// @param[in] tgnode もととなる TgNode
  /// @param[in] primitive_list プリミティブを設定する領域
  /// @param[inout] id プリミティブID
  /// @note id は内部でインクリメントされる．
  TpgPrimitive*
  make_primitive(const LogExpr& expr,
		 const TgNode* tgnode,
		 TpgPrimitive* primitive_list,
		 ymuint& id);

  /// @brief 入力プリミティブの設定を行なう．
  /// @param[in] prim プリミティブ
  /// @param[in] id 入力番号
  void
  set_input_primitive(TpgPrimitive* prim,
		      ymuint id);

  /// @brief 否定付き入力プリミティブの設定を行なう．
  /// @param[in] prim プリミティブ
  /// @param[in] id 入力番号
  void
  set_not_input_primitive(TpgPrimitive* prim,
			  ymuint id);

  /// @brief 論理プリミティブの設定を行なう．
  /// @param[in] prim プリミティブ
  /// @param[in] gate_type ゲートタイプ
  /// @param[in] ni 入力数
  void
  set_logic_primitive(TpgPrimitive* prim,
		      tTgGateType gate_type,
		      ymuint ni);

  /// @brief ノードの TFI にマークをつける．
  /// @note 結果は mTmpMark[node->id()] に格納される．
  void
  dfs_mark(TpgNode* node);

  /// @brief ノードの TFI のマークを消す．
  /// @note 結果は mTmpMark[node->id()] に格納される．
  void
  dfs_unmark(TpgNode* node);

  /// @brief activate_po(), activate_all() の下請け関数
  void
  activate_sub();

  /// @brief 故障を生成する．
  /// @param[in] node 対象のノード
  /// @param[in] is_output 出力の故障の時 true にするフラグ
  /// @param[in] ipos 入力位置
  /// @param[in] val 故障値
  /// @param[in] rep_fault 代表故障
  /// @param[inout] fid 故障ID
  /// @note rep_fault が NULL の場合は自分自身が代表故障となる．
  /// @note この関数内で fid の値がインクリメントされる．
  TpgFault*
  new_fault(TpgNode* node,
	    bool is_output,
	    ymuint ipos,
	    int val,
	    TpgFault* rep_fault,
	    ymuint& fid);


private:
  //////////////////////////////////////////////////////////////////////
  // オブジェクトの生成はクラスメソッドのみが行なう．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] tgnetwork もとのネットワーク
  TpgNetwork(const TgNetwork& tgnetwork);

  /// @brief コピーコンストラクタは実装しない．
  TpgNetwork(const TpgNetwork& src);

  /// @brief 代入演算子も実装しない．
  const TpgNetwork&
  operator=(const TpgNetwork& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // TpgNetwork 関係のメモリ確保を行なうオブジェクト
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
  TpgNode* mNodeArray;

  // TgNode->gid() をキーにしたノードの配列
  TpgNode** mNodeMap;

  // 外部入力ノードの配列
  TpgNode** mInputArray;

  // 外部出力ノードの配列
  TpgNode** mOutputArray;

  // TFI サイズの降順に整列した外部出力ノードの配列
  TpgNode** mOutputArray2;

  // アクティブなノード数
  ymuint32 mActNodeNum;

  // アクティブなノードの配列
  TpgNode** mActNodeArray;

  // activate_sub() で用いられるマーク用の配列
  // サイズは mNodeNum
  bool* mTmpMark;

  // 全故障数
  ymuint32 mFaultNum;

  // 故障の本体の配列
  TpgFault* mFaultChunk;

  // 代表故障数
  ymuint32 mRepFaultNum;

  // 代表故障のポインタ配列
  TpgFault** mRepFaultList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード数を得る．
inline
ymuint
TpgNetwork::node_num() const
{
  return mNodeNum;
}

// @brief 外部入力数を得る．
inline
ymuint
TpgNetwork::input_num() const
{
  return mInputNum;
}

// @brief 外部入力数 + FF数を得る．
inline
ymuint
TpgNetwork::input_num2() const
{
  return mInputNum + mFFNum;
}

// @brief 外部入力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < input_num2() )
inline
TpgNode*
TpgNetwork::input(ymuint pos) const
{
  assert_cond( pos < input_num2(), __FILE__, __LINE__);
  return mInputArray[pos];
}

// @brief 外部出力数を得る．
inline
ymuint
TpgNetwork::output_num() const
{
  return mOutputNum;
}

// @brief 外部出力数 + FF数を得る．
inline
ymuint
TpgNetwork::output_num2() const
{
  return mOutputNum + mFFNum;
}

// @brief 外部出力ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < output_num2() )
inline
TpgNode*
TpgNetwork::output(ymuint pos) const
{
  assert_cond( pos < output_num2(), __FILE__, __LINE__);
  return mOutputArray[pos];
}

// @brief サイズの降順で整列した順番で外部出力ノードを取り出す．
inline
TpgNode*
TpgNetwork::output2(ymuint pos) const
{
  assert_cond( pos < output_num2(), __FILE__, __LINE__);
  return mOutputArray2[pos];
}

// @brief アクティブなノード数を得る．
inline
ymuint
TpgNetwork::active_node_num() const
{
  return mActNodeNum;
}

// @brief アクティブなノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < active_node_num() )
inline
TpgNode*
TpgNetwork::active_node(ymuint pos)
{
  assert_cond( pos < mActNodeNum, __FILE__, __LINE__);
  return mActNodeArray[pos];
}

// @brief アクティブなノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < active_node_num() )
inline
const TpgNode*
TpgNetwork::active_node(ymuint pos) const
{
  assert_cond( pos < mActNodeNum, __FILE__, __LINE__);
  return mActNodeArray[pos];
}

END_NAMESPACE_YM_SATPG

#endif // TPGNETWORK_H
