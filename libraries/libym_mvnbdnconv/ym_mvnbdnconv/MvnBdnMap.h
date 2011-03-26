#ifndef YM_MVNBDNCONV_MVNBDNMAP_H
#define YM_MVNBDNCONV_MVNBDNMAP_H

/// @file ym_mvnbdnconv/MvnBdnMap.h
/// @brief MvnBdnMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvnbdnconv/mvnbdnconv_nsdef.h"
#include "ym_mvn/mvn_nsdef.h"
#include "ym_bdn/bdn_nsdef.h"


BEGIN_NAMESPACE_YM_MVNBDNCONV

//////////////////////////////////////////////////////////////////////
/// @class MvnBdnMap MvnBdnMap.h "MvnBdnMap.h"
/// @brief MvNode と BdNode の対応を記録するクラス
//////////////////////////////////////////////////////////////////////
class MvnBdnMap
{
public:

  /// @brief コンストラクタ
  /// @param[in] n MvNode の ID の最大値
  MvnBdnMap(ymuint n);

  /// @brief デストラクタ
  ~MvnBdnMap();


public:

  /// @brief 登録する．(1ビット版)
  /// @param[in] mvnode MvNode
  /// @param[in] handle BdnNodeHandle
  void
  put(const MvNode* mvnode,
      BdnNodeHandle handle);

  /// @brief 登録する．(ベクタ版)
  /// @param[in] mvnode MvNode
  /// @param[in] index ビット位置
  /// @param[in] handle BdnNodeHandle
  void
  put(const MvNode* mvnode,
      ymuint index,
      BdnNodeHandle handle);

  /// @brief 探す．(1ビット版)
  /// @param[in] mvnode MvNode
  /// @return 対応するハンドルを返す．
  BdnNodeHandle
  get(const MvNode* mvnode) const;

  /// @brief 探す．(ベクタ版)
  /// @param[in] mvnode MvNode
  /// @param[in] index ビット位置
  /// @return 対応するハンドルを返す．
  BdnNodeHandle
  get(const MvNode* mvnode,
      ymuint index) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  vector<vector<BdnNodeHandle> > mArray;

};


/// @relates MvnBdnMap, MvMgr
/// @brief MvnBdnMap の内容を出力する．
void
dump_mvnode_map(ostream& s,
		const MvMgr& mvmgr,
		const MvnBdnMap& mvnode_map);

END_NAMESPACE_YM_MVNBDNCONV

#endif // YM_MVNBDNCONV_MVNBDNMAP_H
