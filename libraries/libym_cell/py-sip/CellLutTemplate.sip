
/// @file CellLutTemplate.sip
/// @brief CellLutTemplate の sip ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellLutTemplate /Abstract/
{
%TypeHeaderCode
#include "cell/CellLutTemplate.h"
using nsYm::CellLutTemplate;
%End

public:

  /// @brief 名前の取得
  const char*
  name() const;

  /// @brief 次元数の取得
  unsigned int
  dimension() const;

  /// @brief 変数型の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  tCellVarType
  variable_type(unsigned int var) const;

  /// @brief インデックス数の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  unsigned int
  index_num(unsigned int var) const;

  /// @brief デフォルトインデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  double
  index(unsigned int var,
	unsigned int pos) const;

private:

  ~CellLutTemplate();

};
