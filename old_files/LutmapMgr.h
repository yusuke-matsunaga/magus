﻿#ifndef LUTMAPMGR_H
#define LUTMAPMGR_H

#error "obsolete"

/// @file LutmapMgr.h
/// @brief LutmapMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.

#include "magus.h"
#include "ym/bnet.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class LutmapMgr LutmapMgr.h "LutmapMgr.h"
/// @brief LUT用のテクノロジマッパー
//////////////////////////////////////////////////////////////////////
class LutmapMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] lut_size LUTの入力数
  /// @param[in] option オプション文字列
  LutmapMgr(
    SizeType lut_size = 5,
    const string& option = string()
  );

  /// @brief デストラクタ
  ~LutmapMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // メンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief LUTの入力数を設定する
  /// @param[in] lut_size LUTの入力数
  void
  set_lut_size(
    SizeType lut_size
  )
  {
    mLutSize = lut_size;
  }

  /// @brief オプション文字列を設定する．
  /// @param[in] option オプション文字列
  void
  set_option(
    const string& option
  );

  /// @brief 面積最小化 DAG covering のヒューリスティック関数
  /// @param[in] src_network もとのネットワーク
  /// @param[out] dat_network マッピング結果
  BnNetwork
  area_map(
    const BnNetwork& src_network
  );

  /// @brief 段数最小化 DAG covering のヒューリスティック関数
  /// @param[in] src_network もとのネットワーク
  /// @param[in] slack 最小段数に対するスラック
  /// @param[out] dat_network マッピング結果
  BnNetwork
  delay_map(
    const BnNetwork& src_network,
    int slack
  );

  /// @brief 直前のマッピング結果のLUT数を返す．
  SizeType
  lut_num()
  {
    return mLutNum;
  }

  /// @brief 直前のマッピング結果の段数を返す．
  SizeType
  depth()
  {
    return mDepth;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // LUTの入力数
  SizeType mLutSize;

  // オプション文字列
  string mOption;

  // アルゴリズムを表す文字列
  string mAlgorithm;

  // ファンアウトモード
  bool mFanoutMode;

  // cut_resubstitution を行う時に true にするフラグ
  bool mDoCutResub;

  // 直前のマッピング結果のLUT数
  SizeType mLutNum;

  // 直前のマッピング結果の段数
  SizeType mDepth;

};

END_NAMESPACE_MAGUS

#endif // LUTMAPMGR_H
