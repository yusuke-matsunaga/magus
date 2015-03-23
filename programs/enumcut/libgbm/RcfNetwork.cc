
/// @file RcfNetwork.cc
/// @brief RcfNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "RcfNetwork.h"
#include "RcfNodeImpl.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス RcfNetwork
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RcfNetwork::RcfNetwork() :
  mAlloc(4096)
{
  mNextConfVar =0;
}

// @brief デストラクタ
RcfNetwork::~RcfNetwork()
{
}

// @brief 外部入力ノードを作る．
// @return 作成したノードのハンドルを返す．
RcfNodeHandle
RcfNetwork::new_input()
{
  ymuint id = mNodeList.size();
  ymuint input_id = mInputList.size();

  void* p = mAlloc.get_memory(sizeof(RcfInputNode));
  RcfNode* node = new (p) RcfInputNode(id, input_id);
  mNodeList.push_back(node);
  mInputList.push_back(node);
  mInputPredList.push_back(0U);
  return RcfNodeHandle(node->id(), false);
}

// @brief ANDゲートを作る．
// @param[in] input0 ファンイン0のハンドル
// @param[in] input1 ファンイン0のハンドル
// @return 作成したノードのハンドルを返す．
RcfNodeHandle
RcfNetwork::new_and(RcfNodeHandle input0,
		    RcfNodeHandle input1)
{
  ymuint id = mNodeList.size();

  void* p = mAlloc.get_memory(sizeof(RcfAndNode));
  RcfNode* node = new (p) RcfAndNode(id, input0, input1);
  mNodeList.push_back(node);
  mFuncNodeList.push_back(node);
  mAndList.push_back(node);
  return RcfNodeHandle(node->id(), false);
}

// @brief LUTを作る．
// @param[in] inputs ファンインのハンドルのリスト
// @return 作成したノードのハンドルを返す．
RcfNodeHandle
RcfNetwork::new_lut(const vector<RcfNodeHandle>& inputs)
{
  ymuint id = mNodeList.size();

  ymuint n = inputs.size();
  void* p = mAlloc.get_memory(sizeof(RcfLutNode) + sizeof(RcfNodeHandle) * (n - 1));
  RcfNode* node = new (p) RcfLutNode(id, mNextConfVar, inputs);
  mNodeList.push_back(node);
  mFuncNodeList.push_back(node);
  mLutList.push_back(node);
  mNextConfVar += node->conf_size();
  return RcfNodeHandle(node->id(), false);
}

// @brief MUXを作る．
// @param[in] inputs ファンインのハンドルのリスト
// @return 作成したノードのハンドルを返す．
// @note inputs のサイズが2のべき乗でないときは0でパディングされる．
RcfNodeHandle
RcfNetwork::new_mux(const vector<RcfNodeHandle>& inputs)
{
  ymuint id = mNodeList.size();

  ymuint n = inputs.size();
  void* p = mAlloc.get_memory(sizeof(RcfMuxNode) + sizeof(RcfNodeHandle) * (n - 1));
  RcfNode* node = new (p) RcfMuxNode(id, mNextConfVar, inputs);
  mNodeList.push_back(node);
  mFuncNodeList.push_back(node);
  mMuxList.push_back(node);
  mNextConfVar += node->conf_size();
  return RcfNodeHandle(node->id(), false);
}

// @brief 外部出力をセットする．
// @param[in] handle 出力にするハンドル
void
RcfNetwork::set_output(RcfNodeHandle handle)
{
  mOutput = handle;
}

// @brief 先行者番号をセットする．
void
RcfNetwork::set_input_pred(ymuint pos,
			   ymuint pred)
{
  ASSERT_COND( pos < input_num() );
  mInputPredList[pos] = (pred << 1) | 1U;
}

// @brief 全ノード数を返す．
// @note ノードIDの最大値 + 1 と一致する．
ymuint
RcfNetwork::node_num() const
{
  return mNodeList.size();
}

// @brief ノードを返す．
// @param[in] id ID番号 ( 0 <= id < node_num() )
const RcfNode*
RcfNetwork::node(ymuint id) const
{
  ASSERT_COND( id < node_num() );
  return mNodeList[id];
}

// @brief 外部入力数を返す．
ymuint
RcfNetwork::input_num() const
{
  return mInputList.size();
}

// @brief 外部入力ノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const RcfNode*
RcfNetwork::input_node(ymuint pos) const
{
  ASSERT_COND( pos < input_num() );
  return mInputList[pos];
}

// @brief 機能ノード(外部入力以外のノード)数を返す．
ymuint
RcfNetwork::func_node_num() const
{
  return mFuncNodeList.size();
}

// @brief 機能ノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < func_node_num() )
const RcfNode*
RcfNetwork::func_node(ymuint pos) const
{
  ASSERT_COND( pos < mFuncNodeList.size() );
  return mFuncNodeList[pos];
}

// @brief ANDノード数を返す．
ymuint
RcfNetwork::and_num() const
{
  return mAndList.size();
}

// @brief ANDノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < and_num() )
const RcfNode*
RcfNetwork::and_node(ymuint pos) const
{
  ASSERT_COND( pos < and_num() );
  return mAndList[pos];
}

// @brief LUTノード数を返す．
ymuint
RcfNetwork::lut_num() const
{
  return mLutList.size();
}

// @brief LUTノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < lut_num() )
const RcfNode*
RcfNetwork::lut_node(ymuint pos) const
{
  ASSERT_COND( pos < lut_num() );
  return mLutList[pos];
}

// @brief MUXノード数を返す．
ymuint
RcfNetwork::mux_num() const
{
  return mMuxList.size();
}

// @brief MUXノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < mux_num() )
const RcfNode*
RcfNetwork::mux_node(ymuint pos) const
{
  ASSERT_COND( pos < mux_num() );
  return mMuxList[pos];
}

// @brief 外部出力のハンドルを返す．
RcfNodeHandle
RcfNetwork::output() const
{
  return mOutput;
}

// @brief 入力の先行者があれば true を返す．
// @param[in] pos 入力の位置番号 ( 0 <= pos < input_num() )
// @param[out] pred 先行者番号
bool
RcfNetwork::get_pred(ymuint pos,
		     ymuint& pred) const
{
  ASSERT_COND( pos < input_num() );
  ymuint v = mInputPredList[pos];
  if ( v == 0U ) {
    return false;
  }
  pred = v >> 1;
  return true;
}

// @brief configuration 変数の数を返す．
ymuint
RcfNetwork::conf_var_num() const
{
  return mNextConfVar;
}

// @brief 出力値を計算する．
// @param[in] ival_list 入力値のリスト
// @param[in] conf_bits 設定変数の値のリスト
// @return 出力値を返す．
bool
RcfNetwork::simulate(const vector<bool>& ival_list,
		     const vector<bool>& conf_bits) const
{
  ymuint ni = input_num();
  ASSERT_COND( ival_list.size() );
  ymuint nc = conf_var_num();
  ASSERT_COND( conf_bits.size() );

  ymuint nn = node_num();
  vector<bool> val_list(nn);

  // API の都合上，入力からのトポロジカル順になっている．
  for (ymuint i = 0; i < nn; ++ i) {
    const RcfNode* node = this->node(i);
    bool val;
    if ( node->is_input() ) {
      val = ival_list[node->input_id()];
    }
    else {
      val = node->simulate(val_list, conf_bits);
    }
    val_list[i] = val;
  }

  RcfNodeHandle oh = output();
  bool oval = val_list[oh.id()];
  if ( oh.inv() ) {
    oval = !oval;
  }
  return oval;
}

END_NAMESPACE_YM
