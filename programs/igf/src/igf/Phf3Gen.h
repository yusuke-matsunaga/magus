#ifndef PHF3GEN_H
#define PHF3GEN_H

/// @file Phf3Gen.h
/// @brief Phf3Gen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

class InputFunc;
class Phf3Node;
class Phf3Edge;

//////////////////////////////////////////////////////////////////////
/// @class Phf3Gen Phf3Gen.h "Phf3Gen.h"
/// @brief PHF を作るクラス
//////////////////////////////////////////////////////////////////////
class Phf3Gen
{
public:

  /// @brief コンストラクタ
  Phf3Gen();

  /// @brief デストラクタ
  ~Phf3Gen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マッピングを求める．
  bool
  mapping(const vector<const RegVect*>& vector_list,
	  const InputFunc& f1,
	  const InputFunc& f2,
	  const InputFunc& f3,
	  vector<ymuint32>& g1,
	  vector<ymuint32>& g2,
	  vector<ymuint32>& g3);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを枝を開放する．
  void
  clear();

  /// @brief ノードを生成する．
  Phf3Node*
  new_node(ymuint32 pat);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 生成したノードを入れておくリスト
  vector<Phf3Node*> mNodeList;

  // 生成した枝を入れておくリスト
  vector<Phf3Edge*> mEdgeList;

};

END_NAMESPACE_YM_IGF

#endif // PHF3GEN_H
