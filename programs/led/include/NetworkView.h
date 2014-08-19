#ifndef NETWORKVIEW_H
#define NETWORKVIEW_H

/// @file NetworkView.h
/// @brief NetworkView のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "led_nsdef.h"
#include "GateType.h"


namespace nsYm {
namespace nsLed {

class GateMgr;
class GateObj;
class GateColor;

//////////////////////////////////////////////////////////////////////
/// @class NetworkView NetworkView.h "NetworkView.h"
/// @brief 回路図を表示するためのウィジェット
//////////////////////////////////////////////////////////////////////
class NetworkView :
  public QWidget
{

  Q_OBJECT

public:

  /// @brief コンストラクタ
  NetworkView(QWidget* parent = NULL);

  /// @brief デストラクタ
  virtual
  ~NetworkView();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ゲートを作る．
  /// @param[in] pos 位置
  /// @param[in] type 型
  /// @param[in] ni 入力数
  GateObj*
  new_gate(const QPoint& pos,
	   GateType type,
	   ymuint ni = 0);

  /// @brief ゲートを作る．
  /// @param[in] x, y 位置
  /// @param[in] type 型
  /// @param[in] ni 入力数
  GateObj*
  new_gate(int x,
	   int y,
	   GateType type,
	   ymuint ni = 0);

  /// @brief 倍率を得る．
  qreal
  scale() const;


public slots:
  //////////////////////////////////////////////////////////////////////
  // スロット
  //////////////////////////////////////////////////////////////////////

  /// @brief 倍率を変更する．
  /// @param[in] scale 倍率
  void
  setScale(qreal scale);

  /// @brief 倍率を変更する．
  /// @param[in] val パーセントで表した倍率値
  void
  setScale2(int val);

  /// @brief グリッド表示を行なう
  void
  enableGrid(int enable);


protected:
  //////////////////////////////////////////////////////////////////////
  // QWidget の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 描画イベントのハンドラ
  virtual
  void
  paintEvent(QPaintEvent* event);

  /// @brief マウスが押された時のハンドラ
  virtual
  void
  mousePressEvent(QMouseEvent* event);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief サイズや倍率が変更されたときに再計算を行なう．
  void
  calc_size();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ゲートを管理するオブジェクト
  GateMgr* mGateMgr;

  // ゲートリスト
  vector<GateObj*> mGateList;

  // 全てのゲートを囲む最小の矩形
  QRect mBoundingBox;

  // 倍率
  qreal mScale;

  // デフォルトのゲート描画設定
  GateColor* mDefaultColor;

  // グリッドの感覚
  ymint32 mGridSize;

  // グリッドの描画を制御するフラグ
  bool mGridEnable;

};

} // nsLed
} // nsYm

#endif // NETWORKVIEW_H
