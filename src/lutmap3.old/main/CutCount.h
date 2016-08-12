#ifndef CUTCOUNT_H
#define CUTCOUNT_H

/// @file CutCount.h
/// @brief CutCount のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "EnumCutOp.h"


BEGIN_NAMESPACE_YM_LUTMAP3

//////////////////////////////////////////////////////////////////////
/// @class CutCount CutCount.h "CutCount.h"
/// @brief 各ノードをカバーするカットの最大サイズを求めるためのクラス
//////////////////////////////////////////////////////////////////////
class CutCount :
  public EnumCutOp
{
public:

  /// @brief コンストラクタ
  CutCount();

  /// @brief デストラクタ
  ~CutCount();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 理論上の下界を計算する．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] limit カットサイズ
  double
  lower_bound(const SbjGraph& sbjgraph,
	      ymuint limit);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 処理の最初に呼ばれる関数
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  virtual
  void
  all_init(const SbjGraph& sbjgraph,
	   ymuint limit);

  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  /// @param[in] node 根のノード
  /// @param[in] cur_pos node の処理順
  virtual
  void
  node_init(const SbjNode* node,
	    ymuint cur_pos);

  /// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
  /// @param[in] root 根のノード
  virtual
  void
  found(const SbjNode* root);

  /// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
  /// @param[in] root 根のノード
  /// @param[in] ni 入力数
  /// @param[in] inputs 入力ノードの配列
  virtual
  void
  found(const SbjNode* root,
	ymuint ni,
	const SbjNode* inputs[]);

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  /// @param[in] node 根のノード
  /// @param[in] cur_pos node の処理順
  /// @param[in] ncuts 列挙されたカット数
  virtual
  void
  node_end(const SbjNode* node,
	   ymuint cur_pos,
	   ymuint ncuts);

  /// @brief 処理の最後に呼ばれる関数
  virtual
  void
  all_end(const SbjGraph& sbjgraph,
	  ymuint limit);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint mNodeNum;

  // 各ノードをカバーするカットの最大サイズを記録する配列
  // サイズは mNodeNum
  vector<ymuint> mCutSizeArray;

  // 作業用のマークを入れる配列
  // サイズは mNodeNum
  vector<bool> mMark;

};

END_NAMESPACE_YM_LUTMAP3

#endif // CUTCOUNT_H
