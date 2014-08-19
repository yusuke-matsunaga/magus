#ifndef IMPAND_H
#define IMPAND_H

/// @file ImpAnd.h
/// @brief ImpAnd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ImpNode.h"
#include "ImpMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class ImpAnd ImpAnd.h "ImpAnd.h"
/// @brief AND ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class ImpAnd :
  public ImpNode
{
  friend class ImpMgr;
public:

  /// @brief コンストラクタ
  /// @param[in] handle0 ファンイン0のハンドル
  /// @param[in] handle1 ファンイン1のハンドル
  ImpAnd(ImpNodeHandle handle0,
	 ImpNodeHandle handle1);

  /// @brief デストラクタ
  virtual
  ~ImpAnd();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AND タイプのときに true を返す．
  virtual
  bool
  is_and() const;

  /// @brief 出力値を返す．
  virtual
  Bool3
  val() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ビットベクタ関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビットベクタ値の計算を行なう．
  virtual
  void
  calc_bitval();


public:
  //////////////////////////////////////////////////////////////////////
  // ラーニング用の含意を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 状態を初期化する．
  virtual
  void
  clear();

  /// @brief 状態を返す．
  virtual
  ymuint32
  cur_state() const;

  /// @brief 状態を表す文字列を返す．
  virtual
  string
  cur_state_str() const;

  /// @brief 状態を元にもどす．
  virtual
  void
  restore(ImpMgr& mgr,
	  ymuint32 val);

  /// @brief unjustified ノードの時 true を返す．
  virtual
  bool
  is_unjustified() const;

  /// @brief justification パタン数を得る．
  virtual
  ymuint
  justification_num();

  /// @brief justification パタン を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < justification_num() )
  /// @return 値割り当て
  virtual
  ImpDst
  get_justification(ymuint pos);

  /// @brief ファンイン0を0にする．
  /// @param[in] mgr ImMgr
  /// @param[in] rec 含意を記録するオブジェクト
  virtual
  bool
  fwd0_imp0(ImpMgr& mgr,
	    ImpRec& rec);

  /// @brief ファンイン0を1にする．
  /// @param[in] mgr ImMgr
  /// @param[in] rec 含意を記録するオブジェクト
  virtual
  bool
  fwd0_imp1(ImpMgr& mgr,
	    ImpRec& rec);

  /// @brief ファンイン1を0にする．
  /// @param[in] mgr ImMgr
  /// @param[in] rec 含意を記録するオブジェクト
  virtual
  bool
  fwd1_imp0(ImpMgr& mgr,
	    ImpRec& rec);

  /// @brief ファンイン1を1にする．
  /// @param[in] mgr ImMgr
  /// @param[in] rec 含意を記録するオブジェクト
  virtual
  bool
  fwd1_imp1(ImpMgr& mgr,
	    ImpRec& rec);

  /// @brief 出力を0にする．
  /// @param[in] mgr ImMgr
  /// @param[in] rec 含意を記録するオブジェクト
  virtual
  bool
  bwd_imp0(ImpMgr& mgr,
	   ImpRec& rec);

  /// @brief 出力を1にする．
  /// @param[in] mgr ImMgr
  /// @param[in] rec 含意を記録するオブジェクト
  virtual
  bool
  bwd_imp1(ImpMgr& mgr,
	   ImpRec& rec);


public:
  //////////////////////////////////////////////////////////////////////
  // 含意を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンイン0を0にする．
  /// @param[in] mgr ImMgr
  virtual
  bool
  fwd0_imp0(ImpMgr& mgr);

  /// @brief ファンイン0を1にする．
  /// @param[in] mgr ImMgr
  virtual
  bool
  fwd0_imp1(ImpMgr& mgr);

  /// @brief ファンイン1を0にする．
  /// @param[in] mgr ImMgr
  virtual
  bool
  fwd1_imp0(ImpMgr& mgr);

  /// @brief ファンイン1を1にする．
  /// @param[in] mgr ImMgr
  virtual
  bool
  fwd1_imp1(ImpMgr& mgr);

  /// @brief 出力を0にする．
  /// @param[in] mgr ImMgr
  virtual
  bool
  bwd_imp0(ImpMgr& mgr);

  /// @brief 出力を1にする．
  /// @param[in] mgr ImMgr
  virtual
  bool
  bwd_imp1(ImpMgr& mgr);

  /// @brief 定数伝搬を行なう．
  /// @param[in] mgr ImMgr
  /// @param[in] val 値
  /// @param[in] ipos 入力位置
  virtual
  void
  prop_const(ImpMgr& mgr,
	     ymuint val,
	     ymuint ipos);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 状態
  // XX:X   0
  // XX:0   1
  // XX:1   -
  // X0:X   -
  // X0:0   2
  // X0:1   -
  // X1:X   3
  // X1:0   -
  // X1:1   -
  // 0X:X   -
  // 0X:0   4
  // 0X:1   -
  // 00:X   -
  // 00:0   5
  // 00:1   -
  // 01:X   -
  // 01:0   6
  // 01:1   -
  // 1X:X   7
  // 1X:0   -
  // 1X:1   -
  // 10:X   -
  // 10:0   8
  // 10:1   -
  // 11:X   -
  // 11:0   -
  // 11:1   9
  enum tState {
    kStXX_X = 0,
    kSt1X_X = 1,
    kStX1_X = 2,
    kStXX_0 = 3,
    kStX0_0 = 4,
    kSt0X_0 = 5,
    kSt00_0 = 6,
    kSt10_0 = 7,
    kSt01_0 = 8,
    kSt11_1 = 9
  };


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を変える．
  /// @param[in] mgr ImMgr
  /// @param[in] val 値
  void
  change_value(ImpMgr& mgr,
	       tState val,
	       bool record = true);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 状態
  tState mState;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 値を変える．
// @param[in] mgr ImpMgr
// @param[in] val 値
inline
void
ImpAnd::change_value(ImpMgr& mgr,
		     tState val,
		     bool record)
{
  if ( record ) {
    mgr.save_value(this, static_cast<ymuint32>(mState));
  }

#if 0
  bool pre = is_unjustified();
#endif
#if DEBUG_CHANGE_VALUE
  string pre_str = cur_state_str();
#endif

  mState = val;

#if DEBUG_CHANGE_VALUE
  string post_str = cur_state_str();
  cout << "node#" << id() << ": " << pre_str << " -> " << post_str << endl;
#endif

#if 0
  bool post = is_unjustified();
  if ( pre ^ post ) {
    if ( post ) {
      mgr.set_unjustified(this);
    }
    else {
      mgr.reset_unjustified(this);
    }
  }
#endif
}

END_NAMESPACE_YM_NETWORKS

#endif // IMPAND_H
