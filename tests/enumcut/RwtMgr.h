#ifndef RWTMGR_H
#define RWTMGR_H

/// @file RwtMgr.h
/// @brief RwtMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"
#include "ym_logic/TvFunc.h"
#include "ym_logic/NpnMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS

class RwtPat;
class RwtNode;

//////////////////////////////////////////////////////////////////////
/// @class RwtMgr RwtMgr.h "RwtMgr.h"
/// @brief RwtNode を管理するクラス
//////////////////////////////////////////////////////////////////////
class RwtMgr
{
public:

  /// @brief コンストラクタ
  RwtMgr();

  /// @brief デストラクタ
  ~RwtMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クリアする．
  void
  clear();

  /// @brief 初期化する．
  /// @param[in] input_num 入力数
  /// @param[in] node_num 内部ノード数
  /// @note 全ノード数は input_num + node_num + 1(定数0) となる．
  void
  init(ymuint input_num,
       ymuint node_num);

  /// @brief ノードの情報を設定する．
  /// @param[in] id ノードの ID番号
  /// @param[in] is_xor XOR ノードの時に true にするフラグ
  /// @param[in] fanin0 ファンイン0のノード番号
  /// @param[in] fanin1 ファンイン1のノード番号
  /// @param[in] inv0 ファンイン0の極性
  /// @param[in] inv1 ファンイン1の極性
  void
  set_node(ymuint id,
	   bool is_xor,
	   ymuint fanin0,
	   ymuint fanin1,
	   bool inv0,
	   bool inv1);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
  ymuint
  input_num() const;

  /// @brief 全ノード数を得る．
  ymuint
  node_num() const;

  /// @brief 論理関数に対するパタンを返す．
  /// @param[in] f 論理関数
  /// @param[out] cmap 変換マップ
  const RwtPat*
  pat(const TvFunc& f,
      NpnMap& cmap);

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードと論理関数を結びつける．
  /// @param[in] node ノード
  /// @param[in] f 論理関数
  void
  set_func(RwtNode* node,
	   const TvFunc& f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数
  ymuint32 mInputNum;

  // 全ノード数
  ymuint32 mNodeNum;

  // ノードの配列
  // 0 〜 input_num() までは予約済み
  RwtNode* mNodeArray;

  // 代表関数をキーにしたパタンのハッシュ表
  hash_map<TvFunc, RwtPat*> mPatMap;

  // NPN マネージャ
  NpnMgr mNpnMgr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 入力数を得る．
inline
ymuint
RwtMgr::input_num() const
{
  return mInputNum;
}

// @brief 全ノード数を得る．
inline
ymuint
RwtMgr::node_num() const
{
  return mNodeNum;
}

END_NAMESPACE_YM_NETWORKS

#endif // RWTMGR_H
