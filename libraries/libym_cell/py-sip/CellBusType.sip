
/// @file CellBusType.sip
/// @brief CellBusType の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellBusType /Abstract/
{
%TypeHeaderCode
#include "cell/CellBusType.h"
using nsYm::CellBusType;
%End

public:

  /// @brief base_type を表す型
  enum tBaseType {
    kArrayType
  };

  /// @brief data_type を表す型
  enum tDataType {
    kBitType
  };

  /// @brief 名前の取得
  const char*
  name() const;
  %MethodCode
  sipRes = sipCpp->name().c_str();
  %End

  /// @brief base_type の取得
  tBaseType
  base_type() const;

  /// @brief data_type の取得
  tDataType
  data_type() const;

  /// @brief ビット幅の取得
  unsigned int
  bit_width() const;

  /// @brief 開始ビットの取得
  int
  bit_from() const;

  /// @brief 終了ビットの取得
  int
  bit_to() const;

  /// @brief 向きの取得
  /// @note true の時，降順を表す．
  bool
  downto() const;


private:

  ~CellBusType();

};
