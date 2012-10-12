#ifndef RWTMGR_H
#define RWTMGR_H

/// @file RwtMgr.h
/// @brief RwtMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"
#include "ym_logic/TvFunc.h"
#include "ym_utils/SimpleAlloc.h"
#include "ym_utils/BinI.h"
#include "ym_utils/BinO.h"


BEGIN_NAMESPACE_YM_NETWORKS

class RwtPat;
class RwtPatList;
class RwtNode;
class RwtPatGen;

//////////////////////////////////////////////////////////////////////
/// @class RwtMgr RwtMgr.h "RwtMgr.h"
/// @brief RwtPat を管理するクラス
/// @note 基本的には最初にデータをセットしたら後は読み出し専用
//////////////////////////////////////////////////////////////////////
class RwtMgr
{
public:

  /// @brief コンストラクタ
  RwtMgr();

  /// @brief デストラクタ
  ~RwtMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
  ymuint
  input_num() const;

  /// @brief 全ノード数を得る．
  ymuint
  node_num() const;

  /// @brief 関数に対応したパタンのリストを得る．
  /// @param[in] f 関数
  /// @return f に対応するパタンリストを返す．
  /// @note 見つからなければ NULL を返す．
  const RwtPatList*
  find_patlist(const TvFunc& f) const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  print(ostream& s) const;

  /// @brief 内容をバイナリダンプする．
  void
  dump(BinO& s) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をコピーする．
  void
  copy(const RwtPatGen& pg);

  /// @brief バイナリファイルを読み込んで内容を設定する．
  void
  restore(BinI& s);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数，ノード数，関数の数を設定する．
  /// @param[in] input_num 入力数
  /// @param[in] node_num ノード数
  /// @param[in] func_num 関数の数
  void
  init(ymuint input_num,
       ymuint node_num,
       ymuint func_num);

  /// @brief ノードの情報を設定する．
  /// @param[in] id ノード番号
  /// @param[in] type ノードの型
  /// @param[in] i0_id ファンイン0のノード番号 + 極性
  /// @param[in] i1_id ファンイン1のノード番号 + 極性
  void
  set_node(ymuint32 id,
	   ymuint32 type,
	   ymuint32 i0_id,
	   ymuint32 i1_id);

  /// @brief RwtPatList を生成する．
  /// @param[in] f 関数
  /// @param[in] np パタンの数
  RwtPatList*
  new_patlist(const TvFunc& f,
	      ymuint np);

  /// @brief パタンの情報を設定する．
  /// @param[in] pat パタン
  /// @param[in] node_num ノード数
  /// @param[in] input_num 入力数＋根の極性情報
  void
  set_pat(RwtPat* pat,
	  ymuint node_num,
	  ymuint input_num);

  /// @brief パタンのノードリストの情報を設定する．
  /// @param[in] pat パタン
  /// @param[in] pos ノードリスト中の位置
  /// @param[in] id ノード番号
  void
  set_pat_node(RwtPat* pat,
	       ymuint pos,
	       ymuint id);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // 入力数
  ymuint32 mInputNum;

  // 全ノード数
  ymuint32 mNodeNum;

  // ノードの配列
  // 0 〜 input_num() までは予約済み
  RwtNode* mNodeArray;

  // ハッシュ表のサイズ
  // ここでは = ハッシュ表の要素数
  ymuint32 mTableSize;

  // ハッシュ表
  RwtPatList** mTable;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 入力数を得る．
inline
ymuint
RwtMgr::input_num() const
{
  return mInputNum;
}

// @brief 全ノード数を得る．
inline
ymuint
RwtMgr::node_num() const
{
  return mNodeNum;
}

END_NAMESPACE_YM_NETWORKS

#endif // RWTMGR_H
