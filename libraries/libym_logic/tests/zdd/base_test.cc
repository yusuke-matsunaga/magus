
/// @file base_test.cc
/// @brief ZDD パッケージのテストその1 --- 基本関数 ---
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/Zdd.h"
#include "YmLogic/ZddMgr.h"

#include "zddtest.h"

#include "CombSet.h"


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
  Zdd one = zddmgr.make_base();
  if ( !one.is_one() ) {
    one.print_set(cout);
    cout << "ERROR[const-1]: !one.is_one()" << endl;
    return false;
  }

  Zdd zero = zddmgr.make_empty();
  if ( !zero.is_zero() ) {
    zero.print_set(cout);
    cout << "ERROR[const-2]: !zero.is_zero()" << endl;
    return false;
  }

  Zdd err = zddmgr.make_error();
  if ( !err.is_error() ) {
    err.print_set(cout);
    cout << "ERROR[const-3]: !err.is_error()" << endl;
    return false;
  }

  Zdd ovf = zddmgr.make_overflow();
  if ( !ovf.is_overflow() ) {
    ovf.print_set(cout);
    cout << "ERROR[const-4]: !ovf.is_overflow()" << endl;
    return false;
  }

  return true;
}

// シングルトンの生成
bool
test_literal(ZddMgr& zddmgr)
{
  Zdd base = zddmgr.make_base();
  Zdd var0 = base;
  var0.swap(VarId(0));
  if ( !check_zdd(zddmgr, var0, "var0", "1") )
    return false;

  Zdd var2 = base;
  var2.swap(VarId(2));
  if ( !check_zdd(zddmgr, var2, "var2", "4") )
    return false;

  Zdd var4 = base;
  var4.swap(VarId(4));
  if ( !check_zdd(zddmgr, var4, "var4", "16") )
    return false;

  return true;
}

// 簡単な二項演算の検査
bool
test_binary(ZddMgr& zddmgr)
{
  Zdd base = zddmgr.make_base();
  Zdd var0 = base; var0.swap(VarId(0));
  Zdd var1 = base; var1.swap(VarId(1));
  Zdd var2 = base; var2.swap(VarId(2));
  Zdd var3 = base; var3.swap(VarId(3));

  Zdd set1 = var0 | var1 | var2 | var3;

  Zdd set2 = base;
  set2.swap(VarId(0));
  set2.swap(VarId(1));
  set2.swap(VarId(2));
  set2.swap(VarId(3));

  if ( !check_zdd(zddmgr, var0 | var1, "var0 | var1", "1, 2") )
    return false;

  if ( !check_zdd(zddmgr, set1, "set1", "1 2 4 8") )
    return false;

  if ( !check_zdd(zddmgr, set2, "set2", "15") )
    return false;

  if ( !check_zdd(zddmgr, var0 & set1, "var0 & set1", "1") )
    return false;

  if ( !check_zdd(zddmgr, set1 & set2, "set1 & set2", "") )
    return false;

  if ( !check_zdd(zddmgr, set1 - var2, "set1 - var2", "1 2 8") )
    return false;

  return true;
}

// cofactor のテスト
bool
test_cofactor(ZddMgr& zddmgr)
{
  Zdd base = zddmgr.make_base();
  Zdd var0 = base; var0.swap(VarId(0));
  Zdd var1 = base; var1.swap(VarId(1));
  Zdd var2 = base; var2.swap(VarId(2));
  Zdd var3 = base; var3.swap(VarId(3));

  Zdd set1 = var0 | var1 | var2 | var3;

  if ( !check_zdd(zddmgr, var0.cofactor0(VarId(0)), "var0.cofactor0(0)", "") )
    return false;

  if ( !check_zdd(zddmgr, var0.cofactor1(VarId(0)), "var0.cofactor1(0)", "") )
    return false;

  if ( !check_zdd(zddmgr, var0.cofactor0(VarId(1)), "var0.cofactor0(1)", "") )
    return false;

  if ( !check_zdd(zddmgr, var0.cofactor1(VarId(1)), "var0.cofactor1(1)", "") )
    return false;

  if ( !check_zdd(zddmgr, set1.cofactor0(VarId(0)), "set1.cofactor0(0)", "") )
    return false;

  if ( !check_zdd(zddmgr, set1.cofactor1(VarId(0)), "set1.cofactor1(0)", "") )
    return false;

  if ( !check_zdd(zddmgr, base, "base", "") )
    return false;

  if ( !check_zdd(zddmgr, base.cofactor0(VarId(0)), "base.cofactor0(0)", "") )
    return false;

  if ( !check_zdd(zddmgr, base.cofactor1(VarId(0)), "base.cofactor1(0)", "") )
    return false;

  return true;
}

#if 0
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

  zdd = zddmgr.make_base();
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
    ODOStream bos(ofs);
    zdd.dump(bos);
  }
  Zdd zdd2;
  {
    ifstream ifs(fn);
    if ( !ifs ) {
      cout << "cannont open input file: " << fn << endl;
      return false;
    }
    IDOStream bis(ifs);
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
#endif

bool
test(ZddMgr& zddmgr)
{
  for (ymuint i = 0; i < 10; ++ i) {
    zddmgr.new_var(VarId(i));
  }
  return test_var_list(zddmgr, 10) &&
    test_const(zddmgr) &&
    test_binary(zddmgr) &&
    test_cofactor(zddmgr)
    ;
}

END_NAMESPACE_YM_ZDD


int
main(int argc,
     char** argv)
{
  using namespace std;
  using nsYm::AssertError;
  using nsYm::ZddMgr;

#if !defined(YM_DEBUG)
  try {
#endif
    ZddMgr zddmgr1("zddmgr", "classic zddmgr");
    if ( !test(zddmgr1) ) {
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
