#ifndef DGNODE_H
#define DGNODE_H

/// @file DgNode.h
/// @brief DgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022, 2023 Yusuke Matsunaga
/// All rights reserved.

#include "dg.h"
#include "ym/Bdd.h"
#include "ym/BddVarSet.h"


BEGIN_NAMESPACE_DG

class DgEdge;

//////////////////////////////////////////////////////////////////////
/// @class DgNode DgNode.h "DgNode.h"
/// @brief Disjoint Graph のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class DgNode
{
public:

  /// @brief コンストラクタ
  DgNode(
    BddMgr& mgr,             ///< [in] BDDマネージャ
    SizeType id,             ///< [in] ID番号
    const Bdd& f,            ///< [in] グローバル関数
    const BddVarSet& support ///< [in] サポート
  ) : mBddMgr{mgr},
      mId{id},
      mGlobalFunc{f},
      mSupport{support},
      mPat1{f.get_onepath()},
      mPat0{f.get_zeropath()}
  {
  }

  /// @brief デストラクタ
  virtual
  ~DgNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  SizeType
  id() const {
    return mId;
  }

  /// @brief LITタイプの時に true を返す．
  virtual
  bool
  is_lit() const;

  /// @brief ORタイプの時に true を返す．
  virtual
  bool
  is_or() const;

  /// @brief XORタイプの時に true を返す．
  virtual
  bool
  is_xor() const;

  /// @brief CPLXタイプの時に true を返す．
  virtual
  bool
  is_cplx() const;

  /// @brief グローバル関数を返す．
  Bdd
  global_func() const
  {
    return mGlobalFunc;
  }

  /// @brief 先頭の変数を返す．
  SizeType
  top() const
  {
    return mSupport.top_var();
  }

  /// @brief サポートを返す．
  const BddVarSet&
  support() const
  {
    return mSupport;
  }

  /// @brief グローバル関数が1となるパタンを得る．
  Bdd
  pat_1() const
  {
    return mPat1;
  }

  /// @brief グローバル関数が0となるパタンを得る．
  Bdd
  pat_0() const
  {
    return mPat0;
  }

  /// @brief ローカル関数を求める．
  virtual
  Bdd
  local_func() const = 0;

  /// @brief subfunction の数を得る．
  virtual
  SizeType
  child_num() const;

  /// @brief subfunction を表す枝を得る．
  virtual
  DgEdge
  child(
    SizeType pos ///< [in] 位置 ( 0 <= pos < child_num() )
  ) const;

  /// @brief 内容を出力する．
  virtual
  void
  print(
    ostream& s ///< [in] 出力ストリーム
  ) const = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BDDマネージャを返す．
  BddMgr&
  mgr() const
  {
    return mBddMgr;
  }

  /// @brief print の共通部分
  void
  print_base(
    ostream& s,        ///< [in] 出力ストリーム
    const string& type ///< [in] 種類を表す文字列
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BDDマネージャ
  // 本体は DgMgr が持っている．
  BddMgr& mBddMgr;

  // ID番号
  SizeType mId;

  // グローバル関数
  Bdd mGlobalFunc;

  // サポート
  BddVarSet mSupport;

  // 出力が1となる入力パタン
  Bdd mPat1;

  // 出力が0となる入力パタン
  Bdd mPat0;

};


//////////////////////////////////////////////////////////////////////
/// @class DgLitNode DgNode.h "DgNode.h"
/// @brief LITタイプの DgNode
//////////////////////////////////////////////////////////////////////
class DgLitNode :
  public DgNode
{
public:

  /// @brief コンストラクタ
  DgLitNode(
    BddMgr& mgr,             ///< [in] BDDマネージャ
    SizeType id,             ///< [in] ID番号
    const Bdd& f,            ///< [in] グローバル関数
    const BddVarSet& support ///< [in] サポート
  ) : DgNode{mgr, id, f, support}
  {
  }

  /// @brief デストラクタ
  ~DgLitNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief LITタイプの時に true を返す．
  bool
  is_lit() const override;

  /// @brief ローカル関数を求める．
  Bdd
  local_func() const override;

  /// @brief 内容を出力する．
  void
  print(
    ostream& s ///< [in] 出力ストリーム
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class DgMidNode DgMidNode.h "DgMidNode.h"
/// @brief 子ノードを持つ共通の親クラス
//////////////////////////////////////////////////////////////////////
class DgMidNode :
  public DgNode
{
public:

  /// @brief コンストラクタ
  DgMidNode(
    BddMgr& mgr,                     ///< [in] BDDマネージャ
    SizeType id,                     ///< [in] ID番号
    const Bdd& f,                    ///< [in] グローバル関数
    const BddVarSet& support,        ///< [in] サポート
    const vector<DgEdge>& child_list ///< [in] 子ノードの枝のリスト
  ) : DgNode{mgr, id, f, support},
      mChildList{child_list}
  {
  }

  /// @brief デストラクタ
  ~DgMidNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief subfunction の数を得る．
  SizeType
  child_num() const override;

  /// @brief subfunction を表す枝を得る．
  DgEdge
  child(
    SizeType pos ///< [in] 位置 ( 0 <= pos < child_num() )
  ) const override;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief pint() の下請け処理を行う．
  void
  print_sub(
    ostream& s,        ///< [in] 出力ストリーム
    const string& type ///< [in] 種類を表す文字列
  ) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 子ノードの枝のリスト
  vector<DgEdge> mChildList;

};


//////////////////////////////////////////////////////////////////////
/// @class DgOrNode DgNode.h "DgNode.h"
/// @brief ORノードを表すクラス
//////////////////////////////////////////////////////////////////////
class DgOrNode :
  public DgMidNode
{
public:

  /// @brief コンストラクタ
  DgOrNode(
    BddMgr& mgr,                     ///< [in] BDDマネージャ
    SizeType id,                     ///< [in] ID番号
    const Bdd& f,                    ///< [in] グローバル関数
    const BddVarSet& support,        ///< [in] サポートリスト
    const vector<DgEdge>& child_list ///< [in] 子ノードの枝のリスト
  ) : DgMidNode{mgr, id, f, support, child_list}
  {
  }

  /// @brief デストラクタ
  ~DgOrNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ORタイプの時に true を返す．
  bool
  is_or() const override;

  /// @brief ローカル関数を求める．
  Bdd
  local_func() const override;

  /// @brief 内容を出力する．
  void
  print(
    ostream& s ///< [in] 出力ストリーム
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class DgXorNode DgNode.h "DgNode.h"
/// @brief XORノードを表すクラス
//////////////////////////////////////////////////////////////////////
class DgXorNode :
  public DgMidNode
{
public:

  /// @brief コンストラクタ
  DgXorNode(
    BddMgr& mgr,                     ///< [in] BDDマネージャ
    SizeType id,                     ///< [in] ID番号
    const Bdd& f,                    ///< [in] グローバル関数
    const BddVarSet& support,        ///< [in] サポートリスト
    const vector<DgEdge>& child_list ///< [in] 子ノードの枝のリスト
  ) : DgMidNode{mgr, id, f, support, child_list}
  {
  }

  /// @brief デストラクタ
  ~DgXorNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief XORタイプの時に true を返す．
  bool
  is_xor() const override;

  /// @brief ローカル関数を求める．
  Bdd
  local_func() const override;

  /// @brief 内容を出力する．
  void
  print(
    ostream& s ///< [in] 出力ストリーム
  ) const override;

};


//////////////////////////////////////////////////////////////////////
/// @class DgCplxNode DgCplxNode.h "DgCplxNode.h"
/// @brief CPLXノードを表すクラス
//////////////////////////////////////////////////////////////////////
class DgCplxNode :
  public DgMidNode
{
public:

  /// @brief コンストラクタ
  DgCplxNode(
    BddMgr& mgr,                     ///< [in] BDDマネージャ
    SizeType id,                     ///< [in] ID番号
    const Bdd& f,                    ///< [in] グローバル関数
    const BddVarSet& support,        ///< [in] サポートリスト
    const vector<DgEdge>& child_list ///< [in] 子ノードの枝のリスト
  ) : DgMidNode{mgr, id, f, support, child_list}
  {
  }

  /// @brief デストラクタ
  ~DgCplxNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief CPLXタイプの時に true を返す．
  bool
  is_cplx() const override;

  /// @brief ローカル関数を求める．
  Bdd
  local_func() const override;

  /// @brief 内容を出力する．
  void
  print(
    ostream& s ///< [in] 出力ストリーム
  ) const override;

};

END_NAMESPACE_DG

#endif // DGNODE_H
