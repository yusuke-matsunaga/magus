#ifndef IRTYPEHANDLE_H
#define IRTYPEHANDLE_H

/// @file IrTypeHandle.h
/// @brief IrTypeHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrTypeHandle IrTypeHandle.h "IrTypeHandle.h"
/// @brief Type を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class IrTypeHandle :
  public IrHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] scope スコープ
  IrTypeHandle(const Type* type,
	       Scope* scope);

  /// @brief デストラクタ
  virtual
  ~IrTypeHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const;

  /// @brief 種類を返す．
  virtual
  HandleType
  handle_type() const;

  /// @brief スコープを返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  Scope*
  scope() const;

  /// @brief 名前付き型を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  const Type*
  named_type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型
  const Type* mType;

  // スコープ
  Scope* mScope;

};

END_NAMESPACE_YM_YMSL

#endif // IRTYPEHANDLE_H
