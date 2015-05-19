#ifndef NLVIEW_H
#define NLVIEW_H

/// @file NlView.h
/// @brief NlView のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include <QWidget>


namespace nsYm { namespace nsNlink {

//////////////////////////////////////////////////////////////////////
/// @class NlView NlView.h "NlView.h"
/// @brief number link の問題を表示するウィジェット
//////////////////////////////////////////////////////////////////////
class NlView :
  public QWidget
{
  Q_OBJECT

public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のウィジェット
  /// @param[in] flags ウィンドウフラグ
  NlView(QWidget* parent = NULL,
	 Qt::WindowFlags flags = 0);

  /// @brief コンストラクタ
  /// @param[in] problem 問題
  /// @param[in] parent 親のウィジェット
  /// @param[in] flags ウィンドウフラグ
  NlView(const NlProblem& problem,
	 QWidget* parent = NULL,
	 Qt::WindowFlags flags = 0);

  /// @brief コンストラクタ
  /// @param[in] solution 解
  /// @param[in] parent 親のウィジェット
  /// @param[in] flags ウィンドウフラグ
  NlView(const NlSolution& solution,
	 QWidget* parent = NULL,
	 Qt::WindowFlags flags = 0);

  /// @brief デストラクタ
  virtual
  ~NlView();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 問題を表示する．
  /// @param[in] problem 問題
  void
  set_problem(const NlProblem& problem);

  /// @brief 解を表示する．
  /// @param[in] solution 解
  void
  set_solution(const NlSolution& solution);


public:
  //////////////////////////////////////////////////////////////////////
  // QWidget で定義されている仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief サイズヒントを返す．
  virtual
  QSize
  sizeHint() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // QWidget のイベントコールバック関数の再定義
  //////////////////////////////////////////////////////////////////////

  /// @brief paint イベント
  virtual
  void
  paintEvent(QPaintEvent* event);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 盤面のサイズを設定する．
  void
  set_size(ymuint width,
	   ymuint height);

  /// @brief xy座標からインデックスを得る．
  ymuint
  xy_to_index(ymuint x,
	      ymuint y) const;

  /// @brief インデックスから xy 座標を得る．
  void
  index_to_xy(ymuint index,
	      ymuint& x,
	      ymuint& y) const;

  /// @brief NlPoint からインデックスを得る．
  ymuint
  point_to_index(const NlPoint& point) const;

  /// @brief インデックスから NlPoint を得る．
  NlPoint
  index_to_point(ymuint index) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 縁の幅
  ymuint mFringeSize;

  // グリッドサイズ
  ymuint mGridSize;

  // 端点の時の内側の枠の幅
  ymuint mInnerMargin;

  // 結線の幅
  ymuint mWireWidth;

  // 問題の幅
  ymuint mWidth;

  // 問題の高さ
  ymuint mHeight;

  // セルの配列
  vector<int> mCellArray;

  // 盤面の幅
  ymuint mBanWidth;

  // 盤面の高さ
  ymuint mBanHeight;

};

} }

#endif // NLVIEW_H
