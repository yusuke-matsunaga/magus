#ifndef TPGNODE_H
#define TPGNODE_H

/// @file TpgNode.h
/// @brief TpgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "networks/tgnet.h"
#include "logic/VarId.h"
#include "logic/Bool3.h"


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

  /// @brief ダミーノードの時 true を返す．
  bool
  is_dummy_node() const;

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

  /// @brief pos 番目のPOのTFIに制限した場合の直近の dominator を得る．
  ///
  /// pos 番目のPOのTFIに含まれていない場合は NULL を返す．
  TpgNode*
  imm_dom(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 故障に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  TpgFault*
  output_fault(int val) const;

  /// @brief 入力の故障を得る．
  /// @param[in] val 故障値 ( 0 / 1 )
  /// @param[in] pos 入力の位置番号
  TpgFault*
  input_fault(int val,
	      ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // SatEngine 用のアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 正常回路用の変数番号をセットする．
  /// @param[in] gvar 正常値を表す変数番号
  void
  set_gvar(VarId gvar);

  /// @brief 故障回路用の変数番号をセットする．
  /// @param[in] fvar 故障値を表す変数番号
  /// @param[in] dvar 故障差(正常値 xor 故障値)を表す変数番号
  void
  set_fvar(VarId fvar,
	   VarId dvar);

  /// @brief 変数番号の割り当て情報をクリアする．
  void
  clear_var();

  /// @brief 正常値を表す変数番号を得る．
  VarId
  gvar() const;

  /// @brief 故障回路用の変数番号が割り当てられていたら true を返す．
  bool
  has_fvar() const;

  /// @brief 故障値を表す変数番号を得る．
  VarId
  fvar() const;

  /// @brief 故障差を表す変数番号を得る．
  VarId
  dvar() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 汎用のマークを扱う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief mark1 を得る．
  bool
  mark1() const;

  /// @brief mark1 をつける．
  void
  set_mark1();

  /// @brief mark1 を消す．
  void
  clear_mark1();

  /// @brief mark2 を得る．
  bool
  mark2() const;

  /// @brief mark2 をつける．
  void
  set_mark2();

  /// @brief mark2 を消す．
  void
  clear_mark2();

  /// @brief mark3 を得る．
  bool
  mark3() const;

  /// @brief mark3 をつける．
  void
  set_mark3();

  /// @brief mark3 を消す．
  void
  clear_mark3();


public:
  //////////////////////////////////////////////////////////////////////
  // mandatory assignment 用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 後方含意で出力に0を割り当てる．
  /// @param[in] from_node 含意元のノード
  /// @param[in] val 値
  /// @param[in] node_list 割り当ての行われたノードを格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  bwd_prop(TpgNode* from_node,
	   Bool3 val,
	   vector<TpgNode*>& node_list);

  /// @brief ファンアウト先に0を伝播する．
  /// @param[in] from_node 含意元のノード
  /// @param[in] val 値
  /// @param[in] node_list 割り当ての行われたノードを格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fanout_prop(TpgNode* from_node,
	      Bool3 val,
	      vector<TpgNode*>& node_list);

  /// @brief controling value を得る．
  /// @note ない場合は kB3X を返す．
  Bool3
  cval() const;

  /// @brief noncontroling valueを得る．
  /// @note ない場合は kB3X を返す．
  Bool3
  nval() const;

  /// @brief 値を得る．
  Bool3
  ma_value() const;

  /// @brief 値をクリアする．
  void
  clear_ma_value();


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

  /// @brief 後方含意を行う．
  /// @param[in] val 値
  /// @param[in] node_list 割り当ての行われたノードを格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  bwd_imp(Bool3 val,
	  vector<TpgNode*>& node_list);

  /// @brief 前方含意を行う．
  /// @param[in] from_node 含意元のノード
  /// @param[in] val 値
  /// @param[in] node_list 割り当ての行われたノードを格納するリスト
  /// @retval true 矛盾なく含意が行われた．
  /// @retval false 矛盾が発生した．
  bool
  fwd_imp(TpgNode* from_node,
	  Bool3 val,
	  vector<TpgNode*>& node_list);


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

  // ファンアウトの配列
  TpgNode** mFanouts;

  // アクティブなファンアウト数
  ymuint32 mActFanoutNum;

  // アクティブなファンアウトの配列
  TpgNode** mActFanouts;

  // TgNode のファンインに対するマッピング
  TpgMap* mInputMap;

  // 出力の故障
  TpgFault* mOutputFault[2];

  // 入力の故障
  TpgFault** mInputFault;

  // いくつかのマークを納めるビットベクタ
  ymuint32 mMarks;

  // 正常回路の変数番号
  VarId mGid;

  // 故障回路の変数番号
  VarId mFid;

  // 故障差の変数番号
  VarId mDid;

  // TFIマークを表すビットアレイ
  ymuint64* mTFIbits;

  // immediate dominator
  TpgNode* mImmDom;

  // TFI ごとの immediate dominator を保持するための補助クラス
  struct ImmDomMap
  {
    ImmDomMap() { }

    ImmDomMap(ymuint64* bits,
	      TpgNode* node) :
      mBitMask(bits),
      mImmDom(node) { }

    // ビットマスク
    ymuint64* mBitMask;

    // immediate dominator
    TpgNode* mImmDom;

  };

  // TFI ごとの immediate dominator の配列
  ImmDomMap* mImmDomMap;

  // controling value
  Bool3 mCval;

  // noncontroling value
  Bool3 mNval;

  // mandatory assignments 用の値
  Bool3 mMaVal;

};


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
  assert_cond( is_input(), __FILE__, __LINE__);
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
  assert_cond( is_output(), __FILE__, __LINE__);
  return (mTypeId >> 3);
}

