#ifndef DTPGPRIMITIVE_H
#define DTPGPRIMITIVE_H

/// @file DtpgPrimitive.h
/// @brief DtpgPrimitive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgPrimitive DtpgPrimitive.h "DtpgPrimitive.h"
/// @brief DtpgNode の cplx_logic タイプの論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class DtpgPrimitive
{
  friend class DtpgNetwork;

private:

  /// @brief コンストラクタ
  DtpgPrimitive();

  /// @brief デストラクタ
  ~DtpgPrimitive();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力タイプの時 true を返す．
  bool
  is_input() const;

  /// @brief 外部入力タイプの時に入力番号を返す．
  ymuint
  input_id() const;

  /// @brief logic タイプの時 true を返す．
  bool
  is_logic() const;

  /// @brief ゲートタイプを得る．
  tTgGateType
  gate_type() const;

  /// @brief ファンイン数を得る．
  ymuint
  fanin_num() const;

  /// @brief ファンインを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
  DtpgPrimitive*
  fanin(ymuint pos) const;

  /// @brief 正常値を表す変数番号をセットする．
  void
  set_gvar(VarId gvar);

  /// @brief 故障値を表す変数番号をセットする．
  void
  set_fvar(VarId fvar);

  /// @brief 正常値を表す変数番号を得る．
  VarId
  gvar() const;

  /// @brief 故障値を表す変数番号を得る．
  VarId
  fvar() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // いくつかのデータをパックしたもの
  // - [0] ノードタイプ
  //   0: 外部入力
  //   1: 論理ノード
  // - [1:31] 入力ノードの場合の通し番号
  //          or ゲートタイプ
  ymuint32 mTypeId;

  // ファンイン数
  ymuint32 mFaninNum;

  // ファンインの配列
  DtpgPrimitive** mFanins;

  // 正常回路の変数番号
  VarId mGid;

  // 故障回路の変数番号
  VarId mFid;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
DtpgPrimitive::DtpgPrimitive()
{
}

// @brief デストラクタ
inline
DtpgPrimitive::~DtpgPrimitive()
{
}

// @brief 外部入力タイプの時 true を返す．
inline
bool
DtpgPrimitive::is_input() const
{
  return (mTypeId & 1U) == 0U;
}

// @brief 外部入力タイプの時に入力番号を返す．
inline
ymuint
DtpgPrimitive::input_id() const
{
  assert_cond( is_input(), __FILE__, __LINE__);
  return mTypeId >> 1;
}

// @brief logic タイプの時 true を返す．
inline
bool
DtpgPrimitive::is_logic() const
{
  return (mTypeId & 1U) == 1U;
}

// @brief ゲートタイプを得る．
inline
tTgGateType
DtpgPrimitive::gate_type() const
{
  return static_cast<tTgGateType>(mTypeId >> 1);
}

// @brief ファンイン数を得る．
inline
ymuint
DtpgPrimitive::fanin_num() const
{
  return mFaninNum;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
inline
DtpgPrimitive*
DtpgPrimitive::fanin(ymuint pos) const
{
  assert_cond( pos < fanin_num(), __FILE__, __LINE__);
  return mFanins[pos];
}

// @brief 正常値を表す変数番号をセットする．
inline
void
DtpgPrimitive::set_gvar(VarId gvar)
{
  mGid = gvar;
}

// @brief 故障値を表す変数番号をセットする．
inline
void
DtpgPrimitive::set_fvar(VarId fvar)
{
  mFid = fvar;
}

// @brief 正常値を表す変数番号を得る．
inline
VarId
DtpgPrimitive::gvar() const
{
  return mGid;
}

// @brief 故障値を表す変数番号を得る．
inline
VarId
DtpgPrimitive::fvar() const
{
  return mFid;
}

END_NAMESPACE_YM_SATPG_DTPG

#endif // DTPGPRIMITIVE_H
