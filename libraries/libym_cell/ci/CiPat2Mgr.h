#ifndef CIPAT2MGR_H
#define CIPAT2MGR_H

/// @file CiPat2Mgr.h
/// @brief CiPat2Mgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_utils/BinI.h"
#include "ym_utils/BinO.h"
#include "ym_utils/Alloc.h"
#include "libcomp/libcomp_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

class CiPat2Graph;

//////////////////////////////////////////////////////////////////////
/// @class CiPat2Mgr CiPat2Mgr.h "CiPat2Mgr.h"
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
class CiPat2Mgr
{
public:

  /// @brief コンストラクタ
  CiPat2Mgr();

  /// @brief デストラクタ
  ~CiPat2Mgr();


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

  /// @brief ノードがAND/XORノードの時にファンイン数を返す．
  /// @param[in] id ノード番号 ( 0 <= id < pg2_node_num() )
  /// @note AND/XORノードでない場合の返り値は不定
  ymuint
  fanin_num(ymuint id) const;

  /// @brief 入力のノード番号を返す．
  /// @param[in] input_id 入力番号 ( 0 <= input_id < input_num() )
  /// @return input_id の入力に対応するノードのノード番号
  ymuint
  input_node(ymuint input_id) const;

  /// @brief 総枝数を返す．
  ymuint
  edge_num() const;

  /// @brief 枝のファンイン元のノード番号を返す．
  /// @param[in] id 枝番号 ( 0 <= id < edge_num() )
  ymuint
  edge_from(ymuint id) const;

  /// @brief 枝のファンアウト先のノード番号を返す．
  /// @param[in] id 枝番号 ( 0 <= id < edge_num() )
  ymuint
  edge_to(ymint id) const;

  /// @brief 枝のファンアウト先の入力位置を返す．
  /// @param[in] id 枝番号 ( 0 <= id < edge_num() )
  ymuint
  edge_pos(ymuint id) const;

  /// @brief 枝の反転属性を返す．
  /// @param[in] id 枝番号 ( 0 <= id < edge_num() )
  bool
  edge_inv(ymuint id) const;

  /// @brief 総パタン数を返す．
  ymuint
  pat_num() const;

  /// @brief パタンを返す．
  /// @param[in] id パタン番号 ( 0 <= id < pat_num() )
  const CellPat2Graph&
  pat(ymuint id) const;

  /// @brief バイナリダンプを行う．
  /// @param[in] bos 出力先のストリーム
  void
  dump(BinO& bos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief LcPatMgr の情報をコピーする．
  /// @param[in] src コピー元
  /// @param[in] alloc メモリアロケータ
  void
  copy(const nsLibcomp::LcPat2Mgr& src,
       Alloc& alloc);

  /// @brief データを読み込んでセットする．
  /// @param[in] bis 入力元のストリーム
  /// @param[in] alloc メモリアロケータ
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  restore(BinI& bis,
	  Alloc& alloc);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を設定する．
  /// @param[in] nn ノード数
  /// @param[in] alloc メモリアロケータ
  void
  set_node_num(ymuint nn,
	       Alloc& alloc);

  /// @brief 枝数を設定する．
  /// @param[in] ne 枝数
  /// @param[in] alloc メモリアロケータ
  void
  set_edge_num(ymuint ne,
	       Alloc& alloc);

  /// @brief パタン数を設定する．
  /// @param[in] np パタン数
  /// @param[in] alloc メモリアロケータ
  void
  set_pat_num(ymuint np,
	      Alloc& alloc);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint32 mNodeNum;

  // ノードの種類+入力番号/ファンイン数を納めた配列
  // サイズは mNodeNum
  ymuint32* mNodeTypeArray;

  // 枝数
  ymuint32 mEdgeNum;

  // ファンインのノード番号＋反転属性を納めた配列
  // サイズは mEdgeNum * 3
  ymuint32* mEdgeArray;

  // パタン数
  ymuint32 mPatNum;

  // パタンの配列
  // サイズは mPatNum
  CiPat2Graph* mPatArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード数を返す．
inline
ymuint
CiPat2Mgr::node_num() const
{
  return mNodeNum;
}

// @brief ノードの種類を返す．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
inline
tCellPatType
CiPat2Mgr::node_type(ymuint id) const
{
  return static_cast<tCellPatType>(mNodeTypeArray[id] & 3U);
}

// @brief ノードが入力ノードの時に入力番号を返す．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
// @note 入力ノードでない場合の返り値は不定
inline
ymuint
CiPat2Mgr::input_id(ymuint id) const
{
  return (mNodeTypeArray[id] >> 2);
}

// @brief ノードがAND/XORノードの時にファンイン数を返す．
// @param[in] id ノード番号 ( 0 <= id < pg2_node_num() )
// @note AND/XORノードでない場合の返り値は不定
inline
ymuint
CiPat2Mgr::fanin_num(ymuint id) const
{
  if ( node_type(id) == kCellPatInput ) {
    return 0;
  }
  return (mNodeTypeArray[id] >> 2);
}

// @brief 入力のノード番号を返す．
// @param[in] input_id 入力番号 ( 0 <= input_id < input_num() )
// @return input_id の入力に対応するノードのノード番号
inline
ymuint
CiPat2Mgr::input_node(ymuint input_id) const
{
  return input_id;
}

// @brief 総枝数を返す．
inline
ymuint
CiPat2Mgr::edge_num() const
{
  return mEdgeNum;
}

// @brief 枝のファンイン元のノード番号を返す．
// @param[in] id 枝番号 ( 0 <= id < edge_num() )
inline
ymuint
CiPat2Mgr::edge_from(ymuint id) const
{
  return mEdgeArray[id * 3 + 0];
}

// @brief 枝のファンアウト先のノード番号を返す．
// @param[in] id 枝番号 ( 0 <= id < edge_num() )
inline
ymuint
CiPat2Mgr::edge_to(ymint id) const
{
  return mEdgeArray[id * 3 + 1];
}

// @brief 枝のファンアウト先の入力位置( 0 or 1 ) を返す．
// @param[in] id 枝番号 ( 0 <= id < edge_num() )
inline
ymuint
CiPat2Mgr::edge_pos(ymuint id) const
{
  return mEdgeArray[id * 3 + 2] >> 1;
}

// @brief 枝の反転属性を返す．
// @param[in] id 枝番号 ( 0 <= id < edge_num() )
inline
bool
CiPat2Mgr::edge_inv(ymuint id) const
{
  return static_cast<bool>(mEdgeArray[id * 3 + 2] & 1U);
}

// @brief 総パタン数を返す．
inline
ymuint
CiPat2Mgr::pat_num() const
{
  return mPatNum;
}

END_NAMESPACE_YM_CELL

#endif // CIPAT2MGR_H
