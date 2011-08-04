
/// @file libym_mislib/MislibNot.cc
/// @brief MislibNot の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MislibNot.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// クラス MislibNot
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibNot::MislibNot(const FileRegion& loc,
		     const MislibNode* child1) :
  MislibNodeImpl(loc),
  mChild1(child1)
{
}

// デストラクタ
MislibNot::~MislibNot()
{
}

// 種類を取り出す．
MislibNode::tType
MislibNot::type() const
{
  return kNot;
}

// @brief 論理式を表す型のときに true を返す．
bool
MislibNot::is_expr() const
{
  return true;
}

// 1番目の子供を取り出す．
const MislibNode*
MislibNot::child1() const
{
  return mChild1;
}

// @brief 対応する論理式を生成する．
// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
LogExpr
MislibNot::to_expr(const hash_map<ShString, ymuint>& name_map) const
{
  return ~child1()->to_expr(name_map);
}

// 内容を出力する．
// デバッグ用
void
MislibNot::dump(ostream& s) const
{
  s << "<NOT>" << endl;
  dump_loc(s);

  s << "<CHILD1>" << endl;
  child1()->dump(s);
  s << "</CHILD1>" << endl;

  s << "</NOT>" << endl;
}

END_NAMESPACE_YM_MISLIB
