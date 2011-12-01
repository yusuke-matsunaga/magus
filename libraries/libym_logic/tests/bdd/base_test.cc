
/// @file libym_logic/tests/bdd/base_test.cc
/// @brief BDD パッケージのテストその1 --- 基本関数 ---
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: base_test.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Bdd.h"
#include "ym_logic/BddMgr.h"
#include "ym_logic/BddVarSet.h"

#include "bddtest.h"



BEGIN_NAMESPACE_YM_BDD

// 変数リストの検査
bool
test_var_list(BddMgr& bddmgr,
	      ymuint n)
{
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
  Bdd one = bddmgr.make_one();
  if ( !one.is_one() ) {
    one.display(cout);
    cout << "ERROR[const-1]: !one.is_one()" << endl;
    return false;
  }

  Bdd zero = bddmgr.make_zero();
  if ( !zero.is_zero() ) {
    zero.display(cout);
    cout << "ERROR[const-2]: !zero.is_zero()" << endl;
    return false;
  }

  Bdd err = bddmgr.make_error();
  if ( !err.is_error() ) {
    err.display(cout);
    cout << "ERROR[const-3]: !err.is_error()" << endl;
    return false;
  }

  Bdd ovf = bddmgr.make_overflow();
  if ( !ovf.is_overflow() ) {
    ovf.display(cout);
    cout << "ERROR[const-4]: !ovf.is_overflow()" << endl;
    return false;
  }

  return true;
}

// リテラル関数の生成
bool
test_literal(BddMgr& bddmgr)
{
  Bdd var0 = bddmgr.make_posiliteral(VarId(0));
  if ( !check_bddv(bddmgr, var0, "make_posiliteral(0)", "0|01") )
    return false;

  Bdd var0bar = bddmgr.make_negaliteral(VarId(0));
  if ( !check_bddv(bddmgr, var0bar, "make_negaliteral(0)", "0|10") )
    return false;

  Bdd var2 = bddmgr.make_literal(VarId(2), kPolPosi);
  if ( !check_bddv(bddmgr, var2, "make_literal(2, kPolPosi)", "2|01") )
    return false;

  Bdd var3bar = bddmgr.make_literal(VarId(3), kPolNega);
  if ( !check_bddv(bddmgr, var3bar, "make_literal(3, kPolNega)", "3|10") )
    return false;

  Bdd zero = bddmgr.make_zero();
  Bdd one = bddmgr.make_one();
  Bdd var4 = bddmgr.make_bdd(VarId(4), zero, one);
  if ( !check_bddv(bddmgr, var4, "make_bdd(4, zero, one)", "4|01") )
    return false;

  return true;
}

// 単項演算
bool
test_unary(BddMgr& bddmgr)
{
  Bdd var0 = bddmgr.make_posiliteral(VarId(0));

  // って否定しかない
  if ( !check_bddv(bddmgr, ~var0, "~var0", "0|10") )
    return false;

  // 否定つき代入
  Bdd bdd1 = var0;
  bdd1.negate();
  if ( !check_bddv(bddmgr, bdd1, "var0.negate()", "0|10") )
    return false;

  return true;
}

// 簡単な二項演算の検査
bool
test_binary(BddMgr& bddmgr)
{
  Bdd var0 = bddmgr.make_posiliteral(VarId(0));
  Bdd var1 = bddmgr.make_posiliteral(VarId(1));

  if ( !check_bddv(bddmgr, var0 & var1, "var0 & var1", "0,1|0001") )
    return false;

  if ( !check_bddv(bddmgr, var0 & ~var1, "var0 & ~var1", "0,1|0010") )
    return false;

  if ( !check_bddv(bddmgr, var0 & ~var1, "var0 & ~var1", "1,0|0100") )
    return false;

  if ( !check_bddv(bddmgr, var0 | var1, "var0 | var1", "0,1|0111") )
    return false;

  if ( !check_bddv(bddmgr, var0 ^ var1, "var0 ^ var1", "0,1|0110") )
    return false;

  Bdd bdd1 = var0;
  if ( !check_bddv(bddmgr, bdd1 &= var1, "var0 &= var1", "0,1|0001") )
    return false;

  Bdd bdd2 = var0;
  if ( !check_bddv(bddmgr, bdd2 |= var1, "var0 |= var1", "0,1|0111") )
    return false;

  Bdd bdd3 = var0;
  if ( !check_bddv(bddmgr, bdd3 ^= var1, "var0 ^= var1", "0,1|0110") )
    return false;

  return true;
}

