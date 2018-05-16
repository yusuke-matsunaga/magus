
/// @file SbjNode.cc
/// @brief SbjNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "SbjNode.h"


BEGIN_NAMESPACE_YM_SBJ

///////////////////////////////////////////////////////////////////////
// クラス SbjNode
///////////////////////////////////////////////////////////////////////

// @brief コンストラクタ(入力)
// @param[in] id ID番号
// @param[in] bipol 両極性が利用可能な時に true になるフラグ
// @param[in] subid 入力/出力番号
SbjNode::SbjNode(int id,
		 bool bipol,
		 int subid) :
  mId(id),
  mSubId(subid)
{
  mFlags = static_cast<ymuint>(SbjNodeType::Input) |
    (static_cast<ymuint>(bipol) << kBiPolShift);
}

// @brief コンストラクタ(出力)
// @param[in] id ID番号
// @param[in] subid 入力/出力番号
// @param[in] input ファンインのハンドル
SbjNode::SbjNode(int id,
		 int subid,
		 SbjHandle input) :
  mId(id),
  mSubId(subid)
{
  mFlags = static_cast<ymuint>(SbjNodeType::Output);
  mFanins[0] = input;

  SbjNode* inode = input.node();
  mFlags |= static_cast<ymuint>(inode->level() << kLevelShift);

  inode->mFlags |= kPoMask;
  inode->mFanoutList.push_back(SbjEdge(this, 0));
}

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] type ノードタイプ
// @param[in] bipol 両極性が利用可能な時に true になるフラグ
// @param[in] subid 入力/出力番号
// @param[in] input0, input1 ファンインのハンドル
SbjNode::SbjNode(int id,
		 SbjNodeType type,
		 SbjHandle input0,
		 SbjHandle input1) :
  mId(id),
  mSubId(0)
{
  mFlags = static_cast<ymuint>(type);
  mFanins[0] = input0;
  mFanins[1] = input1;

  SbjNode* inode0 = input0.node();
  inode0->mFanoutList.push_back(SbjEdge(this, 0));
  SbjNode* inode1 = input1.node();
  inode1->mFanoutList.push_back(SbjEdge(this, 1));
  int level = 0;
  if ( inode0->level() >= inode1->level() ) {
    level = inode0->level();
  }
  else {
    level = inode1->level();
  }
  level += 1;
  mFlags |= static_cast<ymuint>(level << kLevelShift);
}

// デストラクタ
SbjNode::~SbjNode()
{
}

// @brief ID を表す文字列の取得
string
SbjNode::id_str() const
{
  const char* type_str = nullptr;
  if ( is_input() ) {
    type_str = "I";
  }
  else if ( is_output() ) {
    type_str = "O";
  }
  else if ( is_logic() ) {
    type_str = "L";
  }
  else {
    type_str = "X";
  }

  ostringstream buf;
  buf << type_str << id();

  return buf.str();
}

END_NAMESPACE_YM_SBJ
