#ifndef CIPATGRAPH_H
#define CIPATGRAPH_H

/// @file CiPatGraph.h
/// @brief CiPatGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "cell/CellPatGraph.h"
#include "utils/Alloc.h"
#include "utils/IDO.h"
#include "utils/ODO.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiPatGraph CiPatGraph.h "CiPatGraph.h"
/// @brief CellPatGraph の実装クラス
//////////////////////////////////////////////////////////////////////
class CiPatGraph :
  public CellPatGraph
{
public:

  /// @brief コンストラクタ
  CiPatGraph();

  /// @brief デストラクタ
  virtual
  ~CiPatGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // CellPatGraph の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 代表関数番号を返す．
  virtual
  ymuint
  rep_id() const;

  /// @brief 根のノード番号を返す．
  virtual
  ymuint
  root_id() const;

  /// @brief 根の反転属性を返す．
  virtual
  bool
  root_inv() const;

  /// @brief 入力数を返す．
  virtual
  ymuint
  input_num() const;

  /// @brief 枝数を返す．
  virtual
  ymuint
  edge_num() const;

  /// @brief 枝(の番号)を返す．
  /// @param[in] pos 位置 ( 0 <= pos < edge_num() )
  virtual
  ymuint
  edge(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリダンプ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリダンプを行う．
  /// @param[in] bos 出力先のストリーム
  void
  dump(ODO& bos) const;

  /// @brief バイナリファイルを読み込む．
  /// @param[in] bis 入力元のストリーム
  /// @param[in] alloc メモリアロケータ
  void
  restore(IDO& bis,
	  Alloc& alloc);


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @param[in] rep_id 代表番号
  /// @param[in] input_num 入力数
  /// @param[in] edge_num 枝数
  /// @param[in] alloc メモリアロケータ
  void
  init(ymuint rep_id,
       ymuint input_num,
       ymuint edge_num,
       Alloc& alloc);

  /// @brief 枝のデータを設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < edge_num() )
  /// @param[in] edge 枝
  /// @note この関数を呼ぶ前に init() が呼ばれている必要がある．
  void
  set_edge(ymuint pos,
	   ymuint edge);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief mEdgeList を確保する．
  /// @param[in] alloc メモリアロケータ
  /// @note mEdgeNum に値が設定されているものとする．
  void
  alloc_array(Alloc& alloc);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 代表関数番号
  ymuint32 mRepId;

  // 入力数 + 根の反転属性
  ymuint32 mInputNum;

  // 枝数
  ymuint32 mEdgeNum;

  // 枝番号の配列
  ymuint32* mEdgeList;

};

END_NAMESPACE_YM_CELL

#endif // CIPATGRAPH_H
