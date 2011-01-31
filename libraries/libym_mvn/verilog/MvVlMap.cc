
/// @file libym_mvn/verilog/MvVlMap.cc
/// @brief MvVlMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvVlMap.h"
#include "MapRec.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvVlMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvVlMap::MvVlMap() :
  mAlloc(4096)
{
}

// @brief コピーコンストラクタ
MvVlMap::MvVlMap(const MvVlMap& src) :
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

// @brief デストラクタ
MvVlMap::~MvVlMap()
{
  // mAlloc が全てのメモリを解放してくれる．
}

// @brief 内容をクリアする．
void
MvVlMap::clear()
{
  mArray.clear();
  mAlloc.destroy();
}

// @brief 単一の宣言要素を登録する．
// @param[in] id MvNode の ID番号
// @param[in] decl 宣言要素
void
MvVlMap::reg_node(ymuint id,
		  const VlDecl* decl)
{
  if ( mArray.size() <= id ) {
    mArray.resize(id + 1, NULL);
  }
  void* p = mAlloc.get_memory(sizeof(SingleMapRec));
  mArray[id] = new (p) SingleMapRec(decl);
}

// @brief 配列の宣言要素を登録する．
// @param[in] id MvNode の ID番号
// @param[in] declarray 配列宣言要素
// @param[in] offset オフセット
void
MvVlMap::reg_node(ymuint id,
		  const VlDeclArray* declarray,
		  ymuint offset)
{
  if ( mArray.size() <= id ) {
    mArray.resize(id + 1, NULL);
  }
  void* p = mAlloc.get_memory(sizeof(ArrayMapRec));
  mArray[id] = new (p) ArrayMapRec(declarray, offset);
}

// @brief src_id の内容を dst_id にコピーする．
void
MvVlMap::copy(ymuint src_id,
	      ymuint dst_id)
{
  mArray[dst_id] = mArray[src_id];
}

// @brief id に対応する宣言要素が単一要素の時に true を返す．
// @param[in] id MvNode の ID番号
bool
MvVlMap::is_single_elem(ymuint id) const
{
  if ( mArray.size() <= id ) {
    return false;
  }
  return mArray[id]->is_single_elem();
}

// @brief id に対応する宣言要素が配列要素の時に true を返す．
// @param[in] id MvNode の ID番号
bool
MvVlMap::is_array_elem(ymuint id) const
{
  if ( mArray.size() <= id ) {
    return false;
  }
  return mArray[id]->is_array_elem();
}

// @brief id に対応する宣言要素を返す．(単一要素版)
// @param[in] id MvNode の ID番号
// @note is_single_elem(id) == false の時は NULL が返される．
const VlDecl*
MvVlMap::get_single_elem(ymuint id) const
{
  if ( mArray.size() <= id ) {
    return NULL;
  }
  return mArray[id]->get_single_elem();
}

// @brief id に対応する宣言要素を返す．(配列要素版)
// @param[in] id MvNode の ID番号
// @note is_array_elem(id) == false の時は NULL が返される．
const VlDeclArray*
MvVlMap::get_array_elem(ymuint id) const
{
  if ( mArray.size() <= id ) {
    return NULL;
  }
  return mArray[id]->get_array_elem();
}

// @brief id に対応する宣言要素のオフセットを返す．(配列要素版)
// @param[in] id MvNode の ID番号
// @note is_array_elem(id) == false の時は 0 が返される．
ymuint
MvVlMap::get_array_offset(ymuint id) const
{
  if ( mArray.size() <= id ) {
    return 0;
  }
  return mArray[id]->get_array_offset();
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

END_NAMESPACE_YM_MVN
