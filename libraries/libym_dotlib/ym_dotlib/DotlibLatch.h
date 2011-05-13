#ifndef YM_DOTLIB_DOTLIBLATCH_H
#define YM_DOTLIB_DOTLIBLATCH_H

/// @file ym_dotlib/DotlibLatch.h
/// @brief DotlibLatch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/DotlibAttrMap.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibLatch DotlibLatch.h "ym_dotlib/DotlibLatch.h"
/// @brief DotlibNode の木から取り出した Latch の情報を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibLatch :
  public DotlibAttrMap
{
  friend class DotlibNode;

public:

  /// @brief コンストラクタ
  DotlibLatch();

  /// @brief デストラクタ
  ~DotlibLatch();


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

  /// @brief "data_in" を返す．
  const DotlibNode*
  data_in() const;

  /// @brief "enable" を返す．
  const DotlibNode*
  enable() const;

  /// @brief "enable_also" を返す．
  const DotlibNode*
  enable_also() const;

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

  // data_in
  const DotlibNode* mDataIn;

  // enable
  const DotlibNode* mEnable;

  // enable_also
  const DotlibNode* mEnableAlso;

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

#endif // YM_DOTLIB_DOTLIBLATCH_H
