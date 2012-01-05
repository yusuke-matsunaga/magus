
/// @file base_test.cc
/// @brief BDD パッケージのテストその1 --- 基本関数 ---
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Bdd.h"
#include "ym_logic/BddMgr.h"
#include "ym_logic/BddVarSet.h"
#include "ym_logic/BddVector.h"
#include "ym_logic/BddList.h"

#include "bddtest.h"



BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

void
print_header(const char* testname)
{
  cout << testname << endl;
}

void
print_result(bool result)
{
  if ( result ) {
    cout << "OK" << endl;
  }
  else {
    cout << "Error" << endl;
  }
}

END_NONAMESPACE


// 変数リストの検査
bool
test_var_list(BddMgr& bddmgr,
	      ymuint n)
{
  print_header("test_var_list");

  list<VarId> vlist;
  ymuint nn = bddmgr.var_list(vlist);
  bool result = true;
  if ( nn != n ) {
    result = false;
  }
  else {
    ymuint i = 0;
    for (list<VarId>::const_iterator p = vlist.begin();
	 p != vlist.end(); ++ p) {
      if ( *p != VarId(i) ) {
	result = false;
	break;
      }
      ++ i;
    }
  }

  if ( !result ) {
    cout << "ERROR in var_list" << endl;
    for (list<VarId>::const_iterator p = vlist.begin();
	 p != vlist.end(); ++ p) {
      cout << " " << *p;
    }
    cout << endl;
  }
  return result;
}

// 定数ノードの検査
bool
test_const(BddMgr& bddmgr)
{
  print_header("test_const");

  bool stat = true;
  Bdd one = bddmgr.make_one();
  if ( !one.is_one() ) {
    one.print(cout);
    cout << "ERROR[const-1]: !one.is_one()" << endl;
    stat = false;
  }

  Bdd zero = bddmgr.make_zero();
  if ( !zero.is_zero() ) {
    zero.print(cout);
    cout << "ERROR[const-2]: !zero.is_zero()" << endl;
    stat = false;
  }

  Bdd err = bddmgr.make_error();
  if ( !err.is_error() ) {
    err.print(cout);
    cout << "ERROR[const-3]: !err.is_error()" << endl;
    stat = false;
  }

  Bdd ovf = bddmgr.make_overflow();
  if ( !ovf.is_overflow() ) {
    ovf.print(cout);
    cout << "ERROR[const-4]: !ovf.is_overflow()" << endl;
    stat = false;
  }

  return stat;
}

// リテラル関数の生成
bool
test_literal(BddMgr& bddmgr)
{
  print_header("test_literal");

  bool stat = true;
  Bdd var0 = bddmgr.make_posiliteral(VarId(0));
  if ( !check_bddv(bddmgr, var0, "make_posiliteral(0)", "0|01") )
    stat = false;

  Bdd var0bar = bddmgr.make_negaliteral(VarId(0));
  if ( !check_bddv(bddmgr, var0bar, "make_negaliteral(0)", "0|10") )
    stat = false;

  Bdd var2 = bddmgr.make_literal(VarId(2), kPolPosi);
  if ( !check_bddv(bddmgr, var2, "make_literal(2, kPolPosi)", "2|01") )
    stat = false;

  Bdd var3bar = bddmgr.make_literal(VarId(3), kPolNega);
  if ( !check_bddv(bddmgr, var3bar, "make_literal(3, kPolNega)", "3|10") )
    stat = false;

  Bdd zero = bddmgr.make_zero();
  Bdd one = bddmgr.make_one();
  Bdd var4 = bddmgr.make_bdd(VarId(4), zero, one);
  if ( !check_bddv(bddmgr, var4, "make_bdd(4, zero, one)", "4|01") )
    stat = false;

  return stat;
}

