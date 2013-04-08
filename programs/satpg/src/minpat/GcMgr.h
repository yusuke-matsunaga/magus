#ifndef GCMGR_H
#define GCMGR_H

/// @file GcMgr.h
/// @brief GcMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

class GcNode;

//////////////////////////////////////////////////////////////////////
/// @class GcMgr GcMgr.h "GcMgr.h"
/// @brief 彩色問題を解くためのクラス
//////////////////////////////////////////////////////////////////////
class GcMgr
{
public:

  /// @brief コンストラクタ
  GcMgr();

  /// @brief デストラクタ
  ~GcMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を指定して初期化する．
  void
  init(ymuint num);

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief 2つのノードを接続する．
  /// @param[in] id1, id2 2つのノードの番号 ( 0 <= id1, id2 < node_num() )
  void
  connect(ymuint id1,
	  ymuint id2);

  /// @brief 彩色する．
  /// @param[in] color_group 同じ色のノード番号のリストの配列
  /// @return 彩色数を返す．
  ymuint
  coloring(vector<vector<ymuint> >& color_group);

  /// @brief グラフの内容を出力する．
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード数
  ymuint32 mNodeNum;

  // ノードの実体の配列
  GcNode* mNodeArray;

};

END_NAMESPACE_YM_SATPG

#endif // GCMGR_H
