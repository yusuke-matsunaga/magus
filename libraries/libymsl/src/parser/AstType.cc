
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

// @brief プリミティブ型の時 true を返す．
bool
AstType::primitive_type() const
{
  return false;
}

// @brief 名前付き型の時 true を返す．
bool
AstType::named_type() const
{
  return false;
}

// @brief array 型の時 true を返す．
bool
AstType::array_type() const
{
  return false;
}

// @brief set 型の時 true を返す．
bool
AstType::set_type() const
{
  return false;
}

// @brief map 型の時 true を返す．
bool
AstType::map_type() const
{
  return false;
}

// @brief プリミティブ型の時に型を返す．
TypeId
AstType::type_id() const
{
  ASSERT_NOT_REACHED;
  return kVoidType;
}

// @brief 名前付き方の時に名前を返す．
AstSymbol*
AstType::name() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief map 型の時にキーの型を返す．
AstType*
AstType::key_type() const
{
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief array/set/map 型の時に要素の型を返す．
AstType*
AstType::elem_type() const
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

// @brief プリミティブ型の時 true を返す．
bool
AstPrimType::primitive_type() const
{
  return true;
}

// @brief 型を得る．
TypeId
AstPrimType::type_id() const
{
  return mType;
}

// @brief 内容を出力する．
// @param[in] s 出力ストリーム
void
AstPrimType::print(ostream& s) const
{
  s << mType;
}


//////////////////////////////////////////////////////////////////////
// クラス AstNamedType
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 型名
AstNamedType::AstNamedType(AstSymbol* name) :
  AstType(name->file_region()),
  mName(name)
{
}

// @brief デストラクタ
AstNamedType::~AstNamedType()
{
}

// @brief 名前付き型の時 true を返す．
bool
AstNamedType::named_type() const
{
  return true;
}

// @brief 名前付き方の時に名前を返す．
AstSymbol*
AstNamedType::name() const
{
  return mName;
}

// @brief 内容を出力する．
// @param[in] s 出力ストリーム
void
AstNamedType::print(ostream& s) const
{
  s << mName->str_val();
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

// @brief array 型の時 true を返す．
bool
AstArrayType::array_type() const
{
  return true;
}

// @brief array/set/map 型の時に要素の型を返す．
AstType*
AstArrayType::elem_type() const
{
  return mElemType;
}

// @brief 内容を出力する．
// @param[in] s 出力ストリーム
void
AstArrayType::print(ostream& s) const
{
  s << "array(";
  elem_type()->print(s);
  s << ")";
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

// @brief set 型の時 true を返す．
bool
AstSetType::set_type() const
{
  return true;
}

// @brief array/set/map 型の時に要素の型を返す．
AstType*
AstSetType::elem_type() const
{
  return mElemType;
}

// @brief 内容を出力する．
// @param[in] s 出力ストリーム
void
AstSetType::print(ostream& s) const
{
  s << "set(";
  elem_type()->print(s);
  s << ")";
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

// @brief map 型の時 true を返す．
bool
AstMapType::map_type() const
{
  return true;
}

// @brief map 型の時にキーの型を返す．
AstType*
AstMapType::key_type() const
{
  return mKeyType;
}

// @brief array/set/map 型の時に要素の型を返す．
AstType*
AstMapType::elem_type() const
{
  return mElemType;
}

// @brief 内容を出力する．
// @param[in] s 出力ストリーム
void
AstMapType::print(ostream& s) const
{
  s << "map(";
  key_type()->print(s);
  s << ", ";
  elem_type()->print(s);
  s << ")";
}

END_NAMESPACE_YM_YMSL
