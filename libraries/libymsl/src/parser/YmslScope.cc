
/// @file YmslScope.cc
/// @brief YmslScope の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslScope.h"
#include "AstStatement.h"
//#include "AstFuncDecl.h"
//#include "AstVarDecl.h"
#include "SymHandle.h"
//#include "YmslLabel.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslScope
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent_dict 親の辞書
YmslScope::YmslScope(SymDict* parent_dict) :
  mDict(parent_dict)
{
}

// @brief コンストラクタ
// @param[in] parent 親のブロック
YmslScope::YmslScope(YmslScope* parent) :
  mDict(&parent->mDict)
{
}

// @brief デストラクタ
YmslScope::~YmslScope()
{
}

// @brief statement を追加する．
void
YmslScope::add_statement(AstStatement* statement)
{
  mStatementList.push_back(statement);
}

// @brief ラベルを追加する．
// @param[in] item 追加する要素
void
YmslScope::add_label(YmslLabel* item)
{
  mDict.add_label(item);
}

// @brief 関数を追加する．
// @param[in] item 追加する要素
void
YmslScope::add_function(AstFuncDecl* item)
{
  mDict.add_function(item);
}

// @brief 変数定義を追加する．
// @param[in] item 追加する要素
void
YmslScope::add_vardecl(AstVarDecl* item)
{
  mDict.add_vardecl(item);
}

// @brief 名前からラベルを見つける．
// @param[in] name 名前
//
// ここになければ親のブロックを探す．
// それでもなければ NULL を返す．
YmslLabel*
YmslScope::find_label(ShString name) const
{
  return mDict.find_label(name);
}

// @brief 名前から変数宣言を見つける．
// @param[in] name 名前
//
// ここになければ親のブロックを探す．
// それでもなければ NULL を返す．
AstVarDecl*
YmslScope::find_vardecl(ShString name) const
{
  return mDict.find_vardecl(name);
}

// @brief ステートメント数を返す．
ymuint
YmslScope::statement_num() const
{
  return mStatementList.size();
}

// @brief ステートメントを返す．
// @param[in] pos 位置 ( 0 <= pos < statement_num() )
AstStatement*
YmslScope::statement(ymuint pos) const
{
  ASSERT_COND( pos < statement_num() );
  return mStatementList[pos];
}

// @brief 命令コードのサイズを計算する．
ymuint
YmslScope::calc_size() const
{
  ymuint size = 0;
  for (vector<AstStatement*>::const_iterator p = mStatementList.begin();
       p != mStatementList.end(); ++ p) {
    AstStatement* stmt = *p;
    size += stmt->calc_size();
  }
  return size;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
YmslScope::compile(YmslDriver& driver,
		  YmslCodeList& code_list,
		  Ymsl_INT& addr)
{
  for (vector<AstStatement*>::const_iterator p = mStatementList.begin();
       p != mStatementList.end(); ++ p) {
    AstStatement* stmt = *p;
    stmt->compile(driver, code_list, addr);
  }
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
YmslScope::print(ostream& s,
		ymuint indent) const
{
  ymuint n = mStatementList.size();
  for (ymuint i = 0; i < n; ++ i) {
    mStatementList[i]->print(s, indent);
  }
}

END_NAMESPACE_YM_YMSL
