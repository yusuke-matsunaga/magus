#ifndef LIBYM_TECHMAP_PATMATCHER_H
#define LIBYM_TECHMAP_PATMATCHER_H

/// @file libym_techmap/PatMatcher.h
/// @brief PatMatcher のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/techmap_nsdef.h"


BEGIN_NAMESPACE_YM_TECHMAP

class SbjNode;
class PatMgr;
class PatGraph;

//////////////////////////////////////////////////////////////////////
/// @class PatMatcher PatMatcher.h "PatMatcher.h"
/// @brief パタンマッチングを行うクラス
//////////////////////////////////////////////////////////////////////
class PatMatcher
{
public:

  /// @brief コンストラクタ
  PatMatcher();

  /// @brief デストラクタ
  ~PatMatcher();


public:

  /// @brief パタンマッチングを行う．
  /// @param[in] sbj_root サブジェクトグラフの根のノード
  /// @param[in] pat_mgr パタンを管理するクラス
  /// @param[in] pat_graph パタングラフ
  /// @param[out] input_map 入力のマッピング
  /// @retval true マッチした．
  /// @retval false マッチしなかった．
  /// @note input_map の中身は (SbjNode->i() << 1) | pol
  bool
  operator()(const SbjNode* sbj_root,
	     const PatMgr& pat_mgr,
	     const PatGraph& pat_graph,
	     vector<ymuint>& input_map);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief サブジェクトノードとパタンノードをバインドする．
  /// @param[in] sbj_node サブジェクトノード
  /// @param[in] pat_id パタンノードのID
  /// @param[in] inv 反転フラグ
  /// @retval true バインドが成功した．
  /// @retval false バインドが失敗した．
  bool
  bind(const SbjNode* sbj_node,
       ymuint pat_id,
       bool inv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パタンノードの ID をキーとしてサブジェクトノードを入れる配列
  vector<const SbjNode*> mSbjMap;

  // サブジェクトノードの ID をキーとしてパタンノードの ID を
  // 入れる配列
  hash_map<ymuint, ymuint> mPatMap;

  // パタンノードの ID をキーとして極性を入れる配列
  vector<bool> mInvMap;

};

END_NAMESPACE_YM_TECHMAP

#endif // LIBYM_TECHMAP_PATMATCHER_H
