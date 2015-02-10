#ifndef TYPEMGR_H
#define TYPEMGR_H

/// @file TypeMgr.h
/// @brief TypeMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "OpCode.h"
#include "YmUtils/ShString.h"
#include "YmUtils/HashMap.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class TypeMgr TypeMgr.h "TypeMgr.h"
/// @brief Type を管理するクラス
///
/// ここでは組み込み型と派生型の正規化を行う．
/// 名前付きの型(enum と class)は名前空間で管理される．
/// TypeMgr による正規化は行われない．
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

  /// @brief 内容をクリアする．
  void
  clear();

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
		const vector<const Type*>& input_type_list);

  /// @brief enum 型を作る．
  /// @param[in] name 名前
  /// @param[in] elem_list 要素名と値のリスト
  const Type*
  enum_type(ShString name,
	    const vector<pair<ShString, int> >& elem_list);

  /// @brief 演算と入力の型から出力の型を求める．(単項演算用)
  /// @param[in] opcode オペコード
  /// @param[in] op1_type オペランドの型
  /// @param[in] opt1_reqtype オペランドに要求される型
  ///
  /// マッチする型がない場合には NULL を返す．
  const Type*
  calc_type1(OpCode opcode,
	     const Type* op1_type,
	     const Type*& op1_reqtype);

  /// @brief 演算と入力の型から出力の型を求める．(二項演算用)
  /// @param[in] opcode オペコード
  /// @param[in] op1_type, op2_type オペランドの型
  /// @param[in] op1_reqtype, op2_reqtype オペランドに要求される型
  ///
  /// マッチする型がない場合には NULL を返す．
  const Type*
  calc_type2(OpCode opcode,
	     const Type* op1_type,
	     const Type* op2_type,
	     const Type*& op1_reqtype,
	     const Type*& op2_reqtype);

  /// @brief 演算と入力の型から出力の型を求める．(三項演算用)
  /// @param[in] opcode オペコード
  /// @param[in] op1_type, op2_type, op3_type オペランドの型
  /// @param[in] op1_reqtype, op2_reqtype, op3_reqtype オペランドに要求される型
  ///
  /// マッチする型がない場合には NULL を返す．
  const Type*
  calc_type3(OpCode opcode,
	     const Type* op1_type,
	     const Type* op2_type,
	     const Type* op3_type,
	     const Type*& op1_reqtype,
	     const Type*& op2_reqtype,
	     const Type*& op3_reqtype);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み型を登録する．
  void
  init();

  /// @brief プリミティブ型を作る．
  /// @param[in] type_id 型番号
  const Type*
  new_PrimType(TypeId type_id);

  /// @brief 型を登録する．
  /// @param[in] type 登録する型
  void
  reg_type(Type* type);

  /// @brief ハッシュ表を確保する．
  /// @param[in] req_size 表のサイズ
  void
  alloc_table(ymuint req_size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 全ての型を保持するリスト
  vector<Type*> mTypeList;

  // void 型
  const Type* mVoidType;

  // boolean 型
  const Type* mBooleanType;

  // int 型
  const Type* mIntType;

  // float 型
  const Type* mFloatType;

  // string 型
  const Type* mStringType;

  // ハッシュ表のサイズ
  ymuint mHashSize;

  // ハッシュ表を拡張する目安
  ymuint mNextLimit;

  // ハッシュ表
  Type** mHashTable;

  // ハッシュ表に登録されている要素数
  ymuint mHashNum;

};

END_NAMESPACE_YM_YMSL

#endif // IRTYPEMGR_H
