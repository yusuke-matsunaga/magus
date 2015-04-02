#ifndef GENVIDMAP_H
#define GENVIDMAP_H

/// @file GenVidMap.h
/// @brief GenVidMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "VidMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class GenVidMap GenVidMap.h "GenVidMap.h"
/// @brief 汎用の VidMap
//////////////////////////////////////////////////////////////////////
class GenVidMap :
  public VidMap
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_id ノード番号の最大値
  GenVidMap(ymuint max_id);

  /// @brief デストラクタ
  virtual
  ~GenVidMap();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードに関連した変数番号を返す．
  /// @param[in] node 対象のノード
  virtual
  VarId
  operator()(const TpgNode* node) const;

  /// @brief ノードに関連した変数番号を設定する．
  /// @param[in] node 対象のノード
  /// @param[in] vid 変数番号
  void
  set_vid(const TpgNode* node,
	  VarId vid);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数番号を格納する配列
  vector<VarId> mVidArray;

};

END_NAMESPACE_YM_SATPG

#endif // GENVIDMAP_H
