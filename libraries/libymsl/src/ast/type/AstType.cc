
/// @file AstType.cc
/// @brief AstType の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstType.h"
#include "AstPrimType.h"
#include "AstNamedType.h"
#include "AstArrayType.h"
#include "AstSetType.h"
#include "AstMapType.h"
#include "AstSymbol.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AstType::AstType(const FileRegion& loc) :
  Ast(loc)
{
}

// @brief デストラクタ
AstType::~AstType()
{
}

// @brief map 型の時にキーの型を返す．
const AstType*
AstType::key_type() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief array/set/map 型の時に要素の型を返す．
const AstType*
AstType::elem_type() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 名前付き方の時に名前を返す．
const AstExpr*
AstType::name() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス AstPrimType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
// @param[in] loc ファイル位置
AstPrimType::AstPrimType(TypeId type,
			 const FileRegion& loc) :
  AstType(loc),
  mType(type)
{
}

// @brief デストラクタ
AstPrimType::~AstPrimType()
{
}

// @brief 型番号を返す．
TypeId
AstPrimType::type_id() const
{
  return mType;
}


//////////////////////////////////////////////////////////////////////
// クラス AstNamedType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 型名
// @param[in] loc ファイル位置
AstNamedType::AstNamedType(AstExpr* name,
			   const FileRegion& loc) :
  AstType(loc),
  mName(name)
{
}

// @brief デストラクタ
AstNamedType::~AstNamedType()
{
}

// @brief 型番号を返す．
TypeId
AstNamedType::type_id() const
{
  return kNamedType;
}

// @brief 名前付き方の時に名前を返す．
const AstExpr*
AstNamedType::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// AstArrayType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elem_type 要素の型
// @param[in] loc ファイル位置
AstArrayType::AstArrayType(AstType* elem_type,
			   const FileRegion& loc) :
  AstType(loc),
  mElemType(elem_type)
{
}

// @brief デストラクタ
AstArrayType::~AstArrayType()
{
}

// @brief 型番号を返す．
TypeId
AstArrayType::type_id() const
{
  return kArrayType;
}

// @brief array/set/map 型の時に要素の型を返す．
const AstType*
AstArrayType::elem_type() const
{
  return mElemType;
}


//////////////////////////////////////////////////////////////////////
// AstSetType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elem_type 要素の型
// @param[in] loc ファイル位置
AstSetType::AstSetType(AstType* elem_type,
		       const FileRegion& loc) :
  AstType(loc),
  mElemType(elem_type)
{
}

// @brief デストラクタ
AstSetType::~AstSetType()
{
}

// @brief 型番号を返す．
TypeId
AstSetType::type_id() const
{
  return kSetType;
}

// @brief array/set/map 型の時に要素の型を返す．
const AstType*
AstSetType::elem_type() const
{
  return mElemType;
}


//////////////////////////////////////////////////////////////////////
// AstMapType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] key_type キーの方
// @param[in] elem_type 要素の型
// @param[in] loc ファイル位置
AstMapType::AstMapType(AstType* key_type,
		       AstType* elem_type,
		       const FileRegion& loc) :
  AstType(loc),
  mKeyType(key_type),
  mElemType(elem_type)
{
}

// @brief デストラクタ
AstMapType::~AstMapType()
{
}

// @brief 型番号を返す．
TypeId
AstMapType::type_id() const
{
  return kMapType;
}

// @brief map 型の時にキーの型を返す．
const AstType*
AstMapType::key_type() const
{
  return mKeyType;
}

// @brief array/set/map 型の時に要素の型を返す．
const AstType*
AstMapType::elem_type() const
{
  return mElemType;
}

END_NAMESPACE_YM_YMSL
