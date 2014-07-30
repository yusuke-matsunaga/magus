#ifndef CNFDDTEST_H
#define CNFDDTEST_H

/// @file cnfddtest.h
/// @brief テスト用の関数定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/cnfdd_nsdef.h"


BEGIN_NAMESPACE_YM_CNFDD

// cnfdd が spec の文字列で指定された関数と等しいか調べる．
// spec の仕様は以下の通り
// "v1 v2 ..."
// vi : 1つの要素を表す数字．1の立っているビットの要素を含む．
bool
check_cnfdd(CNFddMgr& cnfddmgr,
	    const CNFdd& cnfdd,
	    const char* cnfdd_str,
	    const char* spec);

#if 0
// 論理式を表す文字列からBDDを作る．
// 基本的には BddMgr::make_bdd() を呼ぶだけだが
// エラー時に終了するようになっている．
Bdd
str2bdd(BddMgr& bddmgr,
	const char* expr_str,
	int exit_code = 1);

// 文字列から変数集合を作る．
// 文字列は',' で区切られた数字の列
// エラーが起きたら exit_code で終了する．
BddVarSet
str2varset(BddMgr& bddmgr,
	   const char* vs_str,
	   int exit_code = 1);

// 文字列からリテラル集合を作る．
// 文字列は',' で区切られた '~' + 数字 の列
// '~' は否定のリテラルを表す．
// エラーが起きたら exit_code で終了する．
BddLitSet
str2litset(BddMgr& bddmgr,
	   const char* ls_str,
	   int exit_code = 1);

// support のテスト
// 0 〜 10 までの変数でコファクタリングして2つのコファクターが異なる
//ものがサポートだと思う．
bool
check_support(BddMgr& bddmgr,
	      const char* str);
#endif

END_NAMESPACE_YM_CNFDD

#endif // CNFDDTEST_H
