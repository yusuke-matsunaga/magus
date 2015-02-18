#ifndef YMSLBUILTINFUNC_H
#define YMSLBUILTINFUNC_H

/// @file YmslBuiltinFunc.h
/// @brief YmslBuiltinFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslFunction.h"
#include "YmslValue.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslBuiltinFunc YmslBuiltinFunc.h "YmslBuiltinFunc.h"
/// @brief YMSL の組み込み関数を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslBuiltinFunc :
  public YmslFunction
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] arg_num 引数の数
  YmslBuiltinFunc(ShString name,
		  ymuint arg_num);

  /// @brief デストラクタ
  ~YmslBuiltinFunc();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み関数の時 true を返す．
  virtual
  bool
  is_builtin() const;

  /// @brief 組み込み関数の時の実行関数
  /// @param[in] vsm 仮想マシン
  /// @param[in] base ベースレジスタ
  virtual
  void
  execute(YmslVSM& vsm,
	  Ymsl_INT base) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 本当の実行関数
  /// @param[in] arg_list 引数のリスト
  ///
  /// 実際の派生クラスが実装する必要がある．
  virtual
  YmslValue
  _execute(const vector<YmslValue>& arg_list) const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_YMSL

#endif // YMSLBUILTINFUNC_H
