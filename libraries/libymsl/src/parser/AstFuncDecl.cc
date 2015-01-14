
/// @file AstDeclBase.cc
/// @brief AstDeclBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstFuncDecl.h"
#include "AstList.h"

#include "YmslScope.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstFuncDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 関数名
// @param[in] type 型
// @param[in] param_list パラメータリスト
// @param[in] stmt_list 本体の文
// @param[in] loc ファイル位置
AstFuncDecl::AstFuncDecl(ShString name,
			 AstType* type,
			 AstParamList* param_list,
			 AstStmtList* stmt_list,
			 const FileRegion& loc) :
  AstStatement(loc),
  mName(name),
  mIndex(-1),
  mType(type),
  mParamList(param_list->size()),
  mStmtList(stmt_list->size()),
  mScope(NULL)
{
  ymuint pos = 0;
  for (AstParamList::Iterator p = param_list->begin();
       !p.is_end(); p.next()) {
    mParamList[pos] = *p;
    ++ pos;
  }
  pos = 0;
  for (AstStmtList::Iterator p = stmt_list->begin();
       !p.is_end(); p.next()) {
    mStmtList[pos] = *p;
    ++ pos;
  }
}

// @brief デストラクタ
AstFuncDecl::~AstFuncDecl()
{
}

// @brief 名前を得る．
ShString
AstFuncDecl::name() const
{
  return mName;
}

// @brief インデックス番号を返す．
ymuint
AstFuncDecl::index() const
{
  return mIndex;
}

// @brief 出力の型を返す．
AstType*
AstFuncDecl::type() const
{
  return mType;
}

// @brief パラメータリストを返す．
const vector<AstParam*>&
AstFuncDecl::param_list() const
{
  return mParamList;
}

// @brief 本体のリストを返す．
const vector<AstStatement*>&
AstFuncDecl::stmt_list() const
{
  return mStmtList;
}

// @brief 関数の登録を行う．
// @param[in] parent_scope 親のスコープ
void
AstFuncDecl::phase1(YmslScope* parent_scope)
{
  // parent_scope に関数を登録

  // 自身のスコープを作る．
  mScope = new YmslScope(parent_scope);
}

// @brief スコープの生成と参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstFuncDecl::phase2(YmslScope* parent_scope)
{
  // mParamList を mScope に登録．

  ymuint n = mStmtList.size();
  for (ymuint i = 0; i < n; ++ i) {
    mStmtList[i]->phase2(mScope);
  }
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

END_NAMESPACE_YM_YMSL
