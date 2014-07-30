
/// @file test_utils.cc
/// @brief CNFDD パッケージのテスト用の便利関数
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.



#include "cnfddtest.h"

#include "YmLogic/CNFdd.h"
#include "YmLogic/CNFddMgr.h"


BEGIN_NAMESPACE_YM_CNFDD

// cnfdd が spec の文字列で指定された関数と等しいか調べる．
// spec の仕様は以下の通り
// "v1 v2 ..."
// vi : 1つの要素を表す数字．1の立っているビットの要素を含む．
bool
check_cnfdd(CNFddMgr& cnfddmgr,
	  const CNFdd& cnfdd,
	  const char* cnfdd_str,
	  const char* spec)
{
  vector<ymuint32> v_list;
  string buf;
  for (const char* s = spec; ; ++ s) {
    char c = *s;
    if ( c == ' ' || c == '\0' ) {
      ymuint d = atoi(buf.c_str());
      v_list.push_back(d);
      if ( c == '\0' ) {
	break;
      }
      buf = "";
    }
    else {
      buf += c;
    }
  }

  cout << cnfdd_str
       << ": spec = ";
  for (vector<ymuint32>::iterator p = v_list.begin();
       p != v_list.end(); ++ p) {
    cout << " " << *p;
  }
  cout << endl;

  cnfdd.print_clause(cout);

  return true;
}

#if 0
// 論理式を表す文字列からBDDを作る．
// 基本的には BddMgr::expr_to_bdd() を呼ぶだけだが
// エラー時には exit_code で終了するようになっている．
Bdd
str2bdd(BddMgr& bddmgr,
	const char* expr_str,
	int exit_code)
{
  string err_msg;
  Bdd ans = bddmgr.expr_to_bdd(expr_str, err_msg);
  if ( ans.is_error() ) {
    cerr << "ERROR[str2bdd]: " << err_msg << endl;
    exit(exit_code);
  }
  return ans;
}

// 文字列から変数集合を作る．
// 文字列は',' で区切られた数字の列
// エラーが起きたら exit_code で終了する．
BddVarSet
str2varset(BddMgr& bddmgr,
	   const char* vs_str,
	   int exit_code)
{
  BddVarSet vs(bddmgr);
  string buf;
  for (const char* s = vs_str; ; ++ s) {
    char c = *s;
    if ( c == ',' || c == '\0' ) {
      int d = atoi(buf.c_str());
      VarId var(d);
      vs += BddVarSet(bddmgr, var);
      if ( c == '\0' ) {
	return vs;
      }
      buf = "";
    }
    else if ( isspace(c) ) {
      continue;
    }
    else if ( isdigit(c) ) {
      buf += c;
    }
    else {
      cout << "ERROR[str2varset]: illegal character in " << vs_str << endl;
      exit(exit_code);
    }
  }
}

// 文字列からリテラル集合を作る．
// 文字列は',' で区切られた '~' + 数字 の列
// '~' は否定のリテラルを表す．
// エラーが起きたら exit_code で終了する．
BddLitSet
str2litset(BddMgr& bddmgr,
	   const char* ls_str,
	   int exit_code)
{
  BddLitSet ls(bddmgr);
  string buf;
  tPol pol = kPolPosi;
  for (const char* s = ls_str; ; ++ s) {
    char c = *s;
    if ( c == '~' ) {
      if ( pol == kPolNega ) {
	cout << "ERROR[str2varset]: syntax error in " << ls_str << endl;
	exit(exit_code);
      }
      pol = kPolNega;
    }
    else if ( c == ',' || c == '\0' ) {
      int d = atoi(buf.c_str());
      VarId var(d);
      ls += BddLitSet(bddmgr, var, pol);
      if ( c == '\0' ) {
	return ls;
      }
      pol = kPolPosi;
      buf = "";
    }
    else if ( isspace(c) ) {
      continue;
    }
    else if ( isdigit(c) ) {
      buf += c;
    }
    else {
      cout << "ERROR[str2varset]: illegal character in " << ls_str << endl;
      exit(exit_code);
    }
  }
}

// bdd が str の論理式と等しいかどうか確かめる．
bool
check_bdde(BddMgr& bddmgr,
	   const Bdd& bdd,
	   const char* bdd_str,
	   const char* str)
{
  Bdd bdd2 = str2bdd(bddmgr, str);
  if ( bdd != bdd2 ) {
    bdd.display(cout);
    bdd2.display(cout);
    cout << "ERROR[check_bdde]: " << bdd_str << " != " << str << endl;
    return false;
  }
  return true;
}

// ite_op のテスト
bool
check_ite(BddMgr& bddmgr,
	  const char* if_str,
	  const char* then_str,
	  const char* else_str)
{
  Bdd if_bdd = str2bdd(bddmgr, if_str);
  Bdd then_bdd = str2bdd(bddmgr, then_str);
  Bdd else_bdd = str2bdd(bddmgr, else_str);

  Bdd ans_ref = if_bdd & then_bdd | ~if_bdd & else_bdd;
  Bdd ans = ite_op(if_bdd, then_bdd, else_bdd);
  if ( ans != ans_ref ) {
    ans_ref.display(cout);
    ans.display(cout);
    cout << "ERROR[check_ite]: ite_op( " << if_str << ", " << then_str
	 << ", " << else_str << ")" << endl;
    return false;
  }
  return true;
}

// support のテスト
// 0 〜 10 までの変数でコファクタリングして2つのコファクターが異なる
//ものがサポートだと思う．
bool
check_support(BddMgr& bddmgr,
	      const char* str)
{
  Bdd bdd = str2bdd(bddmgr, str);
  BddVarSet ref_vs(bddmgr);
  for (ymuint i = 0; i < 10; ++ i) {
    VarId var(i);
    Bdd bdd0 = bdd.cofactor(var, kPolNega);
    Bdd bdd1 = bdd.cofactor(var, kPolPosi);
    if ( bdd0 != bdd1 ) {
      ref_vs += BddVarSet(bddmgr, var);
    }
  }
  BddVarSet vs = bdd.support();
  if ( vs != ref_vs ) {
    bdd.display(cout);
    cout << "reference: " << ref_vs << endl;
    cout << "bdd.support(): " << vs << endl;
    cout << "ERROR[check_support]" << endl;
    return false;
  }
  return true;
}
#endif

END_NAMESPACE_YM_CNFDD