// 単項演算
bool
test_unary(BddMgr& bddmgr)
{
  print_header("test_unary");

  Bdd var0 = bddmgr.make_posiliteral(VarId(0));

  bool stat = true;

  // って否定しかない
  if ( !check_bddv(bddmgr, ~var0, "~var0", "0|10") )
    stat = false;

  // 否定つき代入
  Bdd bdd1 = var0;
  bdd1.negate();
  if ( !check_bddv(bddmgr, bdd1, "var0.negate()", "0|10") )
    stat = false;

  return stat;
}

// 簡単な二項演算の検査
bool
test_binary(BddMgr& bddmgr)
{
  print_header("test_binary");

  Bdd var0 = bddmgr.make_posiliteral(VarId(0));
  Bdd var1 = bddmgr.make_posiliteral(VarId(1));

  bool stat = true;

  if ( !check_bddv(bddmgr, var0 & var1, "var0 & var1", "0,1|0001") )
    stat = false;

  if ( !check_bddv(bddmgr, var0 & ~var1, "var0 & ~var1", "0,1|0010") )
    stat = false;

  if ( !check_bddv(bddmgr, var0 & ~var1, "var0 & ~var1", "1,0|0100") )
    stat = false;

  if ( !check_bddv(bddmgr, var0 | var1, "var0 | var1", "0,1|0111") )
    stat = false;

  if ( !check_bddv(bddmgr, var0 ^ var1, "var0 ^ var1", "0,1|0110") )
    stat = false;

  Bdd bdd1 = var0;
  if ( !check_bddv(bddmgr, bdd1 &= var1, "var0 &= var1", "0,1|0001") )
    stat = false;

  Bdd bdd2 = var0;
  if ( !check_bddv(bddmgr, bdd2 |= var1, "var0 |= var1", "0,1|0111") )
    stat = false;

  Bdd bdd3 = var0;
  if ( !check_bddv(bddmgr, bdd3 ^= var1, "var0 ^= var1", "0,1|0110") )
    stat = false;

  return stat;
}

// BddVector の多項演算の検査
bool
test_bddvector(BddMgr& bddmgr)
{
  print_header("test_bddvector");

  bool stat = true;

  Bdd var[20];
  for (ymuint i = 0; i < 20; ++ i) {
    var[i] = bddmgr.make_posiliteral(VarId(i));
  }

  // Bdd
  for (ymuint i = 2; i < 20; ++ i) {
    BddVector bddvec(bddmgr, i);
    for (ymuint j = 0; j < i; ++ j) {
      bddvec[j] = var[j];
    }

    // 多項 AND
    Bdd ans_bdd = bddvec.and_op();
    Bdd ref_bdd = var[0];
    for (ymuint j = 1; j < i; ++ j) {
      ref_bdd &= var[j];
    }
    if ( ans_bdd != ref_bdd ) {
      cout << "Error in BddVector(" << i << ").and_op()" << endl;
      stat = false;
    }

    // 多項 OR
    ans_bdd = bddvec.or_op();
    ref_bdd = var[0];
    for (ymuint j = 1; j < i; ++ j) {
      ref_bdd |= var[j];
    }
    if ( ans_bdd != ref_bdd ) {
      cout << "Error in BddVector(" << i << ").or_op()" << endl;
      stat = false;
    }

    // 多項 XOR
    ans_bdd = bddvec.xor_op();
    ref_bdd = var[0];
    for (ymuint j = 1; j < i; ++ j) {
      ref_bdd ^= var[j];
    }
    if ( ans_bdd != ref_bdd ) {
      cout << "Error in BddVector(" << i << ").ans_op()" << endl;
      stat = false;
    }
  }

  return stat;
}

