#ifndef GATESYMBOL_H
#define GATESYMBOL_H

/// @file GateSymbol.h
/// @brief GateSymbol のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Symbol.h"


BEGIN_NAMESPACE_YM_LED

//////////////////////////////////////////////////////////////////////
/// @class GateSymbol GateSymbol.h "GateSymbol.h"
/// @brief 入力ピンのシンボルを表すクラス
//////////////////////////////////////////////////////////////////////
class GateSymbol :
  public Symbol
{
public:

  /// @brief コンストラクタ
  /// @param[in] ipols 入力の極性 ( 0 で肯定, 1 で否定を表す )
  /// @param[in] opol 出力の極性 ( 0 で肯定, 1 で否定を表す )
  GateSymbol(const vector<int>& ipols,
	     int opol);

  /// @brief デストラクタ
  virtual
  ~GateSymbol();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

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


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスのための便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AND/OR/XOR ゲートの時の座標を計算する．
  void
  calc_points();

  /// @brief OR/XOR ゲートのときに入力側の弧を描く関数
  /// @param[in] painter 描画を行うオブジェクト
  /// @param[in] x0 X 座標
  void
  draw_input_arc(QPainter& painter,
		 ymuint x0);

  /// @brief 入力ピン位置を決めるための下請け関数
  /// @param[in] ni 入力数
  /// @param[out] npins ピンの Y 座標を格納する配列
  static
  void
  distribute_pins(ymuint ni,
		  vector<ymuint>& npins);

  /// @brief AND/OR/XOR の入力辺上のピンの座標を計算する関数
  /// @param[in] x_off X軸のオフセット
  /// @param[in] y_off Y軸のオフセット
  /// @param[in] pos 入力ピン位置
  /// @param[in] npins ピン数
  /// @param[in] or_xor OR/XOR の時 true にするフラグ
  static
  QPoint
  ipos(ymuint x_off,
       ymuint y_off,
       ymuint pos,
       ymuint npins,
       bool or_xor);

  /// @brief 円弧を表す点列を作る．
  /// @param[in] cx 中心の X 座標
  /// @param[in] cy 中心の Y 座標
  /// @param[in] r 半径
  /// @param[in] ndiv 分割数
  /// @param[in] start_rad 開始角度 (ラジアン)
  /// @param[in] end_rad 終了角度 (ラジアン)
  /// @param[out] points 結果の点列を格納する配列
  static
  void
  create_arc(ymuint cx,
	     ymuint cy,
	     ymuint r,
	     ymuint ndiv,
	     ymuint start_rad,
	     ymuint end_rad,
	     vector<QPoint>& points);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力の極性のベクタ
  vector<int> mIpols;

  // 出力の極性
  int mOpol;

};

END_NAMESPACE_YM_LED

#endif // GATESYMBOL_H
