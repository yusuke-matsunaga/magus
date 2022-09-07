#ifndef ENUMCUTOP_H
#define ENUMCUTOP_H

/// @file EnumCutOp.h
/// @brief EnumCutOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"
#include "SbjGraph.h"


BEGIN_NAMESPACE_LUTMAP

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
  /// @return 全 cut 数を返す．
  ///
  /// 内部で下の仮想関数が呼び出される．
  SizeType
  enum_cut(
    const SbjGraph& sbjgraph, ///< [in] 対象のサブジェクトグラフ
    SizeType limit            ///< [in] 入力数の制限
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが定義する必要のある仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 処理の最初に呼ばれる関数
  ///
  /// デフォルトの実装ではなにもしない．
  virtual
  void
  all_init(
    const SbjGraph& sbjgraph, ///< [in] 対象のサブジェクトグラフ
    SizeType limit            ///< [in] カットサイズ
  );

  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  ///
  /// デフォルトの実装ではなにもしない．
  virtual
  void
  node_init(
    const SbjNode* node, ///< [in] 根のノード
    SizeType cur_pos     ///< [in] node の処理順
  );

  /// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
  ///
  /// デフォルトの実装ではなにもしない．
  virtual
  void
  found(
    const SbjNode* root ///< [in] 根のノード
  );

  /// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
  ///
  /// デフォルトの実装ではなにもしない．
  virtual
  void
  found(
    const SbjNode* root,    ///< [in] 根のノード
    SizeType ni,            ///< [in] 入力数
    const SbjNode* inputs[] ///< [in] 入力ノードの配列
  );

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  ///
  /// デフォルトの実装ではなにもしない．
  virtual
  void
  node_end(
    const SbjNode* node, ///< [in] 根のノード
    SizeType cur_pos,    ///< [in] node の処理順
    SizeType ncuts       ///< [in] 列挙されたカット数
  );

  /// @brief 処理の最後に呼ばれる関数
  ///
  /// デフォルトの実装ではなにもしない．
  virtual
  void
  all_end(
    const SbjGraph& sbjgraph, ///< [in] 対象のサブジェクトグラフ
    SizeType limit	      ///< [in] カットサイズ
  );

};

END_NAMESPACE_LUTMAP

#endif // MAGUS_LUTMAP_ENUMCUTOP_H
