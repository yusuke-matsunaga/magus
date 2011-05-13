#ifndef YM_DOTLIB_DOTLIBFF_H
#define YM_DOTLIB_DOTLIBFF_H

/// @file ym_dotlib/DotlibFF.h
/// @brief DotlibFF のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/DotlibAttrMap.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibFF DotlibFF.h "ym_dotlib/DotlibFF.h"
/// @brief DotlibNode の木から取り出した FF の情報を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibFF :
  public DotlibAttrMap
{
  friend class DotlibNode;

public:

  /// @brief コンストラクタ
  DotlibFF();

  /// @brief デストラクタ
  ~DotlibFF();


public:

  /// @brief 内容を初期化する．
  void
  clear();

  /// @brief var1 の名前を返す．
  ShString
  var1_name() const;

  /// @brief var2 の名前を返す．
  ShString
  var2_name() const;

  /// @brief "next_state" を返す．
  const DotlibNode*
  next_state() const;

  /// @brief "clocked_on" を返す．
  const DotlibNode*
  clocked_on() const;

  /// @brief "clocked_on_also" を返す．
  const DotlibNode*
  clocked_on_also() const;

  /// @brief "clear" を返す．
  const DotlibNode*
  clear_pin() const;

  /// @brief "preset" を返す．
  const DotlibNode*
  preset() const;

  /// @brief "clear_preset_var1" を返す．
  ShString
  clear_preset_var1() const;

  /// @brief "clear_preset_var2" を返す．
  ShString
  clear_preset_var2() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // var1
  ShString mVar1;

  // var2
  ShString mVar2;

  // next_state
  const DotlibNode* mNextState;

  // clocked_on
  const DotlibNode* mClockedOn;

  // clocked_on_also
  const DotlibNode* mClockedOnAlso;

  // clear
  const DotlibNode* mClear;

  // preset
  const DotlibNode* mPreset;

  // clear_preset_var1
  ShString mClearPresetVar1;

  // clear_preset_var2
  ShString mClearPresetVar2;

};

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_DOTLIBFF_H
