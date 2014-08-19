#ifndef ENUMCUT2_H
#define ENUMCUT2_H

/// @file EnumCut.h
/// @brief EnumCut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/bdn.h"


BEGIN_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
/// @class EnumCutOp2 EnumCut2.h "EnumCut2.h"
/// @brief カットを見つけた時に呼ばれるクラス
//////////////////////////////////////////////////////////////////////
class EnumCutOp2
{
public:

  /// @brief コンストラクタ
  EnumCutOp2() { }

  /// @brief デストラクタ
  virtual
  ~EnumCutOp2() { }


public:

  /// @brief 処理の最初に呼ばれる関数
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @param[in] mode カット列挙モード
  virtual
  void
  all_init(BdnMgr& sbjgraph,
	   ymuint limit) = 0;

  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  /// @param[in] node 根のノード
  virtual
  void
  node_init(BdnNode* node) = 0;

  /// @brief カットを見つけたときに呼ばれる関数
  virtual
  void
  found_cut(BdnNode* root,
	    ymuint ni,
	    BdnNode** inputs) = 0;

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  /// @param[in] node 根のノード
  virtual
  void
  node_end(BdnNode* node) = 0;

  /// @brief 処理の最後に呼ばれる関数
  virtual
  void
  all_end(BdnMgr& sbjgraph,
	  ymuint limit) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class EnumCut2 EnumCut2.h "EnumCut2.h"
/// @brief カット列挙を行う基底クラス
//////////////////////////////////////////////////////////////////////
class EnumCut2
{

public:

  /// @brief カット列挙を行う．
  /// @param[in] network 対象のネットワーク
  /// @param[in] limit カットサイズの制限
  virtual
  void
  operator()(BdnMgr& network,
	     ymuint limit,
	     EnumCutOp2* op) = 0;

};

END_NAMESPACE_YM

#endif // ENUMCUT2_H