// @brief TFIサイズの昇順に並べた時の出力番号を返す．
inline
ymuint
TpgNode::output_id2() const
{
  assert_cond( is_output(), __FILE__, __LINE__);
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
  assert_cond( is_logic(), __FILE__, __LINE__);
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

// @brief ダミーノードの時 true を返す．
inline
bool
TpgNode::is_dummy_node() const
{
  return (mTypeId & 7U) == 6U;
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
  assert_cond( pos < mFaninNum, __FILE__, __LINE__);
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
  assert_cond( pos < fanout_num(), __FILE__, __LINE__);
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
  assert_cond( pos < mActFanoutNum, __FILE__, __LINE__);
  return mActFanouts[pos];
}

// @brief 出力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
inline
TpgFault*
TpgNode::output_fault(int val) const
{
  return mOutputFault[val % 2];
}

// @brief 入力の故障を得る．
// @param[in] val 故障値 ( 0 / 1 )
// @param[in] pos 入力の位置番号
inline
TpgFault*
TpgNode::input_fault(int val,
		     ymuint pos) const
{
  assert_cond( pos < mFaninNum, __FILE__, __LINE__);
  return mInputFault[pos * 2 + (val % 2)];
}

// @brief mark1 を得る．
inline
bool
TpgNode::mark1() const
{
  return static_cast<bool>((mMarks >> 3) & 1U);
}

// @brief mark1 をつける．
inline
void
TpgNode::set_mark1()
{
  mMarks |= 8U;
}

// @brief mark1 を消す．
inline
void
TpgNode::clear_mark1()
{
  mMarks &= ~8U;
}

// @brief mark2 を得る．
inline
bool
TpgNode::mark2() const
{
  return static_cast<bool>((mMarks >> 4) & 1U);
}

// @brief mark2 をつける．
inline
void
TpgNode::set_mark2()
{
  mMarks |= 16U;
}

// @brief mark2 を消す．
inline
void
TpgNode::clear_mark2()
{
  mMarks &= ~16U;
}

// @brief mark3 を得る．
inline
bool
TpgNode::mark3() const
{
  return static_cast<bool>((mMarks >> 5) & 1U);
}

// @brief mark3 をつける．
inline
void
TpgNode::set_mark3()
{
  mMarks |= 32U;
}

// @brief mark3 を消す．
inline
void
TpgNode::clear_mark3()
{
  mMarks &= ~32U;
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

// @brief pos 番目のPOのTFIに制限した場合の直近の dominator を得る．
//
// pos 番目のPOのTFIに含まれていない場合は NULL を返す．
inline
TpgNode*
TpgNode::imm_dom(ymuint pos) const
{
  ymuint blk = pos / 64;
  ymuint sft = pos % 64;
  ymuint64 mask = (1UL << sft);
  // かならずどれかのエントリにヒットするはずなので
  // ループの終了条件はない．
  // ちょっとギャンブルなコード
  for (ymuint i = 0; ; ++ i) {
    if ( mImmDomMap[i].mBitMask[blk] & mask ) {
      return mImmDomMap[i].mImmDom;
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 正常回路用の変数番号をセットする．
// @param[in] gvar 正常値を表す変数番号
inline
void
TpgNode::set_gvar(VarId gvar)
{
  mGid = gvar;
  mFid = gvar;
  mMarks |= 2U;
}

// @brief 故障回路用の変数番号をセットする．
// @param[in] fvar 故障値を表す変数番号
// @param[in] dvar 故障差(正常値 xor 故障値)を表す変数番号
inline
void
TpgNode::set_fvar(VarId fvar,
		   VarId dvar)
{
  mFid = fvar;
  mDid = dvar;
  mMarks |= 4U;
}

// @brief 変数番号の割り当て情報をクリアする．
inline
void
TpgNode::clear_var()
{
  mMarks &= ~6U;
}

// @brief 正常値を表す変数番号を得る．
inline
VarId
TpgNode::gvar() const
{
  return mGid;
}

// @brief 故障回路用の変数番号が割り当てられていたら true を返す．
inline
bool
TpgNode::has_fvar() const
{
  return static_cast<bool>((mMarks >> 2) & 1U);
}

// @brief 故障値を表す変数番号を得る．
inline
VarId
TpgNode::fvar() const
{
  return mFid;
}

// @brief 故障差を表す変数番号を得る．
inline
VarId
TpgNode::dvar() const
{
  return mDid;
}

// @brief controling value を得る．
// @note ない場合は kB3X を返す．
inline
Bool3
TpgNode::cval() const
{
  return mCval;
}

// @brief noncontroling valueを得る．
// @note ない場合は kB3X を返す．
inline
Bool3
TpgNode::nval() const
{
  return mNval;
}

// @brief 値を得る．
inline
Bool3
TpgNode::ma_value() const
{
  return mMaVal;
}

// @brief 値をクリアする．
inline
void
TpgNode::clear_ma_value()
{
  mMaVal = kB3X;
}

END_NAMESPACE_YM_SATPG

#endif // TPGNODE_H
