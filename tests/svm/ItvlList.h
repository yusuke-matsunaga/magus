#ifndef ITVLLIST_H
#define ITVLLIST_H

/// @file ItvlList.h
/// @brief ItvlList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_logic/Bdd.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class ItvlList ItvlList.h "ItvlList.h"
/// @brief 区間リストを管理するクラス
//////////////////////////////////////////////////////////////////////c
class ItvlList
{
public:

  /// @brief コンストラクタ
  ItvlList();

  /// @brief デストラクタ
  ~ItvlList();


public:

  /// @brief 該当する区間を探す．
  /// @param[in] val 値．この値が lb 以上，ub 未満であるような区間を探す．
  /// @param[out] lb 下限
  /// @param[out] ub 上限
  /// @param[out] f 対応する関数
  /// @retval true 区間が見つかった．
  /// @retval false 区間が見つからなかった．
  bool
  find(double val,
       double& lb,
       double& ub,
       Bdd& f);

  /// @brief 区間を追加する．
  /// @param[in] lb 下限
  /// @param[in] ub 上限
  /// @param[in] f 対応する関数
  void
  add(double lb,
      double ub,
      const Bdd& f);

  /// @brief 要素数を返す．
  ymuint
  size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Node
  {
    // 下限
    double mLb;

    // 上限
    double mUb;

    // 対応する関数
    Bdd mF;

    // 左の子供
    Node* mLchd;

    // 右の子供
    Node* mRchd;

    // 平衡度
    ymint8 mBalance;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief val を含む区間を探す．
  Node*
  find_node(double val,
	    Node* ptr);

  /// @brief ノードを追加する．
  bool
  add_node(Node* node,
	   Node*& ptr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // 根のノード
  Node* mRoot;

  // 要素数
  ymuint32 mSize;

};

END_NAMESPACE_YM

#endif // ITVLLIST_H
