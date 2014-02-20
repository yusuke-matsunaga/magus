#ifndef NPNMGRIMPL_H
#define NPNMGRIMPL_H

/// @file NpnMgrImpl.h
/// @brief NpnMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/NpnMap.h"
#include "utils/UnitAlloc.h"
#include "NpnBaseConf.h"


BEGIN_NAMESPACE_YM_NPN

class NpnConf;

//////////////////////////////////////////////////////////////////////
/// @class NpnMgrImpl NpnMgrImpl.h "NpnMgrImpl.h"
/// @brief NpnMgr の処理を実際に行う実装クラス
//////////////////////////////////////////////////////////////////////
class NpnMgrImpl
{
public:

  /// @brief コンストラクタ
  NpnMgrImpl();

  /// @brief デストラクタ
  ~NpnMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // NpnMgr のための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理関数の正規化を行う．
  /// @param[in] func 対象の論理関数
  /// @param[out] cmap 正規化するための変換マップ(の一つ)
  /// @note func.xform(cmap) の結果が正規化された関数となる．
  /// @note 同位体変換がある時は次の all_map() で取得する．
  void
  cannonical(const TvFunc& func,
	     NpnMap& cmap,
	     int algorithm);

  /// @brief 直前の cannonical の呼び出しにおける NpnMap の全候補を返す．
  /// @param[out] map_list 変換マップを格納するリスト
  void
  all_map(vector<NpnMap>& map_list) const;

  /// @brief w2max_recur の起動回数を返す．
  /// 直前の cannonical の呼び出しにおける
  /// w2max_recur の起動回数を返す．
  ymulong
  w2max_count() const;

  /// @brief w2max_recur の起動回数を返す．
  /// 直前の cannonical の呼び出しにおける
  /// w2max_recur の起動回数を返す．
  ymulong
  tvmax_count() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief DFS
  void
  algorithm0(const NpnConf& conf);

  /// @brief BFS
  void
  algorithm1(const NpnConf& conf);

  /// @brief 極性を展開した後で DFS
  void
  algorithm2(const NpnConf& conf);

  /// @brief 極性を展開した後で BFS
  void
  algorithm3(const NpnConf& conf);

  /// @brief シグネチャが最大になるように極性と順序を決める．
  /// @param[out] conf
  /// @param[in] g0 調べ始める最初のグループ番号
  void
  w2max_recur(NpnConf& conf,
	      ymuint g0);

  /// @brief w2シグネチャが最大になるように極性と順序を決める．
  /// @param[in] conf_list
  /// @param[in] g0 調べ始める最初のグループ番号
  void
  w2max_recur(vector<NpnConf>& conf_list,
	      ymuint g0);

  /// @brief mMaxFunc を設定する．
  void
  set_maxfunc(const TvFunc& func);

  /// @brief NpnConf を取り出す．
  NpnConf*
  new_npnconf();

  /// @brief NpnConf を使用可能リストに戻す．
  void
  free_npnconf(NpnConf* conf);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // w0とw1 を用いた解析情報
  NpnBaseConf mBaseConf;

  // w2max_recur で用いる現在の最適解のリスト
  vector<NpnConf> mMaxList;

  // 1回の cannonical あたりの w2max_recur の起動回数
  ymuint64 mW2max_count;

  // 1回の cannonical あたりの tvmax_recur の起動回数
  ymuint64 mTvmax_count;

  // w2max_recur で用いる現在の最大値
  TvFunc mMaxFunc;

  TvFunc mTmpFunc;

  // w2max_recur で用いる現在の w2 ベクタ
  int mMaxW2[TvFunc::kMaxNi * TvFunc::kMaxNi];

  bool mMaxW2Valid;

  // NpnConf 用のアロケータ
  UnitAlloc mAlloc;

};

END_NAMESPACE_YM_NPN

#endif // NPNMGRIMPL_H