// LogExpr や文字列からBDDを作る関数の検査
bool
test_make_bdd(BddMgr& bddmgr)
{
  LogExpr expr1 = LogExpr::make_posiliteral(VarId(0)) &
    LogExpr::make_negaliteral(VarId(1)) |
    LogExpr::make_posiliteral(VarId(2)) &
    LogExpr::make_posiliteral(VarId(1));
  Bdd bdd1 = bddmgr.expr_to_bdd(expr1);
  if ( !check_bddv(bddmgr, bdd1, "0 & ~1 | 2 & 1", "0, 1, 2|00011101" ) ) {
    return false;
  }

  VarVarMap vvmap;
  vvmap.insert(make_pair(VarId(0), 1));
  vvmap.insert(make_pair(VarId(1), 2));
  vvmap.insert(make_pair(VarId(2), 0));
  Bdd bdd3 = bddmgr.expr_to_bdd(expr1, vvmap);
  if ( !check_bddv(bddmgr, bdd3, "(0 & ~1 | 2 & 1)(0->1, 1->2, 2->0)",
		  "0, 1, 2|00100111") ) {
    return false;
  }

  VarVarMap vvmap2;
  vvmap2.insert(make_pair(VarId(0), 1));
  vvmap2.insert(make_pair(VarId(1), 0));
  Bdd bdd5 = bddmgr.expr_to_bdd(expr1, vvmap2);
  if ( !check_bddv(bddmgr, bdd5, "(0 & ~1 | 2 & 1)(0->1, 1->0)",
		  "0, 1, 2|00110101") ) {
    return false;
  }

  return true;
}

// ite_op のテスト
bool
test_ite(BddMgr& bddmgr)
{
  if ( !check_ite(bddmgr, "0", "1", "2") ) {
    return false;
  }
  if ( !check_ite(bddmgr, "1 & ~3", "1 | 2 & 3", "2 & 3") ) {
    return false;
  }
  return true;
}

// compose のテスト
bool
test_compose(BddMgr& bddmgr)
{
  Bdd bdd1 = str2bdd(bddmgr, "0 & 1 & 2 & 3");
  Bdd bdd2 = str2bdd(bddmgr, "~4 | 5");

  Bdd bdd4 = bdd1.compose(VarId(0), bdd2);
  if ( !check_bdde(bddmgr, bdd4, "0 & 1 & 2 & 3 compose(0, ~4 | 5)",
		   "1 & 2 & 3 & (~4 | 5)") ) {
    return false;
  }

  return true;
}

// (a|e)smooth のテスト
bool
test_smooth(BddMgr& bddmgr)
{
  Bdd bdd1 = str2bdd(bddmgr, "0 & 1 | ~0 & 2");
  VarList vl;
  vl.push_back(VarId(0));
  BddVarSet vs1(bddmgr, vl);
  Bdd bdd2 = bdd1.esmooth(vs1);
  if ( !check_bdde(bddmgr, bdd2, "0 & 1 | ~0 & 2 esmooth(0)", "1 | 2") ) {
    return false;
  }
  Bdd bdd3 = bdd1.asmooth(vs1);
  if ( !check_bdde(bddmgr, bdd3, "0 & 1 | ~0 & 2 asmooth(0)", "1 & 2") ) {
    return false;
  }
  return true;
}

// && 演算子のテスト
bool
test_intersect(BddMgr& bddmgr)
{
  const char* str1 = "0 | 1";
  const char* str2 = "0 & 1";

  Bdd bdd1 = str2bdd(bddmgr, "0 & 1 | 2 & 3");
  Bdd bdd2 = str2bdd(bddmgr, str1);
  if ( !(bdd1 && bdd2) ) {
    bdd1.display(cout);
    bdd2.display(cout);
    cout << "ERROR[44]: bdd1 && bdd2 failed" << endl;
    return false;
  }

  Bdd bdd3 = str2bdd(bddmgr, str2);
  if ( !(bdd1 >= bdd3) ) {
    bdd1.display(cout);
    bdd3.display(cout);
    cout << "ERROR[45]: bdd1 >= bdd3 failed" << endl;
    return false;
  }
  return true;
}

