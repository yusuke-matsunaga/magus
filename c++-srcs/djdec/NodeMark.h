#ifndef NODEMARK_H
#define NODEMARK_H

/// @file NodeMark.h
/// @brief NodeMark のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "dg.h"


BEGIN_NAMESPACE_DG

class DgNode;
class DgEdge;

//////////////////////////////////////////////////////////////////////
/// @class NodeMark NodeMark.h "NodeMark.h"
/// @brief DgNode のマークを管理するクラス
//////////////////////////////////////////////////////////////////////
class NodeMark
{
public:

  /// @brief コンストラクタ
  NodeMark() = default;

  /// @brief デストラクタ
  ~NodeMark() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 再帰的にマークをつける．
  void
  mark_recur(
    DgEdge edge, ///< [in] 枝
    int mval     ///< [in] マークの値
  );

  /// @brief 再帰的にTFIマークをつける．
  int
  tfimark_recur(
    DgEdge edge ///< [in] 枝
  );

  /// @brief マークに基づいて境界ノードに印をつける．
  int
  get_boundary(
    DgEdge edge,              ///< [in] 枝
    vector<DgNode*>& or_list, ///< [out] 境界ノードをファンインに持つ OR ノードのリスト
    vector<DgNode*>& xor_list ///< [out] 境界ノードをファンインに持つ XOR ノードのリスト
  );

  /// @brief 境界ノードを求める．
  void
  find_bnode(
    DgNode* node,          ///< [in] ノード
    vector<DgEdge>& inputs ///< [out] 結果のノードを格納するリスト
  );

  /// @brief マークを返す．
  int
  mark(
    DgNode* node ///< [in] ノード
  )
  {
    if ( mMarkDict.count(node) == 0 ) {
      mMarkDict.emplace(node, Info{});
    }
    auto& info = mMarkDict.at(node);
    return info.mark;
  }

  /// @brief マークをつける．
  void
  set_mark(
    DgNode* node, ///< [in] ノード
    int val       ///< [in] 値
  )
  {
    if ( mMarkDict.count(node) == 0 ) {
      mMarkDict.emplace(node, Info{});
    }
    auto& info = mMarkDict.at(node);
    info.mark = val;
  }

  /// @brief TFIマークを返す．
  int
  tfimark(
    DgNode* node ///< [in] ノード
  )
  {
    if ( mMarkDict.count(node) == 0 ) {
      mMarkDict.emplace(node, Info{});
    }
    auto& info = mMarkDict.at(node);
    return info.tfimark;
  }

  /// @brief TFIマークをつける．
  void
  set_tfimark(
    DgNode* node, ///< [in] ノード
    int val       ///< [in] 値
  )
  {
    if ( mMarkDict.count(node) == 0 ) {
      mMarkDict.emplace(node, Info{});
    }
    auto& info = mMarkDict.at(node);
    info.tfimark = val;
  }

  /// @brief マークと TFIマークが等しい時に true を返す．
  bool
  is_boundary(
    DgNode* node ///< [in] ノード
  )
  {
    if ( mMarkDict.count(node) == 0 ) {
      mMarkDict.emplace(node, Info{});
    }
    auto& info = mMarkDict.at(node);
    return info.mark == info.tfimark;
  }

  /// @brief 境界マークを返す．
  int
  bmark(
    DgNode* node ///< [in] ノード
  )
  {
    if ( mMarkDict.count(node) == 0 ) {
      mMarkDict.emplace(node, Info{});
    }
    auto& info = mMarkDict.at(node);
    return info.bmark;
  }

  /// @brief 境界マークをつける．
  void
  set_bmark(
    DgNode* node, ///< [in] ノード
    int val
  )
  {
    if ( mMarkDict.count(node) == 0 ) {
      mMarkDict.emplace(node, Info{});
    }
    auto& info = mMarkDict.at(node);
    info.bmark = val;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マークの情報
  struct Info
  {
    int mark{0};
    int tfimark{0};
    int bmark{0};
  };

  // ノードのポインタをキーにしてマークを格納する辞書
  unordered_map<DgNode*, Info> mMarkDict;

};

END_NAMESPACE_DG

#endif // NODEMARK_H
