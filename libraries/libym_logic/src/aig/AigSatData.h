
/// @file AigSatData.h
/// @brief AigSatData のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AigNode.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class AigSatData AigSatData.h "AigSatDAta.h"
/// @brief AigNode に関する情報を保持するための構造体
//////////////////////////////////////////////////////////////////////
class AigSatData
{
public:

  /// @brief コンストラクタ
  AigSatData();

  /// @brief デストラクタ
  ~AigSatData();


public:

  /// @brief 変数番号を得る．
  ymuint
  varid() const;

  /// @brief 変数番号を設定する．
  void
  set_varid(ymuint varid);

  /// @brief TFI マークを得る．
  bool
  tfi_mark() const;

  /// @brief TFI マークを付ける．
  void
  set_tfi_mark();

  /// @brief TFI マークを消す．
  void
  clear_tfi_mark();

  /// @brief root マークを得る．
  bool
  root_mark() const;

  /// @brief root マークを付ける．
  void
  set_root_mark();

  /// @brief root マークを消す．
  void
  clear_root_mark();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // SAT 用の変数番号
  ymuint32 mVarId;

  // いくつかのフラグ
  ymuint32 mFlags;


private:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////

  static
  const int kTfiShift = 0;
  static
  const int kRootShift = 1;

  static
  const ymuint32 kTfiMask = (1U << kTfiShift);
  static
  const ymuint32 kRootMask = (1U << kRootShift);

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
AigSatData::AigSatData()
{
  mVarId = 0U;
  mFlags = 0U;
}

// @brief デストラクタ
inline
AigSatData::~AigSatData()
{
}

// @brief 変数番号を得る．
inline
ymuint
AigSatData::varid() const
{
  return mVarId;
}

// @brief 変数番号を設定する．
inline
void
AigSatData::set_varid(ymuint varid)
{
  mVarId = varid;
}

// @brief TFI マークを得る．
inline
bool
AigSatData::tfi_mark() const
{
  return static_cast<bool>((mFlags >> kTfiShift) & 1U);
}

// @brief TFI マークを付ける．
inline
void
AigSatData::set_tfi_mark()
{
  mFlags |= kTfiMask;
}

// @brief TFI マークを消す．
inline
void
AigSatData::clear_tfi_mark()
{
  mFlags &= ~kTfiMask;
}

// @brief root マークを得る．
inline
bool
AigSatData::root_mark() const
{
  return static_cast<bool>((mFlags >> kRootShift) & 1U);
}

// @brief root マークを付ける．
inline
void
AigSatData::set_root_mark()
{
  mFlags |= kRootMask;
}

// @brief root マークを消す．
inline
void
AigSatData::clear_root_mark()
{
  mFlags &= ~kRootMask;
}

END_NAMESPACE_YM_AIG
