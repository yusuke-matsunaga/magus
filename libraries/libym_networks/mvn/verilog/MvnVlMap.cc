
/// @file libym_networks/verilog/MvVlMap.cc
/// @brief MvVlMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/MvnVlMap.h"
#include "MapRec.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス MvnVlMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvnVlMap::MvnVlMap() :
  mAlloc(4096)
{
}

// @brief コピーコンストラクタ
MvnVlMap::MvnVlMap(const MvnVlMap& src) :
  mAlloc(4096),
  mArray(src.mArray.size())
{
  ymuint n = src.mArray.size();
  for (ymuint i = 0; i < n; ++ i) {
    const MapRec* src_rec = src.mArray[i];
    if ( src_rec ) {
      if ( src_rec->is_single_elem() ) {
	reg_node(i, src_rec->get_single_elem());
      }
      else {
	reg_node(i, src_rec->get_array_elem(), src_rec->get_array_offset());
      }
    }
  }
}

// @brief 代入演算子
const MvnVlMap&
MvnVlMap::operator=(const MvnVlMap& src)
{
  if ( &src != this) {
    clear();
    ymuint n = src.mArray.size();
    mArray.resize(n, NULL);
    for (ymuint i = 0; i < n; ++ i) {
      const MapRec* src_rec = src.mArray[i];
      if ( src_rec ) {
	if ( src_rec->is_single_elem() ) {
	  reg_node(i, src_rec->get_single_elem());
	}
	else {
	  reg_node(i, src_rec->get_array_elem(), src_rec->get_array_offset());
	}
      }
    }
  }
  return *this;
}

// @brief デストラクタ
MvnVlMap::~MvnVlMap()
{
  // mAlloc が全てのメモリを解放してくれる．
}

// @brief 内容をクリアする．
void
MvnVlMap::clear()
{
  mArray.clear();
  mAlloc.destroy();
}

// @brief 単一の宣言要素を登録する．
// @param[in] id MvNode の ID番号
// @param[in] decl 宣言要素
void
MvnVlMap::reg_node(ymuint id,
		  const VlDecl* decl)
{
  void* p = mAlloc.get_memory(sizeof(SingleMapRec));
  MapRec* rec = new (p) SingleMapRec(decl);
  put(id, rec);
}

// @brief 配列の宣言要素を登録する．
// @param[in] id MvNode の ID番号
// @param[in] declarray 配列宣言要素
// @param[in] offset オフセット
void
MvnVlMap::reg_node(ymuint id,
		  const VlDeclArray* declarray,
		  ymuint offset)
{
  void* p = mAlloc.get_memory(sizeof(ArrayMapRec));
  MapRec* rec = new (p) ArrayMapRec(declarray, offset);
  put(id, rec);
}

// @brief src_id の内容を dst_id にコピーする．
void
MvnVlMap::copy(ymuint src_id,
	       ymuint dst_id)
{
  put(dst_id, get(src_id));
}

// @brief id に対応する宣言要素が単一要素の時に true を返す．
// @param[in] id MvNode の ID番号
bool
MvnVlMap::is_single_elem(ymuint id) const
{
  MapRec* rec = get(id);
  if ( rec == NULL ) {
    return false;
  }
  return rec->is_single_elem();
}

// @brief id に対応する宣言要素が配列要素の時に true を返す．
// @param[in] id MvNode の ID番号
bool
MvnVlMap::is_array_elem(ymuint id) const
{
  MapRec* rec = get(id);
  if ( rec == NULL ) {
    return false;
  }
  return rec->is_array_elem();
}

// @brief id に対応する宣言要素を返す．(単一要素版)
// @param[in] id MvNode の ID番号
// @note is_single_elem(id) == false の時は NULL が返される．
const VlDecl*
MvnVlMap::get_single_elem(ymuint id) const
{
  MapRec* rec = get(id);
  if ( rec == NULL ) {
    return NULL;
  }
  return rec->get_single_elem();
}

