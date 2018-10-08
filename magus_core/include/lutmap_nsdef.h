#ifndef LUTMAP_NSDEF_H
#define LUTMAP_NSDEF_H

/// @file lutmap_nsdef.h
/// @brief luttmap パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "magus.h"
#include "ym/bnet.h"


/// @brief lutmap 用の名前空間の開始
#define BEGIN_NAMESPACE_LUTMAP \
BEGIN_NAMESPACE_MAGUS \
BEGIN_NAMESPACE(nsLutmap)

/// @brief lutmap 用の名前空間の終了
#define END_NAMESPACE_LUTMAP \
END_NAMESPACE(nsLutmap) \
END_NAMESPACE_MAGUS


BEGIN_NAMESPACE_LUTMAP

/// @brief DAG covering のヒューリスティック関数
/// @param[in] src_network もとのネットワーク
/// @param[in] limit カットサイズ
/// @param[in] mode モードを表す文字列
/// @param[out] dat_network マッピング結果
/// @param[out] lut_num LUT数
/// @param[out] depth 段数
extern
void
lutmap(const BnNetwork& src_network,
       int limit,
       const string& mode,
       BnNetwork& dst_network,
       int& lut_num,
       int& depth);

END_NAMESPACE_LUTMAP

BEGIN_NAMESPACE_MAGUS

using nsLutmap::lutmap;

END_NAMESPACE_MAGUS

#endif // LUTMAP_NSDEF_H