// BddList の多項演算の検査
bool
test_bddlist(BddMgr& bddmgr)
{
  print_header("test_bddlist");

  bool stat = true;

  Bdd var[20];
  for (ymuint i = 0; i < 20; ++ i) {
    var[i] = bddmgr.make_posiliteral(VarId(i));
  }

  // Bdd
  for (ymuint i = 2; i < 20; ++ i) {
    BddList bddlist(bddmgr);
    for (ymuint j = 0; j < i; ++ j) {
      bddlist.push_back(var[j]);
    }

    // 多項 AND
    Bdd ans_bdd = bddlist.and_op();
    Bdd ref_bdd = var[0];
    for (ymuint j = 1; j < i; ++ j) {
      ref_bdd &= var[j];
    }
    if ( ans_bdd != ref_bdd ) {
      cout << "Error in BddList(" << i << ").and_op()" << endl;
      stat = false;
    }

    // 多項 OR
    ans_bdd = bddlist.or_op();
    ref_bdd = var[0];
    for (ymuint j = 1; j < i; ++ j) {
      ref_bdd |= var[j];
    }
    if ( ans_bdd != ref_bdd ) {
      cout << "Error in BddList(" << i << ").or_op()" << endl;
      stat = false;
    }

    // 多項 XOR
    ans_bdd = bddlist.xor_op();
    ref_bdd = var[0];
    for (ymuint j = 1; j < i; ++ j) {
      ref_bdd ^= var[j];
    }
    if ( ans_bdd != ref_bdd ) {
      cout << "Error in BddList(" << i << ").ans_op()" << endl;
      stat = false;
    }
  }

  return stat;
}

// LogExpr や文字列からBDDを作る関数の検査
bool
test_make_bdd(BddMgr& bddmgr)
{
  print_header("test_make_bdd");

  bool stat = true;

  {
    LogExpr expr1 = LogExpr::make_posiliteral(VarId(0)) & LogExpr::make_negaliteral(VarId(1));
    Bdd bdd1 = bddmgr.expr_to_bdd(expr1);
    if ( !check_bddv(bddmgr, bdd1, "0 & ~1", "0, 1|0010" ) ) {
      stat = false;
    }
  }

  LogExpr expr1 =
    (LogExpr::make_posiliteral(VarId(0)) & LogExpr::make_negaliteral(VarId(1))) |
    (LogExpr::make_posiliteral(VarId(2)) & LogExpr::make_posiliteral(VarId(1)));
  Bdd bdd1 = bddmgr.expr_to_bdd(expr1);
  if ( !check_bddv(bddmgr, bdd1, "0 & ~1 | 2 & 1", "0, 1, 2|00011101" ) ) {
    stat = false;
  }

  VarVarMap vvmap;
  vvmap.insert(make_pair(VarId(0), 1));
  vvmap.insert(make_pair(VarId(1), 2));
  vvmap.insert(make_pair(VarId(2), 0));
  Bdd bdd3 = bddmgr.expr_to_bdd(expr1, vvmap);
  if ( !check_bddv(bddmgr, bdd3, "(0 & ~1 | 2 & 1)(0->1, 1->2, 2->0)",
		  "0, 1, 2|00100111") ) {
    stat = false;
  }

  VarVarMap vvmap2;
  vvmap2.insert(make_pair(VarId(0), 1));
  vvmap2.insert(make_pair(VarId(1), 0));
  Bdd bdd5 = bddmgr.expr_to_bdd(expr1, vvmap2);
  if ( !check_bddv(bddmgr, bdd5, "(0 & ~1 | 2 & 1)(0->1, 1->0)",
		  "0, 1, 2|00110101") ) {
    stat = false;
  }

  return stat;
}

// ite_op のテスト
bool
test_ite(BddMgr& bddmgr)
{
  print_header("test_ite");

  bool stat = true;

  if ( !check_ite(bddmgr, "0", "1", "2") ) {
    stat = false;
  }
  if ( !check_ite(bddmgr, "1 & ~3", "1 | 2 & 3", "2 & 3") ) {
    stat = false;
  }
  return stat;;
}

// compose のテスト
bool
test_compose(BddMgr& bddmgr)
{
  print_header("test_compose");

  bool stat = true;

  Bdd bdd1 = str2bdd(bddmgr, "0 & 1 & 2 & 3");
  Bdd bdd2 = str2bdd(bddmgr, "~4 | 5");

  Bdd bdd4 = bdd1.compose(VarId(0), bdd2);
  if ( !check_bdde(bddmgr, bdd4, "0 & 1 & 2 & 3 compose(0, ~4 | 5)",
		   "1 & 2 & 3 & (~4 | 5)") ) {
    stat = false;
  }

  return true;;
}

