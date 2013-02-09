#ifndef XORSYMBOL_H
#define XORSYMBOL_H

/// @file XorSymbol.h
/// @brief XorSymbol のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GateSymbol.h"


BEGIN_NAMESPACE_YM_LED

//////////////////////////////////////////////////////////////////////
/// @class XorSymbol XorSymbol.h "XorSymbol.h"
/// @brief XORゲートのシンボルを表すクラス
//////////////////////////////////////////////////////////////////////
class XorSymbol :
  public GateSymbol
{
public:

  /// @brief コンストラクタ
  /// @param[in] ipols 入力の極性 ( 0 で肯定, 1 で否定を表す )
  /// @param[in] opol 出力の極性 ( 0 で肯定, 1 で否定を表す )
  XorSymbol(const vector<int>& ipols,
	    int opol);

  /// @brief デストラクタ
  virtual
  ~XorSymbol();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
  virtual
  QRect
  bounding_box() const;

#if 0
  /// @brief 入力数を得る．
  virtual
  ymuint
  ipin_num() const;

  /// @brief pos 番目の入力ピン位置を得る．
  /// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
  virtual
  QPoint
  ipin_location(ymuint pos) const;

  /// @brief 出力数を得る．
  virtual
  ymuint
  opin_num() const;

  /// @brief pos 番目の出力ピン位置を得る．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  virtual
  QPoint
  opin_location(ymuint pos) const;
#endif

  /// @brief 描画を行う．
  /// @param[in] painter 描画を行うオブジェクト
  virtual
  void
  draw(QPainter& painter) const;

};

END_NAMESPACE_YM_LED

#endif // XORSYMBOL_H
