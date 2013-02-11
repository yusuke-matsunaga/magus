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

class DrawObj;

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


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AND/OR/XOR ゲートの時の諸元を設定する．
  /// @param[in] ni 入力数
  /// @param[in] gate_w ゲートの幅
  /// @param[in] or_xor OR/XOR ゲートの時 true にするフラグ
  void
  calc_points(ymuint ni,
	      qreal gate_w,
	      bool or_xor);

  /// @brief 描画用のオブジェクトを設定する．
  /// @param[in] obj 設定する描画用オブジェクト
  void
  set_drawobj(DrawObj* obj);

  /// @brief 描画用のオブジェクトを設定する．
  /// @param[in] obj1, obj2 設定する描画用オブジェクト
  void
  set_drawobj(DrawObj* obj1,
	      DrawObj* obj2);

  /// @brief 描画用のオブジェクトを設定する．
  /// @param[in] obj1, obj2, obj3 設定する描画用オブジェクト
  void
  set_drawobj(DrawObj* obj1,
	      DrawObj* obj2,
	      DrawObj* obj3);

  /// @brief 描画用のオブジェクトを設定する．
  /// @param[in] obj_list 設定する描画用オブジェクトのリスト
  void
  set_drawobj(const vector<DrawObj*>& obj_list);


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

  // 描画用のオブジェクトの要素数
  ymuint32 mObjNum;

  // 描画用のオブジェクトの配列
  DrawObj** mObjList;

};

END_NAMESPACE_YM_LED

#endif // SYMBOLIMPL_H
