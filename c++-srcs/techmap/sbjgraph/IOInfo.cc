
/// @file IOInfo.cc
/// @brief IOInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "IOInfo.h"


BEGIN_NAMESPACE_SBJ

//////////////////////////////////////////////////////////////////////
// クラス IOInfo
//////////////////////////////////////////////////////////////////////

// @brief デストラクタ
IOInfo::~IOInfo()
{
}

// @brief ポートに関連付けられている時に true を返す．
bool
IOInfo::is_port_type() const
{
  return false;
}

// @brief 関連付けられているポートを返す．
//
// is_port_type() == true の時のみ意味のある関数
SbjPort*
IOInfo::port() const
{
  return nullptr;
}

// @brief 関連付けられているポート上のビット位置を返す．
//
// is_port_type() == true の時のみ意味のある関数
int
IOInfo::port_bitpos() const
{
  return 0;
}

// @brief DFFの入力の時 true を返す．
bool
IOInfo::is_dff_input() const
{
  return false;
}

// @brief DFFの出力の時 true を返す．
bool
IOInfo::is_dff_output() const
{
  return false;
}

// @brief DFFのクロック端子の時 true を返す．
bool
IOInfo::is_dff_clock() const
{
  return false;
}

// @brief DFFのリセット端子の時 true を返す．
bool
IOInfo::is_dff_clear() const
{
  return false;
}

// @brief DFFのセット端子の時 true を返す．
bool
IOInfo::is_dff_preset() const
{
  return false;
}

// @brief 関連するDFFを返す．
//
// DFF関係のノードの時のみ意味のある関数．
SbjDff*
IOInfo::dff() const
{
  return nullptr;
}

// @brief ラッチの入力の時 true を返す．
bool
IOInfo::is_latch_input() const
{
  return false;
}

// @brief ラッチの出力の時 truue を返す．
bool
IOInfo::is_latch_output() const
{
  return false;
}

// @brief ラッチのイネーブル端子の時 true を返す．
bool
IOInfo::is_latch_enable() const
{
  return false;
}

// @brief ラッチのクリア端子の時 true を返す．
bool
IOInfo::is_latch_clear() const
{
  return false;
}

// @brief ラッチのセット端子の時 true を返す．
bool
IOInfo::is_latch_preset() const
{
  return false;
}

// @brief 関連付けられているラッチを返す．
//
// ラッチ関係のノードの時のみ意味のある関数．
SbjLatch*
IOInfo::latch() const
{
  return nullptr;
}


//////////////////////////////////////////////////////////////////////
// クラス IOPortInfo
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] port 関連付けられているポート
// @param[in] bit そのビット位置
IOPortInfo::IOPortInfo(SbjPort* port,
		       int bit) :
  mPort(port),
  mBit(bit)
{
}

// @brief デストラクタ
IOPortInfo::~IOPortInfo()
{
}

// @brief ポートに関連付けられている時に true を返す．
bool
IOPortInfo::is_port_type() const
{
  return true;
}

// @brief 関連付けられているポートを返す．
//
// is_port_type() == true の時のみ意味のある関数
SbjPort*
IOPortInfo::port() const
{
  return mPort;
}

// @brief 関連付けられているポート上のビット位置を返す．
//
// is_port_type() == true の時のみ意味のある関数
int
IOPortInfo::port_bitpos() const
{
  return mBit;
}


//////////////////////////////////////////////////////////////////////
// クラス IODffInfo
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] dff 関連付けられているDFF
IODffInfo::IODffInfo(SbjDff* dff) :
  mDff(dff)
{
}

// @brief デストラクタ
IODffInfo::~IODffInfo()
{
}

// @brief 関連するDFFを返す．
//
// DFF関係のノードの時のみ意味のある関数．
SbjDff*
IODffInfo::dff() const
{
  return mDff;
}


//////////////////////////////////////////////////////////////////////
// クラス IODffInputInfo
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] dff 関連付けられているDFF
IODffInputInfo::IODffInputInfo(SbjDff* dff) :
  IODffInfo(dff)
{
}

// @brief デストラクタ
IODffInputInfo::~IODffInputInfo()
{
}

