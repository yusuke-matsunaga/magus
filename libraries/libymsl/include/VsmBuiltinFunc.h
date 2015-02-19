#ifndef VSMBUILTINFUNC_H
#define VSMBUILTINFUNC_H

/// @file VsmBuiltinFunc.h
/// @brief VsmBuiltinFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "VsmFunction.h"
#include "VsmValue.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class VsmBuiltinFunc VsmBuiltinFunc.h "VsmBuiltinFunc.h"
/// @brief VSM の組み込み関数を表すクラス
//////////////////////////////////////////////////////////////////////
class VsmBuiltinFunc :
  public VsmFunction
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] arg_num 引数の数
  VsmBuiltinFunc(ShString name,
		 ymuint arg_num);

  /// @brief デストラクタ
  ~VsmBuiltinFunc();


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
  execute(Vsm& vsm,
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
  VsmValue
  _execute(const vector<VsmValue>& arg_list) const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_YMSL

#endif // VSMBUILTINFUNC_H
