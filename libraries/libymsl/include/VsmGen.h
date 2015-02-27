#ifndef VSMGEN_H
#define VSMGEN_H

/// @file VsmGen.h
/// @brief VsmGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "VsmCodeList.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class VsmGen VsmGen.h "VsmGen.h"
/// @brief VSM 用のバイトコードを生成するクラス
//////////////////////////////////////////////////////////////////////
class VsmGen
{
public:

  /// @brief コンストラクタ
  VsmGen();

  /// @brief デストラクタ
  ~VsmGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コード生成を行う．
  /// @param[in] topleve トップレベルのブロック
  /// @param[in] name 名前
  VsmModule*
  code_gen(const IrToplevel* toplevel,
	   ShString name);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コードブロックに対するコード生成を行う．
  /// @param[in] code_block コードブロック
  /// @param[in] builder CodeList ビルダー
  void
  gen_block(const IrCodeBlock* code_block,
	    VsmCodeList::Builder& builder);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};

END_NAMESPACE_YM_YMSL

#endif // VSMGEN_H
