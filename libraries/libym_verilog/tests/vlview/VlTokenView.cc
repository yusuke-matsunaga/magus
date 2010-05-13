
/// @file libym_verilog/tests/vlview/VlTokenView.cc
/// @brief VlTokenView の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlTokenView.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VlTokenView.h"
#include <QTableView>
#include "VlTokenList.h"

  
// @brief コンストラクタ
// @param[in] parent 親のウィジェット
VlTokenView::VlTokenView(QWidget* parent) :
  QTableView(parent)
{
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);
}

// @brief デストラクタ
VlTokenView::~VlTokenView()
{
}

// @brief モデルをセットする．
void
VlTokenView::setModel(VlTokenList* token_list)
{
  mTokenList = token_list;
  QTableView::setModel(token_list);
}

// @brief selection が変化したときに呼ばれる関数
void
VlTokenView::selectionChanged(const QItemSelection& selected,
			      const QItemSelection& deselected)
{
  QAbstractItemView::selectionChanged(selected, deselected);
  
  QList<QModelIndex> index_list = selected.indexes();
  QListIterator<QModelIndex> p(index_list);
  if ( p.hasNext() ) {
    const QModelIndex& index = p.next();
    int row = index.row();
    const FileRegion& file_region = mTokenList->loc(row);
    emit select_token(file_region.start_line(),
		      file_region.start_column(),
		      file_region.end_line(),
		      file_region.end_column());
  }
}
