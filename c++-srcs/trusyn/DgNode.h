#ifndef DGNODE_H
#define DGNODE_H

/// @file DgNode.h
/// @brief DgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "trusyn.h"


BEGIN_NAMESPACE_TRUSYN

class DgNode;

//////////////////////////////////////////////////////////////////////
/// @class DgEdge DgNode.h "DgNode.h"
/// @brief DgNode を指す枝を表すクラス
//////////////////////////////////////////////////////////////////////
class DgEdge
{
public:

  /// @brief 空のコンストラクタ
  DgEdge() = default;

  /// @brief ノードと飯店フラグを指定したコンストラクタ
  DgEdge(
    DgNode* node,    ///< [in] ノード
    bool inv = false ///< [in] 反転フラグ
  );

  /// @brief 定数0の枝を返す．
  static
  DgEdge
  make_zero()
  {
    return DgEdge{0UL};
  }

  /// @brief 定数1の枝を返す．
  static
  DgEdge
  make_one()
  {
    return DgEdge{1UL};
  }

  /// @brief デストラクタ
  ~DgEdge() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0の時に true を返す．
  bool
  is_zero() const
  {
    return mBody == 0UL;
  }

  /// @brief 定数1の時に true を返す．
  bool
  is_one() const
  {
    return mBody == 1UL;
  }

  /// @brief ノードを返す．
  DgNode*
  node() const
  {
    return reinterpret_cast<DgNode*>(mBody & ~1UL);
  }

  /// @brief 反転フラグを返す．
  bool
  inv() const
  {
    return static_cast<bool>(mBody & 1U);
  }

  /// @brief 反転していない枝を返す．
  DgEdge
  normal_edge() const
  )
  {
    return DgEdge{mBody & ~1UL};
  }

  /// @brief グローバル関数を返す．
  TvFunc
  global_func() const
  {
    auto f = node()->global_func();
    if ( inv() ) {
      f = ~f;
    }
    return f;
  }

  /// @brief 反転した枝を返す．
  DgEdge
  operator~() const
  {
    return DgEdge{mBody ^ 1UL};
  }

  /// @brief 自身を反転させる．
  /// @return 自分自身を返す．
  DgEdge&
  negate()
  {
    mBody ^= 1UL;
    return *this;
  }

  /// @brief 反転フラグをかける．
  DgEdge
  operator*(
    bool inv ///< [in] 反転フラグ
  )
  {
    return DgEdge{mBody ^ static_cast<ympuint>(inv)};
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const DgEdge& right
  )
  {
    return mBody == right.mBody;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const DgEdge& right
  )
  {
    return !operator==(right);
  }

  /// @brief 2つの枝が極性違いのときに true を返す．
  static
  bool
  check_complement(
    cosnt DgEdge& left,
    const DgEdge& right
  )
  {
    return (left.mBody ^ right.mBody) == 1UL;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  DgEdge(
    ympuint body
  ) : mBody{body}
  {
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  ympuint mBody{0UL};

};


//////////////////////////////////////////////////////////////////////
/// @class DgNode DgNode.h "DgNode.h"
/// @brief Disjoint Graph のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class DgNode
{
public:

  /// @brief コンストラクタ
  DgNode(
    const TvFunc& f,                 ///< [in] グローバル関数
    const vector<SizeType>& sup_list ///< [in] サポートリスト
  ) : mGlobalFunc{f},
      mSupList{sup_list}
  {
  }

  /// @brief デストラクタ
  virtual
  ~DgNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

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

  /// @brief 入力が全て LIT タイプの時 true を返す．
  virtual
  bool
  is_simple_cplx() const;

  /// @brief CPLXタイプの時に true を返す．
  virtual
  bool
  is_cplx() const;

  /// @brief グローバル関数を返す．
  const TvFunc&
  global_func() const;

  /// @brief 先頭の変数を返す．
  SizeType
  top() const
  {
    return mSupList[0];
  }

  /// @brief サポートリストを返す．
  const vector<SizeType>&
  sup_list() const
  {
    return mSupList;
  }

  /// @brief グローバル関数が1となるパタンを得る．
  virtual
  vector<int>
  pat_1() const;

  /// @brief グローバル関数が0となるパタンを得る．
  virtual
  vector<int>
  pat_0() const;

  /// @brief 外部入力をセンシタイズするためのパタンを得る．
  vector<int>
  pat_i() const;

  /// @brief pat_i でセンシタイズの対象となっている入力番号を返す．
  SizeType
  sense_ipos() const;

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // グローバル関数
  TvFunc mGlobalFunc;

  // サポートリスト
  vector<SizeType> mSupList;

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
    const TvFunc& f, ///< [in] グローバル関数
    SizeType var     ///< [in] 変数インデックス
  ) : DgNode{f, {var}}
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
    const TvFunc& f,                  ///< [in] グローバル関数
    const vector<SizeType>& sup_list, ///< [in] サポートリスト
    const vector<DgEdge>& child_list  ///< [in] 子ノードの枝のリスト
  ) : DgMidNode{f, sup_list},
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
    const TvFunc& f,                  ///< [in] グローバル関数
    const vector<SizeType>& sup_list, ///< [in] サポートリスト
    const vector<DgEdge>& child_list  ///< [in] 子ノードの枝のリスト
  ) : DgMidNode{f, sup_list, child_list}
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
    const TvFunc& f,                  ///< [in] グローバル関数
    const vector<SizeType>& sup_list, ///< [in] サポートリスト
    const vector<DgEdge>& child_list  ///< [in] 子ノードの枝のリスト
  ) : DgMidNode{f, sup_list, child_list}
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
    const TvFunc& f,                  ///< [in] グローバル関数
    const vector<SizeType>& sup_list, ///< [in] サポートリスト
    const vector<DgEdge>& child_list  ///< [in] 子ノードの枝のリスト
  ) : DgMidNode{f, sup_list, child_list}
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

};

END_NAMESPACE_TRUSYN

#endif // DGNODE_H
