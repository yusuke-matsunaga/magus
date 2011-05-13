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
  //////////////////////////////////////////////////////////////////////
  // 列挙型の定義
  //////////////////////////////////////////////////////////////////////

  /// @brief ピンの方向を表す型
  enum tDirection {
    /// @brief 入力
    kInput,
    /// @brief 出力
    kOutput,
    /// @brief 入出力
    kInout,
    /// @brief 内部ピン
    kInternal
  };


public:

  /// @brief コンストラクタ
  DotlibPin();

  /// @brief デストラクタ
  ~DotlibPin();


public:

  /// @brief 内容を初期化する．
  void
  init();

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief "direction" を返す．
  tDirection
  direction() const;

  /// @brief "capacitance" を返す．
  double
  capacitance() const;

  /// @brief "rise_capacitance" を返す．
  double
  rise_capacitance() const;

  /// @brief "fall_capacitance" を返す．
  double
  fall_capacitance() const;

  /// @brief "max_fanout" を返す．
  double
  max_fanout() const;

  /// @brief "min_fanout" を返す．
  double
  min_fanout() const;

  /// @brief "max_capacitance" を返す．
  double
  max_capacitance() const;

  /// @brief "min_capacitance" を返す．
  double
  min_capacitance() const;

  /// @brief "max_transition" を返す．
  double
  max_transition() const;

  /// @brief "min_transition" を返す．
  double
  min_transition() const;

  /// @brief "function" を返す．
  const DotlibNode*
  function() const;

  /// @brief "internal_node" を返す．
  const DotlibNode*
  internal_node() const;

  /// @brief "pin_func_type" を返す．
  const DotlibNode*
  pin_func_type() const;

  /// @brief "timing" グループのリストを得る．
  const list<const DotlibNode*>&
  timing_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ピン名
  ShString mName;

  // "direction"
  tDirection mDirection;

  // "capacitance"
  double mCapacitance;

  // "rise_capacitance"
  double mRiseCapacitance;

  // "fall_capacitance"
  double mFallCapacitance;

  // "max_fanout"
  double mMaxFanout;

  // "min_fanout"
  double mMinFanout;

  // "max_capacitance"
  double mMaxCapacitance;

  // "min_capacitance"
  double mMinCapacitance;

  // "max_transition"
  double mMaxTransition;

  // "min_transition"
  double mMinTransition;

  // "function"
  const DotlibNode* mFunction;

  // "internal_node"
  const DotlibNode* mInternalNode;

  // "pin_func_type"
  const DotlibNode* mPinFuncType;

  // "timing"
  list<const DotlibNode*> mTimingList;

};

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_DOTLIBPIN_H
