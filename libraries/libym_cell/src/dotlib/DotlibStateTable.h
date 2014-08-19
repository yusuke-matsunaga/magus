#ifndef YM_DOTLIB_DOTLIBSTATETABLE_H
#define YM_DOTLIB_DOTLIBSTATETABLE_H

/// @file ym_dotlib/DotlibStateTable.h
/// @brief DotlibStateTable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibFL.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibStateTable DotlibStateTable.h "DotlibStateTable.h"
/// @brief DotlibNode の木から取り出した状態遷移表の情報を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibStateTable :
  public DotlibFL
{
  friend class DotlibNode;

public:

  /// @brief コンストラクタ
  DotlibStateTable();

  /// @brief デストラクタ
  ~DotlibStateTable();


public:

  /// @brief 内容を初期化する．
  void
  init();

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

#endif // YM_DOTLIB_DOTLIBSTATETABLE_H
