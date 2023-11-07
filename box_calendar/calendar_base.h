#ifndef BOX_CALENDAR_BASE_H
#define BOX_CALENDAR_BASE_H

#if defined(_MSC_VER)
#  define CPP_CALENDAR_EXPORT __declspec(dllexport)
#  define CPP_CALENDAR_IMPORT __declspec(dllimport)
#else
#  define CPP_CALENDAR_EXPORT     __attribute__((visibility("default")))
#  define CPP_CALENDAR_IMPORT     __attribute__((visibility("default")))
#endif

#ifdef CPP_CALENDAR_LIB
#define CPP_CALENDAR_API CPP_CALENDAR_EXPORT
#else
#define CPP_CALENDAR_API CPP_CALENDAR_IMPORT
#endif

namespace cppbox {

    // 日历的不同实现版本
    enum CalendarType {

        CALENDAR_V1 = 0,   // 日历实现类的第一个版本，基于查表实现的日历，有效范围(1901 ~ 2099)
    };

    // 四柱
    struct CPP_CALENDAR_API CGanZhi {
        CGanZhi& operator=(const CGanZhi& ganzhi);
        int m_nYGan = -1;
        int m_nYZhi = -1;
        int m_nMGan = -1;
        int m_nMZhi = -1;
        int m_nDGan = -1;
        int m_nDZhi = -1;
        int m_nHGan = -1;
        int m_nHZhi = -1;
    };

    // 日期
    struct CPP_CALENDAR_API CDate {
        bool operator==(const CDate& rh) const;
        CDate();
        CDate(int y, int m, int d);
        CDate(const CDate& date);
        CDate& operator=(const CDate& date);
        int m_nYear = 0;
        int m_nMon = 0;
        int m_nDay = 0;
    };
    // 时间
    struct CPP_CALENDAR_API CTime {
        int m_nHour = 0;
        int m_nMin = 0;
        int m_nSec = 0;
    };
    // 日期和时间
    struct CPP_CALENDAR_API CDateTime {
        CDateTime(int y, int m, int d, int h, int min, int sec);
        CDateTime& operator=(const CDateTime& datetime);
        CDateTime() = default;
        ~CDateTime() = default;
        CDate m_date;
        CTime m_time;
        explicit CDateTime(const CDate& rh);
    };
    struct CPP_CALENDAR_API CJieQi {
        CDateTime       datetime;
        // 二十四节气索引，以 0 为小寒
        int             index;
    };

    // 日历处理基类
    class CPP_CALENDAR_API CCalenderBase {
    public:
        CCalenderBase();
        virtual ~CCalenderBase() = default;
    public:
        // 设置日期并计算，返回是否成功。
        virtual bool setDateTime(const CDateTime& datetime) = 0;
        virtual CDateTime const& getDateTime() const;
        virtual CDateTime const& getLunarDateTime() const;
        // 获取系统时间
        static void getNowDateTime(CDateTime& datetime);
        // 求余数(结果大于0)
        static int getRemainder(int nBase, int nValue);
        // 获取前一天
        virtual void getPreDay(CDateTime& datetime) = 0;
        virtual void getPreDay(CDate& date) = 0;
        // 获取后一天
        virtual void getNextDay(CDateTime& datetime) = 0;
        virtual void getNextDay(CDate& date) = 0;
        // 检查日期格式是否正确
        virtual bool checkFormat(const CDateTime& datetime) = 0;
    public:
        // 返回距离 00:00:00 的秒数
        static int getSecondsFromBase(const CTime& time);
        // 是否是闰年
        static bool isLeap(int nYear);
        // 返回公历日期的后一天日期
        static void getNextDay(const CDateTime& datetime, CDateTime& outtime);
        // 返回两个日期时间的秒数差
        static long long getSecondByTwoDateTime(const CDateTime& datetimeA, const CDateTime& datetimeB);
        // 返回两个时间之间的秒数差
        static int getDiffByTwoTime(const CTime& timeA, const CTime& timeB);
        // 返回距离 1900.1.1 的天数
        static int getDaysFromBase(const CDate& date);
        // 返回两个日期之间的天数差
        static int getDiffByTwoDate(const CDate& dateA, const CDate& dateB);
    public:
        // 获取四柱
        CGanZhi const& getSizhu() const;
        // 获取第一个节气
        CJieQi const& getJieFirst() const;
        // 获取第二个节气
        CJieQi const& getJieSecond() const;
    protected:
        CDateTime       m_datetime;     // 传入的时间日期
        CDateTime       m_ldatetime;    // 计算的农历日期
        bool            m_isLeap{};       // 当月是否是闰月
        bool            m_bigMon{};       // 当月是否是大月
        CJieQi          m_first;        // 当月第一个节气
        CJieQi          m_second;       // 当月第二个节气
        CGanZhi         m_sizhu;        // 此时的四柱
    };

    // 日历类实例生成工厂
    class CPP_CALENDAR_API CCalenderFactory {
    private:
        CCalenderFactory() = default;
        ~CCalenderFactory() = default;
    public:
        // 获取日历类指针
        static CCalenderBase* creatInstance(CalendarType etype);
        // 释放内存
        static void freeCalender(CCalenderBase* pCalender);
    };
}
#endif
