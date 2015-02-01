#ifndef TYPEHANDLE_H
#define TYPEHANDLE_H

/// @file TypeHandle.h
/// @brief TypeHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SymHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class TypeHandle TypeHandle.h "TypeHandle.h"
/// @brief Type を保持する SymHandle
//////////////////////////////////////////////////////////////////////
class TypeHandle :
  public SymHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] scope スコープ
  TypeHandle(const Type* type,
	     Scope* scope);

  /// @brief デストラクタ
  virtual
  ~TypeHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const;

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

#endif // TYPEHANDLE_H
