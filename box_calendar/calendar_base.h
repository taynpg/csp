#ifndef BOX_CALENDAR_BASE_H
#define BOX_CALENDAR_BASE_H

#if defined(DYNAMIC_DLL)
#if defined(_MSC_VER)
#define CPP_CALENDAR_EXPORT __declspec(dllexport)
#define CPP_CALENDAR_IMPORT __declspec(dllimport)
#else
#define CPP_CALENDAR_EXPORT __attribute__((visibility("default")))
#define CPP_CALENDAR_IMPORT __attribute__((visibility("default")))
#endif

#ifdef CPP_CALENDAR_LIB
#define CPP_CALENDAR_API CPP_CALENDAR_EXPORT
#else
#define CPP_CALENDAR_API CPP_CALENDAR_IMPORT
#endif
#else
#define CPP_CALENDAR_API
#if defined(_MSC_VER)
#pragma warning(disable : 4251)
#endif
#endif

namespace cppbox {

// 日历的不同实现版本
enum class CalendarType {
    // 日历实现类的第一个版本，基于查表实现的日历，有效范围(1901 ~ 2099)
    CALENDAR_V1 = 0,
    CALENDAR_V2   // 日历实现类的第二个版本，基于天文历算法实现的日历，有效范围(公元前722年
                  // ~ 9999)
};

// 四柱
struct CGanZhi {
    CGanZhi& operator=(const CGanZhi& ganzhi);
    int yg_ = -1;
    int yz_ = -1;
    int mg_ = -1;
    int mz_ = -1;
    int dg_ = -1;
    int dz_ = -1;
    int hg_ = -1;
    int hz_ = -1;
};

// 日期
struct CPP_CALENDAR_API CDate {
    bool operator==(const CDate& rh) const;
    CDate();
    CDate(int y, int m, int d);
    CDate(const CDate& date);
    CDate& operator=(const CDate& date);
    bool operator!=(const CDate& date);
    int year_ = 0;
    int mon_ = 0;
    int day_ = 0;
};

// 时间
struct CPP_CALENDAR_API CTime {
    int h_ = 0;
    int m_ = 0;
    int s_ = 0;
};

// 日期和时间
struct CPP_CALENDAR_API CDateTime {
    CDateTime(int y, int m, int d, int h, int min, int sec);
    CDateTime& operator=(const CDateTime& datetime);
    CDateTime& operator=(const CDate& date);
    CDateTime() = default;
    ~CDateTime() = default;
    CDate date_;
    CTime time_;
    explicit CDateTime(const CDate& rh);
};

struct CPP_CALENDAR_API CJieQi {
    CDateTime dt_{};
    // 二十四节气索引，以 0 为小寒
    int index_{};
};

// 日历处理基类
class CPP_CALENDAR_API CCalenderBase
{
public:
    CCalenderBase();
    virtual ~CCalenderBase() = default;

public:
    // 仅检查日期格式上的合法性，并不考虑实现方是否支持这个日期范围
    static bool check_format_only(const CDateTime& datetime);
    static bool is_leap(int year);

public:
    // 设置日期并计算，返回是否成功。
    virtual bool set_datetime(const CDateTime& datetime) = 0;
    virtual CDateTime const& get_solar() const;
    virtual CDateTime const& get_lunnar() const;
    // 获取系统时间
    static void now(CDateTime& datetime);
    // 求余数(结果大于0)
    static int remain(int nBase, int nValue);
    // 获取前一天
    virtual void pre(CDateTime& datetime) = 0;
    virtual void pre(CDate& date) = 0;
    // 获取后一天
    virtual void next(CDateTime& datetime) = 0;
    virtual void next(CDate& date) = 0;
    // 检查日期格式是否正确
    virtual bool check_format(const CDateTime& datetime) = 0;
    // 返回两个日期之间的天数差
    virtual int get_diff_day(const CDate& dateA, const CDate& dateB) = 0;
    // 基于基础时间和差值计算新的日期
    virtual void get_diff_sec(const CDateTime& basetime, CDateTime& outtime, long long nSecond) = 0;
    // 返回距离 00:00:00 的秒数
    virtual int get_sec_by_base(const CTime& time) = 0;

    // 复制日期
    static void mcopy(const CDateTime& datetime, CDateTime& outtime);
    // 返回两个时间之间的秒数差
    virtual int get_diff_by_time(const CTime& timeA, const CTime& timeB) = 0;

    // 返回两个日期时间的秒数差
    virtual long long get_sec_by_date(const CDateTime& datetimeA, const CDateTime& datetimeB) = 0;

public:
    // 获取四柱
    CGanZhi const& get_sz() const;
    // 获取第一个节气
    CJieQi const& first_jie() const;
    // 获取第二个节气
    CJieQi const& second_jie() const;

protected:
    CDateTime dt_;        // 传入的时间日期
    CDateTime lunar_;     // 计算的农历日期
    bool leap_{};         // 当月是否是闰月
    bool big_mon_{};      // 当月是否是大月
    CJieQi first_jie_;    // 当月第一个节气
    CJieQi second_jie_;   // 当月第二个节气
    CGanZhi sz_;          // 此时的四柱
};

// 日历类实例生成工厂
class CPP_CALENDAR_API CCalenderFactory
{
private:
    CCalenderFactory() = default;
    ~CCalenderFactory() = default;

public:
    // 获取日历类指针
    static CCalenderBase* creat_instance(CalendarType etype);
    // 释放内存
    static void free(CCalenderBase* pCalender);
};
}   // namespace cppbox
#endif
