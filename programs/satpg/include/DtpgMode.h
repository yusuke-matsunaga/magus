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
	   ymuint val = 0);

  /// @brief デストラクタ
  ~DtpgMode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief モードを返す．
  tDtpgMode
  mode() const;

  /// @brief 値を返す．
  ymuint
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  tDtpgMode mMode;

  ymuint32 mVal;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
DtpgMode::DtpgMode(tDtpgMode mode,
		   ymuint val) :
  mMode(mode),
  mVal(val)
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

// @brief 値を返す．
inline
ymuint
DtpgMode::val() const
{
  return mVal;
}

END_NAMESPACE_YM_SATPG

#endif // DTPGMODE_H
