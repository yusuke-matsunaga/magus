#ifndef LIBYM_CELL_DOTLIB_PTFF_H
#define LIBYM_CELL_DOTLIB_PTFF_H

/// @file libym_cell/dotlib/PtFf.h
/// @brief PtFf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtFf PtFf.h "PtFf.h"
/// @brief ff グループを表すクラス
//////////////////////////////////////////////////////////////////////
class PtFf :
  public PtNode
{
  friend class PtMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name1 変数1の名前
  /// @param[in] name2 変数2の名前
  PtFf(const ShString& name1,
       const ShString& name2);

  /// @brief デストラクタ
  virtual
  ~PtFf();


public:
  //////////////////////////////////////////////////////////////////////
  //　内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性をセットする．
  /// @param[in] attr_name 属性名
  /// @param[in] value 文字列型の値
  bool
  add_simple_attr(const ShString& attr_name,
		  const ShString& value);

public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数1の名前を得る．
  ShString
  name1() const;

  /// @brief 変数2の名前を得る．
  ShString
  name2() const;

  /// @brief clear 属性の値を得る．
  ShString
  clear() const;

  /// @brief clear_preset_var1 属性の値を得る．
  ShString
  clear_preset_var1() const;

  /// @brief clear_preset_var 属性の値を得る．
  ShString
  clear_preset_var2() const;

  /// @brief clocked_on 属性の値を得る．
  ShString
  clocked_on() const;

  /// @brief clocked_on_also 属性の値を得る．
  ShString
  clocked_on_also() const;

  /// @brief next_state 属性の値を得る．
  ShString
  next_state() const;

  /// @brief preset 属性の値を得る．
  ShString
  preset() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数1の名前
  ShString mName1;

  // 変数2の名前
  ShString mName2;

  // clear 属性
  ShString mClear;

  // clear_preset_var1 属性
  ShString mClearPresetVar1;

  // clear_preset_var2 属性
  ShString mClearPresetVar2;

  // clocked_on 属性
  ShString mClockedOn;

  // clocked_on_also 属性
  ShString mClockedOnAlso;

  // next_state 属性
  ShString mNextState;

  // preset 属性
  ShString mPreset;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTFF_H
