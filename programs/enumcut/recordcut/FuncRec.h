#ifndef FUNCREC_H
#define FUNCREC_H

/// @file FuncRec.h
/// @brief FuncRec のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"
#include "EnumCut.h"


BEGIN_NAMESPACE_YM

class FuncMgr;

//////////////////////////////////////////////////////////////////////
/// @class FuncRec FuncRec.h "FuncRec.h"
/// @brief カットを記録するためのクラス
//////////////////////////////////////////////////////////////////////
class FuncRec :
  public EnumCutOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] func_mgr FuncMgr
  FuncRec(FuncMgr& func_mgr);

  /// @brief デストラクタ
  virtual
  ~FuncRec();


public:
  //////////////////////////////////////////////////////////////////////
  // 制御用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 最小カットサイズを設定する．
  void
  set_min_size(ymuint size);

  /// @brief デバッグレベルを設定する．
  void
  set_debug_level(ymuint level);


public:
  //////////////////////////////////////////////////////////////////////
  // EnumCutOp の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 処理の最初に呼ばれる関数
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @param[in] mode カット列挙モード
  virtual
  void
  all_init(const BdnMgr& sbjgraph,
	   ymuint limit);

  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  /// @param[in] node 根のノード
  virtual
  void
  node_init(const BdnNode* node);

  virtual
  void
  found_cut(const BdnNode* root,
	    ymuint ni,
	    const BdnNode** inputs);

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  /// @param[in] node 根のノード
  virtual
  void
  node_end(const BdnNode* node);

  /// @brief 処理の最後に呼ばれる関数
  virtual
  void
  all_end(const BdnMgr& sbjgraph,
	  ymuint limit);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数を管理するオブジェクト
  FuncMgr& mFuncMgr;

  // 現在処理中のノード
  const BdnNode* mCurNode;

  // 現在処理中のノード番号
  ymuint32 mCurPos;

  // 現在のカット数
  ymuint32 mNcCur;

  // 全カット数
  ymuint32 mNcAll;

  // 最小カットサイズ
  ymuint32 mMinSize;

  // デバッグレベル
  ymuint32 mDebugLevel;

};

END_NAMESPACE_YM

#endif // FUNCREC_H
