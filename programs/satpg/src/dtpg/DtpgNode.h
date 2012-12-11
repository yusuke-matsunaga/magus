#ifndef DTPGNODE_H
#define DTPGNODE_H

/// @file DtpgNode.h
/// @brief DtpgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"
#include "ym_logic/VarId.h"
#include "ym_networks/TgNode.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

class DtpgFault;

//////////////////////////////////////////////////////////////////////
/// @class DtpgNode DtpgNode.h "DtpgNode.h"
/// @brief DtpgSat 用のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class DtpgNode
{
  friend class DtpgNetwork;

public:

  enum Mark {
    kNone,
    kTFO,
    kTFI
  };

public:

  /// @brief コンストラクタ
  DtpgNode();

  /// @brief デストラクタ
  ~DtpgNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief もとのノードを得る．
  const TgNode*
  tgnode() const;

  /// @brief タイプ id を得る．
  tTgGateType
  type() const;

  /// @brief 未定義タイプの時 true を返す．
  bool
  is_undef() const;

  /// @brief 外部入力タイプの時 true を返す．
  /// @note FF 出力もここに含まれる．
  bool
  is_input() const;

  /// @brief 外部出力タイプの時 true を返す．
  /// @note FF 入力もここに含まれる．
  bool
  is_output() const;

  /// @brief logic タイプの時 true を返す．
  bool
  is_logic() const;

  /// @brief 組み込み型でない logic タイプの時 true を返す．
  bool
  is_cplx_logic() const;

  /// @brief ID番号を得る．
  ymuint
  id() const;

  /// @brief 外部入力タイプの時に入力番号を返す．
  ymuint
  input_id() const;

  /// @brief 外部出力タイプの時に出力番号を返す．
  ymuint
  output_id() const;

  /// @brief ファンイン数を得る．
  ymuint
  fanin_num() const;

  /// @brief ファンインを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
  DtpgNode*
  fanin(ymuint pos) const;

  /// @brief ファンアウト数を得る．
  ymuint
  fanout_num() const;

  /// @brief ファンアウトを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < fanout_num() )
  DtpgNode*
  fanout(ymuint pos) const;

  /// @brief アクティブなファンアウト数を得る．
  ymuint
  active_fanout_num() const;

  /// @brief アクティブなファンアウトを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < active_fanout_num() )
  DtpgNode*
  active_fanout(ymuint pos) const;

  /// @brief 出力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  DtpgFault*
  output_fault(int val);

  /// @brief 入力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  DtpgFault*
  input_fault(int val,
	      ymuint pos);

  /// @brief マークを得る．
  Mark
  mark() const;

  /// @brief アクティブの場合 true を返す．
  bool
  is_active() const;

  /// @brief 正常回路の変数番号をセットする．
  void
  set_gvar(VarId gvar);

  /// @brief 故障回路の変数番号をセットする．
  void
  set_fvar(VarId fvar);

  /// @brief 故障差の変数番号をセットする．
  void
  set_dvar(VarId dvar);

  /// @brief 正常回路の変数番号を得る．
  VarId
  gvar() const;

  /// @brief 故障回路の変数番号を得る．
  VarId
  fvar() const;

  /// @brief 故障差の変数番号を得る．
  VarId
  dvar() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マークをつける．
  void
  set_mark(Mark mark);

  /// @brief アクティブにする．
  void
  set_active();

  /// @brief アクティブフラグを消す．
  void
  clear_active();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 元のノード
  const TgNode* mTgNode;

  // ID 番号
  ymuint32 mId;

  // ファンイン数
  ymuint32 mFaninNum;

  // ファンインの配列
  DtpgNode** mFanins;

  // ファンアウト数
  ymuint32 mFanoutNum;

  // ファンアウトの配列
  DtpgNode** mFanouts;

  // アクティブなファンアウト数
  ymuint32 mActFanoutNum;

  // アクティブなファンアウトの配列
  DtpgNode** mActFanouts;

  // 出力の故障
  DtpgFault* mOutputFault[2];

  // 入力の故障
  DtpgFault** mInputFault;

  // いくつかのマークを納めるビットベクタ
  ymuint32 mMarks;

  // 正常回路の変数番号
  VarId mGid;

  // 故障回路の変数番号
  VarId mFid;

  // 故障差の変数番号
  VarId mDid;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
DtpgNode::DtpgNode()
{
  // 実際の初期化は DtpgNetwork が行なう．
}

// @brief デストラクタ
inline
DtpgNode::~DtpgNode()
{
}

// @brief もとのノードを得る．
inline
const TgNode*
DtpgNode::tgnode() const
{
  return mTgNode;
}

// @brief タイプ id を得る．
inline
tTgGateType
DtpgNode::type() const
{
  return mTgNode->type();
}

// @brief 未定義タイプの時 true を返す．
inline
bool
DtpgNode::is_undef() const
{
  return mTgNode->is_undef();
}

// @brief 外部入力タイプの時 true を返す．
// @note FF 出力もここに含まれる．
inline
bool
DtpgNode::is_input() const
{
  return mTgNode->is_input();
}

// @brief 外部出力タイプの時 true を返す．
// @note FF 入力もここに含まれる．
inline
bool
DtpgNode::is_output() const
{
  return mTgNode->is_output();
}

// @brief logic タイプの時 true を返す．
inline
bool
DtpgNode::is_logic() const
{
  return mTgNode->is_logic();
}

// @brief 組み込み型でない logic タイプの時 true を返す．
inline
bool
DtpgNode::is_cplx_logic() const
{
  return mTgNode->is_cplx_logic();
}

// @brief ID番号を得る．
inline
ymuint
DtpgNode::id() const
{
  return mId;
}

// @brief 外部入力タイプの時に入力番号を返す．
inline
ymuint
DtpgNode::input_id() const
{
  return mTgNode->lid();
}

// @brief 外部出力タイプの時に出力番号を返す．
inline
ymuint
DtpgNode::output_id() const
{
  return mTgNode->lid();
}

// @brief ファンイン数を得る．
inline
ymuint
DtpgNode::fanin_num() const
{
  return mFaninNum;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
inline
DtpgNode*
DtpgNode::fanin(ymuint pos) const
{
  assert_cond( pos < mFaninNum, __FILE__, __LINE__);
  return mFanins[pos];
}

// @brief ファンアウト数を得る．
inline
ymuint
DtpgNode::fanout_num() const
{
  return mFanoutNum;
}

// @brief ファンアウトを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanout_num() )
inline
DtpgNode*
DtpgNode::fanout(ymuint pos) const
{
  assert_cond( pos < mFanoutNum, __FILE__, __LINE__);
  return mFanouts[pos];
}

// @brief アクティブなファンアウト数を得る．
inline
ymuint
DtpgNode::active_fanout_num() const
{
  return mActFanoutNum;
}

// @brief アクティブなファンアウトを得る．
// @param[in] pos 位置番号 ( 0 <= pos < active_fanout_num() )
inline
DtpgNode*
DtpgNode::active_fanout(ymuint pos) const
{
  assert_cond( pos < mActFanoutNum, __FILE__, __LINE__);
  return mActFanouts[pos];
}

// @brief 出力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
inline
DtpgFault*
DtpgNode::output_fault(int val)
{
  return mOutputFault[val % 2];
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
inline
DtpgFault*
DtpgNode::input_fault(int val,
		      ymuint pos)
{
  assert_cond( pos < mFaninNum, __FILE__, __LINE__);
  return mInputFault[pos * 2 + (val % 2)];
}

// @brief マークを得る．
inline
DtpgNode::Mark
DtpgNode::mark() const
{
  return static_cast<Mark>(mMarks >> 1);
}

// @brief マークをつける．
inline
void
DtpgNode::set_mark(Mark mark)
{
  mMarks &= 1U;
  mMarks |= (mark << 1);
}

// @brief アクティブの場合 true を返す．
inline
bool
DtpgNode::is_active() const
{
  return static_cast<bool>(mMarks & 1U);
}

// @brief アクティブにする．
inline
void
DtpgNode::set_active()
{
  mMarks |= 1U;
}

// @brief アクティブフラグを消す．
inline
void
DtpgNode::clear_active()
{
  mMarks &= ~1U;
}

// @brief 正常回路の変数番号をセットする．
inline
void
DtpgNode::set_gvar(VarId gvar)
{
  mGid = gvar;
}

// @brief 故障回路の変数番号をセットする．
inline
void
DtpgNode::set_fvar(VarId fvar)
{
  mFid = fvar;
}

// @brief 故障差の変数番号をセットする．
inline
void
DtpgNode::set_dvar(VarId dvar)
{
  mDid = dvar;
}

// @brief 正常回路の変数番号
inline
VarId
DtpgNode::gvar() const
{
  return mGid;
}

// @brief 故障回路の変数番号
inline
VarId
DtpgNode::fvar() const
{
  return mFid;
}

// @brief 故障差の変数番号
inline
VarId
DtpgNode::dvar() const
{
  return mDid;
}

END_NAMESPACE_YM_SATPG_DTPG

#endif // DTPGNODE_H
