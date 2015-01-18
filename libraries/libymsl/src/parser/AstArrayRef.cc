
/// @file AstArrayRef.cc
/// @brief AstArrayRef の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstArrayRef.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstArrayRef
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] array 本体の式
// @param[in] index インデックス
// @param[in] loc ファイル位置
AstArrayRef::AstArrayRef(AstExpr* body,
			 AstExpr* index,
			 const FileRegion& loc) :
  AstExpr(loc),
  mArray(body),
  mIndex(index)
{
}

// @brief デストラクタ
AstArrayRef::~AstArrayRef()
{
}

// @brief 種類を返す．
ExprType
AstArrayRef::expr_type() const
{
  return kArrayRef;
}

// @brief 配列本体を返す．
//
// kArrayRef のみ有効
const AstExpr*
AstArrayRef::array_body() const
{
  return mArray;
}

// @brief インデックスを返す．
//
// kArrayRef のみ有効
const AstExpr*
AstArrayRef::array_index() const
{
  return mIndex;
}

#if 0
// @brief 変数の参照を解決する．
void
AstArrayRef::resolve_var(YmslScope* parent_scope)
{
  mArray->resolve_var(parent_scope);
  mIndex->resolve_var(parent_scope);
}

// @brief 式の型を解析する．
// @return 引数の方が間違っていたら false を返す．
//
// 結果としてキャスト演算が挿入される場合もある．
bool
AstArrayRef::type_analysis()
{
}

// @brief 式の型を返す．
const YmslType*
AstArrayRef::type()
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstArrayRef::calc_size()
{
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstArrayRef::compile(YmslDriver& driver,
		     YmslCodeList& code_list,
		     Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstArrayRef::print(ostream& s) const
{
  mArray->print(s);
  s << "[";
  mIndex->print(s);
  s << "]";
}
#endif

END_NAMESPACE_YM_YMSL
