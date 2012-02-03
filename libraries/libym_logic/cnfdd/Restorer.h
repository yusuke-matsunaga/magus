#ifndef RESTORER_H
#define RESTORER_H

/// @file Restorer.h
/// @brief Restorer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CNFddMgrImpl.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class Restorer Restorer.h "Restorer.h"
/// @brief ダンプされた内容を読み込むためのクラス
//////////////////////////////////////////////////////////////////////
class Restorer
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  /// @param[in] s 入力元のストリーム
  Restorer(CNFddMgrImpl* mgr,
	   BinI& s);

  /// @brief デストラクタ
  ~Restorer();


public:

  /// @brief 読み込む．
  /// @return 読み込んだCNFDDの根の枝の数を返す．
  /// @note エラーが起きたら 0 を返す．
  ymuint
  read();

  /// @brief pos 番目の枝を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < read() の返り値 )
  CNFddEdge
  root(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部の枝を指す文字列から枝を得る．
  /// @return 見つからなければ kEdgeError を返す．
  CNFddEdge
  find_edge(const char* str) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  CNFddMgrImpl* mMgr;

  // 入力用のストリーム
  BinI& mStream;

  // 根の枝を格納しておくベクタ
  vector<CNFddEdge> mRootVector;

  // 内部の枝を格納しておくベクタ
  vector<CNFddEdge> mEdgeVector;

};

END_NAMESPACE_YM_CNFDD

#endif // RESTORER_H
