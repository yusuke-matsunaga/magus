#ifndef LRMGR_H
#define LRMGR_H

/// @file LrMgr.h
/// @brief LrMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "logic/AigMgr.h"
#include "AigTemplate.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// local rewrite を行うクラス
//////////////////////////////////////////////////////////////////////
class LrMgr
{
public:

  /// @brief コンストラクタ
  LrMgr();

  /// @brief デストラクタ
  ~LrMgr();


public:

  /// @brief local rewrite を行う．
  /// @param[in] aig_mgr 対象のネットワーク
  void
  local_rewrite(AigMgr& aig_mgr);

  /// @brief ログレベルを設定する．
  void
  set_loglevel(int level);

  /// @brief ログ出力用ストリームを設定する．
  void
  set_logstream(ostream* out);


private:

  /// @brief 与えられた関数に対する AigTemplate を求める．
  /// @param[in] ni 関数の入力数
  /// @param[in] pata 関数ベクタ
  /// @param[out] templ 対応するテンプレート
  /// @retval true テンプレートが見つかった．
  /// @retval false テンプレートが見つからなかった．
  bool
  find_aig(ymuint ni,
	   ymuint32 pat,
	   AigTemplate& templ);

  /// @brief 置き換えテーブルを初期化する．
  void
  init_table();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 2入力関数用のテンプレート
  // 全ての関数に対して用意する．
  AigTemplate aig2table[16];

  // 3入力関数用のテンプレート
  // 全ての関数に対して用意する．
  AigTemplate aig3table[256];

  // 4入力関数用のテンプレート
  // NPN同値類の代表関数に対してのみ用意する．
  AigTemplate aig4table[222];

  // NPN同値類の代表関数のパタンから aig4table の番号を得るためのハッシュ表
  unordered_map<ymuint32, ymuint8> npn4map;

  // ログレベル
  int mLogLevel;

  // ログ出力用のストリーム
  ostream* mLogStream;

};

END_NAMESPACE_YM_AIG

#endif // LRMGR_H
