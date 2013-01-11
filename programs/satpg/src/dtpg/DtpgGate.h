#ifndef DTPGGATE_H
#define DTPGGATE_H

/// @file DtpgGate.h
/// @brief DtpgGate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgGate DtpgGate.h "DtpgGate.h"
/// @brief TgNode に対応するゲートを表すクラス
///
/// 通常は DtpgNode と一致するが，TgNode が kTgGateCplx タイプだった
/// 場合に複数の DtpgNode が一つの TgNode と対応するためこのクラスが
/// 必要となる．
//////////////////////////////////////////////////////////////////////
class DtpgGate
{
  friend class DtpgNetwork;

private:

  /// @brief コンストラクタ
  DtpgGate();

  /// @brief デストラクタ
  ~DtpgGate();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 根(出力)のノードを返す．
  DtpgNode*
  root_node() const;

  /// @brief 入力数を返す．
  ymuint
  input_num();

  /// @brief 出力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  DtpgFault*
  output_fault(int val);

  /// @brief 入力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号 ( 0 <= pos < input_num() )
  DtpgFault*
  input_fault(int val,
	      ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  DtpgNode* mRoot;

  // 入力数
  ymuint32 mInputNum;

  // 出力の故障
  DtpgFault* mOutputFault[2];

  // 入力の故障の配列
  // サイズは　mInputNum * 2;
  DtpgFault** mInputFault;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
DtpgGate::DtpgGate()
{
}

// @brief デストラクタ
inline
DtpgGate::~DtpgGate()
{
}

// @brief 根(出力)のノードを返す．
inline
DtpgNode*
DtpgGate::root_node() const
{
  return mRoot;
}

// @brief 入力数を返す．
inline
ymuint
DtpgGate::input_num()
{
  return mInputNum;
}

// @brief 出力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
inline
DtpgFault*
DtpgGate::output_fault(int val)
{
  return mOutputFault[val & 1U];
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号 ( 0 <= pos < input_num() )
inline
DtpgFault*
DtpgGate::input_fault(int val,
		      ymuint pos)
{
  assert_cond( pos < input_num(), __FILE__, __LINE__);
  return mInputFault[pos * 2 + (val & 1U)];
}

END_NAMESPACE_YM_SATPG_DTPG

#endif // DTPGGATE_H
