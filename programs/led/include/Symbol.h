#ifndef SYMBOL_H
#define SYMBOL_H

/// @file Symbol.h
/// @brief Symbol のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright(C) 2002, 2013 by Yusuke Matsunaga


#include "led_nsdef.h"
#include "GateType.h"


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
  // インスタンスを生成するクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 単純な型のシンボルを生成する．
  /// @param[in] type 型
  /// @param[in] ni 入力数
  /// @note 入力/出力/バッファ/NOT の場合は ni の値は無視される．
  /// @note それ以外の場合は ni は 2 以上でなければならない．
  static
  Symbol*
  new_symbol(GateType type,
	     ymuint ni = 0);


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
  virtual
  void
  draw(QPainter& painter) const = 0;

};

END_NAMESPACE_YM_LED

#endif // SYMBOL_H
