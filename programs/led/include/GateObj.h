#ifndef GATEOBJ_H
#define GATEOBJ_H

/// @file GateObj.h
/// @brief GateObj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright(C) 2002, 2013 by Yusuke Matsunaga


#include "led_nsdef.h"
#include "YmUtils/DlList.h"


BEGIN_NAMESPACE_YM_LED

class GateColor;
class Symbol;

//////////////////////////////////////////////////////////////////////
/// @class GateObj GateObj.h "GateObj.h"
/// @brief ゲートのインスタンスを表すクラス
///
/// 元となるゲートテンプレートとこのインスタンスの位置の情報を持つ．
/// 位置は左上隅の座標
//////////////////////////////////////////////////////////////////////
class GateObj :
  public DlElem
{
public:

  /// @brief コンストラクタ
  /// @param[in] sym シンボル
  /// @param[in] pos 位置
  GateObj(const Symbol* sym,
	  const QPoint& pos = QPoint(0, 0));

  /// @brief コンストラクタ
  /// @param[in] sym シンボル
  /// @param[in] x 位置のX座標
  /// @param[in] y 位置のY座標
  GateObj(const Symbol* sym,
	  ymuint x,
	  ymuint y);

  /// @brief デストラクタ
  ~GateObj();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 描画用の設定を行なう．
  void
  set_color(const GateColor* color);

  /// @brief 位置を設定する．
  /// @param[in] pos 設定する位置
  void
  set_position(const QPoint& pos);

  /// @brief 位置を設定する．
  /// @param[in] x 位置のX座標
  /// @param[in] y 位置のY座標
  void
  set_position(ymuint x,
	       ymuint y);

  /// @brief 位置を取得する．
  QPoint
  position() const;

  /// @brief このゲートを囲む最小の矩形を得る．
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

  /// @brief 描画する．
  void
  draw(QPainter& painter) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // シンボル
  const Symbol* mSymbol;

  // 描画用の設定
  const GateColor* mColor;

  // 位置
  QPoint mPosition;

};

/// @brief GateObj のリスト
typedef DlList<GateObj> GateObjList;

/// @brief GateObjList の反復子
typedef DlListIter<GateObj> GateObjListIter;

/// @brief GateObjList の定数反復子
typedef DlListConstIter<GateObj> GateObjListConstIter;

END_NAMESPACE_YM_LED

#endif // GATEOBJ_H
