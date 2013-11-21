#ifndef YM_DOTLIB_DOTLIBFF_H
#define YM_DOTLIB_DOTLIBFF_H

/// @file ym_dotlib/DotlibFF.h
/// @brief DotlibFF のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibFL.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibFF DotlibFF.h "DotlibFF.h"
/// @brief DotlibNode の木から取り出した FF の情報を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibFF :
  public DotlibFL
{
public:

  /// @brief コンストラクタ
  DotlibFF();

  /// @brief デストラクタ
  ~DotlibFF();


public:

  /// @brief 内容を初期化する．
  bool
  set_data(const DotlibNode* ff_node);

  /// @brief "next_state" を返す．
  const DotlibNode*
  next_state() const;

  /// @brief "clocked_on" を返す．
  const DotlibNode*
  clocked_on() const;

  /// @brief "clocked_on_also" を返す．
  const DotlibNode*
  clocked_on_also() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // next_state
  const DotlibNode* mNextState;

  // clocked_on
  const DotlibNode* mClockedOn;

  // clocked_on_also
  const DotlibNode* mClockedOnAlso;

};

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_DOTLIBFF_H
