#ifndef NLSCANNER_H
#define NLSCANNER_H

/// @file NlScanner.h
/// @brief NlScanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include "YmUtils/Scanner.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlScanner NlScanner.h "NlScanner.h"
/// @brief number link の問題ファイル用の字句解析器
//////////////////////////////////////////////////////////////////////
class NlScanner :
  public Scanner
{
public:

  /// @brief コンストラクタ
  /// @param[in] ido 入力データ
  NlScanner(IDO& ido);

  /// @brief デストラクタ
  ~NlScanner();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief SIZE 行を読み込む．
  /// @param[out] width 読み込んだ幅を入れる変数
  /// @param[out] height 読み込んだ高さを入れる変数
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read_SIZE(ymuint& width,
	    ymuint& height);

  /// @brief LINE_NUM 行を読み込む．
  /// @param[out] num 読み込んだ要素数を入れる変数
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read_LINE_NUM(ymuint& num);

  /// @brief LINE行を読み込む．
  /// @param[out] x1, y1 始点の座標を入れる変数
  /// @param[out] x2, y2 終点の座標を入れる変数
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read_LINE(ymuint& x1,
	    ymuint& y1,
	    ymuint& x2,
	    ymuint& y2);

  /// @brief 解の一行を読み込む．
  /// @param[in] width 幅
  /// @param[out] num_array 番号の配列(サイズは width)
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read_SOLUTION(ymuint width,
		vector<ymuint>& num_array);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 数字を読み込む．
  /// @param[out] num 読み込んだ値を入れる変数
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  expect_NUMBER(ymuint& num);

  /// @brief 指定された文字列を読み込む．
  /// @param[in] str 文字列
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  expect_STRING(const char* str);

  /// @brief '(' を読み込む．
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  expect_LP();

  /// @brief ')' を読み込む．
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  expect_RP();

  /// @brief ',' を読み込む．
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  expect_COMMA();

  /// @brief '-' を読み込む．
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  expect_DASH();

  /// @brief 改行を読み込む．
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  expect_NL();

  /// @brief 空白を読み飛ばす．
  void
  skip_SPACE();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};

END_NAMESPACE_YM_NLINK


#endif // NLSCANNER_H
