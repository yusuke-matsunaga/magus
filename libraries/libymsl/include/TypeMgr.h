#ifndef TYPEMGR_H
#define TYPEMGR_H

/// @file TypeMgr.h
/// @brief TypeMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"
#include "YmUtils/HashMap.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class TypeMgr TypeMgr.h "TypeMgr.h"
/// @brief Type を管理するクラス
//////////////////////////////////////////////////////////////////////
class TypeMgr
{
public:

  /// @brief コンストラクタ
  ///
  /// この時点で組込型だけは登録されている．
  TypeMgr();

  /// @brief デストラクタ
  ~TypeMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief void 型を得る．
  const Type*
  void_type();

  /// @brief boolean 型を得る．
  const Type*
  boolean_type();

  /// @brief int 型を得る．
  const Type*
  int_type();

  /// @brief float 型を得る．
  const Type*
  float_type();

  /// @brief string 型を得る．
  const Type*
  string_type();

  /// @brief array 型を得る．
  /// @param[in] elem_type 要素の型
  ///
  /// 登録されていなければ新たに登録する．
  const Type*
  array_type(const Type* elem_type);

  /// @brief set 型を得る．
  /// @param[in] elem_type 要素の型
  ///
  /// 登録されていなければ新たに登録する．
  const Type*
  set_type(const Type* elem_type);

  /// @brief map 型を得る．
  /// @param[in] elem_type 要素の型
  /// @param[in] key_type キーの型
  ///
  /// 登録されていなければ新たに登録する．
  const Type*
  map_type(const Type* elem_type,
	   const Type* key_type);

  /// @brief function 型を得る．
  /// @param[in] output_type 出力の型
  /// @param[in] input_type_list 入力の型のリスト
  ///
  /// 登録されていなければ新たに登録する．
  const Type*
  function_type(const Type* output_type,
		const vector<Type*>& input_type_list);

  /// @brief enum 型を作る．
  /// @param[in] name 名前
  /// @param[in] elem_list 要素名と値のリスト
  Type*
  enum_type(ShString name,
	    const vector<pair<ShString, int> >& elem_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み型を登録する．
  void
  init();

  /// @brief プリミティブ型を作る．
  /// @param[in] type_id 型番号
  Type*
  new_PrimType(TypeId type_id);

  /// @brief array 型を作る．
  /// @param[in] elem_type 要素の型
  Type*
  new_ArrayType(const Type* elem_type);

  /// @brief set 型を作る．
  /// @param[in] elem_type 要素の型
  Type*
  new_SetType(const Type* elem_type);

  /// @brief map 型を作る．
  /// @param[in] key_type キーの
  /// @param[in] elem_type 要素の型
  Type*
  new_MapType(const Type* key_type,
	      const Type* elem_type);

  /// @brief enum 型を作る．
  /// @param[in] name 名前
  /// @param[in] elem_list 要素名と値のリスト
  Type*
  new_EnumType(ShString name,
	       const vector<pair<ShString, int> >& elem_list);

  /// @brief 型を登録する．
  /// @param[in] type 登録する型
  void
  reg_type(Type* type);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 全ての型を保持するリスト
  vector<Type*> mTypeList;

  // void 型
  Type* mVoidType;

  // boolean 型
  Type* mBooleanType;

  // int 型
  Type* mIntType;

  // float 型
  Type* mFloatType;

  // string 型
  Type* mStringType;

  // array 型のハッシュ表
  HashMap<const Type*, const Type*> mArrayTypeDic;

  // set 型のハッシュ表
  HashMap<const Type*, const Type*> mSetTypeDic;

  // map 型のハッシュ表

  // function 型のハッシュ表

  // struct/class 型のハッシュ表
  HashMap<ShString, const Type*> mClassTypeDic;

};

END_NAMESPACE_YM_YMSL

#endif // IRTYPEMGR_H
