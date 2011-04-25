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

//////////////////////////////////////////////////////////////////////
/// @class PtPin PtPin.h "PtPin.h"
/// @brief ピンを表す PtNode の継承クラス
//////////////////////////////////////////////////////////////////////
class PtPin :
  public PtNode
{
  friend class PtMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] attr_name 属性名(ここでは pin)
  /// @param[in] attr_loc 属性名のファイル上の位置
  /// @param[in] value 値(ここではピン名)
  PtPin(ShString attr_name,
	const FileRegion& attr_loc,
	const PtValue* value);

  /// @brief デストラクタ
  virtual
  ~PtPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供を追加する．
  /// @param[in] node 追加する子供のノード
  virtual
  void
  add_child(PtNode* node);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の数を返す．
  /// @note このクラスでは常に 1
  virtual
  ymuint
  value_num() const;

  /// @brief 値を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
  virtual
  const PtValue*
  value(ymuint pos) const;

  /// @brief 子供のノードの要素数を返す．
  virtual
  ymuint
  child_num() const;

  /// @brief 子供のノードを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < child_num() )
  virtual
  const PtNode*
  child(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // PtPin に独自の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief timing グループの数を返す．
  ymuint
  timing_num() const;

  /// @brief timingグループのパース木を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < timing_num() )
  const PtNode*
  timing(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値(名前)
  const PtValue* mValue;

  // タイミング情報のリスト(配列)
  vector<const PtNode*> mTimingList;

  // タイミング情報以外の子供のリスト(配列)
  vector<const PtNode*> mChildList;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTPIN_H
