#ifndef RCFNETWORK_H
#define RCFNETWORK_H

/// @file RcfNetwork.h
/// @brief RcfNetwork のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "RcfNodeHandle.h"
#include "ym_utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM

class RcfNode;

//////////////////////////////////////////////////////////////////////
/// @class RcfNetwork RcfNetwork.h "RcfNetwork.h"
/// @brief Reconfigurable Network を表すクラス
//////////////////////////////////////////////////////////////////////
class RcfNetwork
{
public:

  /// @brief コンストラクタ
  RcfNetwork();

  /// @brief デストラクタ
  ~RcfNetwork();


public:
  //////////////////////////////////////////////////////////////////////
  // ネットワーク作成用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力ノードを作る．
  /// @return 作成したノードのハンドルを返す．
  RcfNodeHandle
  new_input();

  /// @brief ANDゲートを作る．
  /// @param[in] input0 ファンイン0のハンドル
  /// @param[in] input1 ファンイン0のハンドル
  /// @return 作成したノードのハンドルを返す．
  RcfNodeHandle
  new_and(RcfNodeHandle input0,
	  RcfNodeHandle input1);

  /// @brief ORゲートを作る．
  /// @param[in] input0 ファンイン0のハンドル
  /// @param[in] input1 ファンイン0のハンドル
  /// @return 作成したノードのハンドルを返す．
  RcfNodeHandle
  new_or(RcfNodeHandle input0,
	 RcfNodeHandle input1);

  /// @brief XORゲートを作る．
  /// @param[in] input0 ファンイン0のハンドル
  /// @param[in] input1 ファンイン0のハンドル
  /// @return 作成したノードのハンドルを返す．
  RcfNodeHandle
  new_xor(RcfNodeHandle input0,
	  RcfNodeHandle input1);

  /// @brief XNORゲートを作る．
  /// @param[in] input0 ファンイン0のハンドル
  /// @param[in] input1 ファンイン0のハンドル
  /// @return 作成したノードのハンドルを返す．
  RcfNodeHandle
  new_xnor(RcfNodeHandle input0,
	   RcfNodeHandle input1);

  /// @brief LUTを作る．
  /// @param[in] inputs ファンインのハンドルのリスト
  /// @return 作成したノードのハンドルを返す．
  RcfNodeHandle
  new_lut(const vector<RcfNodeHandle>& inputs);

  /// @brief MUXを作る．
  /// @param[in] inputs ファンインのハンドルのリスト
  /// @return 作成したノードのハンドルを返す．
  /// @note inputs のサイズが2のべき乗でないときは0でパディングされる．
  RcfNodeHandle
  new_mux(const vector<RcfNodeHandle>& inputs);

  /// @brief 外部出力をセットする．
  /// @param[in] handle 出力にするハンドル
  void
  set_output(RcfNodeHandle handle);

  /// @brief 先行者番号をセットする．
  void
  set_input_pred(ymuint pos,
		 ymuint pred);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 全ノード数を返す．
  /// @note ノードIDの最大値 + 1 と一致する．
  ymuint
  node_num() const;

  /// @brief ノードを返す．
  /// @param[in] id ID番号 ( 0 <= id < node_num() )
  const RcfNode*
  node(ymuint id) const;

  /// @brief 外部入力数を返す．
  ymuint
  input_num() const;

  /// @brief 外部入力ノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
  const RcfNode*
  input_node(ymuint pos) const;

  /// @brief ANDノード数を返す．
  ymuint
  and_num() const;

  /// @brief ANDノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < and_num() )
  const RcfNode*
  and_node(ymuint pos) const;

  /// @brief LUTノード数を返す．
  ymuint
  lut_num() const;

  /// @brief LUTノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < lut_num() )
  const RcfNode*
  lut_node(ymuint pos) const;

  /// @brief MUXノード数を返す．
  ymuint
  mux_num() const;

  /// @brief MUXノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < mux_num() )
  const RcfNode*
  mux_node(ymuint pos) const;

  /// @brief 機能ノード(外部入力以外のノード)数を返す．
  ymuint
  fnode_num() const;

  /// @brief 機能ノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < fnode_num() )
  const RcfNode*
  fnode(ymuint pos) const;

  /// @brief 外部出力のハンドルを返す．
  RcfNodeHandle
  output() const;

  /// @brief 入力の先行者があれば true を返す．
  /// @param[in] pos 入力の位置番号 ( 0 <= pos < input_num() )
  /// @param[out] pred 先行者番号
  bool
  get_pred(ymuint pos,
	   ymuint& pred) const;

  /// @brief configuration 変数の数を返す．
  ymuint
  conf_var_num() const;

  /// @brief 出力値を計算する．
  /// @param[in] ival_list 入力値のリスト
  /// @param[in] conf_bits 設定変数の値のリスト
  /// @return 出力値を返す．
  bool
  simulate(const vector<bool>& ival_list,
	   const vector<bool>& conf_bits) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // 全ての RcfNode を格納するベクタ
  vector<RcfNode*> mNodeList;

  // 入力ノードを格納するベクタ
  vector<RcfNode*> mInputList;

  // 入力順の先行者を格納するベクタ
  vector<ymuint32> mInputPredList;

  // ANDノードを格納するベクタ
  vector<RcfNode*> mAndList;

  // LUTノードを格納するベクタ
  vector<RcfNode*> mLutList;

  // MUXノードを格納するベクタ
  vector<RcfNode*> mMuxList;

  // 機能ノードを格納するベクタ
  vector<RcfNode*> mFnodeList;

  // 外部出力のハンドル
  RcfNodeHandle mOutput;

  // 次の configuration 変数番号
  ymuint32 mNextConfVar;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ORゲートを作る．
// @param[in] input0 ファンイン0のハンドル
// @param[in] input1 ファンイン0のハンドル
// @return 作成したノードのハンドルを返す．
inline
RcfNodeHandle
RcfNetwork::new_or(RcfNodeHandle input0,
		   RcfNodeHandle input1)
{
  return ~new_and(~input0, ~input1);
}

// @brief XORゲートを作る．
// @param[in] input0 ファンイン0のハンドル
// @param[in] input1 ファンイン0のハンドル
// @return 作成したノードのハンドルを返す．
inline
RcfNodeHandle
RcfNetwork::new_xor(RcfNodeHandle input0,
		    RcfNodeHandle input1)
{
  RcfNodeHandle n1 = new_and( input0, ~input1);
  RcfNodeHandle n2 = new_and(~input0,  input1);
  return new_or(n1, n2);
}

// @brief XNORゲートを作る．
// @param[in] input0 ファンイン0のハンドル
// @param[in] input1 ファンイン0のハンドル
// @return 作成したノードのハンドルを返す．
inline
RcfNodeHandle
RcfNetwork::new_xnor(RcfNodeHandle input0,
		     RcfNodeHandle input1)
{
  RcfNodeHandle n1 = new_and( input0,  input1);
  RcfNodeHandle n2 = new_and(~input0, ~input1);
  return new_or(n1, n2);
}

END_NAMESPACE_YM

#endif // RCFNETWORK_H
