#ifndef CIPATMGR_H
#define CIPATMGR_H

/// @file CiPatMgr.h
/// @brief CiPatMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiPatMgr CiPatMgr.h "CiPatMgr.h"
/// @brief パタングラフを管理するクラス
///
/// このクラスが持っている情報は
/// - パタングラフ
/// - 各々のパタングラフに対応する論理関数
/// - その論理関数とNPN同値な関数の集合
/// で，最後の関数がセルの論理関数と対応づけられる．
///
/// 情報の設定は専用形式のバイナリファイルを読み込むことでのみ行える．
/// バイナリファイルの生成は patgen/PatGen, pg_dump を参照のこと．
//////////////////////////////////////////////////////////////////////
class CiPatMgr
{
public:

  /// @brief コンストラクタ
  CiPatMgr();

  /// @brief デストラクタ
  ~CiPatMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを読み込んでセットする．
  /// @param[in] s 入力元のストリーム
  /// @param[in] alloc メモリアロケータ
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  load(istream& s,
       AllocBase& alloc);


public:
  //////////////////////////////////////////////////////////////////////
  // パタングラフ関係の情報取得用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パタンの最大の入力数を得る．
  ymuint
  max_input() const;

  /// @brief 総ノード数を返す．
  ymuint
  node_num() const;

  /// @brief ノードの種類を返す．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  tCellPatType
  node_type(ymuint id) const;

  /// @brief ノードが入力ノードの時に入力番号を返す．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  /// @note 入力ノードでない場合の返り値は不定
  ymuint
  input_id(ymuint id) const;

  /// @brief 入力のノード番号を返す．
  /// @param[in] input_id 入力番号 ( 0 <= input_id < input_num() )
  /// @return input_id の入力に対応するノードのノード番号
  ymuint
  input_node(ymuint input_id) const;

  /// @brief 総枝数を返す．
  ymuint
  edge_num() const;

  /// @brief 枝のファンイン元のノード番号を返す．
  /// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
  ymuint
  edge_from(ymuint id) const;

  /// @brief 枝のファンアウト先のノード番号を返す．
  /// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
  ymuint
  edge_to(ymint id) const;

  /// @brief 枝のファンアウト先の入力位置( 0 or 1 ) を返す．
  /// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
  ymuint
  edge_pos(ymuint id) const;

  /// @brief 枝の反転属性を返す．
  /// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
  bool
  edge_inv(ymuint id) const;

  /// @brief 総パタン数を返す．
  ymuint
  pat_num() const;

  /// @brief パタンを返す．
  /// @param[in] id パタン番号 ( 0 <= id < pat_num() )
  const CellPatGraph&
  pat(ymuint id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を初期化する．
  /// @note 以前確保されたメモリは開放される．
  void
  init();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint32 mNodeNum;

  // ノードの種類+入力番号を納めた配列
  // サイズは mNodeNum
  ymuint32* mNodeTypeArray;

  // ファンインのノード番号＋反転属性を納めた配列
  // サイズは mNodeNum * 2
  ymuint32* mEdgeArray;

  // パタン数
  ymuint32 mPatNum;

  // パタンの配列
  // サイズは mPatNum
  CellPatGraph* mPatArray;

};


/// @relates CiPatMgr
/// @brief CiPatMgr の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] patgraph パタングラフ
void
dump(ostream& s,
     const CiPatMgr& patgraph);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード数を返す．
inline
ymuint
CiPatMgr::node_num() const
{
  return mNodeNum;
}

// @brief ノードの種類を返す．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
inline
tCellPatType
CiPatMgr::node_type(ymuint id) const
{
  return static_cast<tCellPatType>(mNodeTypeArray[id] & 3U);
}

// @brief ノードが入力ノードの時に入力番号を返す．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
// @note 入力ノードでない場合の返り値は不定
inline
ymuint
CiPatMgr::input_id(ymuint id) const
{
  return (mNodeTypeArray[id] >> 2);
}

// @brief 入力のノード番号を返す．
// @param[in] input_id 入力番号 ( 0 <= input_id < input_num() )
// @return input_id の入力に対応するノードのノード番号
inline
ymuint
CiPatMgr::input_node(ymuint input_id) const
{
  return input_id;
}

// @brief 総枝数を返す．
inline
ymuint
CiPatMgr::edge_num() const
{
  return node_num() * 2;
}

// @brief 枝のファンイン元のノード番号を返す．
// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
inline
ymuint
CiPatMgr::edge_from(ymuint id) const
{
  return (mEdgeArray[id] >> 1);
}

// @brief 枝のファンアウト先のノード番号を返す．
// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
inline
ymuint
CiPatMgr::edge_to(ymint id) const
{
  return (id / 2);
}

// @brief 枝のファンアウト先の入力位置( 0 or 1 ) を返す．
// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
inline
ymuint
CiPatMgr::edge_pos(ymuint id) const
{
  return (id & 1U);
}

// @brief 枝の反転属性を返す．
// @param[in] id 枝番号 ( 0 <= id < node_num() * 2 )
inline
bool
CiPatMgr::edge_inv(ymuint id) const
{
  return static_cast<bool>(mEdgeArray[id] & 1U);
}

// @brief 総パタン数を返す．
inline
ymuint
CiPatMgr::pat_num() const
{
  return mPatNum;
}

END_NAMESPACE_YM_CELL

#endif // CIPATMGR_H
