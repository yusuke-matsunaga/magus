
/// @file BlifNodeImpl.cc
/// @brief BlifNodeImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "BlifNodeImpl.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

//////////////////////////////////////////////////////////////////////
// クラス BlifNodeImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID 番号
// @param[in] name ノード名
BlifNodeImpl::BlifNodeImpl(ymuint32 id,
			   const char* name) :
  mId(id),
  mName(name)
{
}

// @brief デストラクタ
BlifNodeImpl::~BlifNodeImpl()
{
}

// @brief ID を返す．
ymuint32
BlifNodeImpl::id() const
{
  return mId;
}

// @brief 名前を返す．
const char*
BlifNodeImpl::name() const
{
  return mName;
}

// @brief ファンイン数を得る．
ymuint32
BlifNodeImpl::fanin_num() const
{
  return 0;
}

// @brief ファンインのノードIDを返す．
// @param[in] pos 入力位置 ( 0 <= pos < fanin_num() )
ymuint32
BlifNodeImpl::fanin_id(ymuint32 pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief カバーのキューブ数を得る．
ymuint32
BlifNodeImpl::cube_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 入力キューブのパタンを得る．
// @param[in] c_pos キューブの位置 ( 0 <= c_pos < nc() )
// @param[in] i_pos 入力位置 ( 0 <= i_pos < ni() )
// @note 意味のあるパタンは '0' '1' '-'
char
BlifNodeImpl::cube_pat(ymuint32 c_pos,
		       ymuint32 i_pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return '-';
}

// @brief 出力キューブを表すパタンを得る．
// @note 意味のあるパタンは '0' '1'
char
BlifNodeImpl::opat() const
{
  assert_not_reached(__FILE__, __LINE__);
  return '0';
}

// @brief セルを返す．
const Cell*
BlifNodeImpl::cell() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 入力ノードのID番号を返す．
ymuint32
BlifNodeImpl::inode_id() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief リセット値を返す．
char
BlifNodeImpl::reset_val() const
{
  assert_not_reached(__FILE__, __LINE__);
  return ' ';
}


//////////////////////////////////////////////////////////////////////
// クラス BlifInputNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID 番号
// @param[in] name ノード名
BlifInputNode::BlifInputNode(ymuint32 id,
			     const char* name) :
  BlifNodeImpl(id, name)
{
}

// @brief デストラクタ
BlifInputNode::~BlifInputNode()
{
}

// @brief 型を返す．
BlifNode::tType
BlifInputNode::type() const
{
  return kInput;
}

// @brief 内容を blif 形式で出力する．
// @param[in] s 出力先のストリーム
void
BlifInputNode::write_blif(ostream& s) const
{
  assert_not_reached(__FILE__, __LINE__);
}


//////////////////////////////////////////////////////////////////////
// クラス BlifNodeImpl2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name ノード名
// @param[in] ni ファンイン数
// @param[in] fanins ファンインのID番号の配列
BlifNodeImpl2::BlifNodeImpl2(ymuint32 id,
			     const char* name,
			     ymuint32 ni,
			     const ymuint32* fanins) :
  BlifNodeImpl(id, name),
  mFaninNum(ni),
  mFanins(fanins)
{
}

// @brief デストラクタ
BlifNodeImpl2::~BlifNodeImpl2()
{
}

// @brief ファンイン数を得る．
ymuint32
BlifNodeImpl2::fanin_num() const
{
  return mFaninNum;
}

// @brief ファンインを求める．
// @param[in] pos 入力位置 ( 0 <= pos < ni() )
ymuint32
BlifNodeImpl2::fanin_id(ymuint32 pos) const
{
  assert_cond( pos < fanin_num(), __FILE__, __LINE__);
  return mFanins[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス BlifLogicNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] ni ファンイン数
// @param[in] name ノード名
// @param[in] fanins ファンインのID番号の配列
// @param[in] nc キューブ数
// @param[in] cover_pat 入力キューブのパタンをすべてつなげた文字列
// @param[in] opat 出力キューブのパタン
BlifLogicNode::BlifLogicNode(ymuint32 id,
			     const char* name,
			     ymuint32 ni,
			     const ymuint32* fanins,
			     ymuint32 nc,
			     const char* cover_pat,
			     char opat) :
  BlifNodeImpl2(id, name, ni, fanins),
  mCubeNum(nc),
  mCoverPat(cover_pat),
  mOpat(opat)
{
}

// @brief デストラクタ
BlifLogicNode::~BlifLogicNode()
{
}

// @brief 型を返す．
BlifNode::tType
BlifLogicNode::type() const
{
  return kLogic;
}

// @brief 内容を blif 形式で出力する．
// @param[in] s 出力先のストリーム
void
BlifLogicNode::write_blif(ostream& s) const
{
}


// @brief カバーのキューブ数を得る．
ymuint32
BlifLogicNode::cube_num() const
{
  return mCubeNum;
}

// @brief 入力キューブのパタンを得る．
// @param[in] c_pos キューブの位置 ( 0 <= c_pos < nc() )
// @param[in] i_pos 入力位置 ( 0 <= i_pos < ni() )
// @note 意味のあるパタンは '0' '1' '-'
char
BlifLogicNode::cube_pat(ymuint32 c_pos,
			ymuint32 i_pos) const
{
  assert_cond( c_pos < cube_num(), __FILE__, __LINE__);
  assert_cond( i_pos < fanin_num(), __FILE__, __LINE__);
  return mCoverPat[c_pos * fanin_num() + i_pos];
}

// @brief 出力キューブを表すパタンを得る．
// @note 意味のあるパタンは '0' '1'
char
BlifLogicNode::opat() const
{
  return mOpat;
}


//////////////////////////////////////////////////////////////////////
// クラス BlifLatchNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID 番号
// @param[in] name ノード名
// @param[in] inode_id 入力ノードのID番号
// @param[in] rval リセット値
BlifLatchNode::BlifLatchNode(ymuint32 id,
			     const char* name,
			     ymuint32 inode_id,
			     char rval) :
  BlifNodeImpl(id, name),
  mInodeId(inode_id),
  mResetVal(rval)
{
}

// @brief デストラクタ
BlifLatchNode::~BlifLatchNode()
{
}

// @brief 型を返す．
BlifNode::tType
BlifLatchNode::type() const
{
  return kLatch;
}

// @brief 内容を blif 形式で出力する．
// @param[in] s 出力先のストリーム
void
BlifLatchNode::write_blif(ostream& s) const
{
}

// @brief 入力ノードのID番号を返す．
ymuint32
BlifLatchNode::inode_id() const
{
  return mInodeId;
}

// @brief リセット値を返す．
char
BlifLatchNode::reset_val() const
{
  return mResetVal;
}

END_NAMESPACE_YM_NETWORKS_BLIF
