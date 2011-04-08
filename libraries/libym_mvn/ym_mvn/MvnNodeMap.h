#ifndef LIBYM_MVN_CONV_MVNNODEMAP_H
#define LIBYM_MVN_CONV_MVNNODEMAP_H

/// @file libym_mvn/conv/MvnNodeMap.h
/// @brief MvnNodeMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_sbj/sbj_nsdef.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnNodeMap MvnNodeMap.h "MvnNodeMap.h"
/// @brief MvnNode と SbjNode の対応を記録するクラス
//////////////////////////////////////////////////////////////////////
class MvnNodeMap
{
public:

  /// @brief コンストラクタ
  /// @param[in] n MvnNode の ID の最大値
  MvnNodeMap(ymuint n);

  /// @brief デストラクタ
  ~MvnNodeMap();


public:

  /// @brief 登録する．(1ビット版)
  /// @param[in] mvnode MvnNode
  /// @param[in] sbjhandle SbjHandle
  void
  put(const MvnNode* mvnode,
      SbjHandle sbjhandle);

  /// @brief 登録する．(ベクタ版)
  /// @param[in] mvnode MvnNode
  /// @param[in] index ビット位置
  /// @param[in] sbjhandle SbjHandle
  void
  put(const MvnNode* mvnode,
      ymuint index,
      SbjHandle sbjhandle);

  /// @brief 探す．(1ビット版)
  /// @param[in] mvnode MvnNode
  /// @return 対応するハンドルを返す．
  SbjHandle
  get(const MvnNode* mvnode) const;

  /// @brief 探す．(ベクタ版)
  /// @param[in] mvnode MvnNode
  /// @param[in] index ビット位置
  /// @return 対応するハンドルを返す．
  SbjHandle
  get(const MvnNode* mvnode,
      ymuint index) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  vector<vector<SbjHandle> > mArray;

};


/// @relates MvnNodeMap, MvMgr
/// @brief MvnNodeMap の内容を出力する．
void
dump_mvnode_map(ostream& s,
		const MvnMgr& mvmgr,
		const MvnNodeMap& mvnode_map);

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_CONV_MVNNODEMAP_H
