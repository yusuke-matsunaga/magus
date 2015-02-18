#ifndef YMSLNATIVEFUNC_H
#define YMSLNATIVEFUNC_H

/// @file YmslNativeFunc.h
/// @brief YmslNativeFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslFunction.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslNativeFunc YmslNativeFunc.h "YmslNativeFunc.h"
/// @brief YMSL で記述された関数を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslNativeFunc :
  public YmslFunction
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] arg_num 引数の数
  /// @param[in] code_list コードリスト
  YmslNativeFunc(ShString name,
		 ymuint arg_num,
		 const YmslCodeList& code_list);

  /// @brief デストラクタ
  ~YmslNativeFunc();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コードリスト
  const YmslCodeList& mCodeList;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLNATIVEFUNC_H