//  / 演算子のテスト
bool
test_cofactor(BddMgr& bddmgr)
{
  const char* str = "0 & 2";

  Bdd bdd1 = str2bdd(bddmgr, "0 & 1 | 2 & 3");
  Bdd bdd2 = str2bdd(bddmgr, str);
  Bdd bdd3 = bdd1 / bdd2;
  if ( !check_bdde(bddmgr, bdd3, "0 & 1 | 1 & 2 / 0 & 2", "1 | 3") ) {
    return false;
  }
  return true;
}

// and_exist演算
bool
test_and_exist(BddMgr& bddmgr)
{
  const char* str = "0 | 2";
  VarList vl;
  vl.push_back(VarId(0));

  Bdd bdd1 = str2bdd(bddmgr, "0 & 1 | ~0 & 2");
  Bdd bdd2 = str2bdd(bddmgr, str);
  BddVarSet svars1(bddmgr, vl);
  Bdd bdd3 = and_exist(bdd1, bdd2, svars1);
  if ( !check_bdde(bddmgr, bdd3, "and_exist(bdd1, bdd2, svars1)", "1 | 2") ) {
    return false;
  }

  return true;
}

// isop, prime_cover, minimal_support のテスト
bool
test_isop(BddMgr& bddmgr)
{
  const char* str = "0 & 2 | 1 & 3 | ~1 & ~3 | ~0 & ~2 & ~3";
  Bdd bdd1 = str2bdd(bddmgr, "0 & 1 & 2 | ~0 & 1 & 3 | ~1 & ~2 & ~3");

  Bdd bdd2 = str2bdd(bddmgr, str);
  LogExpr cover1;
  Bdd bdd3 = isop(bdd1, bdd2, cover1);
  if ( !check_bdde(bddmgr, bdd3, "isop(bdd1, bdd2, cover1)",
		   "0 & 2 | 1 & 3 | ~1 & ~3") ) {
    return false;
  }

  LogExpr ans_cover1 = prime_cover(bdd1, bdd2);
  Bdd bdd6 = bddmgr.expr_to_bdd(ans_cover1);
  if ( !check_bdde(bddmgr, bdd6, "prime_cover(bdd1, bdd2)",
		   "0 & 2 | 1 & 3 | ~1 & ~3") ) {
    return false;
  }

  Bdd bdd8 = minimal_support(bdd1, bdd2);
  if ( !check_bdde(bddmgr, bdd8, "minimal_support(bdd1, bdd2)", "0 & 1 & 2 & 3") ) {
    return false;
  }

  return true;
}

// support のテスト
bool
test_support(BddMgr& bddmgr)
{
  if ( !check_support(bddmgr, "0") ) {
    return false;
  }
  if ( !check_support(bddmgr, "0 | 1") ) {
    return false;
  }
  if ( !check_support(bddmgr, "0 & 1 | ~0 & 3") ) {
    return false;
  }
  return true;
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
  if ( !check_scc(bddmgr, "0 & 1 & ~2", "0 & 1 & ~2") ) {
    return false;
  }
  if ( !check_scc(bddmgr, "0 & 1 | ~0 & ~2", "one") ) {
    return false;
  }
  if ( !check_scc(bddmgr, "(0 ^ 1) & 2", "2") ) {
    return false;
  }
  if ( !check_scc(bddmgr, "0 & 1 & ~2 | ~0 & 1 & 2", "1") ) {
    return false;
  }
  if ( !check_scc(bddmgr, "0 | 1 | 2", "one") ) {
    return false;
  }
  return true;
}

// test_symmetry のサブルーティン
bool
check_sym(BddMgr& bddmgr,
	  string expr_str)
{
  Bdd bdd = str2bdd(bddmgr, expr_str.c_str());
  VarVector sup;
  ymuint ni = bdd.support(sup);
  if ( ni < 2 ) {
    // 無意味
    return true;
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
	bdd.display(cout);
	cout << "pos1 : " << pos1 << ", pos2 : " << pos2 << endl;
	cout << "expected result = ";
	if ( expected_result1 ) {
	  cout << "true" << endl;
	}
	else {
	  cout << "false" << endl;
	}
	return false;
      }
      Bdd bdd_00 = bdd_0.cofactor(pos2, kPolNega);
      Bdd bdd_11 = bdd_1.cofactor(pos2, kPolPosi);
      bool expected_result2 = (bdd_00 == bdd_11);
      if ( bdd.check_symmetry(pos1, pos2, kPolNega) != expected_result2 ) {
	cout << "ERROR[test_invsymmetry(negative)]" << endl;
	bdd.display(cout);
	cout << "pos1 : " << pos1 << ", pos2 : " << pos2 << endl;
	cout << "expected result = ";
	if ( expected_result2 ) {
	  cout << "true" << endl;
	}
	else {
	  cout << "false" << endl;
	}
	return false;
      }
    }
  }
  return true;
}

