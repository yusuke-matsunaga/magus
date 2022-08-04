#ifndef DGMGR_H
#define DGMGR_H

/// @file DgMgr.h
/// @brief DgMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "dg.h"
#include "ym/Bdd.h"
#include "ym/BddMgr.h"
#include "ym/bnet.h"


BEGIN_NAMESPACE_DG

class DgEdge;
class DgNode;
class NodeMark;

//////////////////////////////////////////////////////////////////////
/// @class DgMgr DgMgr.h "DgMgr.h"
/// @brief Disjoint Decomposition を行うクラス
//////////////////////////////////////////////////////////////////////
class DgMgr
{
public:

  /// @brief コンストラクタ
  DgMgr();

  /// @brief デストラクタ
  ~DgMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 与えられた関数の Disjoint Decomposition を求める．
  /// @return 分解結果の BnNetwork を返す．
  BnNetwork
  decomp(
    const Bdd& func, ///< [in] 分解を行う関数
    SizeType ni      ///< [in] 入力数
  );

  /// @brief 与えられた関数の DgGraph を得る．
  DgEdge
  make_dg(
    const Bdd& func ///< [in] 分解を行う関数
  );


public:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief decomp の下請け関数
  DgEdge
  decomp_step(
    const Bdd& func ///< [in] 分解を行う関数
  );

  /// @brief コファクターの結果をマージする．
  DgEdge
  merge(
    SizeType index, ///< [in] コファクターを行った変数のインデックス
    DgEdge r0,      ///< [in] 0のコファクターの結果
    DgEdge r1       ///< [in] 1のコファクターの結果
  );

  /// @brief 共通でないファンインを求める．
  void
  find_uncommon_inputs(
    const vector<DgNode*>& node_list, ///< [in] ノードリスト
    NodeMark& mark,                   ///< [in] ノードマーク
    int mval,                         ///< [in] マークの値
    vector<DgEdge>& inputs            ///< [out] 境界ノードを格納するリスト
  );

  /// @brief 共通なファンインを求める．
  void
  find_common_inputs(
    const vector<DgNode*>& node_list1, ///< [in] ノードリスト1
    const vector<DgNode*>& node_list2, ///< [in] ノードリスト2
    NodeMark& mark,                    ///< [in] ノードマーク
    vector<DgEdge>& inputs             ///< [out] 境界ノードを格納するリスト
  );

  /// @brief Case1-OR のマージを行う．
  DgEdge
  case1_or(
    SizeType index,                    ///< [in] インデックス
    const vector<DgEdge>& common_list, ///< [in] 共通の子ノードのリスト
    const vector<DgEdge>& rest0_list,  ///< [in] node0のみの子ノードのリスト
    const vector<DgEdge>& rest1_list   ///< [in] node1のみの子ノードのリスト
  );

  /// @brief Case1-XOR のマージを行う．
  DgEdge
  case1_xor(
    SizeType index,                    ///< [in] インデックス
    const vector<DgEdge>& common_list, ///< [in] 共通の子ノードのリスト
    const vector<DgEdge>& rest0_list,  ///< [in] node0のみの子ノードのリスト
    const vector<DgEdge>& rest1_list,  ///< [in] node1のみの子ノードのリスト
    bool inv0,                         ///< [in] node0の反転フラグ
    bool inv1                          ///< [in] node1の反転フラグ
  );

  /// @brief Case1-CPLX のマージを行う．
  DgEdge
  case1_cplx(
    const Bdd& f,                      ///< [in] グローバル関数
    SizeType index,                    ///< [in] インデックス
    DgNode* node0,                     ///< [in] ノード0
    DgNode* node1,                     ///< [in] ノード1
    const vector<DgEdge>& common_list, ///< [in] 共通の子ノードのリスト
    DgEdge rest0,                      ///< [in] node0のみの子ノード
    DgEdge rest1                       ///< [in] node1のみの子ノード
  );

