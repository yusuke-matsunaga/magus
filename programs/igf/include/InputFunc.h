#ifndef INPUTFUNC_H
#define INPUTFUNC_H

/// @file InputFunc.h
/// @brief InputFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class InputFunc InputFunc.h "InputFunc.h"
/// @brief PHF の入力関数を表すクラス
//////////////////////////////////////////////////////////////////////
class InputFunc
{
public:

  /// @brief デストラクタ
  virtual
  ~InputFunc() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数値を求める．
  virtual
  ymuint32
  eval(const RegVect*) const = 0;

  /// @brief 内容を表示する．
  virtual
  void
  dump(ostream& s) const = 0;

};

END_NAMESPACE_YM_IGF

#endif // INPUTFUNC_H
