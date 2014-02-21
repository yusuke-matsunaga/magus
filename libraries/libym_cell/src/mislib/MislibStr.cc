
/// @file MislibStr.cc
/// @brief MislibStr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MislibStr.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// クラス MislibStr
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibStr::MislibStr(const FileRegion& loc,
		     ShString str) :
  MislibNodeImpl(loc),
  mStr(str)
{
}

// デストラクタ
MislibStr::~MislibStr()
{
}

// 種類を取り出す．
MislibNode::tType
MislibStr::type() const
{
  return kStr;
}

// @brief 論理式を表す型のときに true を返す．
bool
MislibStr::is_expr() const
{
  return true;
}

// 文字列を取り出す
ShString
MislibStr::str() const
{
  return mStr;
}

// @brief 対応する論理式を生成する．
// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
LogExpr
MislibStr::to_expr(const hash_map<ShString, ymuint>& name_map) const
{
  hash_map<ShString, ymuint>::const_iterator p = name_map.find(str());
  assert_cond( p != name_map.end(), __FILE__, __LINE__);
  ymuint id = p->second;
  VarId var(id);
  return LogExpr::make_posiliteral(var);
}

// 内容を出力する．
// デバッグ用
void
MislibStr::dump(ostream& s) const
{
  s << "<STR>" << endl;
  dump_loc(s);
  s << ShString(str()) << endl
    << "</STR>" << endl;
}

END_NAMESPACE_YM_MISLIB
