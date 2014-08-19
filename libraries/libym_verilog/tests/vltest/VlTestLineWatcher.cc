
/// @file VlTestLineWatcher.cc
/// @brief VlTestLineWatcher の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VlTestLineWatcher.h"


BEGIN_NAMESPACE_YM_VERILOG

// コンストラクタ
VlTestLineWatcher::VlTestLineWatcher(int line_unit) :
  mLineUnit(line_unit)
{
}

// デストラクタ
VlTestLineWatcher::~VlTestLineWatcher()
{
}

// 改行ごとの処理関数
void
VlTestLineWatcher::event_proc(int line)
{
  if ( (line % mLineUnit) == 0 ) {
    cout << "line: " << line << endl;
  }
}

END_NAMESPACE_YM_VERILOG
