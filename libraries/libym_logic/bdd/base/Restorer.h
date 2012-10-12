#ifndef RESTORE_H
#define RESTORE_H

/// @file Restorer.h
/// @brief Restorer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrImpl.h"
#include "ym_utils/BinI.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// ダンプされた内容を読み込むためのクラス
//////////////////////////////////////////////////////////////////////
class Restorer
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr BddMgr
  /// @param[in] s 入力元のストリーム
  Restorer(BddMgrImpl* mgr,
	   BinI& s);

  /// @brief デストラクタ
  ~Restorer();


public:

  /// @brief 読み込む．
  /// @return 読み込んだBDDの根の枝の数を返す．
  /// @note エラーが起きたら 0 を返す．
  ymuint64
  read();

  /// @brief pos 番目の枝を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < read() の返り値 )
  BddEdge
  root(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを読んで次の枝を得る．
  /// @return 見つからなければ kEdgeError を返す．
  BddEdge
  read_edge();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BddMgrImpl へのポインタ
  BddMgrImpl* mMgr;

  // 入力用のストリーム
  BinI& mStream;

  // 根の枝を格納しておくベクタ
  vector<BddEdge> mRootVector;

  // 内部の枝を格納しておくベクタ
  vector<BddEdge> mEdgeVector;

};

END_NAMESPACE_YM_BDD

#endif
