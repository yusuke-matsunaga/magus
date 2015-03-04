#ifndef VSMNATIVEMODULE_H
#define VSMNATIVEMODULE_H

/// @file VsmNativeModule.h
/// @brief VsmNativeModule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "VsmModule.h"
#include "VsmCodeList.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class VsmNativeModule VsmNativeModule.h "VsmNativeModule.h"
/// @brief YMSLネイティブの VsmModule
//////////////////////////////////////////////////////////////////////
class VsmNativeModule :
  public VsmModule
{
public:

  /// @brief コンストラクタ
  /// @param[in] module_builder モジュール用のビルダー
  /// @param[in] toplevel_builder トップレベルのコードビルダー
  VsmNativeModule(VsmModule::Builder& module_builder,
		  VsmCodeList::Builder& toplevel_builder);

  /// @brief デストラクタ
  ~VsmNativeModule();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief トップレベルの実行を行う．
  /// @param[in] vsm 仮想マシン
  virtual
  void
  execute_toplevel(Vsm& vsm) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // トップレベルのコード
  VsmCodeList mToplevelCode;

  //
};

END_NAMESPACE_YM_YMSL

#endif // VSMNATIVEMODULE_H
