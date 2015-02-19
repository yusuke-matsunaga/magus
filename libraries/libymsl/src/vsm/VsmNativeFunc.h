#ifndef VSMNATIVEFUNC_H
#define VSMNATIVEFUNC_H

/// @file VsmNativeFunc.h
/// @brief VsmNativeFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "VsmFunction.h"
#include "VsmCodeList.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class VsmNativeFunc VsmNativeFunc.h "VsmNativeFunc.h"
/// @brief YMSL で記述された関数を表すクラス
//////////////////////////////////////////////////////////////////////
class VsmNativeFunc :
  public VsmFunction
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] arg_num 引数の数
  /// @param[in] code_list_builder コードリストの初期化用オブジェクト
  VsmNativeFunc(ShString name,
		ymuint arg_num,
		const VsmCodeList::Builder& code_list_builder);

  /// @brief デストラクタ
  ~VsmNativeFunc();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コードリスト
  VsmCodeList mCodeList;

};

END_NAMESPACE_YM_YMSL

#endif // VSMNATIVEFUNC_H
