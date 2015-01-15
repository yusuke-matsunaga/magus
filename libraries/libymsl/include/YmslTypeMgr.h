#ifndef YMSLTYPEMGR_H
#define YMSLTYPEMGR_H

/// @file YmslTypeMgr.h
/// @brief YmslTypeMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"
#include "YmUtils/HashMap.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslTypeMgr YmslTypeMgr.h "YmslTypeMgr.h"
/// @brief YmslType を管理するクラス
//////////////////////////////////////////////////////////////////////
class YmslTypeMgr
{
public:

  /// @brief コンストラクタ
  ///
  /// この時点で組込型だけは登録されている．
  YmslTypeMgr();

  /// @brief デストラクタ
  ~YmslTypeMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief void 型を得る．
  const YmslType*
  void_type();

  /// @brief boolean 型を得る．
  const YmslType*
  boolean_type();

  /// @brief int 型を得る．
  const YmslType*
  int_type();

  /// @brief float 型を得る．
  const YmslType*
  float_type();

  /// @brief string 型を得る．
  const YmslType*
  string_type();

  /// @brief array 型を得る．
  /// @param[in] elem_type 要素の型
  ///
  /// 登録されていなければ新たに登録する．
  const YmslType*
  array_type(const YmslType* elem_type);

  /// @brief set 型を得る．
  /// @param[in] elem_type 要素の型
  ///
  /// 登録されていなければ新たに登録する．
  const YmslType*
  set_type(const YmslType* elem_type);

  /// @brief map 型を得る．
  /// @param[in] elem_type 要素の型
  /// @param[in] key_type キーの型
  ///
  /// 登録されていなければ新たに登録する．
  const YmslType*
  map_type(const YmslType* elem_type,
	   const YmslType* key_type);

  /// @brief function 型を得る．
  /// @param[in] output_type 出力の型
  /// @param[in] input_type_list 入力の型のリスト
  ///
  /// 登録されていなければ新たに登録する．
  const YmslType*
  function_type(const YmslType* output_type,
		const vector<YmslType*>& input_type_list);

  /// @brief 型を登録する．
  /// @param[in] type 登録する型
  void
  reg_type(YmslType* type);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み型を登録する．
  void
  init();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 全ての型を保持するリスト
  vector<YmslType*> mTypeList;

  // void 型
  YmslType* mVoidType;

  // boolean 型
  YmslType* mBooleanType;

  // int 型
  YmslType* mIntType;

  // float 型
  YmslType* mFloatType;

  // string 型
  YmslType* mStringType;

  // array 型のハッシュ表
  HashMap<const YmslType*, const YmslType*> mArrayTypeDic;

  // set 型のハッシュ表
  HashMap<const YmslType*, const YmslType*> mSetTypeDic;

  // map 型のハッシュ表

  // function 型のハッシュ表

  // struct/class 型のハッシュ表
  HashMap<ShString, const YmslType*> mClassTypeDic;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLTYPEMGR_H
