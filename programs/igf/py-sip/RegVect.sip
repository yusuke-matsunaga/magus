
/// @file RegVect.sip
/// @brief RegVect の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class RegVect
{
%TypeHeaderCode
#include "RegVect.h"
using nsYm::nsIgf::RegVect;
%End

private:

  /// @brief コンストラクタ
  /// @param[in] size ベクタのサイズ
  /// @param[in] index インデックス
  RegVect(unsigned int size,
	  unsigned int index);

  /// @brief デストラクタ
  ~RegVect();


public:

  /// @brief ベクタの長さを返す．
  unsigned int
  size() const;

  /// @brief インデックスを返す．
  unsigned int
  index() const;

  /// @brief 値を返す．
  /// @param[in] pos 位置 ( 0 <= pos < size() )
  /// @note 返される値は 0 か 1 のみ
  unsigned int
  val(unsigned int pos) const;

};