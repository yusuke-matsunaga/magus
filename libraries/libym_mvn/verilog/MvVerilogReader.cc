
/// @file libym_mvn/verilog_reader/MvVerilogReader.cc
/// @brief MvVerilogReader の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvVerilogReader.h"
#include "ReaderImpl.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
MvVerilogReader::MvVerilogReader() :
  mImpl(new nsVerilog::ReaderImpl)
{
}

// @brief デストラクタ
MvVerilogReader::~MvVerilogReader()
{
  delete mImpl;
}

// @brief 内部のデータをクリアする．
void
MvVerilogReader::clear()
{
  mImpl->clear();
}

// @brief verilog 形式のファイルを読み込む．
// @param[in] filename ファイル名
// @param[in] searchpath サーチパス
// @param[in] watcher_list 行番号ウォッチャーのリスト
// @retval true 正常に読み込めた．
// @retval false 読込中にエラーが起こった．
bool
MvVerilogReader::read(const string& filename,
		      const SearchPathList& searchpath,
		      const list<VlLineWatcher*> watcher_list)
{
  return mImpl->read(filename, searchpath, watcher_list);
}

// @brief 今まで読み込んだ情報からネットワークを生成する．
// @param[in] mgr ネットワーク生成用のマネージャ
// @retval true 正常に処理を行った．
// @retval false 生成中にエラーが起こった．
bool
MvVerilogReader::gen_network(MvMgr& mgr,
			     vector<pair<const VlDeclArray*, ymuint> >& node_map)
{
  return mImpl->gen_network(mgr, node_map);
}

// @brief メッセージハンドラを付加する．
void
MvVerilogReader::add_msg_handler(MsgHandler* msg_handler)
{
  mImpl->add_msg_handler(msg_handler);
}

END_NAMESPACE_YM_MVN
