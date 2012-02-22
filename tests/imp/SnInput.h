#ifndef SNINPUT_H
#define SNINPUT_H

/// @file SnInput.h
/// @brief SnInput のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "StrNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class SnInput SnInput.h "SnInput.h"
/// @brief StrImp で用いられるノード
//////////////////////////////////////////////////////////////////////
class SnInput :
  public StrNode
{
  friend class ImpMgr;
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  SnInput(ymuint id);

  /// @brief デストラクタ
  virtual
  ~SnInput();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力のときに true を返す．
  virtual
  bool
  is_input() const;

  /// @brief 出力値を返す．
  Bool3
  val() const;

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

  /// @brief 状態を元にもどす．
  virtual
  void
  restore(ymuint32 val);

  /// @brief ファンイン0を0にする．
  /// @param[in] mgr ImMgr
  /// @param[out] imp_list 含意の結果を格納するリスト
  virtual
  bool
  fwd0_imp0(ImpMgr& mgr,
	    vector<ImpCell>& imp_list);

  /// @brief ファンイン0を1にする．
  /// @param[in] mgr ImMgr
  /// @param[out] imp_list 含意の結果を格納するリスト
  virtual
  bool
  fwd0_imp1(ImpMgr& mgr,
	    vector<ImpCell>& imp_list);

  /// @brief ファンイン1を0にする．
  /// @param[in] mgr ImMgr
  /// @param[out] imp_list 含意の結果を格納するリスト
  virtual
  bool
  fwd1_imp0(ImpMgr& mgr,
	    vector<ImpCell>& imp_list);

  /// @brief ファンイン1を1にする．
  /// @param[in] mgr ImMgr
  /// @param[out] imp_list 含意の結果を格納するリスト
  virtual
  bool
  fwd1_imp1(ImpMgr& mgr,
	    vector<ImpCell>& imp_list);

  /// @brief 出力を0にする．
  /// @param[in] mgr ImMgr
  /// @param[out] imp_list 含意の結果を格納するリスト
  virtual
  bool
  bwd_imp0(ImpMgr& mgr,
	   vector<ImpCell>& imp_list);

  /// @brief 出力を1にする．
  /// @param[in] mgr ImMgr
  /// @param[out] imp_list 含意の結果を格納するリスト
  virtual
  bool
  bwd_imp1(ImpMgr& mgr,
	   vector<ImpCell>& imp_list);


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

#endif // SNINPUT_H
