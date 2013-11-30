#ifndef IMPINPUT_H
#define IMPINPUT_H

/// @file ImpInput.h
/// @brief ImpInput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ImpNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class ImpInput ImpInput.h "ImpInput.h"
/// @brief 外部入力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class ImpInput :
  public ImpNode
{
  friend class ImpMgr;
public:

  /// @brief コンストラクタ
  ImpInput();

  /// @brief デストラクタ
  virtual
  ~ImpInput();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力のときに true を返す．
  virtual
  bool
  is_input() const;

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
  // 含意を行う関数
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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 状態
  // X: 0
  // 0: 1
  // 1: 2
  enum tState {
    kStX = 0,
    kSt0 = 1,
    kSt1 = 2
  } mState;

};

END_NAMESPACE_YM_NETWORKS

#endif // IMPINPUT_H
