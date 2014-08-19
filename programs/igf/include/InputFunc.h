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
/// @brief 入力ハッシュ関数を表すクラス
///
/// 登録ベクタ(RegVect) を入力としてハッシュ値を出力する関数
/// シグネチャ関数とも呼ぶ．
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

  /// @brief 出力のビット幅を返す．
  virtual
  ymuint
  output_width() const = 0;

  /// @brief 関数値を求める．
  /// @param[in] rv 登録ベクタ
  virtual
  ymuint32
  eval(const RegVect* rv) const = 0;

  /// @brief 内容を表示する．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  dump(ostream& s) const = 0;

};

END_NAMESPACE_YM_IGF

#endif // INPUTFUNC_H
