#ifndef YMUTILSTESTREGISTRY_H
#define YMUTILSTESTREGISTRY_H

/// @file YmUtilsTestRegistry.h
/// @brief YmUtilsTestRegistry のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include <cppunit/extensions/TestFactoryRegistry.h>


//////////////////////////////////////////////////////////////////////
/// @class YmUtilsTestRegistry YmUtilsTestRegistry.h
/// @brief libym_utils 用の CppUnit テストレジストリ
//////////////////////////////////////////////////////////////////////
class YmUtilsTestRegistry :
  public CppUnit::TestFactoryRegistry
{
public:

  /// @brief コンストラクタ
  YmUtilsTestRegistry();

  /// @brief デストラクタ
  virtual
  ~YmUtilsTestRegistry();

};



#endif // YMUTILSTESTREGISTRY_H
