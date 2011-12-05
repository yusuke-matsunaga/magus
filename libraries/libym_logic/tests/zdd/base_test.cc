
/// @file base_test.cc
/// @brief ZDD パッケージのテストその1 --- 基本関数 ---
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Zdd.h"
#include "ym_logic/ZddMgr.h"

#include "zddtest.h"



BEGIN_NAMESPACE_YM_ZDD

// 変数リストの検査
bool
test_var_list(ZddMgr& zddmgr,
	      ymuint n)
{
  list<VarId> vlist;
  ymuint nn = zddmgr.var_list(vlist);
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
test_const(ZddMgr& zddmgr)
{
  Zdd one = zddmgr.make_one();
  if ( !one.is_one() ) {
    one.display(cout);
    cout << "ERROR[const-1]: !one.is_one()" << endl;
    return false;
  }

  Zdd zero = zddmgr.make_zero();
  if ( !zero.is_zero() ) {
    zero.display(cout);
    cout << "ERROR[const-2]: !zero.is_zero()" << endl;
    return false;
  }

  Zdd err = zddmgr.make_error();
  if ( !err.is_error() ) {
    err.display(cout);
    cout << "ERROR[const-3]: !err.is_error()" << endl;
    return false;
  }

  Zdd ovf = zddmgr.make_overflow();
  if ( !ovf.is_overflow() ) {
    ovf.display(cout);
    cout << "ERROR[const-4]: !ovf.is_overflow()" << endl;
    return false;
  }

  return true;
}

// リテラル関数の生成
bool
test_literal(ZddMgr& zddmgr)
{
  Zdd var0 = zddmgr.make_posiliteral(VarId(0));
  if ( !check_zddv(zddmgr, var0, "make_posiliteral(0)", "0|01") )
    return false;

  Zdd var0bar = zddmgr.make_negaliteral(VarId(0));
  if ( !check_zddv(zddmgr, var0bar, "make_negaliteral(0)", "0|10") )
    return false;

  Zdd var2 = zddmgr.make_literal(VarId(2), kPolPosi);
  if ( !check_zddv(zddmgr, var2, "make_literal(2, kPolPosi)", "2|01") )
    return false;

  Zdd var3bar = zddmgr.make_literal(VarId(3), kPolNega);
  if ( !check_zddv(zddmgr, var3bar, "make_literal(3, kPolNega)", "3|10") )
    return false;

  Zdd zero = zddmgr.make_zero();
  Zdd one = zddmgr.make_one();
  Zdd var4 = zddmgr.make_zdd(VarId(4), zero, one);
  if ( !check_zddv(zddmgr, var4, "make_zdd(4, zero, one)", "4|01") )
    return false;

  return true;
}

// 簡単な二項演算の検査
bool
test_binary(ZddMgr& zddmgr)
{
  Zdd var0 = zddmgr.make_posiliteral(VarId(0));
  Zdd var1 = zddmgr.make_posiliteral(VarId(1));

  if ( !check_zddv(zddmgr, var0 & var1, "var0 & var1", "0,1|0001") )
    return false;

  if ( !check_zddv(zddmgr, var0 & ~var1, "var0 & ~var1", "0,1|0010") )
    return false;

  if ( !check_zddv(zddmgr, var0 & ~var1, "var0 & ~var1", "1,0|0100") )
    return false;

  if ( !check_zddv(zddmgr, var0 | var1, "var0 | var1", "0,1|0111") )
    return false;

  if ( !check_zddv(zddmgr, var0 ^ var1, "var0 ^ var1", "0,1|0110") )
    return false;

  Zdd zdd1 = var0;
  if ( !check_zddv(zddmgr, zdd1 &= var1, "var0 &= var1", "0,1|0001") )
    return false;

  Zdd zdd2 = var0;
  if ( !check_zddv(zddmgr, zdd2 |= var1, "var0 |= var1", "0,1|0111") )
    return false;

  Zdd zdd3 = var0;
  if ( !check_zddv(zddmgr, zdd3 ^= var1, "var0 ^= var1", "0,1|0110") )
    return false;

  return true;
}

// LogExpr や文字列からBDDを作る関数の検査
bool
test_make_zdd(ZddMgr& zddmgr)
{
  LogExpr expr1 = LogExpr::make_posiliteral(VarId(0)) &
    LogExpr::make_negaliteral(VarId(1)) |
    LogExpr::make_posiliteral(VarId(2)) &
    LogExpr::make_posiliteral(VarId(1));
  Zdd zdd1 = zddmgr.expr_to_zdd(expr1);
  if ( !check_zddv(zddmgr, zdd1, "0 & ~1 | 2 & 1", "0, 1, 2|00011101" ) ) {
    return false;
  }

  VarVarMap vvmap;
  vvmap.insert(make_pair(VarId(0), 1));
  vvmap.insert(make_pair(VarId(1), 2));
  vvmap.insert(make_pair(VarId(2), 0));
  Zdd zdd3 = zddmgr.expr_to_zdd(expr1, vvmap);
  if ( !check_zddv(zddmgr, zdd3, "(0 & ~1 | 2 & 1)(0->1, 1->2, 2->0)",
		  "0, 1, 2|00100111") ) {
    return false;
  }

  VarVarMap vvmap2;
  vvmap2.insert(make_pair(VarId(0), 1));
  vvmap2.insert(make_pair(VarId(1), 0));
  Zdd zdd5 = zddmgr.expr_to_zdd(expr1, vvmap2);
  if ( !check_zddv(zddmgr, zdd5, "(0 & ~1 | 2 & 1)(0->1, 1->0)",
		  "0, 1, 2|00110101") ) {
    return false;
  }

  return true;
}

// support のテスト
bool
test_support(ZddMgr& zddmgr)
{
  if ( !check_support(zddmgr, "0") ) {
    return false;
  }
  if ( !check_support(zddmgr, "0 | 1") ) {
    return false;
  }
  if ( !check_support(zddmgr, "0 & 1 | ~0 & 3") ) {
    return false;
  }
  return true;
}

// minterm_count のテスト
bool
test_minterm_count(ZddMgr& zddmgr)
{
  Zdd zdd = str2zdd(zddmgr, "0 & 2 | 1 & 3 | ~1 & ~3");
  mpz_class mc = zdd.minterm_count(4);
  if ( mc != 10 ) {
    cout << "minterm_count(0 & 2 | 1 & 3 | ~1 & ~3) != 10" << endl;
    return false;
  }

  zdd = zddmgr.make_one();
  for (ymuint i = 0; i < 100; ++ i) {
    Zdd zdd1 = zddmgr.make_posiliteral(VarId(i));
    zdd &= zdd1;
  }
  mc = zdd.minterm_count(100);
  if ( mc != 1 ) {
    cout << "mc != 1" << endl;
    cout << "mc = " << mc.get_str() << endl;
    return false;
  }
  Zdd zdd2 = ~zdd;
  mpz_class mc2 = zdd2.minterm_count(100);
  if ( mc2 != (mpz_class(1) << 100) - mpz_class(1) ) {
    cout << "mc != 2^100 - 1" << endl;
    return false;
  }
  return true;
}

// dump/restore のテスト
bool
test_dump(ZddMgr& zddmgr)
{
  Zdd zdd = str2zdd(zddmgr, "0 & 2 | 1 & 3 | ~1 & ~3");
  const char* fn = "/tmp/magus_zdd_base_test";
  {
    ofstream ofs(fn);
    if ( !ofs ) {
      cout << "cannot open output file: " << fn << endl;
      return false;
    }
    BinOStream bos(ofs);
    zdd.dump(bos);
  }
  Zdd zdd2;
  {
    ifstream ifs(fn);
    if ( !ifs ) {
      cout << "cannont open input file: " << fn << endl;
      return false;
    }
    BinIStream bis(ifs);
    zdd2 = zddmgr.restore(bis);
  }
  if ( zdd != zdd2 ) {
    cout << "ERROR[test_dump]" << endl;
    zdd.display(cout);
    zdd2.display(cout);
    return false;
  }
  return true;
}

bool
test(ZddMgr& zddmgr)
{
  for (ymuint i = 0; i < 10; ++ i) {
    zddmgr.new_var(VarId(i));
  }
  return test_var_list(zddmgr, 10) &&
    test_const(zddmgr) &&
    test_unary(zddmgr) &&
    test_binary(zddmgr) &&
    test_make_zdd(zddmgr) &&
    test_cofactor(zddmgr) &&
    test_support(zddmgr) &&
    test_minterm_count(zddmgr) &&
    test_dump(zddmgr)
    ;
}

END_NAMESPACE_YM_BDD


int
main(int argc,
     char** argv)
{
  using namespace std;
  using nsYm::AssertError;
  using nsYm::ZddMgr;

  try {
    ZddMgr zddmgr1("bmc", "classic zddmgr");
    if ( !test(zddmgr1) ) {
      return 1;
    }
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }

  return 0;
}
