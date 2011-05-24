#ifndef LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDFFCLASS_H
#define LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDFFCLASS_H

/// @file libym_techmap/cellmap/libdump/LdFFClass.h
/// @brief LdFFClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"


BEGIN_NAMESPACE_YM_CELLMAP_LIBDUMP

//////////////////////////////////////////////////////////////////////
/// @class LdFFClass LdFFClass.h "LdFFClass.h"
/// @brief FFのクラスを表すクラス
/// ここでは，ピン位置の違いを無視した同機能のFFの集合をクラスと呼ぶ．
//////////////////////////////////////////////////////////////////////
class LdFFClass
{
  friend class LdFFMgr;

private:

  /// @brief コンストラクタ
  LdFFClass();

  /// @brief デストラクタ
  ~LdFFClass();


public:

};

END_NAMESPACE_YM_CELLMAP_LIBDUMP

#endif // LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDFFCLASS_H
