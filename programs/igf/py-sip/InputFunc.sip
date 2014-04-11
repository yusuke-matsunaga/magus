
/// @file InputFunc.sip
/// @brief InputFunc の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class InputFunc
{
%TypeHeaderCode
#include "InputFunc.h"
using nsYm::nsIgf::InputFunc;
%End

  /// @brief 出力のビット幅を返す．
  unsigned int
  output_width() const;

  /// @brief 関数値を求める．
  /// @param[in] rv 登録ベクタ
  unsigned int
  eval(const RegVect* rv) const;


private:

  InputFunc();

};