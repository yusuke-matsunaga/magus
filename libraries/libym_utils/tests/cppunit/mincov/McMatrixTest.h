#ifndef MCMATRIXTEST_H
#define MCMATRIXTEST_H

/// @file McMatrixTest.h
/// @brief McMatrixTest のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "McMatrix.h"


BEGIN_NAMESPACE_YM_MINCOV

using namespace CppUnit;

//////////////////////////////////////////////////////////////////////
/// @class McMatrixTest McMatrixTest.h
/// @brief McMatrix のテストフィクスチャクラス
//////////////////////////////////////////////////////////////////////
class McMatrixTest :
  public TestFixture
{
  // テストスイートの登録
  CPPUNIT_TEST_SUITE( McMatrixTest );
  CPPUNIT_TEST( test_empty_matrix );
  CPPUNIT_TEST( test_clear );
  CPPUNIT_TEST( test_simple_matrix );
  CPPUNIT_TEST( test_delete_row );
  CPPUNIT_TEST( test_delete_col );
  CPPUNIT_TEST_SUITE_END();

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief setup 関数
  void
  setUp();

  /// @brief teardown 関数
  void
  tearDown();

  /// @brief 空の行列のテスト
  void
  test_empty_matrix();

  /// @brief clear のテスト
  void
  test_clear();

  /// @brief 簡単な行列のテスト
  void
  test_simple_matrix();

  /// @breif delete_row のテスト
  void
  test_delete_row();

  /// @brief delete_col のテスト
  void
  test_delete_col();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 本物と検証用の配列を用意する．
  /// @param[in] row_num 行数
  /// @param[in] col_num 列数
  void
  new_matrix(ymuint row_num,
	     ymuint col_num);

  /// @brief McMatrix の内容を一次元ベクタに変換する．
  void
  conv_matrix(const McMatrix& matrix,
	      vector<bool>& bitvector);


private:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  // 本物の配列
  McMatrix* mMatrix1;

  // 行数
  ymuint32 mRowNum;

  // 列数
  ymuint32 mColNum;

  // 検証用の配列
  // サイズは mRowNum * mColNum;
  vector<bool> mVerMatrix;

};

END_NAMESPACE_YM_MINCOV

#endif // MCMATRIXTEST_H
