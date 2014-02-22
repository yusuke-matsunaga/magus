#ifndef ELBGFROOT_H
#define ELBGFROOT_H

/// @file ElbGfRoot.h
/// @brief ElbGfRoot のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "verilog/vl/VlNamedObj.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class ElbGfRoot ElbGfRoot.h "ElbGfRoot.h"
/// @brief GfBlock の親となるクラス
/// @note スコープとしての親ではなく，名前による検索のためのオブジェクト
//////////////////////////////////////////////////////////////////////
class ElbGfRoot :
  public VlNamedObj
{
protected:

  /// @brief コンストラクタ
  ElbGfRoot() { }

  /// @brief デストラクタ
  virtual
  ~ElbGfRoot() { }


public:
  //////////////////////////////////////////////////////////////////////
  // ElbGfRoot の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 子供のスコープを追加する．
  virtual
  void
  add(int index,
      const VlNamedObj* block) = 0;

  /// @brief 子供のスコープを取り出す．
  virtual
  const VlNamedObj*
  elem_by_index(int index) = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // ELBGFROOT_H
