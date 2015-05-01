#ifndef TPGNODE_H
#define TPGNODE_H

/// @file TpgNode.h
/// @brief TpgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "Val3.h"
#include "YmNetworks/tgnet.h"
#include "YmLogic/VarId.h"


BEGIN_NAMESPACE_YM_SATPG

class TpgMap;

//////////////////////////////////////////////////////////////////////
/// @class TpgNode TpgNode.h "TpgNode.h"
/// @brief SATPG 用のノードを表すクラス
///
/// 基本的には TgNetwork の TgNode に対応しているが，
/// もとの TgNode が組み込み型でない場合には複数の TpgNode を組み合わ
/// せてもとの TgNode を表す．
/// そのため，場合によってはファンインの故障を表すための仮想的な
/// ノードを挿入する場合もある．
//////////////////////////////////////////////////////////////////////
class TpgNode
{
  friend class TpgNetwork;

public:

  /// @brief コンストラクタ
  TpgNode();

  /// @brief デストラクタ
  ~TpgNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 構造情報を得る関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を得る．
  ymuint
  id() const;

  /// @brief 名前を得る．
  ///
  /// NULL が返される場合もある．
  const char*
  name() const;

  /// @brief 外部入力タイプの時 true を返す．
  /// @note FF 出力もここに含まれる．
  bool
  is_input() const;

  /// @brief 外部入力タイプの時に入力番号を返す．
  ///
  /// is_input() が false の場合の返り値は不定
  ymuint
  input_id() const;

  /// @brief 外部出力タイプの時 true を返す．
  /// @note FF 入力もここに含まれる．
  bool
  is_output() const;

  /// @brief 外部出力タイプの時に出力番号を返す．
  ///
  /// is_output() が false の場合の返り値は不定
  ymuint
  output_id() const;

  /// @brief TFIサイズの昇順に並べた時の出力番号を返す．
  ymuint
  output_id2() const;

  /// @brief logic タイプの時 true を返す．
  bool
  is_logic() const;

  /// @brief ゲートタイプを得る．
  ///
  /// is_logic() が false の場合の返り値は不定
  tTgGateType
  gate_type() const;

  /// @brief controling value を得る．
  /// @note ない場合は kValX を返す．
  Val3
  cval() const;

  /// @brief noncontroling valueを得る．
  /// @note ない場合は kValX を返す．
  Val3
  nval() const;

  /// @brief controling output value を得る．
  /// @note ない場合は kValX を返す．
  Val3
  coval() const;

  /// @brief noncontroling output value を得る．
  /// @note ない場合は kValX を返す．
  Val3
  noval() const;

  /// @brief 値のノードの時 true を返す．
  ///
  /// is_logic() が true の時のみ意味を持つ．
  bool
  is_root() const;

  /// @brief TgNode のファンインに対応するノードを返す．
  /// @param[in] pos もとの TgNode の入力の位置番号 (!!!)
  ///
  /// is_root() が true の時のみ意味を持つ．
  TpgNode*
  input_map(ymuint pos) const;

  /// @brief TgNode のファンインに対応するノードのファンイン番号を返す．
  /// @param[in] pos もとの TgNode の入力の位置番号 (!!!)
  ///
  /// is_root() が true の時のみ意味を持つ．
  ymuint
  ipos_map(ymuint pos) const;

  /// @brief 内部ノードの時 true を返す．
  ///
  /// is_logic() が true の時のみ意味を持つ．
  bool
  is_internal() const;

  /// @brief ファンイン数を得る．
  ymuint
  fanin_num() const;

  /// @brief ファンインを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
  TpgNode*
  fanin(ymuint pos) const;

  /// @brief ファンアウト数を得る．
  ymuint
  fanout_num() const;

  /// @brief ファンアウトを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < fanout_num() )
  TpgNode*
  fanout(ymuint pos) const;

  /// @brief アクティブなファンアウト数を得る．
  ymuint
  active_fanout_num() const;

  /// @brief アクティブなファンアウトを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < active_fanout_num() )
  TpgNode*
  active_fanout(ymuint pos) const;

  /// @brief アクティブの場合 true を返す．
  bool
  is_active() const;

