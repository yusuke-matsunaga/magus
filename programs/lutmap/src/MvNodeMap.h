#ifndef LUTMAP_SRC_MVNODEMAP_H
#define LUTMAP_SRC_MVNODEMAP_H

/// @file lutmap/src/MvNodeMap.h
/// @brief MvNodeMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_lutmap/lutmap_nsdef.h"
#include "ym_techmap/techmap_nsdef.h"
#include "ym_mvn/MvNode.h"


BEGIN_NAMESPACE_YM_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class MvNodeMap MvNodeMap.h "MvNodeMap.h"
/// @brief MvNode と SbjNode の対応を記録するクラス
//////////////////////////////////////////////////////////////////////
class MvNodeMap
{
public:

  /// @brief コンストラクタ
  /// @param[in] n MvNode の ID の最大値
  MvNodeMap(ymuint n);

  /// @brief デストラクタ
  ~MvNodeMap();


public:

  /// @brief 登録する．(1ビット版)
  /// @param[in] mvnode MvNode
  /// @param[in] sbjnode SbjNode
  /// @param[in] inv 反転属性
  void
  put(const nsMvn::MvNode* mvnode,
      SbjNode* sbjnode,
      bool inv);

  /// @brief 登録する．(ベクタ版)
  /// @param[in] mvnode MvNode
  /// @param[in] index ビット位置
  /// @param[in] sbjnode SbjNode
  /// @param[in] inv 反転属性
  void
  put(const nsMvn::MvNode* mvnode,
      ymuint index,
      SbjNode* sbjnode,
      bool inv);

  /// @brief 探す．(1ビット版)
  bool
  get(const nsMvn::MvNode* mvnode,
      SbjNode*& sbjnode,
      bool& inv) const;

  /// @brief 探す．(ベクタ版)
  bool
  get(const nsMvn::MvNode* mvnode,
      ymuint index,
      SbjNode*& sbjnode,
      bool& inv) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  vector<vector<ympuint> > mArray;

};

END_NAMESPACE_YM_LUTMAP

#endif // LUTMAP_SRC_MVNODEMAP_H