// (a|e)smooth のテスト
bool
test_smooth(BddMgr& bddmgr)
{
  print_header("test_smooth");

  bool stat = true;

  Bdd bdd1 = str2bdd(bddmgr, "0 & 1 | ~0 & 2");
  VarList vl;
  vl.push_back(VarId(0));
  BddVarSet vs1(bddmgr, vl);
  Bdd bdd2 = bdd1.esmooth(vs1);
  if ( !check_bdde(bddmgr, bdd2, "0 & 1 | ~0 & 2 esmooth(0)", "1 | 2") ) {
    stat = false;
  }
  Bdd bdd3 = bdd1.asmooth(vs1);
  if ( !check_bdde(bddmgr, bdd3, "0 & 1 | ~0 & 2 asmooth(0)", "1 & 2") ) {
    stat = false;
  }

  return true;;
}

// && 演算子のテスト
bool
test_intersect(BddMgr& bddmgr)
{
  print_header("test_intersect");

  bool stat = true;

  const char* str1 = "0 | 1";
  const char* str2 = "0 & 1";

  Bdd bdd1 = str2bdd(bddmgr, "0 & 1 | 2 & 3");
  Bdd bdd2 = str2bdd(bddmgr, str1);
  if ( !(bdd1 && bdd2) ) {
    bdd1.print(cout);
    bdd2.print(cout);
    cout << "ERROR[44]: bdd1 && bdd2 failed" << endl;
    stat = false;
  }

  Bdd bdd3 = str2bdd(bddmgr, str2);
  if ( !(bdd1 >= bdd3) ) {
    bdd1.print(cout);
    bdd3.print(cout);
    cout << "ERROR[45]: bdd1 >= bdd3 failed" << endl;
    stat = false;
  }

  return stat;;
}

//  cofactor 演算子のテスト
bool
test_scofactor(BddMgr& bddmgr)
{
  print_header("test_scofactor");

  bool stat = true;

  Bdd bdd = str2bdd(bddmgr, "0 & 1 | ~2 & 3");

  Bdd bdd0_0 = bdd.cofactor(VarId(0), kPolNega);
  if ( !check_bdde(bddmgr, bdd0_0, "(0 & 1 | ~2 & 3) / ~0", "~2 & 3") )
    stat = false;

  Bdd bdd0_1 = bdd.cofactor(VarId(0), kPolPosi);
  if ( !check_bdde(bddmgr, bdd0_1, "(0 & 1 | ~2 & 3) / 0", "1 | ~2 & 3") )
    stat = false;

  Bdd bdd1_0 = bdd.cofactor(VarId(1), kPolNega);
  if ( !check_bdde(bddmgr, bdd1_0, "(0 & 1 | ~2 & 3) / ~1", "~2 & 3") )
    stat = false;

  Bdd bdd1_1 = bdd.cofactor(VarId(1), kPolPosi);
  if ( !check_bdde(bddmgr, bdd1_1, "(0 & 1 | ~2 & 3) / 1", "0 | ~2 & 3") )
    stat = false;

  Bdd bdd2_0 = bdd.cofactor(VarId(2), kPolNega);
  if ( !check_bdde(bddmgr, bdd2_0, "(0 & 1 | ~2 & 3) / ~2", "0 & 1 | 3") )
    stat = false;

  Bdd bdd2_1 = bdd.cofactor(VarId(2), kPolPosi);
  if ( !check_bdde(bddmgr, bdd2_1, "(0 & 1 | ~2 & 3) / 2", "0 & 1") )
    stat = false;

  return stat;
}

//  / 演算子のテスト
bool
test_cofactor(BddMgr& bddmgr)
{
  print_header("test_cofactor");

  bool stat = true;

  const char* str = "0 & 2";

  Bdd bdd1 = str2bdd(bddmgr, "0 & 1 | 2 & 3");
  Bdd bdd2 = str2bdd(bddmgr, str);
  Bdd bdd3 = bdd1 / bdd2;
  if ( !check_bdde(bddmgr, bdd3, "0 & 1 | 1 & 2 / 0 & 2", "1 | 3") ) {
    stat = false;
  }
  return stat;
}

