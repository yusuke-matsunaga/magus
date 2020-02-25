#ifndef CUTMGR_H
#define CUTMGR_H

/// @file CutMgr.h
/// @brief CutMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap.h"


BEGIN_NAMESPACE_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class CutMgr CutMgr.h "CutMgr.h"
/// @brief カットを管理するクラス
//////////////////////////////////////////////////////////////////////
class CutMgr
{
public:

  /// @brief コンストラクタ
  CutMgr() = default;

  /// @brief デストラクタ
  ~CutMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カットを生成する．
  /// @param[in] root カットの根のノード
  /// @param[in] ni カットの入力数
  /// @param[in] inputs カットの入力のノードの配列
  Cut*
  new_cut(const SbjNode* root,
	  int ni,
	  const SbjNode* inputs[]);

  /// @brief このオブジェクトが管理しているすべてのカットを削除する．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ここで確保したメモリチャンクのリスト
  vector<char*> mMemList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief デストラクタ
inline
CutMgr::~CutMgr()
{
  clear();
}

// @brief カットを生成する．
// @param[in] root カットの根のノード
// @param[in] ni カットの入力数
// @param[in] inputs カットの入力のノードの配列
inline
Cut*
CutMgr::new_cut(const SbjNode* root,
		int ni,
		const SbjNode* inputs[])
{
  SizeType size = sizeof(Cut) + (ni - 1) * sizeof(const SbjNode*);
  char* p = new char[size];
  mMemList.push_back(p);
  return new (p) Cut(root, ni, inputs);
}

// @brief このオブジェクトが管理しているすべてのカットを削除する．
inline
void
CutMgr::clear()
{
  for ( auto p: mMemList ) {
    delete [] p;
  }
  mMemList.clear();
}

END_NAMESPACE_LUTMAP

#endif // CUTMGR_H
