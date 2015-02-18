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
  /// @param[in] arg_num 引数の数
  YmslFunction(ShString name,
	       ymuint arg_num);

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

  /// @brief 引数の数
  ymuint
  arg_num() const;

  /// @brief 組み込み関数の時 true を返す．
  virtual
  bool
  is_builtin() const = 0;

  /// @brief 組み込み関数の時の実行関数
  /// @param[in] vsm 仮想マシン
  /// @param[in] base ベースレジスタ
  virtual
  void
  execute(YmslVSM& vsm,
	  Ymsl_INT base) const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数名
  ShString mName;

  // 引数の数
  ymuint mArgNum;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLFUNCTION_H