// check_symmetry のテスト
bool
test_symmetry(BddMgr& bddmgr)
{
  if ( !check_sym(bddmgr, "0 & 2 | ~0 & 1 & ~2") ) {
    return false;
  }
  if ( !check_sym(bddmgr, "0 & 1 | ~0 & ~1 & 2") ) {
    return false;
  }
  if ( !check_sym(bddmgr, "0 & 2 | 0 & ~1 | ~0 & 1 | ~0 & ~1 & ~2") ) {
    return false;
  }
  if ( !check_sym(bddmgr, "~0 & 1 & ~2 | 0 & ~1 | 0 & 2") ) {
    return false;
  }
  if ( !check_sym(bddmgr, "~0 & ~1 & 2 | 0 & 1 & ~2") ) {
    return false;
  }
  if ( !check_sym(bddmgr, "~0 & 1 & 2 | 0 & ~1 & ~2") ) {
    return false;
  }
  if ( !check_sym(bddmgr, "0 & 2 | ~0 & 1 & ~2") ) {
    return false;
  }
  return true;
}

// minterm_count のテスト
bool
test_minterm_count(BddMgr& bddmgr)
{
  Bdd bdd = str2bdd(bddmgr, "0 & 2 | 1 & 3 | ~1 & ~3");
  mpz_class mc = bdd.minterm_count(4);
  if ( mc != 10 ) {
    cout << "minterm_count(0 & 2 | 1 & 3 | ~1 & ~3) != 10" << endl;
    return false;
  }

  bdd = bddmgr.make_one();
  for (ymuint i = 0; i < 100; ++ i) {
    Bdd bdd1 = bddmgr.make_posiliteral(VarId(i));
    bdd &= bdd1;
  }
  mc = bdd.minterm_count(100);
  if ( mc != 1 ) {
    cout << "mc != 1" << endl;
    cout << "mc = " << mc.get_str() << endl;
    return false;
  }
  Bdd bdd2 = ~bdd;
  mpz_class mc2 = bdd2.minterm_count(100);
  if ( mc2 != (mpz_class(1) << 100) - mpz_class(1) ) {
    cout << "mc != 2^100 - 1" << endl;
    return false;
  }
  return true;
}

// dump/restore のテスト
bool
test_dump(BddMgr& bddmgr)
{
  Bdd bdd = str2bdd(bddmgr, "0 & 2 | 1 & 3 | ~1 & ~3");
  const char* fn = "/tmp/magus_bdd_base_test";
  {
    ofstream ofs(fn);
    if ( !ofs ) {
      cout << "cannot open output file: " << fn << endl;
      return false;
    }
    BinOStream bos(ofs);
    bdd.dump(bos);
  }
  Bdd bdd2;
  {
    ifstream ifs(fn);
    if ( !ifs ) {
      cout << "cannont open input file: " << fn << endl;
      return false;
    }
    BinIStream bis(ifs);
    bdd2 = bddmgr.restore(bis);
  }
  if ( bdd != bdd2 ) {
    cout << "ERROR[test_dump]" << endl;
    bdd.display(cout);
    bdd2.display(cout);
    return false;
  }
  return true;
}

bool
test(BddMgr& bddmgr)
{
  for (ymuint i = 0; i < 10; ++ i) {
    bddmgr.new_var(VarId(i));
  }
  return test_var_list(bddmgr, 10) &&
    test_const(bddmgr) &&
    test_unary(bddmgr) &&
    test_binary(bddmgr) &&
    test_make_bdd(bddmgr) &&
    test_ite(bddmgr) &&
    test_compose(bddmgr) &&
    test_smooth(bddmgr) &&
    test_intersect(bddmgr) &&
    test_cofactor(bddmgr) &&
    test_and_exist(bddmgr) &&
    test_isop(bddmgr) &&
    test_support(bddmgr) &&
    test_scc(bddmgr) &&
    test_minterm_count(bddmgr) &&
    test_symmetry(bddmgr) &&
    test_dump(bddmgr)
    ;
}

END_NAMESPACE_YM_BDD


int
main(int argc,
     char** argv)
{
  using namespace std;
  using nsYm::AssertError;
  using nsYm::BddMgr;

  try {
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
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }

  return 0;
}
