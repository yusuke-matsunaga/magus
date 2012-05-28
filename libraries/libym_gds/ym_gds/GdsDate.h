#ifndef YM_GDS_GDSDATE_H
#define YM_GDS_GDSDATE_H

/// @file ym_gds/GdsDate.h
/// @brief GdsDate のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/gds_nsdef.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsDate GdsDate.h "ym_gds/GdsDate.h"
/// @brief 日付情報を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsDate
{
public:

  /// @brief 空のコンストラクタ
  /// @note 内容は不定
  GdsDate();

  /// @brief 値を指定したコンストラクタ
  /// @param[in] year 年 ( 1970 年が 0 )
  /// @param[in] month 月
  /// @param[in] day 日
  /// @param[in] hour 時間
  /// @param[in] min 分
  /// @param[in] sec 秒
  GdsDate(ymuint year,
	  ymuint month,
	  ymuint day,
	  ymuint hour,
	  ymuint min,
	  ymuint sec);

  /// @brief デストラクタ
  ~GdsDate();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 値をセットする．
  /// @param[in] year 年 ( 1970 年が 0 )
  /// @param[in] month 月
  /// @param[in] day 日
  /// @param[in] hour 時間
  /// @param[in] min 分
  /// @param[in] sec 秒
  void
  set(ymuint year,
      ymuint month,
      ymuint day,
      ymuint hour,
      ymuint min,
      ymuint sec);

  /// @brief 年を得る
  ymuint
  year() const;

  /// @brief 月を得る．
  ymuint
  month() const;

  /// @brief 日を得る．
  ymuint
  day() const;

  /// @brief 時間を得る．
  ymuint
  hour() const;

  /// @brief 分を得る．
  ymuint
  minute() const;

  /// @brief 秒を得る．
  ymuint
  second() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 年 ( 1970 年が 0 )
  ymuint8 mYear;

  // 月
  ymuint8 mMonth;

  // 日
  ymuint8 mDay;

  // 時間
  ymuint8 mHour;

  // 分
  ymuint8 mMin;

  // 秒
  ymuint8 mSec;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @note 内容は不定
inline
GdsDate::GdsDate()
{
}

// @brief 値を指定したコンストラクタ
// @param[in] year 年 ( 1970 年が 0 )
// @param[in] month 月
// @param[in] day 日
// @param[in] hour 時間
// @param[in] min 分
// @param[in] sec 秒
inline
GdsDate::GdsDate(ymuint year,
		 ymuint month,
		 ymuint day,
		 ymuint hour,
		 ymuint min,
		 ymuint sec)
{
  set(year, month, day, hour, min, sec);
}

// @brief デストラクタ
inline
GdsDate::~GdsDate()
{
}

// @brief 値をセットする．
// @param[in] year 年 ( 1970 年が 0 )
// @param[in] month 月
// @param[in] day 日
// @param[in] hour 時間
// @param[in] min 分
// @param[in] sec 秒
inline
void
GdsDate::set(ymuint year,
	     ymuint month,
	     ymuint day,
	     ymuint hour,
	     ymuint min,
	     ymuint sec)
{
  mYear = year;
  mMonth = month;
  mDay = day;
  mHour = hour;
  mMin = min;
  mSec = sec;
}

// @brief 年を得る
inline
ymuint
GdsDate::year() const
{
  return mYear;
}

// @brief 月を得る．
inline
ymuint
GdsDate::month() const
{
  return mMonth;
}

// @brief 日を得る．
inline
ymuint
GdsDate::day() const
{
  return mDay;
}

// @brief 時間を得る．
inline
ymuint
GdsDate::hour() const
{
  return mHour;
}

// @brief 分を得る．
inline
ymuint
GdsDate::minute() const
{
  return mMin;
}

// @brief 秒を得る．
inline
ymuint
GdsDate::second() const
{
  return mSec;
}

END_NAMESPACE_YM_GDS

#endif // YM_GDS_GDSDATE_H
