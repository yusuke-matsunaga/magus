
/// @file AstDeclBase.cc
/// @brief AstDeclBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstFuncDecl.h"
#include "AstList.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstFuncDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] type 型
// @param[in] param_list パラメータリスト
// @param[in] stmt 本体の文
// @param[in] loc ファイル位置
AstFuncDecl::AstFuncDecl(ShString name,
			 AstType* type,
			 AstParamList* param_list,
			 AstStatement* stmt,
			 const FileRegion& loc) :
  AstStatement(loc),
  mName(name),
  mType(type),
  mParamList(param_list->size()),
  mStmt(stmt)
{
  ymuint pos = 0;
  for (AstParamList::Iterator p = param_list->begin();
       !p.is_end(); p.next()) {
    mParamList[pos] = *p;
    ++ pos;
  }
}

// @brief デストラクタ
AstFuncDecl::~AstFuncDecl()
{
}

// @brief 種類を返す．
StmtType
AstFuncDecl::stmt_type() const
{
  return kFuncDecl;
}

// @brief 名前を得る．
ShString
AstFuncDecl::name() const
{
  return mName;
}

// @brief 出力の型を返す．
const AstType*
AstFuncDecl::type() const
{
  return mType;
}

// @brief パラメータリストの要素数を返す．
//
// kFuncDecl のみ有効
ymuint
AstFuncDecl::param_num() const
{
  return mParamList.size();
}

// @brief パラメータリストの要素を返す．
// @param[in] pos 位置 ( 0 <= pos < param_num() )
//
// kFuncDecl のみ有効
const AstParam*
AstFuncDecl::param(ymuint pos) const
{
  ASSERT_COND( pos < param_num() );
  return mParamList[pos];
}

// @brief 本体の文を返す．
//
// kFuncDecl, kFor, kDoWhile, kWhile, kIf のみ有効
const AstStatement*
AstFuncDecl::stmt() const
{
  return mStmt;
}

#if 0
// @brief 要素の生成と関数以外の参照解決を行う．
// @param[in] parent_scope 親のスコープ
// @param[in] type_mgr 型マネージャ
void
AstFuncDecl::phase1(YmslScope* parent_scope,
		    YmslTypeMgr* type_mgr)
{
#if 0
  // parent_scope に関数を登録

  // 自身のスコープを作る．
  mScope = new YmslScope(parent_scope);

  // mParamList を mScope に登録．

  ymuint n = mStmtList.size();
  for (ymuint i = 0; i < n; ++ i) {
    mStmtList[i]->phase1(mScope, type_mgr);
  }
#endif
}

// @brief 参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstFuncDecl::phase2(YmslScope* parent_scope)
{
#if 0
  ymuint n = mStmtList.size();
  for (ymuint i = 0; i < n; ++ i) {
    mStmtList[i]->phase2(mScope);
  }
#endif
}

// @brief 命令コードのサイズを計算する．
ymuint
AstFuncDecl::calc_size()
{
  return 0;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstFuncDecl::compile(YmslDriver& driver,
		     YmslCodeList& code_list,
		     Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstFuncDecl::print(ostream& s,
		   ymuint indent) const
{
}
#endif

END_NAMESPACE_YM_YMSL
