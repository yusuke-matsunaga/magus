#ifndef IOINFO_H
#define IOINFO_H

/// @file IOInfo.h
/// @brief IOInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2017, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "sbj_nsdef.h"


BEGIN_NAMESPACE_SBJ

//////////////////////////////////////////////////////////////////////
/// @class IOInfo IOInfo.h "IOInfo.h"
/// @brief 入出力ノードに関係する情報を表すクラス
///
/// 実際のデータ構造は継承クラスが実装する．
//////////////////////////////////////////////////////////////////////
class IOInfo
{
public:

  /// @brief デストラクタ
  virtual
  ~IOInfo() = default;

  /// @brief ポートに関連付けられている時に true を返す．
  virtual
  bool
  is_port_type() const;

  /// @brief 関連付けられているポートを返す．
  ///
  /// is_port_type() == true の時のみ意味のある関数
  virtual
  SbjPort*
  port() const;

  /// @brief 関連付けられているポート上のビット位置を返す．
  ///
  /// is_port_type() == true の時のみ意味のある関数
  virtual
  SizeType
  port_bitpos() const;

  /// @brief DFFの入力の時 true を返す．
  virtual
  bool
  is_dff_input() const;

  /// @brief DFFの出力の時 true を返す．
  virtual
  bool
  is_dff_output() const;

  /// @brief DFFのクロック端子の時 true を返す．
  virtual
  bool
  is_dff_clock() const;

  /// @brief DFFのリセット端子の時 true を返す．
  virtual
  bool
  is_dff_clear() const;

  /// @brief DFFのセット端子の時 true を返す．
  virtual
  bool
  is_dff_preset() const;

  /// @brief 関連するDFFを返す．
  ///
  /// DFF関係のノードの時のみ意味のある関数．
  virtual
  SbjDff*
  dff() const;

  /// @brief ラッチの入力の時 true を返す．
  virtual
  bool
  is_latch_input() const;

  /// @brief ラッチの出力の時 truue を返す．
  virtual
  bool
  is_latch_output() const;

  /// @brief ラッチのイネーブル端子の時 true を返す．
  virtual
  bool
  is_latch_enable() const;

  /// @brief ラッチのクリア端子の時 true を返す．
  virtual
  bool
  is_latch_clear() const;

  /// @brief ラッチのセット端子の時 true を返す．
  virtual
  bool
  is_latch_preset() const;

  /// @brief 関連付けられているラッチを返す．
  ///
  /// ラッチ関係のノードの時のみ意味のある関数．
  virtual
  SbjLatch*
  latch() const;

};


//////////////////////////////////////////////////////////////////////
/// @class IOPortInfo IOInfo.h "IOInfo.h"
/// @brief ポートタイプの IOInfo
//////////////////////////////////////////////////////////////////////
class IOPortInfo :
public IOInfo
{
public:

  /// @brief コンストラクタ
  IOPortInfo(
    SbjPort* port, ///< [in] 関連付けられているポート
    SizeType bit   ///< [in] そのビット位置
  );

  /// @brief デストラクタ
  ~IOPortInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ポートに関連付けられている時に true を返す．
  bool
  is_port_type() const override;

  /// @brief 関連付けられているポートを返す．
  ///
  /// is_port_type() == true の時のみ意味のある関数
  SbjPort*
  port() const override;

  /// @brief 関連付けられているポート上のビット位置を返す．
  ///
  /// is_port_type() == true の時のみ意味のある関数
  SizeType
  port_bitpos() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポート
  SbjPort* mPort;

  // ビット位置
  SizeType mBit;

};


//////////////////////////////////////////////////////////////////////
/// @class IODffInputInfo IOInfo.h "IOInfo.h"
/// @brief DFF に関連付けられているクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class IODffInfo :
  public IOInfo
{
public:

  /// @brief コンストラクタ
  IODffInfo(
    SbjDff* dff ///< [in] 関連付けられているDFF
  );

  /// @brief デストラクタ
  ~IODffInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関連するDFFを返す．
  ///
  /// DFF関係のノードの時のみ意味のある関数．
  SbjDff*
  dff() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // DFF
  SbjDff* mDff;

};