  /// @brief pos 番目のPOのTFIに含まれている時 true を返す．
  bool
  is_in_TFI_of(ymuint pos) const;

  /// @brief 直近の dominator を得る．
  TpgNode*
  imm_dom() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 故障に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  const TpgFault*
  output_fault(int val) const;

  /// @brief 入力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  const TpgFault*
  input_fault(int val,
	      ymuint pos) const;

  /// @brief このノードに関係する故障数を返す．
  ymuint
  fault_num() const;

  /// @brief このノードに関係する故障を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
  const TpgFault*
  fault(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

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

  // ID 番号
  ymuint32 mId;

  // 名前
  const char* mName;

  // いくつかのデータをパックしたもの
  // - [0:2] ノードタイプ
  //   0: 未使用
  //   1: 外部入力
  //   2: 外部出力
  //   4: 論理ノード(外部ノード)
  //   5: 論理ノード(内部ノード)
  //   6: 論理ノード(ダミーノード)
  // - [3:31] 入力/出力ノードの場合の通し番号
  //          or ゲートタイプ
  ymuint32 mTypeId;

  // ファンイン数
  ymuint32 mFaninNum;

  // ファンインの配列
  TpgNode** mFanins;

  // ファンアウト数
  ymuint32 mFanoutNum;

  ymuint32 mFanoutsSize;

  // ファンアウトの配列
  TpgNode** mFanouts;

  // アクティブなファンアウト数
  ymuint32 mActFanoutNum;

  // アクティブなファンアウトの配列
  TpgNode** mActFanouts;

  // TgNode のファンインに対するマッピング
  TpgMap* mInputMap;

  // 出力の故障
  const TpgFault* mOutputFault[2];

  // 入力の故障
  const TpgFault** mInputFault;

  // 故障リスト
  const TpgFault** mFaultList;

  // 故障リストの要素数
  ymuint32 mFaultNum;

  // いくつかのマークを納めるビットベクタ
  ymuint32 mMarks;

  // TFIマークを表すビットアレイ
  ymuint64* mTFIbits;

  // immediate dominator
  TpgNode* mImmDom;

  // controling value
  Val3 mCval;

  // noncontroling value
  Val3 mNval;

  // controling output value
  Val3 mCOval;

  // noncontroling output value
  Val3 mNOval;

};

/// @relates TpgNode
/// @brief ノード名を出力する
/// @param[in] s 出力先のストリーム
/// @param[in] node 対象のノード
void
print_node(ostream& s,
	   const TpgNode* node);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
TpgNode::TpgNode()
{
  // 実際の初期化は TpgNetwork が行なう．
}

// @brief デストラクタ
inline
TpgNode::~TpgNode()
{
}

// @brief ID番号を得る．
inline
ymuint
TpgNode::id() const
{
  return mId;
}

// @brief 名前を得る．
inline
const char*
TpgNode::name() const
{
  return mName;
}

// @brief 外部入力タイプの時 true を返す．
// @note FF 出力もここに含まれる．
inline
bool
TpgNode::is_input() const
{
  return (mTypeId & 7U) == 1U;
}

// @brief 外部入力タイプの時に入力番号を返す．
inline
ymuint
TpgNode::input_id() const
{
  ASSERT_COND( is_input() );
  return (mTypeId >> 3);
}

// @brief 外部出力タイプの時 true を返す．
// @note FF 入力もここに含まれる．
inline
bool
TpgNode::is_output() const
{
  return (mTypeId & 7U) == 2U;
}

// @brief 外部出力タイプの時に出力番号を返す．
inline
ymuint
TpgNode::output_id() const
{
  ASSERT_COND( is_output() );
  return (mTypeId >> 3);
}

// @brief TFIサイズの昇順に並べた時の出力番号を返す．
inline
ymuint
TpgNode::output_id2() const
{
  ASSERT_COND( is_output() );
  return mFanoutNum;
}

// @brief logic タイプの時 true を返す．
inline
bool
TpgNode::is_logic() const
{
  return (mTypeId & 4U) == 4U;
}

// @brief ゲートタイプを得る．
inline
tTgGateType
TpgNode::gate_type() const
{
  ASSERT_COND( is_logic() );
  return static_cast<tTgGateType>((mTypeId >> 3) & 15U);
}

// @brief 値のノードの時 true を返す．
//
// is_logic() が true の時のみ意味を持つ．
inline
bool
TpgNode::is_root() const
{
  return (mTypeId & 7U) == 4U;
}

// @brief 内部ノードの時 true を返す．
//
// is_logic() が true の時のみ意味を持つ．
inline
bool
TpgNode::is_internal() const
{
  return (mTypeId & 7U) == 5U;
}

// @brief ファンイン数を得る．
inline
ymuint
TpgNode::fanin_num() const
{
  return mFaninNum;
}

// @brief ファンインを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanin_num() )
inline
TpgNode*
TpgNode::fanin(ymuint pos) const
{
  ASSERT_COND( pos < mFaninNum );
  return mFanins[pos];
}

// @brief ファンアウト数を得る．
inline
ymuint
TpgNode::fanout_num() const
{
  if ( is_output() ) {
    return 0;
  }
  return mFanoutNum;
}

// @brief ファンアウトを得る．
// @param[in] pos 位置番号 ( 0 <= pos < fanout_num() )
inline
TpgNode*
TpgNode::fanout(ymuint pos) const
{
  ASSERT_COND( pos < fanout_num() );
  return mFanouts[pos];
}

// @brief アクティブなファンアウト数を得る．
inline
ymuint
TpgNode::active_fanout_num() const
{
  return mActFanoutNum;
}

// @brief アクティブなファンアウトを得る．
// @param[in] pos 位置番号 ( 0 <= pos < active_fanout_num() )
inline
TpgNode*
TpgNode::active_fanout(ymuint pos) const
{
  ASSERT_COND( pos < mActFanoutNum );
  return mActFanouts[pos];
}

// @brief 出力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
inline
const TpgFault*
TpgNode::output_fault(int val) const
{
  return mOutputFault[val % 2];
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
inline
const TpgFault*
TpgNode::input_fault(int val,
		     ymuint pos) const
{
  ASSERT_COND( pos < mFaninNum );
  return mInputFault[pos * 2 + (val % 2)];
}

// @brief このノードに関係する故障数を返す．
inline
ymuint
TpgNode::fault_num() const
{
  return mFaultNum;
}

// @brief このノードに関係する故障を返す．
// @param[in] pos 位置番号 ( 0 <= pos < fault_num() )
inline
const TpgFault*
TpgNode::fault(ymuint pos) const
{
  ASSERT_COND( pos < mFaultNum );
  return mFaultList[pos];
}

// @brief アクティブの場合 true を返す．
inline
bool
TpgNode::is_active() const
{
  return static_cast<bool>(mMarks & 1U);
}

// @brief アクティブにする．
inline
void
TpgNode::set_active()
{
  mMarks |= 1U;
}

// @brief アクティブフラグを消す．
inline
void
TpgNode::clear_active()
{
  mMarks &= ~1U;
}

// @brief pos 番目のPOのTFIに含まれている時 true を返す．
inline
bool
TpgNode::is_in_TFI_of(ymuint pos) const
{
  ymuint blk = pos / 64;
  ymuint sft = pos % 64;
  return static_cast<bool>((mTFIbits[blk] >> sft) & 1U);
}

// @brief 直近の dominator を得る．
inline
TpgNode*
TpgNode::imm_dom() const
{
  return mImmDom;
}

// @brief controling value を得る．
// @note ない場合は kValX を返す．
inline
Val3
TpgNode::cval() const
{
  return mCval;
}

// @brief noncontroling valueを得る．
// @note ない場合は kValX を返す．
inline
Val3
TpgNode::nval() const
{
  return mNval;
}

// @brief controling output value を得る．
// @note ない場合は kValX を返す．
inline
Val3
TpgNode::coval() const
{
  return mCOval;
}

// @brief noncontroling output value を得る．
// @note ない場合は kValX を返す．
inline
Val3
TpgNode::noval() const
{
  return mNOval;
}

END_NAMESPACE_YM_SATPG

#endif // TPGNODE_H
