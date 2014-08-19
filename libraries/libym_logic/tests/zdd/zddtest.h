#ifndef ZDDTEST_H
#define ZDDTEST_H

/// @file zddtest.h
/// @brief テスト用の関数定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/zdd_nsdef.h"


BEGIN_NAMESPACE_YM_ZDD

// zdd が spec の文字列で指定された関数と等しいか調べる．
// spec の仕様は以下の通り
// "v1 v2 ..."
// vi : 1つの要素を表す数字．1の立っているビットの要素を含む．
bool
check_zdd(ZddMgr& zddmgr,
	  const Zdd& zdd,
	  const char* zdd_str,
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

END_NAMESPACE_YM_ZDD

#endif // ZDDTEST_H
