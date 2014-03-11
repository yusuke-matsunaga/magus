#ifndef MINCOVTESTREGISTRY_H
#define MINCOVTESTREGISTRY_H

/// @file MincovTestRegistry.h
/// @brief MincovTestRegistry のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include <cppunit/extensions/TestFactoryRegistry.h>


//////////////////////////////////////////////////////////////////////
/// @class MincovTestRegistry MincovTestRegistry.h
/// @brief mincov グループのテストレジストリ
//////////////////////////////////////////////////////////////////////
class MincovTestRegistry :
  public CppUnit::TestFactoryRegistry
{
public:

  /// @brief コンストラクタ
  MincovTestRegistry();

  /// @brief デストラクタ
  virtual
  ~MincovTestRegistry();

};

#endif // MINCOVTESTREGISTRY_H
