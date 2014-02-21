
/// @file base_test.cc
/// @brief CNFDD パッケージのテストその1 --- 基本関数 ---
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "logic/CNFdd.h"
#include "logic/CNFddMgr.h"

#include "cnfddtest.h"


BEGIN_NAMESPACE_YM_CNFDD

// 変数リストの検査
bool
test_var_list(CNFddMgr& cnfddmgr,
	      ymuint n)
{
  list<VarId> vlist;
  ymuint nn = cnfddmgr.var_list(vlist);
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
test_const(CNFddMgr& cnfddmgr)
{
  CNFdd one = cnfddmgr.make_base();
  if ( !one.is_one() ) {
    one.print_clause(cout);
    cout << "ERROR[const-1]: !one.is_one()" << endl;
    return false;
  }

  CNFdd zero = cnfddmgr.make_empty();
  if ( !zero.is_zero() ) {
    zero.print_clause(cout);
    cout << "ERROR[const-2]: !zero.is_zero()" << endl;
    return false;
  }

  CNFdd err = cnfddmgr.make_error();
  if ( !err.is_error() ) {
    err.print_clause(cout);
    cout << "ERROR[const-3]: !err.is_error()" << endl;
    return false;
  }

  CNFdd ovf = cnfddmgr.make_overflow();
  if ( !ovf.is_overflow() ) {
    ovf.print_clause(cout);
    cout << "ERROR[const-4]: !ovf.is_overflow()" << endl;
    return false;
  }

  return true;
}

// シングルトンの生成
bool
test_literal(CNFddMgr& cnfddmgr)
{
  CNFdd base = cnfddmgr.make_base();

  CNFdd var0 = base;
  var0.add_posiliteral(VarId(0));
  if ( !check_cnfdd(cnfddmgr, var0, "var0", "1") )
    return false;

  CNFdd var1 = base;
  var1.add_negaliteral(VarId(0));
  if ( !check_cnfdd(cnfddmgr, var0, "~var0", "1") )
    return false;

  CNFdd var2 = base;
  var2.add_posiliteral(VarId(2));
  if ( !check_cnfdd(cnfddmgr, var2, "var2", "4") )
    return false;

  CNFdd var4 = base;
  var4.add_negaliteral(VarId(4));
  if ( !check_cnfdd(cnfddmgr, var4, "~var4", "16") )
    return false;

  return true;
}

// 簡単な二項演算の検査
bool
test_binary(CNFddMgr& cnfddmgr)
{
  CNFdd base = cnfddmgr.make_base();
  CNFdd var0 = base; var0.add_posiliteral(VarId(0));
  CNFdd var1 = base; var1.add_posiliteral(VarId(1));
  CNFdd var2 = base; var2.add_posiliteral(VarId(2));
  CNFdd var3 = base; var3.add_posiliteral(VarId(3));

  CNFdd set1 = var0 | var1 | var2 | var3;

  CNFdd set2 = base;
  set2.add_posiliteral(VarId(0));
  set2.add_posiliteral(VarId(1));
  set2.add_posiliteral(VarId(2));
  set2.add_posiliteral(VarId(3));

  if ( !check_cnfdd(cnfddmgr, var0 | var1, "var0 | var1", "1, 2") )
    return false;

  if ( !check_cnfdd(cnfddmgr, set1, "set1", "1 2 4 8") )
    return false;

  if ( !check_cnfdd(cnfddmgr, set2, "set2", "15") )
    return false;

  if ( !check_cnfdd(cnfddmgr, var0 & set1, "var0 & set1", "1") )
    return false;

  if ( !check_cnfdd(cnfddmgr, set1 & set2, "set1 & set2", "") )
    return false;

  if ( !check_cnfdd(cnfddmgr, set1 - var2, "set1 - var2", "1 2 8") )
    return false;

  return true;
}

// cofactor のテスト
bool
test_cofactor(CNFddMgr& cnfddmgr)
{
  CNFdd base = cnfddmgr.make_base();
  CNFdd var0 = base; var0.add_posiliteral(VarId(0));
  CNFdd var1 = base; var1.add_posiliteral(VarId(1));
  CNFdd var2 = base; var2.add_posiliteral(VarId(2));
  CNFdd var3 = base; var3.add_posiliteral(VarId(3));

  CNFdd set1 = var0 | var1 | var2 | var3;

  if ( !check_cnfdd(cnfddmgr, var0.cofactor_0(VarId(0)), "var0.cofactor_0(0)", "") )
    return false;

  if ( !check_cnfdd(cnfddmgr, var0.cofactor_p(VarId(0)), "var0.cofactor_p(0)", "") )
    return false;

  if ( !check_cnfdd(cnfddmgr, var0.cofactor_0(VarId(1)), "var0.cofactor_0(1)", "") )
    return false;

  if ( !check_cnfdd(cnfddmgr, var0.cofactor_n(VarId(1)), "var0.cofactor_n(1)", "") )
    return false;

  if ( !check_cnfdd(cnfddmgr, set1.cofactor_0(VarId(0)), "set1.cofactor_0(0)", "") )
    return false;

  if ( !check_cnfdd(cnfddmgr, set1.cofactor_p(VarId(0)), "set1.cofactor_p(0)", "") )
    return false;

  if ( !check_cnfdd(cnfddmgr, base, "base", "") )
    return false;

  if ( !check_cnfdd(cnfddmgr, base.cofactor_0(VarId(0)), "base.cofactor_0(0)", "") )
    return false;

  if ( !check_cnfdd(cnfddmgr, base.cofactor_p(VarId(0)), "base.cofactor_p(0)", "") )
    return false;

  return true;
}

#if 0
// support のテスト
bool
test_support(CNFddMgr& cnfddmgr)
{
  if ( !check_support(cnfddmgr, "0") ) {
    return false;
  }
  if ( !check_support(cnfddmgr, "0 | 1") ) {
    return false;
  }
  if ( !check_support(cnfddmgr, "0 & 1 | ~0 & 3") ) {
    return false;
  }
  return true;
}

// minterm_count のテスト
bool
test_minterm_count(CNFddMgr& cnfddmgr)
{
  CNFdd cnfdd = str2cnfdd(cnfddmgr, "0 & 2 | 1 & 3 | ~1 & ~3");
  mpz_class mc = cnfdd.minterm_count(4);
  if ( mc != 10 ) {
    cout << "minterm_count(0 & 2 | 1 & 3 | ~1 & ~3) != 10" << endl;
    return false;
  }

  cnfdd = cnfddmgr.make_base();
  for (ymuint i = 0; i < 100; ++ i) {
    CNFdd cnfdd1 = cnfddmgr.make_posiliteral(VarId(i));
    cnfdd &= cnfdd1;
  }
  mc = cnfdd.minterm_count(100);
  if ( mc != 1 ) {
    cout << "mc != 1" << endl;
    cout << "mc = " << mc.get_str() << endl;
    return false;
  }
  CNFdd cnfdd2 = ~cnfdd;
  mpz_class mc2 = cnfdd2.minterm_count(100);
  if ( mc2 != (mpz_class(1) << 100) - mpz_class(1) ) {
    cout << "mc != 2^100 - 1" << endl;
    return false;
  }
  return true;
}

// dump/restore のテスト
bool
test_dump(CNFddMgr& cnfddmgr)
{
  CNFdd cnfdd = str2cnfdd(cnfddmgr, "0 & 2 | 1 & 3 | ~1 & ~3");
  const char* fn = "/tmp/magus_cnfdd_base_test";
  {
    ofstream ofs(fn);
    if ( !ofs ) {
      cout << "cannot open output file: " << fn << endl;
      return false;
    }
    ODOStream bos(ofs);
    cnfdd.dump(bos);
  }
  CNFdd cnfdd2;
  {
    ifstream ifs(fn);
    if ( !ifs ) {
      cout << "cannont open input file: " << fn << endl;
      return false;
    }
    IDOStream bis(ifs);
    cnfdd2 = cnfddmgr.restore(bis);
  }
  if ( cnfdd != cnfdd2 ) {
    cout << "ERROR[test_dump]" << endl;
    cnfdd.display(cout);
    cnfdd2.display(cout);
    return false;
  }
  return true;
}
#endif

bool
test(CNFddMgr& cnfddmgr)
{
  for (ymuint i = 0; i < 10; ++ i) {
    cnfddmgr.new_var(VarId(i));
  }
  return test_var_list(cnfddmgr, 10) &&
    test_const(cnfddmgr) &&
    test_binary(cnfddmgr) &&
    test_cofactor(cnfddmgr)
    ;
}

END_NAMESPACE_YM_CNFDD


int
main(int argc,
     char** argv)
{
  using namespace std;
  using nsYm::AssertError;
  using nsYm::CNFddMgr;

#if !defined(YM_DEBUG)
  try {
#endif
    CNFddMgr cnfddmgr1("cnfddmgr", "classic cnfddmgr");
    if ( !test(cnfddmgr1) ) {
      return 1;
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
