#ifndef YM_NETWORKS_TGNETWORK_H
#define YM_NETWORKS_TGNETWORK_H

/// @file ym_networks/TgNetwork.h
/// @brief TgNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


/// @defgroup TgNetwork Technology Dependent Gate Network
///
/// ni 入力(ni >= 0), 1出力の論理要素 (TgNode) のネットワーク
/// 個々の TgNode は ni 入力 1 出力の論理関数を実現している．
/// 実装としては論理式 (LogExpr) で表されるが，個々の TgNode
/// はその実体を持たず，論理関数 ID のみを保持する．
/// TgNetwork 中に論理関数 ID と論理式ベクタの対応表を持つ．
/// ただし典型的な関数は別途 tGateType で定義しておく．
///
/// TgNode のファンアウト先のノードとその入力ピンの情報を TgEdge で表す．


#include "ym_networks/tgnet.h"
#include "ym_logic/LogExpr.h"
#include "ym_utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_NETWORKS_TGNET

class NameHash;
class LogicMgr;

//////////////////////////////////////////////////////////////////////
/// @class TgNetwork TgNetwork.h "ym_networks/TgNetwork.h"
/// @brief ATPG対象のネットワークを表すクラス
//////////////////////////////////////////////////////////////////////
class TgNetwork
{
public:

  /// @brief コンストラクタ
  TgNetwork();

  /// @brief デストラクタ
  ~TgNetwork();


public:
  //////////////////////////////////////////////////////////////////////
  // read-only のメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 全ノード数を得る．
  ymuint
  node_num() const;

  /// @brief pos 番めのノードを得る．
  /// @param[in] pos ノードの gid() に対応する．
  /// @note 0 <= pos < node_num()
  const TgNode*
  node(ymuint32 pos) const;

  /// @brief 名前からノードを検索する
  /// @param[in] name 検索対象の名前
  /// @return name という名前のノードを返す．
  const TgNode*
  find_node(const char* name) const;

  /// @brief 外部入力数を得る．
  ymuint
  input_num1() const;

  /// @brief 外部入力数 + FFノード数を得る．
  ymuint
  input_num2() const;

  /// @brief pos 番めの入力ノードを得る．
  /// @param[in] pos はノードの lid() に対応する．
  /// @note 0 <= pos < input_num2()
  /// @note input_num1() 以下は外部入力．それ以降は FF出力
  const TgNode*
  input(ymuint32 pos) const;

  /// @brief 外部出力数を得る．
  ymuint
  output_num1() const;

  /// @brief 外部出力数 + FF ノード数を得る．
  ymuint
  output_num2() const;

  /// @brief pos 番めの外部出力ノードを得る．
  /// @param[in] pos はノードの lid() に対応する．
  /// @note 0 <= pos < output_num2()
  /// @note output_num1() 以下は外部出力．それ以降は FF入力
  const TgNode*
  output(ymuint32 pos) const;

  /// @brief logic ノード数を得る．
  ymuint
  logic_num() const;

  /// @brief pos 番めの logic ノードを得る．
  /// @param[in] pos はノードの lid() に対応する．
  /// @note 0 <= pos < logic_num()
  const TgNode*
  logic(ymuint32 pos) const;

  /// @brief ソート順で pos 番めの logic ノードを得る．
  /// @param[in] pos 取り出すノードの位置
  /// @note 0 <= pos < logic_num()
  const TgNode*
  sorted_logic(ymuint32 pos) const;

  /// @brief FF ノード数を得る．
  ymuint
  ff_num() const;

  /// @brief 中で使われている論理関数の数を得る．
  ymuint
  func_num() const;

  /// @brief 関数を取り出す．
  /// @note param[in] id 関数番号 ( 0 <= id < func_num() )
  const TvFunc&
  get_func(ymuint id) const;

  /// @brief 関数の論理式を取り出す．
  /// @note param[in] id 関数番号 ( 0 <= id < func_num() )
  LogExpr
  get_lexp(ymuint id) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 新しいノードを生成する．
  TgNode*
  new_node();

  /// @brief 名前からノードを検索する
  /// @param[in] name 検索対象の名前
  /// @param[in] create_flag true なら存在しない時に新規に生成する．
  /// @return name という名前のノードを返す．
  TgNode*
  find_node(const char* name,
	    bool create_flag);

  /// @brief ノードを外部入力に設定する．
  /// @param[in] node 対象のノード
  void
  set_to_input(TgNode* node);

  /// @brief ノードを外部出力に設定する．
  /// @param[in] node 対象のノード
  void
  set_to_output(TgNode* node);

  /// @brief ノードを論理ノードに設定する．
  /// @param[in] node 対象のノード
  /// @param[in] expr 論理式
  void
  set_to_logic(TgNode* node,
	       const LogExpr& expr);

