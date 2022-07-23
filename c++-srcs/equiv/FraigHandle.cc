
/// @file FraigHandle.cc
/// @brief FraigHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FraigHandle.h"
#include "FraigNode.h"


BEGIN_NAMESPACE_FRAIG

//////////////////////////////////////////////////////////////////////
// FraigHandle
//////////////////////////////////////////////////////////////////////

// @brief 外部入力ノードへのハンドルのとき true を返す．
bool
FraigHandle::is_input() const
{
  auto n = node();
  if ( n && n->is_input() ) {
    return true;
  }
  else {
    return false;
  }
}

// @brief 外部入力ノードへのハンドルのとき，入力番号を返す．
//
// is_input() の時のみ意味を持つ．
SizeType
FraigHandle::input_id() const
{
  auto n = node();
  if ( n && n->is_input() ) {
    return n->input_id();
  }
  return 0;
}

// @relates FraigHandle
// @brief 内容を出力する関数
ostream&
operator<<(
  ostream& s,
  FraigHandle src
)
{
  if ( src.is_zero() ) {
    s << "ZERO";
  }
  else if ( src.is_one() ) {
    s << "ONE";
  }
  else {
    if ( src.inv() ) {
      s << "~";
    }
    if ( src.is_input() ) {
      s << "I" << src.input_id();
    }
    else {
      s << "A" << src.node()->id();
    }
  }
  return s;
}

END_NAMESPACE_FRAIG
