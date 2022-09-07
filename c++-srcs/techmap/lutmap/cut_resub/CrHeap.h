#ifndef MAGUS_LUTMAP_CRHEAP_H
#define MAGUS_LUTMAP_CRHEAP_H

/// @file lutmap/CrHeap.h
/// @brief CrHeap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"


BEGIN_NAMESPACE_LUTMAP

class CrNode;

//////////////////////////////////////////////////////////////////////
/// @class CrHeap CrHeap.h "CrHeap.h"
/// @brief CrNode 用のヒープ木
//////////////////////////////////////////////////////////////////////
class CrHeap
{
public:

  /// @brief コンストラクタ
  CrHeap() = default;

  /// @brief デストラクタ
  ~CrHeap() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  init(
    SizeType max_n ///< [in] 最大要素数
  );

  /// @brief ヒープに追加する．
  void
  put(
    CrNode* node ///< [in] 追加するノード
  );

  /// @brief ヒープの先頭要素を抜き出す．
  CrNode*
  get();

  /// @brief ヒープから削除する．
  void
  remove(
    CrNode* node ///< [in] 削除するノード
  );

  /// @brief 値の更新に伴ってヒープの再構築を行う．
  void
  update(
    CrNode* node, ///< [in] ノード
    SizeType gain ///< [in] 値
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 引数の位置にある要素を適当な位置まで沈めてゆく
  void
  move_down(
    SizeType pos ///< [in] 位置
  );

  /// @brief 引数の位置にある要素を適当な位置まで上げてゆく
  void
  move_up(
    SizeType pos ///< [in] 位置
  );

  /// @brief pos の位置にノードを置く
  void
  set(
    SizeType pos, ///< [in] 位置
    CrNode* node  ///< [in] ノード
  );

  /// @brief pos の位置の要素を返す．
  CrNode*
  get(
    SizeType pos ///< [in] 位置
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ゲインを指標としたヒープ木
  vector<CrNode*> mHeap;

  // ヒープ木の要素数
  SizeType mNum{0};

};

END_NAMESPACE_LUTMAP

#endif // MAGUS_LUTMAP_CRHEAP_H
