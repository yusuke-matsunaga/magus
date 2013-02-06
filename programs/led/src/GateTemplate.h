#ifndef GATETEMPLATE_H
#define GATETEMPLATE_H

/// @file led/GateTemplate.h
/// @brief ゲートを描画するためのテンプレート
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright(C) 2002, 2013 by Yusuke Matsunaga


#include <QPoint>
#include <QRect>


BEGIN_NAMESPACE_YM_GLV

//////////////////////////////////////////////////////////////////////
/// @class GateTemplate GateTemplate.h "GateTemplate.h"
/// @brief ゲートを描画するためのテンプレート
//////////////////////////////////////////////////////////////////////
class GateTemplate
{
  friend class LedScene;

private:

  /// @brief コンストラクタ
  /// @param[in] ni 入力数
  /// @param[in] no 出力数
  GateTemplate(ymuint ni,
	       ymuint no);

  /// @brief デストラクタ
  ~GateTemplate();


public:

  /// @brief 入力数を得る．
  ymuint
  input_num() const;

  /// @brief 出力数を得る．
  ymuint
  output_num() const;

  /// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
  QRect
  bounding_box() const;

  /// @brief すべての入力ピン位置を得る．
  const vector<QPoint>&
  ipin_locations() const;

  /// @brief pos 番目の入力ピン位置を得る．
  /// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
  QPoint
  ipin_location(ymuint pos) const;

  /// @brief すべての出力ピン位置を得る．
  const vector<QPoint>&
  opin_locations() const;

  /// @brief pos 番目の出力ピン位置を得る．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  QPoint
  opin_location(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // このゲートを囲む最小の矩形の対角線上の点
  QRect mBoundingBox;

  // 入力ピン位置の座標のベクタ
  vector<QPoint> mIpinLocations;

  // 出力ピン位置の座標のベクタ
  vector<QPoint> mOpinLocations;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ni 入力数
// @param[in] no 出力数
inline
GateTemplate::GateTemplate(ymuint ni,
			   ymuint no) :
  mIpinLocations(ni),
  mOpinLocations(no)
{
}

// @brief デストラクタ
inline
GateTemplate::~GateTemplate()
{
}

// @brief 入力数を得る．
inline
ymuint
GateTemplate::input_num() const
{
  return mIpinLocations.size();
}

// @brief 出力数を得る．
inline
ymuint
GateTemplate::output_num() const
{
  return mOpinLocations.size();
}

// @brief このゲートを囲む最小の矩形を表す左上と右下の点を得る．
inline
QRect
GateTemplate::bounding_box() const
{
  return mBoundingBox;
}

// @brief すべての入力ピン位置を得る．
inline
const vector<QPoint>&
GateTemplate::ipin_locations() const
{
  return mIpinLocations;
}

// @brief pos 番目の入力ピン位置を得る．
inline
QPoint
GateTemplate::ipin_location(ymuint pos) const
{
  return mIpinLocations[pos];
}

// @brief すべての出力ピン位置を得る．
inline
const vector<QPoint>&
GateTemplate::opin_locations() const
{
  return mOpinLocations;
}

// @brief pos 番目の出力ピン位置を得る．
inline
QPoint
GateTemplate::opin_location(ymuint pos) const
{
  return mOpinLocations[pos];
}

END_NAMESPACE_YM_GLV

#endif // GATETEMPLATE_H
