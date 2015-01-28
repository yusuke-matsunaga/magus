#ifndef FUNCHANDLE_H
#define FUNCHANDLE_H

/// @file FuncHandle.h
/// @brief FuncHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SymHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class FuncHandle FuncHandle.h "FuncHandle.h"
/// @brief Function を保持する SymHandle
//////////////////////////////////////////////////////////////////////
class FuncHandle :
  public SymHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] func 関数
  FuncHandle(Function* func);

  /// @brief デストラクタ
  virtual
  ~FuncHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const;

  /// @brief 関数を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  Function*
  function() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数
  Function* mFunc;

};

END_NAMESPACE_YM_YMSL

#endif // FUNCHANDLE_H
