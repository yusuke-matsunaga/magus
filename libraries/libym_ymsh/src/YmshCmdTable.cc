
/// @file YmshCmdTable.cc
/// @brief YmshCmdTable の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmshCmdTable.h"
#include "ymsh/YmshCmd.h"


BEGIN_NAMESPACE_YM_YMSH

BEGIN_NONAMESPACE

inline
ymuint
hash_func(const char* str)
{
  ymuint h = 0;
  ymuint c;
  for ( ; (c = static_cast<ymuint>(*str)); ++ str) {
    h = h * 37 + c;
  }
  return h;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス YmshCmdTable
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmshCmdTable::YmshCmdTable()
{
  alloc_table(1024);
}

// @brief デストラクタ
YmshCmdTable::~YmshCmdTable()
{
  clear();
}

// @brief 内容を空にする．
void
YmshCmdTable::clear()
{
  for (ymuint i = 0; i < mTableSize; ++ i) {
    mTable[i] = NULL;
  }
  mNum = 0;
}

// @brief 登録されている要素数を返す．
ymuint
YmshCmdTable::num() const
{
  return mNum;
}

// @brief 登録されているコマンド名のリストを返す．
// @param[out] name_list 答を格納するリスト
void
YmshCmdTable::get_name_list(vector<const char*>& name_list) const
{
  name_list.clear();
  for (ymuint i = 0; i < mTableSize; ++ i) {
    for (YmshCmd* cmd = mTable[i]; cmd; cmd = cmd->mLink) {
      name_list.push_back(cmd->name());
    }
  }
}

// @brief コマンドの追加を行う．
// @param[in] cmd コマンド
// @retval true 追加が成功した．
// @retval false 追加が失敗した．同名のコマンドが登録されていた．
bool
YmshCmdTable::reg_obj(YmshCmd* cmd)
{
  const char* name = cmd->name();
  ymuint hval = hash_func(name);
  ymuint idx = hval % mTableSize;
  for (YmshCmd* cmd1 = mTable[idx]; cmd1; cmd1 = cmd1->mLink) {
    if ( strcmp(cmd1->name(), name) == 0 ) {
      // 同名のコマンドが登録されていた．
      return false;
    }
  }

  if ( mNum >= mNextLimit ) {
    // テーブルを2倍にする．
    ymuint old_size = mTableSize;
    YmshCmd** old_table = mTable;
    alloc_table(old_size * 2);
    for (ymuint i = 0; i < old_size; ++ i) {
      for (YmshCmd* cmd1 = old_table[i]; cmd1; ) {
	YmshCmd* cur_cmd = cmd1;
	cmd1 = cur_cmd->mLink;
	ymuint idx1 = hash_func(cur_cmd->name()) % mTableSize;
	cur_cmd->mLink = mTable[idx1];
	mTable[idx1] = cur_cmd;
      }
    }
    delete [] old_table;
    // テーブルサイズが変わったのでインデックスを更新する．
    idx = hval % mTableSize;
  }

  cmd->mLink = mTable[idx];
  mTable[idx] = cmd;
  ++ mNum;

  return true;
}

// @brief コマンドをテーブルから取り除く
// @param[in] name 取り除く名前
// @note name というコマンドが登録されていなければなにもしない．
void
YmshCmdTable::unreg_obj(const char* name)
{
  ymuint idx = hash_func(name) % mTableSize;
  YmshCmd** prev = &mTable[idx];
  while ( *prev ) {
    YmshCmd* cmd = *prev;
    if ( strcmp(cmd->name(), name) == 0 ) {
      *prev = cmd->mLink;
      -- mNum;
      break;
    }
    prev = &(cmd->mLink);
  }
}

// @brief name という名のコマンドを探す．
// @param[in] name 名前
// @return 該当するコマンドを返す．
// @note 見つからなかった場合には NULL が返される．
YmshCmd*
YmshCmdTable::find_obj(const char* name) const
{
  ymuint idx = hash_func(name) % mTableSize;
  for (YmshCmd* cmd = mTable[idx]; cmd; cmd = cmd->mLink) {
    if ( strcmp(cmd->name(), name) == 0 ) {
      return cmd;
    }
  }
  return NULL;
}

// @brief テーブルの領域を確保する．
// @param[in] req_size 要求するサイズ
void
YmshCmdTable::alloc_table(ymuint req_size)
{
  mTableSize = req_size;
  mNextLimit = static_cast<ymuint>(mTableSize * 1.8);
  mTable = new YmshCmd*[mTableSize];
  for (ymuint i = 0; i < mTableSize; ++ i) {
    mTable[i] = NULL;
  }
}

END_NAMESPACE_YM_YMSH