// and_exist演算
bool
test_and_exist(BddMgr& bddmgr)
{
  print_header("test_and_exist");

  bool stat = true;

  const char* str = "0 | 2";
  VarList vl;
  vl.push_back(VarId(0));

  Bdd bdd1 = str2bdd(bddmgr, "0 & 1 | ~0 & 2");
  Bdd bdd2 = str2bdd(bddmgr, str);
  BddVarSet svars1(bddmgr, vl);
  Bdd bdd3 = and_exist(bdd1, bdd2, svars1);
  if ( !check_bdde(bddmgr, bdd3, "and_exist(bdd1, bdd2, svars1)", "1 | 2") ) {
    stat = false;
  }

  return stat;;
}

// isop, prime_cover, minimal_support のテスト
bool
test_isop(BddMgr& bddmgr)
{
  print_header("test_isop");

  bool stat = true;

  const char* str = "0 & 2 | 1 & 3 | ~1 & ~3 | ~0 & ~2 & ~3";
  Bdd bdd1 = str2bdd(bddmgr, "0 & 1 & 2 | ~0 & 1 & 3 | ~1 & ~2 & ~3");

  Bdd bdd2 = str2bdd(bddmgr, str);
  LogExpr cover1;
  Bdd bdd3 = isop(bdd1, bdd2, cover1);
  if ( !check_bdde(bddmgr, bdd3, "isop(bdd1, bdd2, cover1)",
		   "0 & 2 | 1 & 3 | ~1 & ~3") ) {
    stat = false;
  }

  LogExpr ans_cover1 = prime_cover(bdd1, bdd2);
  Bdd bdd6 = bddmgr.expr_to_bdd(ans_cover1);
  if ( !check_bdde(bddmgr, bdd6, "prime_cover(bdd1, bdd2)",
		   "0 & 2 | 1 & 3 | ~1 & ~3") ) {
    stat = false;
  }

  Bdd bdd8 = minimal_support(bdd1, bdd2);
  if ( !check_bdde(bddmgr, bdd8, "minimal_support(bdd1, bdd2)", "0 & 1 & 2 & 3") ) {
    stat = false;
  }

  return stat;
}

// support のテスト
bool
test_support(BddMgr& bddmgr)
{
  print_header("test_support");

  bool stat = true;

  if ( !check_support(bddmgr, "0") ) {
    stat = false;
  }
  if ( !check_support(bddmgr, "0 | 1") ) {
    stat = false;
  }
  if ( !check_support(bddmgr, "0 & 1 | ~0 & 3") ) {
    stat = false;
  }

  return stat;
}

// test_scc のサブルーティン
bool
check_scc(BddMgr& bddmgr,
	  string expr_str,
	  const char* scc_str)
{
  Bdd bdd = str2bdd(bddmgr, expr_str.c_str());
  Bdd scc = bdd.SCC();
  string tmp = "SCC(" + expr_str + ")";

  return check_bdde(bddmgr, scc, tmp.c_str(), scc_str);
}

// SCC のテスト
bool
test_scc(BddMgr& bddmgr)
{
  print_header("test_scc");

  bool stat = true;

  if ( !check_scc(bddmgr, "0 & 1 & ~2", "0 & 1 & ~2") ) {
    stat = false;
  }
  if ( !check_scc(bddmgr, "0 & 1 | ~0 & ~2", "one") ) {
    stat = false;
  }
  if ( !check_scc(bddmgr, "(0 ^ 1) & 2", "2") ) {
    stat = false;
  }
  if ( !check_scc(bddmgr, "0 & 1 & ~2 | ~0 & 1 & 2", "1") ) {
    stat = false;
  }
  if ( !check_scc(bddmgr, "0 | 1 | 2", "one") ) {
    stat = false;
  }

  return stat;
}

