#ifndef IRTYPEMGR_H
#define IRTYPEMGR_H

/// @file IrTypeMgr.h
/// @brief IrTypeMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"
#include "YmUtils/HashMap.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrTypeMgr IrTypeMgr.h "IrTypeMgr.h"
/// @brief IrType を管理するクラス
//////////////////////////////////////////////////////////////////////
class IrTypeMgr
{
public:

  /// @brief コンストラクタ
  ///
  /// この時点で組込型だけは登録されている．
  IrTypeMgr();

  /// @brief デストラクタ
  ~IrTypeMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief void 型を得る．
  const IrType*
  void_type();

  /// @brief boolean 型を得る．
  const IrType*
  boolean_type();

  /// @brief int 型を得る．
  const IrType*
  int_type();

  /// @brief float 型を得る．
  const IrType*
  float_type();

  /// @brief string 型を得る．
  const IrType*
  string_type();

  /// @brief array 型を得る．
  /// @param[in] elem_type 要素の型
  ///
  /// 登録されていなければ新たに登録する．
  const IrType*
  array_type(const IrType* elem_type);

  /// @brief set 型を得る．
  /// @param[in] elem_type 要素の型
  ///
  /// 登録されていなければ新たに登録する．
  const IrType*
  set_type(const IrType* elem_type);

  /// @brief map 型を得る．
  /// @param[in] elem_type 要素の型
  /// @param[in] key_type キーの型
  ///
  /// 登録されていなければ新たに登録する．
  const IrType*
  map_type(const IrType* elem_type,
	   const IrType* key_type);

  /// @brief function 型を得る．
  /// @param[in] output_type 出力の型
  /// @param[in] input_type_list 入力の型のリスト
  ///
  /// 登録されていなければ新たに登録する．
  const IrType*
  function_type(const IrType* output_type,
		const vector<IrType*>& input_type_list);

  /// @brief enum 型を作る．
  /// @param[in] name 名前
  /// @param[in] elem_list 要素名と値のリスト
  IrType*
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
  IrType*
  new_PrimType(TypeId type_id);

  /// @brief array 型を作る．
  /// @param[in] elem_type 要素の型
  IrType*
  new_ArrayType(const IrType* elem_type);

  /// @brief set 型を作る．
  /// @param[in] elem_type 要素の型
  IrType*
  new_SetType(const IrType* elem_type);

  /// @brief map 型を作る．
  /// @param[in] key_type キーの
  /// @param[in] elem_type 要素の型
  IrType*
  new_MapType(const IrType* key_type,
	      const IrType* elem_type);

  /// @brief enum 型を作る．
  /// @param[in] name 名前
  /// @param[in] elem_list 要素名と値のリスト
  IrType*
  new_EnumType(ShString name,
	       const vector<pair<ShString, int> >& elem_list);

  /// @brief 型を登録する．
  /// @param[in] type 登録する型
  void
  reg_type(IrType* type);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 全ての型を保持するリスト
  vector<IrType*> mTypeList;

  // void 型
  IrType* mVoidType;

  // boolean 型
  IrType* mBooleanType;

  // int 型
  IrType* mIntType;

  // float 型
  IrType* mFloatType;

  // string 型
  IrType* mStringType;

  // array 型のハッシュ表
  HashMap<const IrType*, const IrType*> mArrayTypeDic;

  // set 型のハッシュ表
  HashMap<const IrType*, const IrType*> mSetTypeDic;

  // map 型のハッシュ表

  // function 型のハッシュ表

  // struct/class 型のハッシュ表
  HashMap<ShString, const IrType*> mClassTypeDic;

};

END_NAMESPACE_YM_YMSL

#endif // IRTYPEMGR_H
