#ifndef LIBYM_VERILOG_ELABORATOR_INCLUDE_ELBRANGE_H
#define LIBYM_VERILOG_ELABORATOR_INCLUDE_ELBRANGE_H

/// @file libym_verilog/elaborator/include/ElbRange.h
/// @brief ElbRange のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/vl/VlRange.h"
#include "ym_verilog/pt/PtP.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbRangeSrc ElbRange.h "ElbRange.h"
/// @brief ElbRange を作るための情報を持つデータ構造
//////////////////////////////////////////////////////////////////////
class ElbRangeSrc
{
public:

  /// @brief コンストラクタ
  ElbRangeSrc(const PtRange* pt_range,
	      const PtExpr* left,
	      const PtExpr* right,
	      int left_val,
	      int right_val);

  /// @brief デストラクタ
  ~ElbRangeSrc();


public:

  /// @brief パース木の範囲定義を返す．
  const PtRange*
  pt_range() const;

  /// @brief 範囲の MSB の式を返す．
  const PtExpr*
  left_range() const;

  /// @brief 範囲の LSB の式を返す．
  const PtExpr*
  right_range() const;

  /// @brief 範囲の MSB の値を返す．
  int
  left_range_val() const;

  /// @brief 範囲の LSB の値を返す．
  int
  right_range_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パース木の範囲定義
  const PtRange* mPtRange;

  // 範囲の MSB の式
  const PtExpr* mLeftRange;

  // 範囲の LSB の式
  const PtExpr* mRightRange;

  // 範囲の MSB
  int mLeftVal;

  // 範囲の LSB
  int mRightVal;

};


//////////////////////////////////////////////////////////////////////
/// @class ElbRange ElbRange.h "ElbRange.h"
/// @brief 範囲を表すクラス
/// IEEE Std 1364-2001 26.6.10 Object range
//////////////////////////////////////////////////////////////////////
class ElbRange :
  public VlRange
{
protected:

  /// @brief コンストラクタ
  ElbRange() { }

  /// @brief デストラクタ
  virtual
  ~ElbRange() { }


public:
  //////////////////////////////////////////////////////////////////////
  // ElbRange の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を設定する．
  /// @param[in] src 元となる情報
  virtual
  void
  set(const ElbRangeSrc& src) = 0;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
ElbRangeSrc::ElbRangeSrc(const PtRange* pt_range,
			 const PtExpr* left,
			 const PtExpr* right,
			 int left_val,
			 int right_val) :
  mPtRange(pt_range),
  mLeftRange(left),
  mRightRange(right),
  mLeftVal(left_val),
  mRightVal(right_val)
{
}

// @brief デストラクタ
inline
ElbRangeSrc::~ElbRangeSrc()
{
}

// @brief パース木の範囲定義を返す．
inline
const PtRange*
ElbRangeSrc::pt_range() const
{
  return mPtRange;
}

// @brief 範囲の MSB の式を返す．
inline
const PtExpr*
ElbRangeSrc::left_range() const
{
  return mLeftRange;
}

// @brief 範囲の LSB の式を返す．
inline
const PtExpr*
ElbRangeSrc::right_range() const
{
  return mRightRange;
}

// @brief 範囲の MSB の値を返す．
inline
int
ElbRangeSrc::left_range_val() const
{
  return mLeftVal;
}

// @brief 範囲の LSB の値を返す．
inline
int
ElbRangeSrc::right_range_val() const
{
  return mRightVal;
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_INCLUDE_ELBRANGE_H
