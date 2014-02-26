#ifndef DTPGMODE_H
#define DTPGMODE_H

/// @file DtpgMode.h
/// @brief DtpgMode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgMode DtpgMode.h "DtpgMode.h"
/// @brief DTPG のモードを表すクラス
//////////////////////////////////////////////////////////////////////
class DtpgMode
{
public:

  /// @brief コンストラクタ
  explicit
  DtpgMode(tDtpgMode mode,
	   ymuint ffr2_limit = 0);

  /// @brief デストラクタ
  ~DtpgMode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief モードを返す．
  tDtpgMode
  mode() const;

  /// @brief FFR2 モードの時に制限値を返す．
  ymuint
  ffr2_limit() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  tDtpgMode mMode;

  ymuint32 mFfrLimit;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
DtpgMode::DtpgMode(tDtpgMode mode,
		   ymuint ffr2_limit) :
  mMode(mode),
  mFfrLimit(ffr2_limit)
{
}

// @brief デストラクタ
inline
DtpgMode::~DtpgMode()
{
}

// @brief モードを返す．
inline
tDtpgMode
DtpgMode::mode() const
{
  return mMode;
}

// @brief FFR2 モードの時に制限値を返す．
inline
ymuint
DtpgMode::ffr2_limit() const
{
  return mFfrLimit;
}

END_NAMESPACE_YM_SATPG

#endif // DTPGMODE_H
