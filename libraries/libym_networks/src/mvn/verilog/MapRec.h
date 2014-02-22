#ifndef MAPREC_H
#define MAPREC_H

/// @file MapRec.h
/// @brief MapRec のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "networks/mvn.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
/// @class MapRec MapRec.h "MapRec.h"
/// @brief VlDecl か VlDeclArray + offset の情報を表すクラス
//////////////////////////////////////////////////////////////////////
class MapRec
{
  friend class MvnVlMap;

protected:

  /// @brief デストラクタ
  virtual
  ~MapRec() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素が単一要素の時に true を返す．
  virtual
  bool
  is_single_elem() const = 0;

  /// @brief 宣言要素が配列要素の時に true を返す．
  virtual
  bool
  is_array_elem() const = 0;

  /// @brief 宣言要素を返す．(単一要素版)
  /// @note is_single_elem() == false の時は NULL が返される．
  virtual
  const VlDecl*
  get_single_elem() const = 0;

  /// @brief 宣言要素を返す．(配列要素版)
  /// @note is_array_elem(id) == false の時は NULL が返される．
  virtual
  const VlDeclArray*
  get_array_elem() const = 0;

  /// @brief 宣言要素のオフセットを返す．(配列要素版)
  /// @note is_array_elem(id) == false の時は 0 が返される．
  virtual
  ymuint
  get_array_offset() const = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class SingleMapRec MapRec.h "MapRec.h"
/// @brief 単一要素用の MapRec
//////////////////////////////////////////////////////////////////////
class SingleMapRec :
  public MapRec
{
  friend class MvnVlMap;

private:

  /// @brief コンストラクタ
  /// @param[in] decl 単一の宣言要素
  SingleMapRec(const VlDecl* decl);

  /// @brief デストラクタ
  virtual
  ~SingleMapRec();


public:
  //////////////////////////////////////////////////////////////////////
  // MapRec の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素が単一要素の時に true を返す．
  bool
  is_single_elem() const;

  /// @brief 宣言要素が配列要素の時に true を返す．
  bool
  is_array_elem() const;

  /// @brief 宣言要素を返す．(単一要素版)
  /// @note is_single_elem() == false の時は NULL が返される．
  const VlDecl*
  get_single_elem() const;

  /// @brief 宣言要素を返す．(配列要素版)
  /// @note is_array_elem(id) == false の時は NULL が返される．
  const VlDeclArray*
  get_array_elem() const;

  /// @brief 宣言要素のオフセットを返す．(配列要素版)
  /// @note is_array_elem(id) == false の時は 0 が返される．
  ymuint
  get_array_offset() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言要素
  const VlDecl* mDecl;

};


//////////////////////////////////////////////////////////////////////
/// @class ArrayMapRec MapRec.h "MapRec.h"
/// @brief 配列要素用の MapRec
//////////////////////////////////////////////////////////////////////
class ArrayMapRec :
  public MapRec
{
  friend class MvnVlMap;

private:

  /// @brief コンストラクタ
  /// @param[in] declarray 配列要素
  /// @param[in] offset オフセット
  ArrayMapRec(const VlDeclArray* declarray,
	      ymuint offset);

  /// @brief デストラクタ
  virtual
  ~ArrayMapRec();


public:
  //////////////////////////////////////////////////////////////////////
  // MapRec の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 宣言要素が単一要素の時に true を返す．
  bool
  is_single_elem() const;

  /// @brief 宣言要素が配列要素の時に true を返す．
  bool
  is_array_elem() const;

  /// @brief 宣言要素を返す．(単一要素版)
  /// @note is_single_elem() == false の時は NULL が返される．
  const VlDecl*
  get_single_elem() const;

  /// @brief 宣言要素を返す．(配列要素版)
  /// @note is_array_elem(id) == false の時は NULL が返される．
  const VlDeclArray*
  get_array_elem() const;

  /// @brief 宣言要素のオフセットを返す．(配列要素版)
  /// @note is_array_elem(id) == false の時は 0 が返される．
  ymuint
  get_array_offset() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 宣言要素
  const VlDeclArray* mDeclArray;

  // オフセット
  ymuint mOffset;

};

END_NAMESPACE_YM_NETWORKS_MVN

#endif // MAPREC_H
