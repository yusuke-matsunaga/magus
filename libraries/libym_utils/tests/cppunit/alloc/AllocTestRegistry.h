#ifndef ALLOCTESTREGISTRY_H
#define ALLOCTESTREGISTRY_H

/// @file AllocTestRegistry.h
/// @brief AllocTestRegistry のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>



//////////////////////////////////////////////////////////////////////
/// @class AllocTestRegistry AllocTestRegistry.h
/// @brief alloc グループのテストレジストリ
//////////////////////////////////////////////////////////////////////
class AllocTestRegistry :
  public CppUnit::TestFactoryRegistry
{
public:

  /// @brief コンストラクタ
  AllocTestRegistry();

  /// @brief デストラクタ
  virtual
  ~AllocTestRegistry();

};



#endif // ALLOCTESTREGISTRY_H