  /// @brief CASE1-CPLX(Type 2) のマージを行う．
  DgEdge
  case1_cplx2(
    const Bdd& f,                     ///< [in] グローバル関数
    SizeType index,                   ///< [in] インデックス
    const vector<DgEdge>& child_list, ///< [in] 子ノードのリスト
    SizeType pos                      ///< [in] マージ対象の子ノードの位置
  );

  /// @brief CASE2-OR のマージを行う．
  DgEdge
  case2_or(
    SizeType index, ///< [in] インデックス
    bool inv,       ///< [in] インデックスの極性
    DgNode* node0,  ///< [in] ORノード
    bool inv0,      ///< [in] node0 の極性
    SizeType pos    ///< [in] 子ノードの位置
  );

  /// @brief CASE2-XOR のマージを行う．
  DgEdge
  case2_xor(
    SizeType index, ///< [in] インデックス
    bool inv,       ///< [in] インデックスの極性
    DgNode* node0,  ///< [in] XORノード
    bool inv0,      ///< [in] node0 の極性
    SizeType pos,   ///< [in] 子ノードの位置
    bool inv1       ///< [in] 他方の枝の極性
  );

  /// @brief CASE2-CPLX のマージを行う．
  DgEdge
  case2_cplx(
    const Bdd& f,    ///< [in] グローバル関数
    SizeType index,  ///< [in] インデックス
    DgEdge cedge,    ///< [in] マージ対象の枝
    bool inv1,
    bool inv2,
    DgNode* node
  );

  /// @brief LITノードを作る．
  DgEdge
  make_lit(
    SizeType index ///< [in] 変数番号
  );

  /// @brief ORノードを作る．
  DgEdge
  make_or(
    const vector<DgEdge>& child_list ///< [in] 子ノードのリスト
  );

  /// @brief ORノードを作る．
  DgEdge
  make_xor(
    const vector<DgEdge>& child_list ///< [in] 子ノードのリスト
  );

  /// @brief LITノードとのANDを作る．
  DgEdge
  make_lit_and(
    SizeType index, ///< [in] 変数番号
    bool inv,       ///< [in] 反転属性
    DgEdge e        ///< [in] もう一方の枝
  );

  /// @brief LITノードとのORを作る．
  DgEdge
  make_lit_or(
    SizeType index, ///< [in] 変数番号
    bool inv,       ///< [in] 反転属性
    DgEdge e        ///< [in] もう一方の枝
  );

  /// @brief LITノードとのXORを作る．
  DgEdge
  make_lit_xor(
    SizeType index, ///< [in] 変数番号
    bool inv,       ///< [in] 反転属性
    DgEdge e        ///< [in] もう一方の枝
  );

  /// @brief CPLXノードを作る．
  DgEdge
  make_cplx(
    const Bdd& f,                    ///< [in] グローバル関数
    const vector<DgEdge>& child_list ///< [in] 子ノードのリスト
  );

  /// @brief 子供が全てリテラルの CPLXノードを作る．
  DgEdge
  make_simple_cplx(
    const Bdd& f,
    const vector<SizeType>& sup_list
  );

  /// @brief 関数に対応する DgEdge を探す．
  /// @retval true 存在した．
  /// @retval false 存在しなかった．
  bool
  find_node(
    const Bdd& f,  ///< [in] 対象の関数
    DgEdge& result ///< [out] 結果の枝を格納する変数
  ) const;

  /// @brief 関数に対応する DgEdge を登録する．
  void
  put_node(
    const Bdd& f, ///< [in] 対象の関数
    DgEdge result ///< [in] 分解結果の枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BDDマネージャ
  BddMgr mBddMgr;

  // DgNode のリスト
  vector<unique_ptr<DgNode>> mNodeList;

  // 関数をキーにしてDgEdgeを記録する辞書
  unordered_map<Bdd, DgEdge> mEdgeDict;

};

END_NAMESPACE_DG

#endif // DGMGR_H
