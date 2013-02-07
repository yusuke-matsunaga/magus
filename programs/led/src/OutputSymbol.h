#ifndef OUTPUTSYMBOL_H
#define OUTPUTSYMBOL_H

/// @file OutputSymbol.h
/// @brief OutputSymbol のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Symbol.h"


BEGIN_NAMESPACE_YM_LED

//////////////////////////////////////////////////////////////////////
/// @class OutputSymbol OutputSymbol.h "OutputSymbol.h"
/// @brief 出力ピンのシンボルを表すクラス
//////////////////////////////////////////////////////////////////////
class OutputSymbol :
  public Symbol
{
public:

  /// @brief コンストラクタ
  OutputSymbol();

  /// @brief デストラクタ
  virtual
  ~OutputSymbol();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
  virtual
  QRect
  bounding_box() const;

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

  /// @brief 描画を行う．
  /// @param[in] painter 描画を行うオブジェクト
  /// @param[in] ul_pos オブジェクトの左上の位置
  /// @param[in] rect 描画矩形
  virtual
  void
  draw(QPainter& painter,
       const QPoint& ul_pos,
       const QRect& rect) const;

};

END_NAMESPACE_YM_LED

#endif // OUTPUTSYMBOL_H
