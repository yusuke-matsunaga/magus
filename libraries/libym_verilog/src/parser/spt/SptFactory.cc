
/// @file SptFactory.cc
/// @brief SptFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// Pt クラス(の継承クラス)を生成するファクトリクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptFactory::SptFactory(Alloc& alloc) :
  PtiFactory(alloc)
{
}

// デストラクタ
SptFactory::~SptFactory()
{
}

// @brief 文字列領域の確保
const char*
SptFactory::new_string(const char* str)
{
  ymuint len = strlen(str) + 1;
  char* dst = static_cast<char*>(alloc().get_memory(len));
  strcpy(dst, str);
  return dst;
}

// 確保したオブジェクトに関する情報を出力する．
void
SptFactory::dump_profile(ostream& s) const
{
  // 未実装
}

END_NAMESPACE_YM_VERILOG
