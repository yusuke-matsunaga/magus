#ifndef SYMBOL_H
#define SYMBOL_H

/// @file Symbol.h
/// @brief Symbol のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright(C) 2002, 2013 by Yusuke Matsunaga


#include "led_nsdef.h"


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

  /// @brief 入力を生成する．
  static
  Symbol*
  input_symbol();

  /// @brief 出力を生成する．
  static
  Symbol*
  output_symbol();

  /// @brief バッファを生成する．
  static
  Symbol*
  buffer_symbol();

  /// @brief NOT ゲートを生成する．
  static
  Symbol*
  not_symbol();

  /// @brief AND ゲートを生成する．
  /// @param[in] ni 入力数
  static
  Symbol*
  and_symbol(ymuint ni);

  /// @brief OR ゲートを生成する．
  /// @param[in] ni 入力数
  static
  Symbol*
  or_symbol(ymuint ni);

  /// @brief XOR ゲートを生成する．
  /// @param[in] ni 入力数
  static
  Symbol*
  xor_symbol(ymuint ni);

  /// @brief 出力に否定のドットをつけたシンボルを生成する．
  /// @param[in] src_symbol もととなるシンボル．
  static
  Symbol*
  output_inv_symbol(Symbol* src_symbol);

  /// @brief 入力に否定のドットをつけたシンボルを生成する．
  /// @param[in] src_symbol もととなるシンボル．
  /// @param[in] pols 入力の否定の情報 (0 で肯定，1 で否定)
  static
  Symbol*
  input_inv_symbol(Symbol* src_symbol,
		   const vector<int>& pols);


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
