#ifndef ASTSETTYPE_H
#define ASTSETTYPE_H

/// @file AstSetType.h
/// @brief AstSetType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstSetType AstSetType.h "AstSetType.h"
/// @brief array 型を表す AstType
//////////////////////////////////////////////////////////////////////
class AstSetType :
  public AstType
{
public:

  /// @brief コンストラクタ
  /// @param[in] elem_type 要素の型
  /// @param[in] loc ファイル位置
  AstSetType(AstType* elem_type,
	     const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstSetType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  TypeId
  type_id() const;

  /// @brief array/set/map 型の時に要素の型を返す．
  virtual
  const AstType*
  elem_type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素の型
  AstType* mElemType;

};

END_NAMESPACE_YM_YMSL


#endif // ASTSETTYPE_H
