#ifndef AIGMGRIMPL_H
#define AIGMGRIMPL_H

/// @file AigMgrImpl.h
/// @brief AigMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/AigMgr.h"
#include "AigNode.h"
#include "ym_utils/SimpleAlloc.h"
#include "ym_utils/ItvlMgr.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class AigMgrImpl AigMgrImpl.h "AigMgrImpl.h"
/// @brief AigMgr の実装クラス
//////////////////////////////////////////////////////////////////////
class AigMgrImpl
{
public:

  /// @brief コンストラクタ
  AigMgrImpl();

  /// @brief デストラクタ
  virtual
  ~AigMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // AigMgr の機能を実現しているメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノード数を得る．
  ymuint
  input_num() const;

  /// @brief 入力ノードを取り出す．
  /// @param[in] id 入力番号
  AigNode*
  input_node(VarId id) const;

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief ノードを取り出す．
  /// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
  /// @note ANDノードの他に入力ノードも含まれる．
  AigNode*
  node(size_t pos) const;

  /// @brief 定数0関数をつくる．
  Aig
  make_zero();

  /// @brief 定数1関数をつくる．
  Aig
  make_one();

  /// @brief 外部入力を作る．
  /// @param[in] id 入力番号
  Aig
  make_input(VarId id);

  /// @brief 2つのノードの AND を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  Aig
  make_and(Aig edge1,
	   Aig edge2);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを作る．
  AigNode*
  new_node();

  /// @brief ハッシュ表を拡大する．
  /// @note 現在のサイズが0のときは適当な大きさに初期化される．
  static
  void
  alloc_table(AigNode**& table,
	      ymuint req_size,
	      ymuint& size,
	      ymuint& limit);


  /// @brief AigNode のハッシュ関数
  static
  ymuint
  hash_func(Aig handle1,
	    Aig handle2);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードを確保するためのアロケータ
  SimpleAlloc mAlloc;

  // ID 番号をキーにしたノードの配列
  // 全てのノードのリスト(mInputNodes + mAndNodes)
  vector<AigNode*> mAllNodes;

  // 入力ノード数
  ymuint32 mInputNum;

  // 入力ノードのハッシュ表
  AigNode** mInputHashTable;

  // 入力ノードのハッシュ表のサイズ
  ymuint32 mInputHashSize;

  // 入力ノードのハッシュ表を拡大する目安
  ymuint32 mInputNextLimit;

  // AND ノード数
  ymuint32 mAndNum;

  // AND ノードのハッシュ表
  AigNode** mAndHashTable;

  // AND ノードのハッシュ表のサイズ
  ymuint32 mAndHashSize;

  // AND ノードのハッシュ表を拡大する目安
  ymuint32 mAndNextLimit;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 入力ノード数を得る．
inline
ymuint
AigMgrImpl::input_num() const
{
  return mInputNum;
}

// @brief ノード数を得る．
inline
ymuint
AigMgrImpl::node_num() const
{
  return mAllNodes.size();
}

// @brief ノードを取り出す．
// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
// @note ANDノードの他に入力ノードも含まれる．
inline
AigNode*
AigMgrImpl::node(size_t pos) const
{
  return mAllNodes[pos];
}

// @brief 定数0関数をつくる．
inline
Aig
AigMgrImpl::make_zero()
{
  return Aig(NULL, false);
}

// @brief 定数1関数をつくる．
inline
Aig
AigMgrImpl::make_one()
{
  return Aig(NULL, true);
}

// AigNode のハッシュ関数
inline
ymuint
AigMgrImpl::hash_func(Aig handle1,
		      Aig handle2)
{
  return handle1.mPackedData + handle2.mPackedData;
}

END_NAMESPACE_YM_AIG

#endif // LIBYM_AIG_AIGMGRIMPL_H
