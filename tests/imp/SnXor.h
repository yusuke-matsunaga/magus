#ifndef SNXOR_H
#define SNXOR_H

/// @file SnXor.h
/// @brief SnXor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "StrNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class SnXor SnXor.h "SnXor.h"
/// @brief StrImp で用いられるノード
//////////////////////////////////////////////////////////////////////
class SnXor :
  public StrNode
{
  friend class ImpMgr;
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] node0 ファンイン0のノード
  /// @param[in] inv0 ファンイン0の極性
  /// @param[in] node1 ファンイン1のノード
  /// @param[in] inv1 ファンイン1の極性
  SnXor(ymuint id,
	StrNode* node0,
	bool inv0,
	StrNode* node1,
	bool inv1);

  /// @brief デストラクタ
  ~SnXor();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief XOR タイプのときに true を返す．
  virtual
  bool
  is_xor() const;

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

  /// @brief 状態を元にもどす．
  virtual
  void
  restore(ymuint32 val);

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
  ImpCell
  get_justification(ymuint pos);

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
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 状態
  // XX:X   0
  // XX:0   1
  // XX:1   2
  // X0:X   3
  // X0:0   -
  // X0:1   -
  // X1:X   4
  // X1:0   -
  // X1:1   -
  // 0X:X   5
  // 0X:0   -
  // 0X:1   -
  // 00:X   -
  // 00:0   6
  // 00:1   -
  // 01:X   -
  // 01:0   -
  // 01:1   7
  // 1X:X   8
  // 1X:0   -
  // 1X:1   -
  // 10:X   -
  // 10:0   -
  // 10:1   9
  // 11:X   -
  // 11:0   10
  // 11:1   -
  enum tState {
    kStXX_X = 0,
    kStX0_X = 1,
    kStX1_X = 2,
    kSt0X_X = 3,
    kSt1X_X = 4,
    kStXX_0 = 5,
    kSt00_0 = 6,
    kSt11_0 = 7,
    kStXX_1 = 8,
    kSt01_1 = 9,
    kSt10_1 = 10
  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を変える．
  /// @param[in] mgr ImpMgr
  /// @param[in] val 値
  void
  change_value(ImpMgr& mgr,
	       tState val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 状態
  tState mState;

};

END_NAMESPACE_YM_NETWORKS

#endif // SNXOR_H
