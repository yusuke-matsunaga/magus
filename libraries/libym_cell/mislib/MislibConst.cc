
/// @file libym_mislib/MislibConst.cc
/// @brief MislibConst0, MislibConst1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibPtImpl1.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MislibConst.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// クラス MislibConst0
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibConst0::MislibConst0(const FileRegion& loc) :
  MislibNodeImpl(loc)
{
}

// デストラクタ
MislibConst0::~MislibConst0()
{
}

// 種類を取り出す．
MislibNode::tType
MislibConst0::type() const
{
  return kConst0;
}

// @brief 論理式を表す型のときに true を返す．
bool
MislibConst0::is_expr() const
{
  return true;
}

// @brief 対応する論理式を生成する．
// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
LogExpr
MislibConst0::to_expr(const hash_map<ShString, ymuint>& name_map) const
{
  return LogExpr::make_zero();
}

// 内容を出力する．
// デバッグ用
void
MislibConst0::dump(ostream& s) const
{
  s << "<CONST0>" << endl;
  dump_loc(s);
  s << "</CONST0>" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibConst1
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibConst1::MislibConst1(const FileRegion& loc) :
  MislibNodeImpl(loc)
{
}

// デストラクタ
MislibConst1::~MislibConst1()
{
}

// 種類を取り出す．
MislibNode::tType
MislibConst1::type() const
{
  return kConst1;
}

// @brief 論理式を表す型のときに true を返す．
bool
MislibConst1::is_expr() const
{
  return true;
}

// @brief 対応する論理式を生成する．
// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
LogExpr
MislibConst1::to_expr(const hash_map<ShString, ymuint>& name_map) const
{
  return LogExpr::make_one();
}

// 内容を出力する．
// デバッグ用
void
MislibConst1::dump(ostream& s) const
{
  s << "<CONST1>" << endl;
  dump_loc(s);
  s << "</CONST1>" << endl;
}

END_NAMESPACE_YM_MISLIB