//////////////////////////////////////////////////////////////////////
/// @class IODffInputInfo IOInfo.h "IOInfo.h"
/// @brief DFFの入力タイプの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class IODffInputInfo :
  public IODffInfo
{
public:

  /// @brief コンストラクタ
  IODffInputInfo(
    SbjDff* dff ///< [in] 関連付けられているDFF
  );

  /// @brief デストラクタ
  ~IODffInputInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief DFFの入力の時 true を返す．
  bool
  is_dff_input() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class IODffOUtputInfo IOInfo.h "IOInfo.h"
/// @brief DFFの出力タイプの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class IODffOutputInfo :
  public IODffInfo
{
public:

  /// @brief コンストラクタ
  IODffOutputInfo(
    SbjDff* dff ///< [in] 関連付けられているDFF
  );

  /// @brief デストラクタ
  ~IODffOutputInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief DFFの出力の時 true を返す．
  bool
  is_dff_output() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class IODffClockInfo IOInfo.h "IOInfo.h"
/// @brief DFFのクロック端子の情報を表すクラス
//////////////////////////////////////////////////////////////////////
class IODffClockInfo :
  public IODffInfo
{
public:

  /// @brief コンストラクタ
  IODffClockInfo(
    SbjDff* dff ///< [in] 関連付けられているDFF
  );

  /// @brief デストラクタ
  ~IODffClockInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief DFFのクロック端子の時 true を返す．
  bool
  is_dff_clock() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class IODffClearInfo IOInfo.h "IOInfo.h"
/// @brief DFFのクリア端子の情報を表すクラス
//////////////////////////////////////////////////////////////////////
class IODffClearInfo :
  public IODffInfo
{
public:

  /// @brief コンストラクタ
  IODffClearInfo(
    SbjDff* dff ///< [in] 関連付けられているDFF
  );

  /// @brief デストラクタ
  ~IODffClearInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief DFFのクリア端子の時 true を返す．
  bool
  is_dff_clear() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class IODffPresetInfo IOInfo.h "IOInfo.h"
/// @brief DFFのセット端子情報を表すクラス
//////////////////////////////////////////////////////////////////////
class IODffPresetInfo :
  public IODffInfo
{
public:

  /// @brief コンストラクタ
  IODffPresetInfo(
    SbjDff* dff ///< [in] 関連付けられているDFF
  );

  /// @brief デストラクタ
  ~IODffPresetInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief DFFのセット端子の時 true を返す．
  bool
  is_dff_preset() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class IOLatchInfo IOInfo.h "IOInfo.h"
/// @brief ラッチに関連付けられるタイプの基底クラス
//////////////////////////////////////////////////////////////////////
class IOLatchInfo :
  public IOInfo
{
public:

  /// @brief コンストラクタ
  IOLatchInfo(
    SbjLatch* latch ///< [in] 関連付けられているラッチ
  );

  /// @brief デストラクタ
  ~IOLatchInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関連付けられているラッチを返す．
  ///
  /// ラッチ関係のノードの時のみ意味のある関数．
  SbjLatch*
  latch() const override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ラッチ
  SbjLatch* mLatch;

};


//////////////////////////////////////////////////////////////////////
/// @class IOLatchInputInfo IOInfo.h "IOInfo.h"
/// @brief ラッチの入力タイプの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class IOLatchInputInfo :
  public IOLatchInfo
{
public:

  /// @brief コンストラクタ
  IOLatchInputInfo(
    SbjLatch* latch ///< [in] 関連付けられているラッチ
  );

  /// @brief デストラクタ
  ~IOLatchInputInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ラッチの入力の時 true を返す．
  bool
  is_latch_input() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class IOLatchOutputInfo IOInfo.h "IOInfo.h"
/// @brief ラッチの出力タイプの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class IOLatchOutputInfo :
  public IOLatchInfo
{
public:

  /// @brief コンストラクタ
  IOLatchOutputInfo(
    SbjLatch* latch ///< [in] 関連付けられているラッチ
  );

  /// @brief デストラクタ
  ~IOLatchOutputInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ラッチの出力の時 truue を返す．
  bool
  is_latch_output() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class IOLatchEnableInfo IOInfo.h "IOInfo.h"
/// @brief ラッチのイネーブル端子の情報を表すクラス
//////////////////////////////////////////////////////////////////////
class IOLatchEnableInfo :
  public IOLatchInfo
{
public:

  /// @brief コンストラクタ
  IOLatchEnableInfo(
    SbjLatch* latch ///< [in] 関連付けられているラッチ
  );

  /// @brief デストラクタ
  ~IOLatchEnableInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ラッチのイネーブル端子の時 true を返す．
  bool
  is_latch_enable() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class IOLatchClearInfo IOInfo.h "IOInfo.h"
/// @brief ラッチのクリア端子の情報を表すクラス
//////////////////////////////////////////////////////////////////////
class IOLatchClearInfo :
  public IOLatchInfo
{
public:

  /// @brief コンストラクタ
  IOLatchClearInfo(
    SbjLatch* latch ///< [in] 関連付けられているラッチ
  );

  /// @brief デストラクタ
  ~IOLatchClearInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ラッチのクリア端子の時 true を返す．
  bool
  is_latch_clear() const override;

};


//////////////////////////////////////////////////////////////////////
/// @class IOLatchPresetInfo IOInfo.h "IOInfo.h"
/// @brief ラッチのセット端子の情報を表すクラス
//////////////////////////////////////////////////////////////////////
class IOLatchPresetInfo :
  public IOLatchInfo
{
public:

  /// @brief コンストラクタ
  IOLatchPresetInfo(
    SbjLatch* latch ///< [in] 関連付けられているラッチ
  );

  /// @brief デストラクタ
  ~IOLatchPresetInfo() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ラッチのセット端子の時 true を返す．
  bool
  is_latch_preset() const override;

};

END_NAMESPACE_SBJ

#endif // IOINFO_H
