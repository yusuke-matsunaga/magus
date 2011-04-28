#ifndef LIBYM_CELL_DOTLIB_PTPIN_H
#define LIBYM_CELL_DOTLIB_PTPIN_H

/// @file libym_cell/dotlib/PtPin.h
/// @brief PtPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

#if 0
//////////////////////////////////////////////////////////////////////
/// @class PtPin PtPin.h "PtPin.h"
/// @brief ピンを表す PtNode の継承クラス
//////////////////////////////////////////////////////////////////////
class PtPin :
  public PtNode
{
  friend class PtMgr;
  friend class PinHandler;

protected:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  PtPin(const ShString& name);

  /// @brief デストラクタ
  virtual
  ~PtPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief timing グループの数を返す．
  ymuint
  timing_num() const;

  /// @brief timingグループのパース木を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < timing_num() )
  const PtTiming*
  timing(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // タイミング情報のリスト(配列)
  vector<const PtTiming*> mTimingList;

};
#endif

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTPIN_H
