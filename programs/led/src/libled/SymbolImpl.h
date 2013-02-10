#ifndef SYMBOLIMPL_H
#define SYMBOLIMPL_H

/// @file SymbolImpl
/// @brief SymbolImpl
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Symbol.h"


BEGIN_NAMESPACE_YM_LED

//////////////////////////////////////////////////////////////////////
/// @class SymbolImpl SymbolImpl.h "SymbolImpl.h"
/// @brief Symbol の実装クラス
//////////////////////////////////////////////////////////////////////
class SymbolImpl :
  public Symbol
{
public:

  /// @brief コンストラクタ
  SymbolImpl();

  /// @brief デストラクタ
  ~SymbolImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力に設定する．
  void
  set_to_input();

  /// @brief 出力に設定する．
  void
  set_to_output();

  /// @brief バッファに設定する．
  void
  set_to_buffer();

  /// @brief AND ゲートに設定する．
  /// @param[in] ni 入力数
  void
  set_to_and(ymuint ni);

  /// @brief OR ゲートに設定する．
  /// @param[in] ni 入力数
  void
  set_to_or(ymuint ni);

  /// @brief XOR ゲートに設定する．
  /// @param[in] ni 入力数
  void
  set_to_xor(ymuint ni);


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
  virtual
  void
  draw(QPainter& painter) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 描画用の PainterPath を得る．
  QPainterPath&
  path();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // シンボルを囲む矩形
  QRect mBBox;

  // 入力ピンの位置の配列
  vector<QPoint> mIpinList;

  // 出力ピンの位置の配列
  vector<QPoint> mOpinList;

  // 描画用の PainterPath
  QPainterPath mPath;

};

END_NAMESPACE_YM_LED

#endif // SYMBOLIMPL_H
