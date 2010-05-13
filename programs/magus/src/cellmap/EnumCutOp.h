#ifndef MAGUS_CELLMAP_ENUMCUTOP_H
#define MAGUS_CELLMAP_ENUMCUTOP_H

/// @file magus/cellmap/EnumCutOp.h
/// @brief EnumCutOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EnumCutOp.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.


#include "cellmap.h"


BEGIN_NAMESPACE_MAGUS_CELLMAP

class SbjGraph;
class SbjNode;
class MapRecord;
class EnumCut;

/// @brief cut 列挙モード
enum tCutMode {
  /// @brief tree cut を列挙する
  kCmTree,
  /// @brief DF cut を列挙する
  kCmDF,
  /// @brief minimum depth cut を列挙する
  kCmMinDepth,
  /// @brief 全ての cut を列挙する
  kCmAll
};


//////////////////////////////////////////////////////////////////////
/// @class EnumCutOp
/// @brief EnumCut 中でカットを見つけた時に呼び出されるクラス
/// @note 実際にはこのクラスを継承したクラスを用意する必要がある．
//////////////////////////////////////////////////////////////////////
class EnumCutOp
{
  friend class EnumCut;
  friend class EnumCut2;
  
protected:

  /// @brief コンストラクタ
  EnumCutOp();

  /// @brief デストラクタ
  virtual
  ~EnumCutOp();


public:
  
  /// @brief カットの列挙を行う．
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit 入力数の制限
  /// @param[in] mode cut 列挙モード
  /// @return 全 cut 数を返す．
  /// @note 内部で下の仮想関数が呼び出される．
  size_t
  enum_cut(const SbjGraph& sbjgraph,
	   size_t limit,
	   tCutMode mode);


private:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが定義する必要のある仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 処理の最初に呼ばれる関数
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @param[in] mode カット列挙モード
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  all_init(const SbjGraph& sbjgraph,
	   size_t limit,
	   tCutMode mode);
  
  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  /// @param[in] node 根のノード
  /// @param[in] cur_pos node の処理順
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  node_init(SbjNode* node,
	    size_t cur_pos);

  /// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
  /// @param[in] root 根のノード
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  found(SbjNode* root);

  /// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
  /// @param[in] root 根のノード
  /// @param[in] ni 入力数
  /// @param[in] inputs 入力ノードの配列
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  found(SbjNode* root,
	size_t ni,
	SbjNode* inputs[]);

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  /// @param[in] node 根のノード
  /// @param[in] cur_pos node の処理順
  /// @param[in] ncuts 列挙されたカット数
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  node_end(SbjNode* node,
	   size_t cur_pos,
	   size_t ncuts);

  /// @brief 処理の最後に呼ばれる関数
  /// @note デフォルトの実装ではなにもしない．
  virtual
  void
  all_end(const SbjGraph& sbjgraph,
	  size_t limit,
	  tCutMode mode);
  
};

END_NAMESPACE_MAGUS_CELLMAP

#endif // MAGUS_CELLMAP_ENUMCUTOP_H
