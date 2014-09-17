
/// @file MislibNodeImpl.cc
/// @brief MislibNodeImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MislibNodeImpl.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// クラス MislibNodeImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc 位置情報
MislibNodeImpl::MislibNodeImpl(const FileRegion& loc) :
  mLoc(loc)
{
}

// デストラクタ
MislibNodeImpl::~MislibNodeImpl()
{
}

// @brief 位置情報を取り出す．
FileRegion
MislibNodeImpl::loc() const
{
  return mLoc;
}

// @brief 論理式を表す型のときに true を返す．
// @note 定数0と定数1は false を返す．
// @note デフォルトでは false を返す．
bool
MislibNodeImpl::is_expr() const
{
  return false;
}

// 文字列を取り出す
// デフォルトでは空文字列を返す．
ShString
MislibNodeImpl::str() const
{
  return ShString(static_cast<const char*>(0));
}

// 数値を取り出す
// デフォルトでは 0.0 を返す．
double
MislibNodeImpl::num() const
{
  return 0.0;
}

// 1番目の子供を取り出す．
// デフォルトでは NULL を返す．
const MislibNode*
MislibNodeImpl::child1() const
{
  return NULL;
}

// 2番目の子供を取り出す．
// デフォルトでは NULL を返す．
const MislibNode*
MislibNodeImpl::child2() const
{
  return NULL;
}

// @brief 対応する論理式を生成する．
// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
Expr
MislibNodeImpl::to_expr(const NameMap& name_map) const
{
  // デフォルトではエラーとなる．
  assert_not_reached(__FILE__, __LINE__);
  return Expr();
}

// ピン名を表すオブジェクトを取り出す．
// デフォルトでは NULL を返す．
const MislibNode*
MislibNodeImpl::name() const
{
  return NULL;
}

// 極性情報を表すオブジェクトを取り出す．
// デフォルトでは NULL を返す．
const MislibNode*
MislibNodeImpl::phase() const
{
  return NULL;
}

// 入力負荷を表すオブジェクトを取り出す．
// デフォルトでは NULL を返す．
const MislibNode*
MislibNodeImpl::input_load() const
{
  return NULL;
}

// 最大駆動負荷を表すオブジェクトを取り出す．
// デフォルトでは NULL を返す．
const MislibNode*
MislibNodeImpl::max_load() const
{
  return NULL;
}

// 立ち上がり固定遅延を表すオブジェクトを取り出す．
// デフォルトでは NULL を返す．
const MislibNode*
MislibNodeImpl::rise_block_delay() const
{
  return NULL;
}

// 立ち上がり負荷依存遅延を表すオブジェクトを取り出す．
// デフォルトでは NULL を返す．
const MislibNode*
MislibNodeImpl::rise_fanout_delay() const
{
  return NULL;
}

// 立ち下がり固定遅延を表すオブジェクトを取り出す．
// デフォルトでは NULL を返す．
const MislibNode*
MislibNodeImpl::fall_block_delay() const
{
  return NULL;
}

// 立ち下がり負荷依存遅延を表すオブジェクトを取り出す．
// デフォルトでは NULL を返す．
const MislibNode*
MislibNodeImpl::fall_fanout_delay() const
{
  return NULL;
}

// 次のピンを設定する．
// デフォルトでは何もしない．
void
MislibNodeImpl::set_next(MislibNodeImpl* pin)
{
}

// 次のピンを取り出す．
// デフォルトでは NULL を返す．
const MislibNode*
MislibNodeImpl::next() const
{
  return NULL;
}

// 末尾に要素を追加する．
// デフォルトでは何もしない．
void
MislibNodeImpl::push_back(MislibNodeImpl* pin)
{
}

// 先頭の要素を取り出す．
// デフォルトでは NULL を返す．
const MislibNode*
MislibNodeImpl::top() const
{
  return NULL;
}

// @brief 面積を表すオブジェクトを返す．
// @note デフォルトでは NULL を返す．
const MislibNode*
MislibNodeImpl::area() const
{
  return NULL;
}

// @brief 出力ピン名を表すオブジェクトを返す．
// @note デフォルトでは NULL を返す．
const MislibNode*
MislibNodeImpl::opin_name() const
{
  return NULL;
}

// @brief 出力の論理式を表すオブジェクトを返す．
// @note デフォルトでは NULL を返す．
const MislibNode*
MislibNodeImpl::opin_expr() const
{
  return NULL;
}

// @brief 入力ピンの先頭を表すオブジェクトを返す．
// @note デフォルトでは NULL を返す．
const MislibNode*
MislibNodeImpl::ipin_top() const
{
  return NULL;
}

// 位置を出力する．
void
MislibNodeImpl::dump_loc(ostream& s) const
{
  s << "<LOC>" << loc() << "</LOC>" << endl;
}

END_NAMESPACE_YM_MISLIB
