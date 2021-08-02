#ifndef LUTMAPMGR_H
#define LUTMAPMGR_H

/// @file LutmapMgr.h
/// @brief LutmapMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2018, 2021 Yusuke Matsunaga
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
  LutmapMgr(
    SizeType lut_size = 5,          ///< [in] LUTの入力数
    const string& option = string() ///< [in] オプション文字列
  );

  /// @brief デストラクタ
  ~LutmapMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // メンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief LUTの入力数を設定する
  void
  set_lut_size(
    SizeType lut_size ///< [in] LUTの入力数
  )
  {
    mLutSize = lut_size;
  }

  /// @brief オプション文字列を設定する．
  void
  set_option(
    const string& option ///< [in] オプション文字列
  );

  /// @brief 面積最小化 DAG covering のヒューリスティック関数
  void
  area_map(
    const BnNetwork& src_network, ///< [in] もとのネットワーク
    BnNetwork& dst_network        ///< [out] マッピング結果
  );

  /// @brief 段数最小化 DAG covering のヒューリスティック関数
  void
  delay_map(
    const BnNetwork& src_network, ///< [in] もとのネットワーク
    int slack,                    ///< [in] 最小段数に対するスラック
    BnNetwork& dst_network        ///< [out] マッピング結果
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
