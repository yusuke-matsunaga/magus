
/// @file YmsatMsgHandler.h
/// @brief YmsatMsgHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/SatMsgHandler.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// ymsat 用の SatMsgHandler
//////////////////////////////////////////////////////////////////////
class YmsatMsgHandler :
  public SatMsgHandler
{
public:

  /// @brief コンストラクタ
  YmsatMsgHandler(ostream& s);

  /// @brief デストラクタ
  virtual
  ~YmsatMsgHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ヘッダの出力
  virtual
  void
  print_header();

  /// @brief メッセージ出力関数
  virtual
  void
  operator()(const SatStats& stats);

  /// @brief 終了メッセージの出力
  /// @param[in] stats SAT ソルバの内部状態
  virtual
  void
  print_tailer(const SatStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力ストリーム
  ostream& mS;

};

END_NAMESPACE_YM
