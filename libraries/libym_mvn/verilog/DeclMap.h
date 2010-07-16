#ifndef LIBYM_MVN_VERILOG_DECLMAP_H
#define LIBYM_MVN_VERILOG_DECLMAP_H

/// @file libym_mvn/verilog/DeclMap.h
/// @brief DeclMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_verilog/vl/VlFwd.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_MVN_VERILOG

using nsYm::nsVerilog::VlDecl;

//////////////////////////////////////////////////////////////////////
/// @class DeclMap DeclMap.h "DeclMap.h"
/// @brief VlDecl と MvNode の対応付けを行うハッシュ表
//////////////////////////////////////////////////////////////////////
class DeclMap
{
public:

  /// @brief コンストラクタ
  DeclMap();

  /// @brief デストラクタ
  ~DeclMap();


public:

  /// @brief 内部のデータをクリアする．
  void
  clear();

  /// @brief 登録する(単一要素の場合)
  /// @param[in] decl 宣言要素
  /// @param[in] node 対応するノード
  void
  add(const VlDecl* decl,
      MvNode* node);

  /// @brief 登録する(配列の場合)
  /// @param[in] decl 宣言要素
  /// @param[in] offset
  /// @param[in] node 対応するノード
  void
  add(const VlDecl* decl,
      ymuint offset,
      MvNode* node);

  /// @brief 対応するノードを取り出す．
  /// @param[in] decl 宣言要素
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型の場合には NULL を返す．
  MvNode*
  get(const VlDecl* decl) const;

  /// @brief 対応するノードを取り出す(配列型)．
  /// @param[in] decl 宣言要素
  /// @param[in] offset オフセット
  /// @return 対応するノードを返す．
  /// @note 登録されていない場合と配列型でない場合，
  /// オフセットが範囲外の場合には NULL を返す．
  MvNode*
  get(const VlDecl* decl,
      ymuint offset) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Cell
  {
    // 宣言要素
    const VlDecl* mDecl;

    // オフセット
    ymuint32 mOffset;
    
    // 対応するノード
    MvNode* mNode;

    // 次の要素を指すリンク
    Cell* mLink;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Cell を登録する．
  void
  put_cell(const VlDecl* decl,
	   ymuint offset,
	   MvNode* node);

  /// @brief Cell を探す．
  Cell*
  find_cell(const VlDecl* decl,
	    ymuint offset) const;

  /// @brief テーブルの領域を確保する．
  /// @param[in] size 必要なサイズ
  void
  alloc_table(ymuint size);

  /// @brief ハッシュ値を計算する．
  ymuint
  hash_func(const VlDecl* decl,
	    ymuint offset) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Cell の確保用アロケータ
  UnitAlloc mAlloc;

  // ハッシュ表のサイズ
  ymuint32 mSize;

  // ハッシュ表
  Cell** mTable;

  // ハッシュ表を拡大するしきい値
  ymuint32 mLimit;

  // 要素数
  ymuint32 mNum;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 登録する(単一要素の場合)
// @param[in] decl 宣言要素
// @param[in] node 対応するノード
inline
void
DeclMap::add(const VlDecl* decl,
	     MvNode* node)
{
  Cell* cell = find_cell(decl, 0);
  if ( cell ) {
    cell->mNode = node;
  }
  else {
    put_cell(decl, 0, node);
  }
}

// @brief 登録する(配列の場合)
// @param[in] decl 宣言要素
// @param[in] offset
// @param[in] node 対応するノード
inline
void
DeclMap::add(const VlDecl* decl,
	     ymuint offset,
	     MvNode* node)
{
  Cell* cell = find_cell(decl, offset);
  if ( cell ) {
    cell->mNode = node;
  }
  else {
    put_cell(decl, offset, node);
  }
}

// @brief 対応するノードを取り出す．
// @param[in] decl 宣言要素
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型の場合には NULL を返す．
inline
MvNode*
DeclMap::get(const VlDecl* decl) const
{
  Cell* cell = find_cell(decl, 0);
  if ( cell ) {
    return cell->mNode;
  }
  return NULL;
}

// @brief 対応するノードを取り出す(配列型)．
// @param[in] decl 宣言要素
// @param[in] offset オフセット
// @return 対応するノードを返す．
// @note 登録されていない場合と配列型でない場合，
// オフセットが範囲外の場合には NULL を返す．
inline
MvNode*
DeclMap::get(const VlDecl* decl,
	     ymuint offset) const
{
  Cell* cell = find_cell(decl, offset);
  if ( cell ) {
    return cell->mNode;
  }
  return NULL;
}

END_NAMESPACE_YM_MVN_VERILOG

#endif // LIBYM_MVN_VERILOG_DECLMAP_H
