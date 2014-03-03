
/// @file CellBus.sip
/// @brief CellBus の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellBus /Abstract/
{
%TypeHeaderCode
#include "cell/CellBus.h"
using nsYm::CellBus;
%End

public:

  /// @brief 名前の取得
  const char*
  name() const;
  %MethodCode
  sipRes = sipCpp->name().c_str();
  %End

  /// @brief バスの型の取得
  const CellBusType*
  bus_type() const;

  /// @brief ピン数の取得
  unsigned int
  pin_num() const;

  /// @brief ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
  const CellPin*
  pin(unsigned int pos) const;


private:

  ~CellBus();

};