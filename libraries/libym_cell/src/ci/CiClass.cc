
/// @file CiClass.cc
/// @brief CiClass の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CiClass.h"
#include "YmCell/CellLibrary.h"
#include "YmCell/CellGroup.h"
#include "YmLogic/NpnMapM.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiClass
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiClass::CiClass() :
  mIdmapNum(0),
  mIdmapList(NULL),
  mGroupNum(0),
  mGroupList(NULL)
{
}

// @brief デストラクタ
CiClass::~CiClass()
{
  // mIdmapList, mGroupList は CellMgr が管理している．
}

// @brief ID番号を返す．
// @note CellLibrary::npn_class(id) で返されるオブジェクトの id() は id となる．
ymuint
CiClass::id() const
{
  return mId;
}

// @brief 同位体変換の個数を得る．
// @note 恒等変換は含まない．
ymuint
CiClass::idmap_num() const
{
  return mIdmapNum;
}

// @brief 同位体変換を得る．
// @param[in] pos 位置番号 ( 0 <= pos < idmap_num() )
const NpnMapM&
CiClass::idmap(ymuint pos) const
{
  assert_cond( pos < mIdmapNum, __FILE__, __LINE__);
  return mIdmapList[pos];
}

// @brief NPN同値類の数を返す．
ymuint
CiClass::group_num() const
{
  return mGroupNum;
}

// @brief CellGroup を返す．
// @param[in] pos 位置番号 ( 0 <= pos < func_num() )
const CellGroup*
CiClass::cell_group(ymuint pos) const
{
  assert_cond( pos < mGroupNum, __FILE__, __LINE__);
  return mGroupList[pos];
}

// @brief 初期化する．
// @param[in] idmap_list 同位体変換リスト
// @param[in] group_list グループのリスト
// @param[in] alloc メモリアロケータ
void
CiClass::init(const vector<NpnMapM>& idmap_list,
	      const vector<const CellGroup*>& group_list,
	      Alloc& alloc)
{
  mIdmapNum = idmap_list.size();
  mGroupNum = group_list.size();
  alloc_array(alloc);

  for (ymuint i = 0; i < mIdmapNum; ++ i) {
    mIdmapList[i] = idmap_list[i];
  }

  for (ymuint i = 0; i < mGroupNum; ++ i) {
    mGroupList[i] = group_list[i];
  }
}

// @brief バイナリダンプを行う．
// @param[in] bos 出力先のストリーム
void
CiClass::dump(ODO& bos) const
{
  bos << mIdmapNum
      << mGroupNum;

  // 同位体変換情報のダンプ
  for (ymuint i = 0; i < mIdmapNum; ++ i) {
    bos << mIdmapList[i];
  }

  // グループ情報のダンプ
  for (ymuint j = 0; j < mGroupNum; ++ j) {
    const CellGroup* group = mGroupList[j];
    ymuint32 group_id = group->id();
    bos << group_id;
  }
}

// @brief バイナリファイルを読み込む．
// @param[in] bis 入力元のストリーム
// @param[in] library セルライブラリ
// @param[in] alloc メモリアロケータ
void
CiClass::restore(IDO& bis,
		 const CellLibrary& library,
		 Alloc& alloc)
{
  bis >> mIdmapNum
      >> mGroupNum;
  alloc_array(alloc);

  // 同位体変換情報の設定
  for (ymuint i = 0; i < mIdmapNum; ++ i) {
    bis >> mIdmapList[i];
  }

  // グループ情報の設定
  for (ymuint i = 0; i < mGroupNum; ++ i) {
    ymuint32 group_id;
    bis >> group_id;
    const CellGroup* group = library.group(group_id);
    mGroupList[i] = group;
  }
}

// @brief 配列領域の確保を行う．
// @param[in] alloc メモリアロケータ
// @note mIdmapNum, mGroupNum が適切に設定されている必要がある．
void
CiClass::alloc_array(Alloc& alloc)
{
  if ( mIdmapNum > 0 ) {
    void* p = alloc.get_memory(sizeof(NpnMapM) * mIdmapNum);
    mIdmapList = new (p) NpnMapM[mIdmapNum];
  }
  else {
    mIdmapList = NULL;
  }

  if ( mGroupNum > 0 ) {
    void* p = alloc.get_memory(sizeof(const CellGroup*) * mGroupNum);
    mGroupList = new (p) const CellGroup*[mGroupNum];
  }
  else {
    mGroupList = NULL;
  }
}

END_NAMESPACE_YM_CELL
