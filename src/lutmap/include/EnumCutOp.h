#ifndef ENUMCUTOP_H
#define ENUMCUTOP_H

/// @file EnumCutOp.h
/// @brief EnumCutOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "SbjGraph.h"


BEGIN_NAMESPACE_YM_LUTMAP

class EnumCut;

//////////////////////////////////////////////////////////////////////
/// @class EnumCutOp
/// @brief EnumCut 中でカットを見つけた時に呼び出されるクラス
/// @note 実際にはこのクラスを継承したクラスを用意する必要がある．
//////////////////////////////////////////////////////////////////////
class EnumCutOp
{
  friend class EnumCut;

protected:

  /// @brief コンストラクタ
  EnumCutOp();

  /// @brief デストラクタ
  virtual
  ~EnumCutOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カットの列挙を行う．
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit 入力数の制限
  /// @return 全 cut 数を返す．
  ///
  /// 内部で下の仮想関数が呼び出される．
  int
  enum_cut(const SbjGraph& sbjgraph,
	   int limit);


private:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが定義する必要のある仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 処理の最初に呼ばれる関数
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  ///
  /// デフォルトの実装ではなにもしない．
  virtual
  void
  all_init(const SbjGraph& sbjgraph,
	   int limit);

  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  /// @param[in] node 根のノード
  /// @param[in] cur_pos node の処理順
  ///
  /// デフォルトの実装ではなにもしない．
  virtual
  void
  node_init(const SbjNode* node,
	    int cur_pos);

  /// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
  /// @param[in] root 根のノード
  ///
  /// デフォルトの実装ではなにもしない．
  virtual
  void
  found(const SbjNode* root);

  /// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
  /// @param[in] root 根のノード
  /// @param[in] ni 入力数
  /// @param[in] inputs 入力ノードの配列
  ///
  /// デフォルトの実装ではなにもしない．
  virtual
  void
  found(const SbjNode* root,
	int ni,
	const SbjNode* inputs[]);

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  /// @param[in] node 根のノード
  /// @param[in] cur_pos node の処理順
  /// @param[in] ncuts 列挙されたカット数
  ///
  /// デフォルトの実装ではなにもしない．
  virtual
  void
  node_end(const SbjNode* node,
	   int cur_pos,
	   int ncuts);

  /// @brief 処理の最後に呼ばれる関数
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  all_end(const SbjGraph& sbjgraph,
	  int limit);

};

END_NAMESPACE_YM_LUTMAP

#endif // MAGUS_LUTMAP_ENUMCUTOP_H