// test_symmetry のサブルーティン
bool
check_sym(BddMgr& bddmgr,
	  string expr_str)
{
  bool stat = true;

  Bdd bdd = str2bdd(bddmgr, expr_str.c_str());
  VarVector sup;
  ymuint ni = bdd.support(sup);
  if ( ni < 2 ) {
    // 無意味
    return stat;
  }

  for (ymuint i = 0; i < ni - 1; ++ i) {
    VarId pos1 = sup[i];
    Bdd bdd_0 = bdd.cofactor(pos1, kPolNega);
    Bdd bdd_1 = bdd.cofactor(pos1, kPolPosi);
    for (ymuint j = i + 1; j < ni; ++ j) {
      VarId pos2 = sup[j];
      Bdd bdd_01 = bdd_0.cofactor(pos2, kPolPosi);
      Bdd bdd_10 = bdd_1.cofactor(pos2, kPolNega);
      bool expected_result1 = (bdd_01 == bdd_10);
      if ( bdd.check_symmetry(pos1, pos2, kPolPosi) != expected_result1 ) {
	cout << "ERROR[test_symmetry(positive)]" << endl;
	bdd.print(cout);
	cout << "pos1 : " << pos1 << ", pos2 : " << pos2 << endl;
	cout << "expected result = ";
	if ( expected_result1 ) {
	  cout << "true" << endl;
	}
	else {
	  cout << "false" << endl;
	}
	stat = false;
      }
      Bdd bdd_00 = bdd_0.cofactor(pos2, kPolNega);
      Bdd bdd_11 = bdd_1.cofactor(pos2, kPolPosi);
      bool expected_result2 = (bdd_00 == bdd_11);
      if ( bdd.check_symmetry(pos1, pos2, kPolNega) != expected_result2 ) {
	cout << "ERROR[test_invsymmetry(negative)]" << endl;
	bdd.print(cout);
	cout << "pos1 : " << pos1 << ", pos2 : " << pos2 << endl;
	cout << "expected result = ";
	if ( expected_result2 ) {
	  cout << "true" << endl;
	}
	else {
	  cout << "false" << endl;
	}
	cout << "bdd_00" << endl;
	bdd_00.print(cout);
	cout << "bdd_11" << endl;
	bdd_11.print(cout);
	stat = false;
      }
    }
  }
  return stat;
}

// check_symmetry のテスト
bool
test_symmetry(BddMgr& bddmgr)
{
  print_header("test_symmetry");

  bool stat = true;

  if ( !check_sym(bddmgr, "0 & 2 | ~0 & 1 & ~2") ) {
    stat = false;
  }
  if ( !check_sym(bddmgr, "0 & 1 | ~0 & ~1 & 2") ) {
    stat = false;
  }
  if ( !check_sym(bddmgr, "0 & 2 | 0 & ~1 | ~0 & 1 | ~0 & ~1 & ~2") ) {
    stat = false;
  }
  if ( !check_sym(bddmgr, "~0 & 1 & ~2 | 0 & ~1 | 0 & 2") ) {
    stat = false;
  }
  if ( !check_sym(bddmgr, "~0 & ~1 & 2 | 0 & 1 & ~2") ) {
    stat = false;
  }
  if ( !check_sym(bddmgr, "~0 & 1 & 2 | 0 & ~1 & ~2") ) {
    stat = false;
  }
  if ( !check_sym(bddmgr, "0 & 2 | ~0 & 1 & ~2") ) {
    stat = false;
  }

  return stat;
}

// node_count のテスト
bool
test_node_count(BddMgr& bddmgr)
{
  print_header("test_node_count");

  bool stat = true;

  Bdd bdd1 = str2bdd(bddmgr, "0 ^ 1 ^ 2");
  ymuint64 n = bdd1.node_count();
  if ( n != 3 ) {
    cout << "node_count(0 ^ 1 ^ 2) != 3" << endl;
    stat = false;
  }

  return stat;
}

