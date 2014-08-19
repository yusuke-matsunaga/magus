#ifndef VERILOG_VLLINEWATCHER_H
#define VERILOG_VLLINEWATCHER_H

/// @file YmVerilog/VlLineWatcher.h
/// @brief VlLineWatcher のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/verilog.h"
#include "YmUtils/Binder.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlLineWatcher VlLineWatcher.h <ym_YmVerilog/VlLineWatcher.h>
/// @ingroup VlParser
/// @brief Verilog-HDL 用の Lex 中で行番号を監視するクラス
///
/// このクラスは純粋仮想クラスなので継承したクラスを作らなければ
/// 使えない．
///
/// 具体的には
/// void event_proc(int ) というメンバ関数を実装する必要がある．
///
/// 作成したオブジェクトは VlParser::reg_watcher(VlLineWatcher*)
/// でパーサーに登録する．
/// パーサーは改行文字を読み込む度に登録された VlLineWatcher に対して
/// event_proc(int) を呼び出す．この時の引数は現在の行番号
//////////////////////////////////////////////////////////////////////
class VlLineWatcher :
  public T1Binder<int>
{
};

END_NAMESPACE_YM_VERILOG

#endif // VERILOG_VLLINEWATCHER_H
