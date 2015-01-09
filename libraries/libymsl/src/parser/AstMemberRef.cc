
/// @file AstMemberRef.cc
/// @brief AstMemberRef の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstMemberRef.h"
#include "AstSymbol.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstMemberRef
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id オブジェクト名
// @param[in] member メンバ名
// @param[in] loc ファイル位置
AstMemberRef::AstMemberRef(AstExpr* id,
			   AstSymbol* member,
			   const FileRegion& loc) :
  AstExpr(loc),
  mId(id),
  mMember(member)
{
}

// @brief デストラクタ
AstMemberRef::~AstMemberRef()
{
}

// @brief 変数の参照を解決する．
void
AstMemberRef::resolve_var(YmslScope* parent_scope)
{
  mId->resolve_var(parent_scope);
}

// @brief 式の型を解析する．
// @return 引数の方が間違っていたら false を返す．
//
// 結果としてキャスト演算が挿入される場合もある．
bool
AstMemberRef::type_analysis()
{
}

// @brief 式の型を返す．
const YmslType*
AstMemberRef::type()
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstMemberRef::calc_size()
{
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstMemberRef::compile(YmslDriver& driver,
		      YmslCodeList& code_list,
		      Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstMemberRef::print(ostream& s) const
{
  mId->print(s);
  s << "." << mMember->str_val();
}

END_NAMESPACE_YM_YMSL