// @brief id に対応する宣言要素を返す．(配列要素版)
// @param[in] id MvNode の ID番号
// @note is_array_elem(id) == false の時は NULL が返される．
const VlDeclArray*
MvnVlMap::get_array_elem(ymuint id) const
{
  MapRec* rec = get(id);
  if ( rec == NULL ) {
    return NULL;
  }
  return rec->get_array_elem();
}

// @brief id に対応する宣言要素のオフセットを返す．(配列要素版)
// @param[in] id MvNode の ID番号
// @note is_array_elem(id) == false の時は 0 が返される．
ymuint
MvnVlMap::get_array_offset(ymuint id) const
{
  MapRec* rec = get(id);
  if ( rec == NULL ) {
    return 0;
  }
  return rec->get_array_offset();
}

// @brief 要素を設定する．
// @param[in] id ID番号
// @param[in] elem 設定する要素
void
MvnVlMap::put(ymuint id,
	      MapRec* elem)
{
  if ( mArray.size() <= id ) {
    mArray.resize(id + 1, NULL);
  }
  mArray[id] = elem;
}

// @brief 要素を取り出す．
// @param[in] id ID番号
// @note id が範囲外の時は NULL が返される．
MapRec*
MvnVlMap::get(ymuint id) const
{
  if ( mArray.size() <= id ) {
    return NULL;
  }
  return mArray[id];
}


//////////////////////////////////////////////////////////////////////
// クラス SingleMapRec
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] decl 単一の宣言要素
SingleMapRec::SingleMapRec(const VlDecl* decl) :
  mDecl(decl)
{
}

// @brief デストラクタ
SingleMapRec::~SingleMapRec()
{
}

// @brief 宣言要素が単一要素の時に true を返す．
bool
SingleMapRec::is_single_elem() const
{
  return true;
}

// @brief 宣言要素が配列要素の時に true を返す．
bool
SingleMapRec::is_array_elem() const
{
  return false;
}

// @brief 宣言要素を返す．(単一要素版)
// @note is_single_elem() == false の時は NULL が返される．
const VlDecl*
SingleMapRec::get_single_elem() const
{
  return mDecl;
}

// @brief 宣言要素を返す．(配列要素版)
// @note is_array_elem(id) == false の時は NULL が返される．
const VlDeclArray*
SingleMapRec::get_array_elem() const
{
  return NULL;
}

// @brief 宣言要素のオフセットを返す．(配列要素版)
// @note is_array_elem(id) == false の時は 0 が返される．
ymuint
SingleMapRec::get_array_offset() const
{
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス ArrayMapRec
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] declarray 配列要素
// @param[in] offset オフセット
ArrayMapRec::ArrayMapRec(const VlDeclArray* declarray,
			 ymuint offset) :
  mDeclArray(declarray),
  mOffset(offset)
{
}

// @brief デストラクタ
ArrayMapRec::~ArrayMapRec()
{
}

// @brief 宣言要素が単一要素の時に true を返す．
bool
ArrayMapRec::is_single_elem() const
{
  return false;
}

// @brief 宣言要素が配列要素の時に true を返す．
bool
ArrayMapRec::is_array_elem() const
{
  return true;
}

// @brief 宣言要素を返す．(単一要素版)
// @note is_single_elem() == false の時は NULL が返される．
const VlDecl*
ArrayMapRec::get_single_elem() const
{
  return NULL;
}

// @brief 宣言要素を返す．(配列要素版)
// @note is_array_elem(id) == false の時は NULL が返される．
const VlDeclArray*
ArrayMapRec::get_array_elem() const
{
  return mDeclArray;
}

// @brief 宣言要素のオフセットを返す．(配列要素版)
// @note is_array_elem(id) == false の時は 0 が返される．
ymuint
ArrayMapRec::get_array_offset() const
{
  return mOffset;
}

END_NAMESPACE_YM_NETWORKS
