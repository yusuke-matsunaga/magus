#ifndef PHFGEN_H
#define PHFGEN_H

/// @file PhfGen.h
/// @brief PhfGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

class InputFunc;
class PhfNode;
class PhfEdge;

//////////////////////////////////////////////////////////////////////
/// @class PhfGen PhfGen.h "PhfGen.h"
/// @brief PHF を作るクラス
//////////////////////////////////////////////////////////////////////
class PhfGen
{
public:

  /// @brief コンストラクタ
  PhfGen();

  /// @brief デストラクタ
  ~PhfGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マッピングを求める．
  bool
  mapping(const vector<RegVect*>& vector_list,
	  const vector<const InputFunc*>& f_list,
	  vector<vector<ymuint32>* >& g_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを枝を開放する．
  void
  clear();

  /// @brief ノードを生成する．
  PhfNode*
  new_node(ymuint32 pat);

  /// @brief 枝を生成する．
  PhfEdge*
  new_edge(ymuint id,
	   const vector<PhfNode*>& node_list,
	   ymuint32 val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 生成したノードを入れておくリスト
  vector<PhfNode*> mNodeList;

  // 生成した枝を入れておくリスト
  vector<PhfEdge*> mEdgeList;

};

END_NAMESPACE_YM_IGF

#endif // PHFGEN_H