// minterm_count のテスト
bool
test_minterm_count(BddMgr& bddmgr)
{
  print_header("test_minterm_count");

  bool stat = true;

  Bdd bdd = str2bdd(bddmgr, "0 & 2 | 1 & 3 | ~1 & ~3");
  mpz_class mc = bdd.minterm_count(4);
  if ( mc != 10 ) {
    cout << "minterm_count(0 & 2 | 1 & 3 | ~1 & ~3) != 10" << endl;
    stat = false;
  }

  for (ymuint n = 2; n <= 100; ++ n) {
    bdd = bddmgr.make_one();
    for (ymuint i = 0; i < n; ++ i) {
      Bdd bdd1 = bddmgr.make_posiliteral(VarId(i));
      bdd &= bdd1;
    }
    mc = bdd.minterm_count(n);
    if ( mc != 1 ) {
      cout << "mc != 1" << endl;
      cout << "mc = " << mc.get_str() << endl;
      stat = false;
    }
    Bdd bdd2 = ~bdd;
    mpz_class mc2 = bdd2.minterm_count(n);
    if ( mc2 != (mpz_class(1) << n) - mpz_class(1) ) {
      cout << "mc != 2^" << n << " - 1" << endl;
      stat = false;
    }
  }

  return stat;
}

bool
check_walsh0(const Bdd& bdd,
	     ymuint nvar)
{
  mpz_class w0 = bdd.walsh0(nvar);
  mpz_class n1 = bdd.minterm_count(nvar);
  mpz_class n0 = (~bdd).minterm_count(nvar);
  mpz_class w0_expected = n0 - n1;
  if ( w0 != w0_expected ) {
    cout << "walsh0 error" << endl;
    bdd.print(cout);
    cout << "bdd.walsh0() = " << w0 << endl
	 << "expected     = " << w0_expected << endl
	 << endl;
    return false;
  }
  return true;
}

// walsh0 のテスト
bool
test_walsh0(BddMgr& bddmgr)
{
  print_header("test_walsh0");

  bool stat = true;

  Bdd bdd = str2bdd(bddmgr, "0 & 2 | 1 & 3 | ~1 & ~3");
  if ( !check_walsh0(bdd, 4) ) {
    stat = false;
  }

  for (ymuint n = 2; n <= 100; ++ n) {
    bdd = bddmgr.make_one();
    for (ymuint i = 0; i < n; ++ i) {
      Bdd bdd1 = bddmgr.make_posiliteral(VarId(i));
      bdd &= bdd1;
    }
    if ( !check_walsh0(bdd, n) ) {
      stat = false;
    }
    Bdd bdd2 = ~bdd;
    if ( !check_walsh0(bdd2, n) ) {
      stat = false;
    }
  }

  return stat;
}

bool
check_walsh1(const Bdd& bdd,
	     ymuint nvar)
{
  for (ymuint i = 0; i < nvar; ++ i) {
    mpz_class w1 = bdd.walsh1(VarId(i), nvar);
    Bdd bdd0 = bdd.cofactor(VarId(i), kPolNega);
    Bdd bdd1 = bdd.cofactor(VarId(i), kPolPosi);
    mpz_class n0 = bdd0.walsh0(nvar - 1);
    mpz_class n1 = bdd1.walsh0(nvar - 1);
    mpz_class w1_expected = n0 - n1;
    if ( w1 != w1_expected ) {
      cout << "walsh1(" << i << ") error" << endl;
      bdd.print(cout);
      cout << "bdd.walsh1(" << i << ") = " << w1 << endl
	   << "expected     = " << w1_expected << endl
	   << endl;
      return false;
    }
  }
  return true;
}

