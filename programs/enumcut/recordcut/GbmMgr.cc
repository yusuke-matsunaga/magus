
/// @file GbmMgr.cc
/// @brief GbmMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmMgr.h"
#include "GbmNodeImpl.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GbmMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GbmMgr::GbmMgr() :
  mAlloc(4096)
{
  mNextConfVar =0;
}

// @brief デストラクタ
GbmMgr::~GbmMgr()
{
}

// @brief 外部入力ノードを作る．
// @return 作成したノードのハンドルを返す．
GbmNodeHandle
GbmMgr::new_input()
{
  ymuint id = mNodeList.size();
  ymuint input_id = mInputList.size();

  void* p = mAlloc.get_memory(sizeof(GbmInputNode));
  GbmNode* node = new (p) GbmInputNode(id, input_id);
  mNodeList.push_back(node);
  mInputList.push_back(node);
  return GbmNodeHandle(node->id(), false);
}

// @brief ANDゲートを作る．
// @param[in] input0 ファンイン0のハンドル
// @param[in] input1 ファンイン0のハンドル
// @return 作成したノードのハンドルを返す．
GbmNodeHandle
GbmMgr::new_and(GbmNodeHandle input0,
		GbmNodeHandle input1)
{
  ymuint id = mNodeList.size();

  void* p = mAlloc.get_memory(sizeof(GbmAndNode));
  GbmNode* node = new (p) GbmAndNode(id, input0, input1);
  mNodeList.push_back(node);
  mAndList.push_back(node);
  return GbmNodeHandle(node->id(), false);
}

// @brief LUTを作る．
// @param[in] inputs ファンインのハンドルのリスト
// @return 作成したノードのハンドルを返す．
GbmNodeHandle
GbmMgr::new_lut(const vector<GbmNodeHandle>& inputs)
{
  ymuint id = mNodeList.size();

  ymuint n = inputs.size();
  void* p = mAlloc.get_memory(sizeof(GbmLutNode) + sizeof(GbmNodeHandle) * (n - 1));
  GbmNode* node = new (p) GbmLutNode(id, mNextConfVar, inputs);
  mNodeList.push_back(node);
  mLutList.push_back(node);
  mNextConfVar += node->conf_size();
  return GbmNodeHandle(node->id(), false);
}

// @brief MUXを作る．
// @param[in] inputs ファンインのハンドルのリスト
// @return 作成したノードのハンドルを返す．
// @note inputs のサイズが2のべき乗でないときは0でパディングされる．
GbmNodeHandle
GbmMgr::new_mux(const vector<GbmNodeHandle>& inputs)
{
  ymuint id = mNodeList.size();

  ymuint n = inputs.size();
  void* p = mAlloc.get_memory(sizeof(GbmMuxNode) + sizeof(GbmNodeHandle) * (n - 1));
  GbmNode* node = new (p) GbmMuxNode(id, mNextConfVar, inputs);
  mNodeList.push_back(node);
  mMuxList.push_back(node);
  mNextConfVar += node->conf_size();
  return GbmNodeHandle(node->id(), false);
}

// @brief 関数ノードを作る．
// @param[in] f 関数
// @param[in] inputs ファンインのハンドルのリスト
GbmNodeHandle
GbmMgr::new_func(const TvFunc& f,
		 const vector<GbmNodeHandle>& inputs)
{
  ymuint id = mNodeList.size();

  ymuint n = f.input_num();
  assert_cond( inputs.size() == n, __FILE__, __LINE__);
  void* p = mAlloc.get_memory(sizeof(GbmFuncNode) + sizeof(GbmNodeHandle) * (n - 1));
  GbmNode* node = new (p) GbmFuncNode(id, f, inputs);
  mNodeList.push_back(node);

  return GbmNodeHandle(node->id(), false);
}

// @brief 全ノード数を返す．
// @note ノードIDの最大値 + 1 と一致する．
ymuint
GbmMgr::node_num() const
{
  return mNodeList.size();
}

// @brief ノードを返す．
// @param[in] id ID番号 ( 0 <= id < node_num() )
const GbmNode*
GbmMgr::node(ymuint id) const
{
  assert_cond( id < node_num(), __FILE__, __LINE__);
  return mNodeList[id];
}

// @brief 外部入力数を返す．
ymuint
GbmMgr::input_num() const
{
  return mInputList.size();
}

// @brief 外部入力ノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const GbmNode*
GbmMgr::input_node(ymuint pos) const
{
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return mInputList[pos];
}

// @brief ANDノード数を返す．
ymuint
GbmMgr::and_num() const
{
  return mAndList.size();
}

// @brief ANDノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < and_num() )
const GbmNode*
GbmMgr::and_node(ymuint pos) const
{
  assert_cond( pos < and_num(), __FILE__, __LINE__);
  return mAndList[pos];
}

// @brief LUTノード数を返す．
ymuint
GbmMgr::lut_num() const
{
  return mLutList.size();
}

// @brief LUTノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < lut_num() )
const GbmNode*
GbmMgr::lut_node(ymuint pos) const
{
  assert_cond( pos < lut_num(), __FILE__, __LINE__);
  return mLutList[pos];
}

// @brief MUXノード数を返す．
ymuint
GbmMgr::mux_num() const
{
  return mMuxList.size();
}

// @brief MUXノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < mux_num() )
const GbmNode*
GbmMgr::mux_node(ymuint pos) const
{
  assert_cond( pos < mux_num(), __FILE__, __LINE__);
  return mMuxList[pos];
}

// @brief configuration 変数の数を返す．
ymuint
GbmMgr::conf_var_num() const
{
  return mNextConfVar;
}

END_NAMESPACE_YM
