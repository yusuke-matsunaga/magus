
/// @file MvVerilogReader.cc
/// @brief MvVerilogReader の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/MvnVerilogReader.h"
#include "ReaderImpl.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

// @brief コンストラクタ
MvnVerilogReader::MvnVerilogReader() :
  mImpl(new nsVerilog::ReaderImpl)
{
}

// @brief デストラクタ
MvnVerilogReader::~MvnVerilogReader()
{
  delete mImpl;
}

// @brief 内部のデータをクリアする．
void
MvnVerilogReader::clear()
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
MvnVerilogReader::read(const string& filename,
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
MvnVerilogReader::gen_network(MvnMgr& mgr,
			      MvnVlMap& node_map)
{
  return mImpl->gen_network(mgr, NULL, node_map);
}

// @brief 今まで読み込んだ情報からネットワークを生成する．
// @param[in] mgr ネットワーク生成用のマネージャ
// @param[in] library セルライブラリ
// @param[out] node_map MvNode と宣言要素の対応付けを保持する配列
// @retval true 正常に処理を行った．
// @retval false 生成中にエラーが起こった．
bool
MvnVerilogReader::gen_network(MvnMgr& mgr,
			      const CellLibrary* library,
			      MvnVlMap& node_map)
{
  return mImpl->gen_network(mgr, library, node_map);
}

END_NAMESPACE_YM_NETWORKS_MVN
