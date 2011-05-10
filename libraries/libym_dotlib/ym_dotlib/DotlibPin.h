#ifndef YM_DOTLIB_DOTLIBPIN_H
#define YM_DOTLIB_DOTLIBPIN_H

/// @file ym_dotlib/DotlibPin.h
/// @brief DotlibPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/DotlibAttrMap.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibPin DotlibPin.h "ym_dotlib/DotlibPin.h"
/// @brief DotlibNode の木から取り出したピンの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibPin :
  public DotlibAttrMap
{
  friend class DotlibNode;

public:

  /// @brief コンストラクタ
  DotlibPin();

  /// @brief デストラクタ
  ~DotlibPin();


public:

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief "bit_width" を返す．
  const DotlibNode*
  bit_width() const;

  /// @brief "capacitance" を返す．
  const DotlibNode*
  capacitance() const;

  /// @brief "clock" を返す．
  const DotlibNode*
  clock() const;

  /// @brief "direction" を返す．
  const DotlibNode*
  direction() const;

  /// @brief "fall_capacitance" を返す．
  const DotlibNode*
  fall_capacitance() const;

  /// @brief "fanout_load" を返す．
  const DotlibNode*
  fanout_load() const;

  /// @brief "function" を返す．
  const DotlibNode*
  function() const;

  /// @brief "internal_node" を返す．
  const DotlibNode*
  internal_node() const;

  /// @brief "max_capacitance" を返す．
  const DotlibNode*
  max_capacitance() const;

  /// @brief "max_fanout" を返す．
  const DotlibNode*
  max_fanout() const;

  /// @brief "max_transition" を返す．
  const DotlibNode*
  max_transition() const;

  /// @brief "min_capacitance" を返す．
  const DotlibNode*
  min_capacitance() const;

  /// @brief "min_fanout" を返す．
  const DotlibNode*
  min_fanout() const;

  /// @brief "min_transition" を返す．
  const DotlibNode*
  min_transition() const;

  /// @brief "pin_func_type" を返す．
  const DotlibNode*
  pin_func_type() const;

  /// @brief "rise_capacitance" を返す．
  const DotlibNode*
  rise_capacitance() const;

  /// @brief "timing" グループのリストを得る．
  const list<const DotlibNode*>&
  timing_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ピン名
  ShString mName;

  // "bit_width"
  const DotlibNode* mBitWidth;

  // "capacitance"
  const DotlibNode* mCapacitance;

  // "clock"
  const DotlibNode* mClock;

  // "direction"
  const DotlibNode* mDirection;

  // "fall_capacitance"
  const DotlibNode* mFallCapacitance;

  // "fanout_load"
  const DotlibNode* mFanoutLoad;

  // "function"
  const DotlibNode* mFunction;

  // "internal_node"
  const DotlibNode* mInternalNode;

  // "max_capacitance"
  const DotlibNode* mMaxCapacitance;

  // "max_fanout"
  const DotlibNode* mMaxFanout;

  // "max_transition"
  const DotlibNode* mMaxTransition;

  // "min_capacitance"
  const DotlibNode* mMinCapacitance;

  // "min_fanout"
  const DotlibNode* mMinFanout;

  // "min_transition"
  const DotlibNode* mMinTransition;

  // "pin_func_type"
  const DotlibNode* mPinFuncType;

  // "rise_capacitance"
  const DotlibNode* mRiseCapacitance;

  // "timing"
  list<const DotlibNode*> mTimingList;

};

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_DOTLIBPIN_H
