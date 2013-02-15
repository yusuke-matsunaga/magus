#ifndef SYMBOL_H
#define SYMBOL_H

/// @file Symbol.h
/// @brief Symbol のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "led_nsdef.h"
#include "GateType.h"


BEGIN_NAMESPACE_YM_LED

class DrawObj;

//////////////////////////////////////////////////////////////////////
/// @class SymbolImpl SymbolImpl.h "SymbolImpl.h"
/// @brief Symbol の実装クラス
//////////////////////////////////////////////////////////////////////
class Symbol
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] ni 入力数
  /// @note 入力/出力/バッファ/NOT の場合は ni の値は無視される．
  /// @note それ以外の場合は ni は 2 以上でなければならない．
  Symbol(GateType type,
	 ymuint ni = 0);

  /// @brief デストラクタ
  ~Symbol();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
  QRect
  bounding_box() const;

  /// @brief 入力数を得る．
  ymuint
  ipin_num() const;

  /// @brief pos 番目の入力ピン位置を得る．
  /// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
  QPoint
  ipin_location(ymuint pos) const;

  /// @brief 出力数を得る．
  ymuint
  opin_num() const;

  /// @brief pos 番目の出力ピン位置を得る．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  QPoint
  opin_location(ymuint pos) const;

  /// @brief シンボルの描画を行う．
  /// @param[in] painter 描画を行うオブジェクト
  void
  draw_symbol(QPainter& painter) const;

  /// @brief 枠線の描画を行う．
  /// @param[in] painter 描画を行うオブジェクト
  void
  draw_box(QPainter& painter) const;


private:
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
  /// @param[in] oinv 出力に否定を持つとき true にするフラグ
  void
  set_to_buffer(bool oinv);

  /// @brief AND ゲートに設定する．
  /// @param[in] ni 入力数
  /// @param[in] oinv 出力に否定を持つとき true にするフラグ
  void
  set_to_and(ymuint ni,
	     bool oinv);

  /// @brief OR ゲートに設定する．
  /// @param[in] ni 入力数
  /// @param[in] oinv 出力に否定を持つとき true にするフラグ
  void
  set_to_or(ymuint ni,
	    bool oinv);

  /// @brief XOR ゲートに設定する．
  /// @param[in] ni 入力数
  /// @param[in] oinv 出力に否定を持つとき true にするフラグ
  void
  set_to_xor(ymuint ni,
	     bool oinv);


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AND/OR/XOR ゲートの時の諸元を設定する．
  /// @param[in] ni 入力数
  /// @param[in] gate_w ゲートの幅
  /// @param[in] or_xor OR/XOR ゲートの時 true にするフラグ
  ymuint
  calc_points(ymuint ni,
	      qreal gate_w,
	      bool or_xor);

  /// @brief 描画用のオブジェクトを設定する．
  /// @param[in] obj 設定する描画用オブジェクト
  void
  set_drawobj(DrawObj* obj);

  /// @brief 描画用のオブジェクトを設定する．
  /// @param[in] obj_list 設定する描画用オブジェクトのリスト
  void
  set_drawobj(const vector<DrawObj*>& obj_list);

  /// @brief bounding box の計算を行う．
  void
  calc_bounding_box();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // シンボルを囲む矩形
  QRect mBoundingBox;

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

#endif // SYMBOL_H
