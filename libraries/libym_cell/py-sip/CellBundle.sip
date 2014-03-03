
/// @file CellBundle.sip
/// @brief CellBundle の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellBundle /Abstract/
{
%TypeHeaderCode
#include "cell/CellBundle.h"
using nsYm::CellBundle;
%End

public:

  /// @brief 名前の取得
  const char*
  name() const;
  %MethodCode
  sipRes = sipCpp->name().c_str();
  %End

  /// @brief ピン数の取得
  unsigned int
  pin_num() const;

  /// @brief ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
  const CellPin*
  pin(unsigned int pos) const;

private:

  ~CellBundle();

};