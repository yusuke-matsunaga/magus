#ifndef MINPATNAIVE_H
#define MINPATNAIVE_H

/// @file MinPatNaive.h
/// @brief MinPatNaive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatBase.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class MinPatNaive MinPatNaive.h "MinPatNaive.h"
/// @brief 単純な MinPat
//////////////////////////////////////////////////////////////////////
class MinPatNaive :
  public MinPatBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] group_dominance グループ支配を計算する．
  MinPatNaive(bool group_dominance);

  /// @brief デストラクタ
  ~MinPatNaive();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障リストを設定する．
  void
  set_fid_list(const vector<ymuint>& src_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  /// @param[in] fid_list 検出された故障番号のリスト
  /// @param[in] tvmgr テストベクタマネージャ
  /// @param[in] fsim2 2値の故障シミュレータ(検証用)
  virtual
  void
  init(const vector<ymuint>& fid_list,
       TvMgr& tvmgr,
       Fsim& fsim2);

  /// @brief 対象の全故障数を返す．
  virtual
  ymuint
  fault_num();

  /// @brief 故障番号のリストを返す．
  virtual
  const vector<ymuint>&
  fid_list();

  /// @brief 最初の故障を選ぶ．
  virtual
  ymuint
  get_first_fault();

  /// @brief 次に処理すべき故障を選ぶ．
  /// @param[in] fgmgr 故障グループを管理するオブジェクト
  /// @param[in] group_list 現在のグループリスト
  ///
  /// 故障が残っていなければ NULL を返す．
  virtual
  ymuint
  get_next_fault(FgMgr& fgmgr,
		 const vector<ymuint>& group_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障番号のリスト
  vector<ymuint> mFidList;

  // 次の故障の位置
  ymuint mNextPos;

};

END_NAMESPACE_YM_SATPG

#endif // MINPATNAIVE_H
