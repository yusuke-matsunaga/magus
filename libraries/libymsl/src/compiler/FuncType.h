#ifndef FUNCTYPE_H
#define FUNCTYPE_H

/// @file FuncType.h
/// @brief FuncType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "Type.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class FuncType FuncType.h "FuncType.h"
/// @brief 関数タイプ
//////////////////////////////////////////////////////////////////////
class FuncType :
  public Type
{
public:

  /// @brief コンストラクタ
  /// @param[in] output_type 出力の型
  /// @param[in] input_type_list 入力の型のリスト
  FuncType(const Type* output_type,
	   const vector<const Type*>& input_type_list);

  /// @brief デストラクタ
  virtual
  ~FuncType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  TypeId
  type_id() const;

  /// @brief 関数の出力の型を返す．
  ///
  /// function のみ有効
  virtual
  const Type*
  function_output_type() const;

  /// @brief 関数の入力数を返す．
  ///
  /// function のみ有効
  virtual
  ymuint
  function_input_num() const;

  /// @brief 関数の入力の型を返す．
  /// @param[in] pos 入力番号 ( 0 <= pos < function_input_num() )
  ///
  /// function のみ有効
  virtual
  const Type*
  function_input_type(ymuint pos) const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力の型
  const Type* mOutputType;

  // 入力の型
  vector<const Type*> mInputType;

};

END_NAMESPACE_YM_YMSL

#endif // FUNCTYPE_H
