#ifndef YM_DOTLIB_DOTLIBFL_H
#define YM_DOTLIB_DOTLIBFL_H

/// @file ym_dotlib/DotlibFL.h
/// @brief DotlibFL のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibAttrMap.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibFL DotlibFL.h "DotlibFL.h"
/// @brief DotlibFF と DotlibLatch の基底クラス
//////////////////////////////////////////////////////////////////////
class DotlibFL :
  public DotlibAttrMap
{
  friend class DotlibNode;

public:

  /// @brief コンストラクタ
  DotlibFL();

  /// @brief デストラクタ
  ~DotlibFL();


public:

  /// @brief 内容を初期化する．
  void
  init();

  /// @brief var1 の名前を返す．
  ShString
  var1_name() const;

  /// @brief var2 の名前を返す．
  ShString
  var2_name() const;

  /// @brief "clear" を返す．
  const DotlibNode*
  clear() const;

  /// @brief "preset" を返す．
  const DotlibNode*
  preset() const;

  /// @brief "clear_preset_var1" を返す．
  ymuint
  clear_preset_var1() const;

  /// @brief "clear_preset_var2" を返す．
  ymuint
  clear_preset_var2() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // var1
  ShString mVar1;

  // var2
  ShString mVar2;

  // clear
  const DotlibNode* mClear;

  // preset
  const DotlibNode* mPreset;

  // clear_preset_var1
  ymuint mClearPresetVar1;

  // clear_preset_var2
  ymuint mClearPresetVar2;

};

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_DOTLIBFF_H
