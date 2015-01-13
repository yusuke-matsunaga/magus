#ifndef YMSLFUNCTION_H
#define YMSLFUNCTION_H

/// @file YmslFunction.h
/// @brief YmslFunction のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslFunction YmslFunction.h "YmslFunction.h"
/// @brief YMSL の関数を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslFunction
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] output_type 出力の型
  /// @param[in] input_type_list 入力の型のリスト
  /// @param[in] index インデックス
  YmslFunction(ShString name,
	       const YmslType* output_type,
	       const vector<const YmslType*>& input_type_list,
	       int index);

  /// @brief デストラクタ
  virtual
  ~YmslFunction();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数名を返す．
  ShString
  name() const;

  /// @brief 出力の型を返す．
  const YmslType*
  output_type() const;

  /// @brief 入力数を返す．
  ymuint
  input_num() const;

  /// @brief 入力の型の返す．
  /// @param[in] pos 位置 ( 0 <= pos < input_num() )
  const YmslType*
  input_type(ymuint pos) const;

  /// @brief 関数インデックスを返す．
  int
  index() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数名
  ShString mName;

  // 出力の型
  const YmslType* mOutputType;

  // 入力の数
  ymuint mInputNum;

  // 入力の型の配列
  const YmslType** mInputType;

  // インデックス
  int mIndex;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLFUNCTION_H