  /// @brief ノードを論理ノードに設定する．
  /// @param[in] node 対象のノード
  /// @param[in] gate_type ゲートタイプ
  /// @param[in] ni 入力数
  void
  set_to_builtin_logic(TgNode* node,
		       tTgGateType gate_type,
		       ymuint ni);

  /// @brief ノードを論理ノードに設定する．
  /// @param[in] node 対象のノード
  /// @param[in] ni 入力数
  /// @param[in] aux_id 補助ID
  void
  set_to_cplx_logic(TgNode* node,
		    ymuint ni,
		    ymuint aux_id);

  /// @brief ノードを FF ノードに設定する．
  /// @param[in] ffin FF-in に設定するノード
  /// @param[in] ffout FF-out に設定するノード
  void
  set_to_ff(TgNode* ffin,
	    TgNode* ffout);

  /// @brief ノード間の接続を行う．
  /// @param[in] from_node ファンイン元のノード
  /// @param[in] to_node ファンアウト先のノード
  /// @param[in] to_ipos ファンアウト先のノードの入力ピン番号
  void
  connect(TgNode* from_node,
	  TgNode* to_node,
	  ymuint to_ipos);

  /// @brief ネットワークの設定後の処理を行う．
  void
  wrap_up();


private:

  /// @brief ノードを論理ノードに設定する．
  /// @param[in] node 対象のノード
  /// @param[in] gate_type 論理タイプ
  /// @param[in] ni 入力数
  /// @param[in] aux_id 論理タイプが kTgCplx の場合の補助ID
  void
  set_to_logic(TgNode* node,
	       tTgGateType gate_type,
	       ymuint ni,
	       ymuint aux_id);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // 全ノードの配列
  vector<TgNode*> mNodeArray;

  // 入力ノードの配列
  vector<TgNode*> mInputArray;

  // FF-out の配列
  vector<TgNode*> mFFOutArray;

  // 出力ノードの配列
  vector<TgNode*> mOutputArray;

  // FF-in の配列
  vector<TgNode*> mFFInArray;

  // logic ノードの配列
  vector<TgNode*> mLogicArray;

  // ソートした logic ノードの配列
  vector<TgNode*> mSortedArray;

  // 名前をキーにしてノードを格納するハッシュ表
  NameHash* mNameHash;

  // 論理式を管理するオブジェクト
  LogicMgr* mLogicMgr;

};


/// @relates TgNetwork
/// @brief TgNetwork の内容をダンプする関数
void
dump(ostream& s,
     const TgNetwork& network);


//////////////////////////////////////////////////////////////////////
// TgNetwork のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief 全ノード数を得る．
inline
ymuint
TgNetwork::node_num() const
{
  return mNodeArray.size();
}

// @brief pos 番めのノードを得る．
// @node pos はノードの gid() に対応する．
inline
const TgNode*
TgNetwork::node(ymuint32 pos) const
{
  return mNodeArray[pos];
}

// @brief 外部入力数を得る．
inline
ymuint
TgNetwork::input_num1() const
{
  return mInputArray.size();
}

// @brief 外部入力数 + FF ノード数を得る．
inline
ymuint
TgNetwork::input_num2() const
{
  return input_num1() + ff_num();
}

// @brief pos 番めの外部入力ノードを得る．
inline
const TgNode*
TgNetwork::input(ymuint32 pos) const
{
  if ( pos < input_num1() ) {
    return mInputArray[pos];
  }
  else {
    return mFFOutArray[pos - input_num1()];
  }
}

// @brief 外部出力数を得る．
inline
ymuint
TgNetwork::output_num1() const
{
  return mOutputArray.size();
}

// @brief 外部出力数 + FF ノード数を得る．
inline
ymuint
TgNetwork::output_num2() const
{
  return output_num1() + ff_num();
}

// @brief pos 番めの外部出力ノードを得る．
inline
const TgNode*
TgNetwork::output(ymuint32 pos) const
{
  if ( pos < output_num1() ) {
    return mOutputArray[pos];
  }
  else {
    return mFFInArray[pos - output_num1()];
  }
}

// @brief logic ノード数を得る．
inline
ymuint
TgNetwork::logic_num() const
{
  return mLogicArray.size();
}

// @brief pos 番めの logic ノードを得る．
inline
const TgNode*
TgNetwork::logic(ymuint32 pos) const
{
  return mLogicArray[pos];
}

// @brief FF ノード数を得る．
inline
ymuint
TgNetwork::ff_num() const
{
  return mFFOutArray.size();
}

// @brief ソート順で pos 番めの logic ノードを得る．
inline
const TgNode*
TgNetwork::sorted_logic(ymuint32 pos) const
{
  return mSortedArray[pos];
}

END_NAMESPACE_YM_NETWORKS_TGNET

#endif // YM_NETWORKS_TGNETWORK_H
