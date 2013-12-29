#ifndef ENUMCUT_H
#define ENUMCUT_H

/// @file EnumCut.h
/// @brief EnumCut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"


BEGIN_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
/// @class EnumCutOp EnumCut.h "EnumCut.h"
/// @brief カットを見つけた時に呼ばれるクラス
//////////////////////////////////////////////////////////////////////
class EnumCutOp
{
public:

  /// @brief コンストラクタ
  EnumCutOp() { }

  /// @brief デストラクタ
  virtual
  ~EnumCutOp() { }


public:

  /// @brief 処理の最初に呼ばれる関数
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @param[in] mode カット列挙モード
  virtual
  void
  all_init(const BdnMgr& sbjgraph,
	   ymuint limit) = 0;

  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  /// @param[in] node 根のノード
  virtual
  void
  node_init(const BdnNode* node) = 0;

  /// @brief カットを見つけたときに呼ばれる関数
  virtual
  void
  found_cut(const BdnNode* root,
	    ymuint ni,
	    const BdnNode** inputs) = 0;

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  /// @param[in] node 根のノード
  virtual
  void
  node_end(const BdnNode* node) = 0;

  /// @brief 処理の最後に呼ばれる関数
  virtual
  void
  all_end(const BdnMgr& sbjgraph,
	  ymuint limit) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class EnumCut EnumCut.h "EnumCut.h"
/// @brief カット列挙を行う基底クラス
//////////////////////////////////////////////////////////////////////
class EnumCut
{

public:

  /// @brief カット列挙を行う．
  /// @param[in] network 対象のネットワーク
  /// @param[in] limit カットサイズの制限
  virtual
  void
  operator()(const BdnMgr& network,
	     ymuint limit,
	     EnumCutOp* op) = 0;

};

END_NAMESPACE_YM

#endif // ENUMCUT_H
