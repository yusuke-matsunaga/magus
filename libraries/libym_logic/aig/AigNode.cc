
/// @file AigNode.cc
/// @brief AigNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "AigNode.h"


BEGIN_NAMESPACE_YM_AIG

BEGIN_NONAMESPACE

inline
AigNode*
unpack_node(ympuint pack)
{
  return reinterpret_cast<AigNode*>(pack & ~1UL);
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス Aig
//////////////////////////////////////////////////////////////////////

// @brief ノードの通し番号を得る．
ymuint
Aig::node_id() const
{
  AigNode* aignode = unpack_node(mPackedData);
  if ( aignode ) {
    return aignode->node_id();
  }
  else {
    return 0;
  }
}

// @brief 外部入力ノードへのハンドルのとき true を返す．
bool
Aig::is_input() const
{
  AigNode* aignode = unpack_node(mPackedData);
  return aignode != NULL && aignode->is_input();
}

// @brief 外部入力ノードへのハンドルのとき，入力番号を返す．
// @note is_input() の時のみ意味を持つ．
VarId
Aig::input_id() const
{
  AigNode* aignode = unpack_node(mPackedData);
  if ( aignode ) {
    return aignode->input_id();
  }
  else {
    return VarId(0);
  }
}

// @brief ANDノードへのハンドルのとき true を返す．
bool
Aig::is_and() const
{
  AigNode* aignode = unpack_node(mPackedData);
  return aignode != NULL && aignode->is_and();
}

// @brief pos で指示されたファンインのハンドルを得る．
// @note pos は 0 か 1 でなければならない．
// @note is_and() の時のみ意味を持つ．
Aig
Aig::fanin(ymuint pos) const
{
  AigNode* aignode = unpack_node(mPackedData);
  if ( aignode ) {
    return aignode->fanin(pos);
  }
  else {
    return Aig();
  }
}

// @brief fanin0 のハンドルを得る．
// @note is_and() の時のみ意味を持つ．
Aig
Aig::fanin0() const
{
  AigNode* aignode = unpack_node(mPackedData);
  if ( aignode ) {
    return aignode->fanin0();
  }
  else {
    return Aig();
  }
}

// @brief fanin1 のハンドルを得る．
// @note is_and() の時のみ意味を持つ．
Aig
Aig::fanin1() const
{
  AigNode* aignode = unpack_node(mPackedData);
  if ( aignode ) {
    return aignode->fanin1();
  }
  else {
    return Aig();
  }
}


/// @relates Aig
/// @brief 内容を出力する関数
ostream&
operator<<(ostream& s,
	   Aig src)
{
  if ( src.is_zero() ) {
    s << "CONST0";
  }
  else if ( src.is_one() ) {
    s << "CONST1";
  }
  else {
    if ( src.inv() ) {
      s << "~";
    }
    s << src.node_id();
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
// AIG(And-Inverter Graph) のノードを表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ノード番号
AigNode::AigNode(ymuint id) :
  mFlags(id << kSftN)
{
}

// デストラクタ
AigNode::~AigNode()
{
}

// @brief 入力タイプに設定する．
// @param[in] input_id 入力番号
void
AigNode::set_input(VarId input_id)
{
  mFlags |= (1U << kSftT);
  mFanins[0].mPackedData = input_id.val();
}

// @brief ANDタイプに設定する．
// @param[in] fanin0, fanin1 ファンインのハンドル
void
AigNode::set_and(Aig fanin0,
		 Aig fanin1)
{
  mFlags &= ~(1U << kSftT);
  mFanins[0] = fanin0;
  mFanins[1] = fanin1;
}

END_NAMESPACE_YM_AIG
