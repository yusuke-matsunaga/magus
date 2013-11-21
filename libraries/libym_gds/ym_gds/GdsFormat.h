#ifndef YM_GDS_GDSFORMAT_H
#define YM_GDS_GDSFORMAT_H

/// @file ym_gds/GdsFormat.h
/// @brief GdsFormat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/gds_nsdef.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsFormat GdsFormat.h "ym_gds/GdsFormat.h"
/// @brief FORMAT を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsFormat
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  /// @param[in] type フォーマットタイプ ( 0 - 3 )
  GdsFormat(ymint16 type);

  /// @brief デストラクタ
  ~GdsFormat();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief フォーマットタイプを返す．
  int
  type() const;

  /// @brief マスク数を返す．
  ymuint
  mask_num() const;

  /// @brief マスクを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < mask_num() )
  const char*
  mask(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // フォーマットタイプ
  ymuint8 mType;

  // マスク数
  ymuint32 mMaskNum;

  // マスクの配列
  GdsString* mMasks[1];

};

END_NAMESPACE_YM_GDS

#endif // YM_GDS_GDSFORMAT_H
