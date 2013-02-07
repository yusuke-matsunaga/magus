#ifndef SYMBOL_H
#define SYMBOL_H

/// @file Symbol.h
/// @brief Symbol のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright(C) 2002, 2013 by Yusuke Matsunaga


#include "led_nsdef.h"
#include <QPoint>
#include <QRect>


BEGIN_NAMESPACE_YM_LED

//////////////////////////////////////////////////////////////////////
/// @class Symbol Symbol.h "Symbol.h"
/// @brief ゲートを描画するためのテンプレート
//////////////////////////////////////////////////////////////////////
class Symbol
{
public:

  /// @brief デストラクタ
  virtual
  ~Symbol() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
  virtual
  QRect
  bounding_box() const = 0;

  /// @brief 入力数を得る．
  virtual
  ymuint
  ipin_num() const = 0;

  /// @brief pos 番目の入力ピン位置を得る．
  /// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
  virtual
  QPoint
  ipin_location(ymuint pos) const = 0;

  /// @brief 出力数を得る．
  virtual
  ymuint
  opin_num() const = 0;

  /// @brief pos 番目の出力ピン位置を得る．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  virtual
  QPoint
  opin_location(ymuint pos) const = 0;

  /// @brief 描画を行う．
  /// @param[in] painter 描画を行うオブジェクト
  /// @param[in] ul_pos オブジェクトの左上の位置
  /// @param[in] rect 描画矩形
  virtual
  void
  draw(QPainter& painter,
       const QPoint& ul_pos,
       const QRect& rect) const = 0;

};

END_NAMESPACE_YM_LED

#endif // SYMBOL_H
