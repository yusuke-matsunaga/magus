
/// @file GbmNodeImpl.cc
/// @brief GbmNodeImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmNodeImpl.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GbmNodeImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
GbmNodeImpl::GbmNodeImpl(ymuint id) :
  mId(id)
{
}

// @brief デストラクタ
GbmNodeImpl::~GbmNodeImpl()
{
}

// @brief ID番号を返す．
ymuint
GbmNodeImpl::id() const
{
  return mId;
}

// @brief 外部入力ノードの時 true を返す．
bool
GbmNodeImpl::is_input() const
{
  return false;
}

// @brief ANDノードの時 true を返す．
bool
GbmNodeImpl::is_and() const
{
  return false;
}

// @brief LUTノードの時 true を返す．
bool
GbmNodeImpl::is_lut() const
{
  return false;
}

// @brief 外部入力番号を返す．
// @note is_input() == true の時のみ意味を持つ．
ymuint
GbmNodeImpl::input_id() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief ファンイン数を返す．
// @note 外部入力ノードの場合は常に0
// @note AND ノードの場合は常に2
ymuint
GbmNodeImpl::fanin_num() const
{
  return 0;
}

// @brief ファンインのハンドルを返す．
// @param[in] pos ファンイン番号 ( 0 <= pos < fanin_num() )
GbmNodeHandle
GbmNodeImpl::fanin(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return GbmNodeHandle();
}


//////////////////////////////////////////////////////////////////////
// クラス GbmInputNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] input_id 入力番号
GbmInputNode::GbmInputNode(ymuint id,
			   ymuint input_id) :
  GbmNodeImpl(id),
  mInputId(input_id)
{
}

// @brief デストラクタ
GbmInputNode::~GbmInputNode()
{
}

// @brief 外部入力ノードの時 true を返す．
bool
GbmInputNode::is_input() const
{
  return true;
}

// @brief 外部入力番号を返す．
// @note is_input() == true の時のみ意味を持つ．
ymuint
GbmInputNode::input_id() const
{
  return mInputId;
}


//////////////////////////////////////////////////////////////////////
// クラス GbmAndNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] fanin0, fanin1 ファンインのハンドル
GbmAndNode::GbmAndNode(ymuint id,
		       GbmNodeHandle fanin0,
		       GbmNodeHandle fanin1) :
  GbmNodeImpl(id)
{
  mFanin[0] = fanin0;
  mFanin[1] = fanin1;
}

// @brief デストラクタ
GbmAndNode::~GbmAndNode()
{
}

// @brief ANDノードの時 true を返す．
bool
GbmAndNode::is_and() const
{
  return true;
}

// @brief ファンイン数を返す．
// @note 外部入力ノードの場合は常に0
// @note AND ノードの場合は常に2
ymuint
GbmAndNode::fanin_num() const
{
  return 2;
}

// @brief ファンインのハンドルを返す．
// @param[in] pos ファンイン番号 ( 0 <= pos < fanin_num() )
GbmNodeHandle
GbmAndNode::fanin(ymuint pos) const
{
  assert_cond( pos < 2, __FILE__, __LINE__);
  return mFanin[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス GbmLutNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] fanin_list ファンインのリスト
GbmLutNode::GbmLutNode(ymuint id,
		       const vector<GbmNodeHandle>& fanin_list) :
  GbmNodeImpl(id),
  mFaninNum(fanin_list.size())
{
  for (ymuint i = 0; i < mFaninNum; ++ i) {
    mFanin[i] = fanin_list[i];
  }
}

// @brief デストラクタ
GbmLutNode::~GbmLutNode()
{
}

// @brief LUTノードの時 true を返す．
bool
GbmLutNode::is_lut() const
{
  return true;
}

// @brief ファンイン数を返す．
// @note 外部入力ノードの場合は常に0
// @note AND ノードの場合は常に2
ymuint
GbmLutNode::fanin_num() const
{
  return mFaninNum;
}

// @brief ファンインのハンドルを返す．
// @param[in] pos ファンイン番号 ( 0 <= pos < fanin_num() )
GbmNodeHandle
GbmLutNode::fanin(ymuint pos) const
{
  assert_cond( pos < mFaninNum, __FILE__, __LINE__);
  return mFanin[pos];
}

END_NAMESPACE_YM
