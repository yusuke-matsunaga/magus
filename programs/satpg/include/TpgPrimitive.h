#ifndef TPGPRIMITIVE_H
#define TPGPRIMITIVE_H

/// @file TpgPrimitive.h
/// @brief TpgPrimitive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "networks/tgnet.h"
#include "logic/Literal.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class TpgPrimitive TpgPrimitive.h "TpgPrimitive.h"
/// @brief TpgNode の cplx_logic タイプの論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class TpgPrimitive
{
  friend class TpgNetwork;

public:

  /// @brief コンストラクタ
  TpgPrimitive();

  /// @brief デストラクタ
  ~TpgPrimitive();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード内で有効なID番号を返す．
  ymuint
  subid() const;

  /// @brief 外部入力タイプの時 true を返す．
  bool
  is_input() const;

  /// @brief 否定つき外部入力タイプの時 true を返す．
  bool
  is_not_input() const;

  /// @brief 外部入力タイプ/否定付き外部入力タイプの時に入力番号を返す．
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
  TpgPrimitive*
  fanin(ymuint pos) const;

  /// @brief 正常値を表すリテラルをセットする．
  void
  set_glit(Literal lit);

  /// @brief 故障値を表すリテラルをセットする．
  void
  set_flit(Literal lit);

  /// @brief 正常値を表すリテラルを得る．
  Literal
  glit() const;

  /// @brief 故障値を表すリテラルを得る．
  Literal
  flit() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード内のID番号
  ymuint32 mSubId;

  // いくつかのデータをパックしたもの
  // - [0:1] ノードタイプ
  //   0: 外部入力
  //   1: 否定付き外部入力
  //   2: 論理ノード
  // - [2:31] 入力ノードの場合の通し番号
  //          or ゲートタイプ
  ymuint32 mTypeId;

  // ファンイン数
  ymuint32 mFaninNum;

  // ファンインの配列
  TpgPrimitive** mFanins;

  // 正常回路のリテラル
  Literal mGlit;

  // 故障回路のリテラル
  Literal mFlit;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
TpgPrimitive::TpgPrimitive()
{
}

// @brief デストラクタ
inline
TpgPrimitive::~TpgPrimitive()
{
}

// @brief ノード内で有効なID番号を返す．
inline
ymuint
TpgPrimitive::subid() const
{
  return mSubId;
}

// @brief 外部入力タイプの時 true を返す．
inline
bool
TpgPrimitive::is_input() const
{
  return (mTypeId & 3U) == 0U;
}

// @brief 否定付き外部入力タイプの時 true を返す．
inline
bool
TpgPrimitive::is_not_input() const
{
  return (mTypeId & 3U) == 1U;
}

// @brief 外部入力タイプの時に入力番号を返す．
inline
ymuint
TpgPrimitive::input_id() const
{
  assert_cond( is_input() || is_not_input(), __FILE__, __LINE__);
  return mTypeId >> 2;
}

// @brief logic タイプの時 true を返す．
inline
bool
TpgPrimitive::is_logic() const
{
  return (mTypeId & 3U) == 2U;
}

// @brief ゲートタイプを得る．
inline
tTgGateType
TpgPrimitive::gate_type() const
{
  return static_cast<tTgGateType>(mTypeId >> 2);
}

// @brief ファンイン数を得る．
inline
ymuint
TpgPrimitive::fanin_num() const
{
  return mFaninNum;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
inline
TpgPrimitive*
TpgPrimitive::fanin(ymuint pos) const
{
  assert_cond( pos < fanin_num(), __FILE__, __LINE__);
  return mFanins[pos];
}

// @brief 正常値を表すリテラルをセットする．
inline
void
TpgPrimitive::set_glit(Literal lit)
{
  mGlit = lit;
}

// @brief 故障値を表すリテラルをセットする．
inline
void
TpgPrimitive::set_flit(Literal lit)
{
  mFlit = lit;
}

// @brief 正常値を表すリテラルを得る．
inline
Literal
TpgPrimitive::glit() const
{
  return mGlit;
}

// @brief 故障値を表すリテラルを得る．
inline
Literal
TpgPrimitive::flit() const
{
  return mFlit;
}

END_NAMESPACE_YM_SATPG

#endif // TPGPRIMITIVE_H
