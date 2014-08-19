
/// @file src/pt/PtNode.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_BB

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
PtNode::PtNode(const FileRegion& file_region) :
  mFileRegion(file_region)
{
}

// @param[in]
// @brief デストラクタ
PtNode::~PtNode()
{
}

// @brief ID 番号を返す．
// @note ID型のノードの時に意味を持つ．
ymuint
PtNode::id() const
{
  return 0;
}

// @brief 定数値を返す．
// @note 定数型のノードの時に意味を持つ．
int
PtNode::cvalue() const
{
  return 0;
}

// @brief オペランド数を返す．
// @note 演算型のノードの時に意味を持つ．
ymuint
PtNode::operand_num() const
{
  return 0;
}

// @brief オペランドを返す．
// @param[in] pos 位置番号 ( 0 <= pos < operand_num() )
// @note 演算型のノードの時に意味を持つ．
PtNode*
PtNode::operand(ymuint pos) const
{
  return NULL;
}

END_NAMESPACE_YM_BB
