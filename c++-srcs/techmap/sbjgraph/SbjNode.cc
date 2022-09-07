
/// @file SbjNode.cc
/// @brief SbjNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "SbjNode.h"


BEGIN_NAMESPACE_SBJ

///////////////////////////////////////////////////////////////////////
// クラス SbjNode
///////////////////////////////////////////////////////////////////////

// @brief コンストラクタ(入力)
SbjNode::SbjNode(
  SizeType id,
  bool bipol,
  SizeType subid
) : mId{id},
    mSubId{subid}
{
  mFlags = static_cast<ymuint>(SbjNodeType::Input) |
    (static_cast<ymuint>(bipol) << kBiPolShift);
}

// @brief コンストラクタ(出力)
SbjNode::SbjNode(
  SizeType id,
  SizeType subid,
  SbjHandle input
) : mId{id},
    mSubId{subid}
{
  mFlags = static_cast<ymuint>(SbjNodeType::Output);
  mFanins[0] = input;

  auto inode = input.node();
  mFlags |= static_cast<ymuint>(inode->level() << kLevelShift);

  inode->mFlags |= kPoMask;
  inode->mFanoutList.push_back(SbjEdge(this, 0));
}

// @brief コンストラクタ
SbjNode::SbjNode(
  SizeType id,
  SbjNodeType type,
  SbjHandle input0,
  SbjHandle input1
) : mId{id},
    mSubId{0}
{
  mFlags = static_cast<ymuint>(type);
  mFanins[0] = input0;
  mFanins[1] = input1;

  auto inode0 = input0.node();
  inode0->mFanoutList.push_back(SbjEdge(this, 0));
  auto inode1 = input1.node();
  inode1->mFanoutList.push_back(SbjEdge(this, 1));
  SizeType level = 0;
  if ( inode0->level() >= inode1->level() ) {
    level = inode0->level();
  }
  else {
    level = inode1->level();
  }
  level += 1;
  mFlags |= static_cast<ymuint>(level << kLevelShift);
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

END_NAMESPACE_SBJ