// @brief DFFの入力の時 true を返す．
bool
IODffInputInfo::is_dff_input() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス IODffOutputInfo
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] dff 関連付けられているDFF
IODffOutputInfo::IODffOutputInfo(SbjDff* dff) :
  IODffInfo(dff)
{
}

// @brief デストラクタ
IODffOutputInfo::~IODffOutputInfo()
{
}

// @brief DFFの出力の時 true を返す．
bool
IODffOutputInfo::is_dff_output() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス IODffClockInfo
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] dff 関連付けられているDFF
IODffClockInfo::IODffClockInfo(SbjDff* dff) :
  IODffInfo(dff)
{
}

// @brief デストラクタ
IODffClockInfo::~IODffClockInfo()
{
}

// @brief DFFのクロック端子の時 true を返す．
bool
IODffClockInfo::is_dff_clock() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス IODffClearInfo
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] dff 関連付けられているDFF
IODffClearInfo::IODffClearInfo(SbjDff* dff) :
  IODffInfo(dff)
{
}

// @brief デストラクタ
IODffClearInfo::~IODffClearInfo()
{
}

// @brief DFFのクリア端子の時 true を返す．
bool
IODffClearInfo::is_dff_clear() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス IODffPresetInfo
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] dff 関連付けられているDFF
IODffPresetInfo::IODffPresetInfo(SbjDff* dff) :
  IODffInfo(dff)
{
}

// @brief デストラクタ
IODffPresetInfo::~IODffPresetInfo()
{
}

// @brief DFFのセット端子の時 true を返す．
bool
IODffPresetInfo::is_dff_preset() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス IOLatchInfo
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] latch 関連付けられているラッチ
IOLatchInfo::IOLatchInfo(SbjLatch* latch) :
  mLatch(latch)
{
}

// @brief デストラクタ
IOLatchInfo::~IOLatchInfo()
{
}

// @brief 関連付けられているラッチを返す．
//
// ラッチ関係のノードの時のみ意味のある関数．
SbjLatch*
IOLatchInfo::latch() const
{
  return mLatch;
}


//////////////////////////////////////////////////////////////////////
// クラス IOLatchInputInfo
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] latch 関連付けられているラッチ
IOLatchInputInfo::IOLatchInputInfo(SbjLatch* latch) :
  IOLatchInfo(latch)
{
}

// @brief デストラクタ
IOLatchInputInfo::~IOLatchInputInfo()
{
}

// @brief ラッチの入力の時 true を返す．
bool
IOLatchInputInfo::is_latch_input() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス IOLatchOutputInfo
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] latch 関連付けられているラッチ
IOLatchOutputInfo::IOLatchOutputInfo(SbjLatch* latch) :
  IOLatchInfo(latch)
{
}

// @brief デストラクタ
IOLatchOutputInfo::~IOLatchOutputInfo()
{
}

// @brief ラッチの出力の時 truue を返す．
bool
IOLatchOutputInfo::is_latch_output() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス IOLatchEnableInfo
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] latch 関連付けられているラッチ
IOLatchEnableInfo::IOLatchEnableInfo(SbjLatch* latch) :
  IOLatchInfo(latch)
{
}

// @brief デストラクタ
IOLatchEnableInfo::~IOLatchEnableInfo()
{
}

// @brief ラッチのイネーブル端子の時 true を返す．
bool
IOLatchEnableInfo::is_latch_enable() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス IOLatchClearInfo
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] latch 関連付けられているラッチ
IOLatchClearInfo::IOLatchClearInfo(SbjLatch* latch) :
  IOLatchInfo(latch)
{
}

// @brief デストラクタ
IOLatchClearInfo:: ~IOLatchClearInfo()
{
}

// @brief ラッチのクリア端子の時 true を返す．
bool
IOLatchClearInfo::is_latch_clear() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス IOLatchPresetInfo
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] latch 関連付けられているラッチ
IOLatchPresetInfo::IOLatchPresetInfo(SbjLatch* latch) :
  IOLatchInfo(latch)
{
}

// @brief デストラクタ
IOLatchPresetInfo::~IOLatchPresetInfo()
{
}

// @brief ラッチのセット端子の時 true を返す．
bool
IOLatchPresetInfo::is_latch_preset() const
{
  return true;
}

END_NAMESPACE_SBJ
