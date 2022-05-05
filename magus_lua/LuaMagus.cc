
/// @file LuaMagus.cc
/// @brief LuaMagus の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LuaMagus.h"


BEGIN_NAMESPACE_MAGUS

//@ brief Magus 拡張に関する初期化を行う．
//
// この関数を呼ばないと Luapp と同等になる．
void
LuaMagus::open_Magus()
{
  init_Bnet();
}

END_NAMESPACE_MAGUS
