#ifndef CUTMGR_H
#define CUTMGR_H

/// @file CutMgr.h
/// @brief CutMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018, 2022 Yusuke Matsunaga
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
  ~CutMgr()
  {
    clear();
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カットを生成する．
  Cut*
  new_cut(
    const SbjNode* root,    ///< [in] カットの根のノード
    SizeType ni,            ///< [in] カットの入力数
    const SbjNode* inputs[] ///< [in] カットの入力のノードの配列
  )
  {
    SizeType size = sizeof(Cut) + (ni - 1) * sizeof(const SbjNode*);
    char* p = new char[size];
    mMemList.push_back(p);
    return new (p) Cut(root, ni, inputs);
  }

  /// @brief このオブジェクトが管理しているすべてのカットを削除する．
  void
  clear()
  {
    for ( auto p: mMemList ) {
      delete [] p;
    }
    mMemList.clear();
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ここで確保したメモリチャンクのリスト
  vector<char*> mMemList;

};

END_NAMESPACE_LUTMAP

#endif // CUTMGR_H