// walsh1 のテスト
bool
test_walsh1(BddMgr& bddmgr)
{
  print_header("test_walsh1");

  bool stat = true;

  Bdd bdd = str2bdd(bddmgr, "0 & 2 | 1 & 3 | ~1 & ~3");
  if ( !check_walsh1(bdd, 4) ) {
    stat = false;
  }

  for (ymuint n = 2; n <= 100; ++ n) {
    bdd = bddmgr.make_one();
    for (ymuint i = 0; i < n; ++ i) {
      Bdd bdd1 = bddmgr.make_posiliteral(VarId(i));
      bdd &= bdd1;
    }
    if ( !check_walsh1(bdd, n) ) {
      stat = false;
    }
    Bdd bdd2 = ~bdd;
    if ( !check_walsh1(bdd2, n) ) {
      stat = false;
    }
  }

  return stat;
}

// dump/restore のテスト
bool
test_dump(BddMgr& bddmgr)
{
  print_header("test_dump");

  bool stat = true;

  Bdd bdd = str2bdd(bddmgr, "0 & 2 | 1 & 3 | ~1 & ~3");
  const char* fn = "/tmp/magus_bdd_base_test";
  {
    ofstream ofs(fn);
    if ( !ofs ) {
      cout << "cannot open output file: " << fn << endl;
      stat = false;
    }
    BinOStream bos(ofs);
    bdd.dump(bos);
  }
  Bdd bdd2(bddmgr);
  {
    ifstream ifs(fn);
    if ( !ifs ) {
      cout << "cannont open input file: " << fn << endl;
      stat = false;
    }
    BinIStream bis(ifs);
    bdd2.restore(bis);
  }
  if ( bdd != bdd2 ) {
    cout << "ERROR[test_dump]" << endl;
    bdd.print(cout);
    bdd2.print(cout);
    stat = false;
  }

  return stat;
}

bool
test(BddMgr& bddmgr)
{
  bool stat = true;

  for (ymuint i = 0; i < 10; ++ i) {
    bddmgr.new_var(VarId(i));
  }

  if ( !test_var_list(bddmgr, 10) )
    stat = false;

  if ( !test_const(bddmgr) )
    stat = false;

  if ( !test_unary(bddmgr) )
    stat = false;

  if ( !test_binary(bddmgr) )
    stat = false;

  if ( !test_bddvector(bddmgr) )
    stat = false;

  if ( !test_bddlist(bddmgr) )
    stat = false;

  if ( !test_make_bdd(bddmgr) )
    stat = false;

  if ( !test_ite(bddmgr) )
    stat = false;

  if ( !test_compose(bddmgr) )
    stat = false;

  if ( !test_smooth(bddmgr) )
    stat = false;

  if ( !test_intersect(bddmgr) )
    stat = false;

  if ( !test_scofactor(bddmgr) )
    stat = false;

  if ( !test_cofactor(bddmgr) )
    stat = false;

  if ( !test_and_exist(bddmgr) )
    stat = false;

  if ( !test_isop(bddmgr) )
    stat = false;

  if ( !test_support(bddmgr) )
    stat = false;


  if ( !test_scc(bddmgr) )
    stat = false;

  if ( !test_node_count(bddmgr) )
    stat = false;

  if ( !test_minterm_count(bddmgr) )
    stat = false;

  if ( !test_walsh0(bddmgr) )
    stat = false;

  if ( !test_walsh1(bddmgr) )
    stat = false;

  if ( !test_symmetry(bddmgr) )
    stat = false;

  if ( !test_dump(bddmgr) )
    stat = false;

  cout << "test end" << endl;

  return stat;
}

END_NAMESPACE_YM_BDD


int
main(int argc,
     char** argv)
{
  using namespace std;
  using nsYm::AssertError;
  using nsYm::BddMgr;

#if !defined(YM_DEBUG)
  try {
#endif
    BddMgr bddmgr1("bmc", "classic bddmgr");
    if ( !test(bddmgr1) ) {
      return 1;
    }
    BddMgr bddmgr2("bmm", "fixed order bddmgr");
    if ( !test(bddmgr2) ) {
      return 2;
    }
    BddMgr bddmgr3("bmm", "reorder bddmgr", "reorder");
    if ( !test(bddmgr3) ) {
      return 3;
    }
#if !defined(YM_DEBUG)
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }
#endif

  return 0;
}
