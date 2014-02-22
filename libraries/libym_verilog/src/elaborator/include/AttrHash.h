#ifndef ATTRHASH_H
#define ATTRHASH_H

/// @file AttrHash.h
/// @brief AttrHash のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "verilog/verilog.h"
#include "utils/Alloc.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class AttrHash AttrHash.h "AttrHash.h"
/// @brief VlObj に付帯する VlAttribute を管理するクラス
//////////////////////////////////////////////////////////////////////
class AttrHash
{
public:

  /// @brief コンストラクタ
  /// @param[in] alloc メモリ確保用のアロケータ
  AttrHash(Alloc& alloc);

  /// @brief デストラクタ
  ~AttrHash();


public:

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 属性を追加する．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] def 定義側の属性の時 true とするフラグ
  /// @param[in] attr_list 付加する属性リスト
  void
  add(const VlObj* obj,
      bool def,
      ElbAttrList* attr_list);

  /// @brief 属性を取り出す．
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] def 定義側の属性の時 true とするフラグ
  ElbAttrList*
  find(const VlObj* obj,
       bool def) const;

  /// @brief このオブジェクトが使用しているメモリ量を返す．
  ymuint
  allocated_size() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Cell
  {
    // 対象のオブジェクト
    const VlObj* mObj;

    // 対応する属性リスト
    ElbAttrList* mAttrList[2];

    // ハッシュ上の次の要素を指すポインタ
    Cell* mLink;

  };


private:

  /// @brief 新しい Cell を生成する．
  /// @param[in] obj 対象のオブジェクト
  Cell*
  new_cell(const VlObj* obj);

  /// @brief オブジェクトから対応する Cell を取り出す．
  /// @param[in] obj 対象のオブジェクト
  Cell*
  find_cell(const VlObj* obj) const;

  /// @brief テーブルの領域を確保する．
  void
  alloc_table(ymuint size);

  /// @brief ハッシュ値を計算する．
  ymuint
  hash_func(const VlObj* obj) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Cell の確保用アロケータ
  Alloc& mAlloc;

  // ハッシュ表のサイズ
  ymuint32 mSize;

  // ハッシュ表
  Cell** mTable;

  // ハッシュ表を拡大するしきい値
  ymuint32 mLimit;

  // 要素数
  ymuint32 mNum;

};

END_NAMESPACE_YM_VERILOG

#endif // ATTRHASH_H
