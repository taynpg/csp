#ifndef BOX_QIMEN_BASE_
#define BOX_QIMEN_BASE_

#include "calendar_base.h"

#if defined(_MSC_VER)
#  define CPP_QIMEN_EXPORT __declspec(dllexport)
#  define CPP_QIMEN_IMPORT __declspec(dllimport)
#else
#  define CPP_QIMEN_EXPORT     __attribute__((visibility("default")))
#  define CPP_QIMEN_IMPORT     __attribute__((visibility("default")))
#endif

#ifdef CPP_QIMEN_LIB
#define CPP_QIMEN_API CPP_QIMEN_EXPORT
#else
#define CPP_QIMEN_API CPP_QIMEN_IMPORT
#endif

namespace cppbox {

    using cb = CCalenderBase;
    constexpr int g_num = 9;
    enum QIMEN_STYLE {
        SHIJIA_ZHUANPAN_CHAOJIE_ZHIRUN = 0,     // 时家转盘，超接置润法
        SHIJIA_ZHUANPAN_YINPAN,                 // 时家转盘，阴盘
        NOT_DEFINE                              // 未定义
    };


    /*
    *   本类中涉及的下标默认内容：
    *   以 Pre 结尾的是原始位置：
    *   以 Turn 结尾的是旋转位置：
    *   以 Re 结尾的是计算结果
    *
    *   排盘逻辑，无论各派别的定义，排法如何，都基于本程序的原始定义，
    *   《排盘前设置好位置和各个符号的初始对应关系》
    *   最终生成的结果均已本程序定义的位置排序
    *
    *      -----------------------------------------------------
    *      |                 |                |                |
    *      |     位置 4      |     位置 3     |     位置 2     |
    *      |                 |                |                |
    *      |                 |                |                |
    *      -----------------------------------------------------
    *      |                 |                |                |
    *      |     位置 5      |    位置 8      |     位置 1     |
    *      |                 |                |                |
    *      |                 |                |                |
    *      -----------------------------------------------------
    *      |                 |                |                |
    *      |     位置 6      |     位置 7     |    起始位置 0  |
    *      |                 |                |                |
    *      |                 |                |                |
    *      -----------------------------------------------------
    *   
    * 
    *   下面是固定定义，不同的盘需要先设定它们的原始位置和旋转位置：
    *       -1     0     1     2     3      4     5     6     7      8
    *      为空  天蓬  天芮  天冲   天辅  天禽  天心  天柱   天任   天英
    *      为空  休门  死门  伤门   杜门  死门  开门  惊门   生门   景门
    *      为空  值符  腾蛇  太阴   六合  腾蛇  白虎  玄武   九地   九天
    *      为空  坎卦  坤卦  震卦   巽卦  中五  乾卦  兑卦   艮卦   离卦
    */

    // 排盘的参数信息
    struct CPP_QIMEN_API QiInfomation {

        int         nJu{};        // 局数，默认: 0 为程序自动选择，其他指定。
        CDateTime   datetime{};   // 时间日期
    };

    class CPP_QIMEN_API CQimen {
    public:
        CQimen();
        virtual ~CQimen();
    public:
        // 初始化
        virtual void Init();
        // 进行计算
        virtual bool Run(const QiInfomation& info, CalendarType type) = 0;
        virtual bool BaseRun(const QiInfomation& info, CalendarType type);
        // 设置九宫的原始九星位置[位置-九星]
        virtual void setJiuXingPre();
        // 设置九星的旋转位置[相对顺序]
        virtual void setJiuXingTurn();
        // 设置九宫的数字对应[位置-数字(数字从0开始)]
        virtual void setGongBaseNum();      
        // 设置八门的初始位置[位置-八门]
        virtual void setBaMenPre();
        // 设置八门的旋转位置
        virtual void setBaMenTurn();
        // 设置八神的初始位置[位置-八神]
        virtual void setBaShenPre();
        // 设置寄宫[固定位置]
        virtual void setJiGong(int nGong);
        // 设置十二地支位置对应
        virtual void setDizhi();
        // 获取六十甲子下标
        static int getJiaziIndex(int nTianIndex, int nDiIndex);
        // 查找内容所在的下标
        static int getIndex(int* pData, int nSize, int nValue);
        // 获取下 n 个值所在的原始位置(正值代表向下数，负值代表向上数)
        // static int getNextPosition(int* pData, int nSize, int nCur, int nValue);
    public:
        // 设置初始内容之后，执行此函数，对初始内容整理并生成所需的信息。
        void prepare();
    public:
        // 排地盘
        virtual void genDiPan() = 0;
        // 查找值符，值使
        virtual void genZhi() = 0;
        // 排九星
        virtual void genJiuXing() = 0;
        // 排八门
        virtual void genBaMen() = 0;
        // 排八神
        virtual void genBaShen() = 0;
        // 排天盘
        virtual void genTianPan() = 0;
        // 排旬空马星
        virtual void genOther() = 0;
    public:
        // 获取天盘 (size = 9)
        const int* getTianPan() const;
        // 获取地盘 (size = 9)
        const int* getDiPan() const;
        // 获取九星 (size = 9)
        const int* getJiuXing() const;
        const int* getJiuXingPre() const;
        // 获取八门 (size = 9)
        const int* getBaMen() const;
        const int* getBaMenPre() const;
        // 获取八神 (size = 9)
        const int* getBaShen() const;
        // 获取马星
        int getMaXing() const;
        // 获取空亡 (size = 2)
        const int* getKong() const;
        // 获取旬空信息
        const int* getXunKong() const;
        // 获取日历
        const CCalenderBase* getCalendar() const;
        // 获取值班人员
        int getDuty() const;
        // 获取寄宫
        int getJiGong() const;
        // 获取三元
        int getYuan() const;
        // 获取当日节气
        int getJieQi() const;
        // 获取局数
        int getJushu() const;
        // 获取是否是阴遁
        bool getIsYinDun() const;
    protected:
        int                         m_nJushu{};               // 局数
        int                         m_JiuXingRe[g_num]{};     // 九星的计算结果
        int                         m_JiuXingPre[g_num]{};    // 九星的原始位置
        int                         m_JiuXingTurn[g_num]{};   // 九星的旋转位置，不足9个的从后向前留空
        int                         m_nGongBaseNum[g_num]{};  // 宫对应的数字
        int                         m_nBamenPre[g_num]{};     // 八门的原始位置
        int                         m_nBamenRe[g_num]{};      // 八门的计算结果
        int                         m_nBamenTurn[g_num]{};    // 八门的旋转位置
        int                         m_nBaShenPre[g_num]{};    // 八神的原始位置
        int                         m_nBaShenRe[g_num]{};     // 八神的计算结果
        int                         m_nTianPan[g_num]{};      // 天盘
        int                         m_nDiPan[g_num]{};        // 地盘
        int                         m_nJiGong{};              // 天禽星寄宫
        int                         m_nMaXing{};              // 马星位置
        int                         m_nKongWang[2]{};         // 空亡位置
        int                         m_nXunKong[8]{};          // 寻空位置
        bool                        m_isYinDun{};             // 是否是阴遁
        int                         m_nYuan{};                // 三元 0 手动, 1下，2中，3上
        bool                        m_auto{};                 // 是否是自动排局
        int                         m_nzhifu{};               // 值符
        int                         m_nzhishi{};              // 值使
        int                         m_nmaxing{};              // 马星
        int                         m_nJieQi{};               // 当日节气
        int                         m_nJiaZi{};               // 当日六十甲子
    protected:
        CDateTime                   m_datetime;
        CCalenderBase*              m_pCal{};                 // 日历实例
        int                         m_nContra[g_num]{};       // 基位置和定义顺序的对照表
        int                         m_sanhe[12]{};            // 地支三和
        int                         m_zhichong[12]{};         // 地支相冲
        int                         m_dizhi[12]{};            // 十二地支的位置
        CalendarType                m_calType;                // 日历的类型
    };

    // 奇门工厂类
    class CPP_QIMEN_API CQimenFactory {
    private:
        CQimenFactory() = default;
        ~CQimenFactory() = default;
    public:
        // 获取实例
        static CQimen* createInstance(QIMEN_STYLE type);
        // 释放实例
        static void freeInstance(CQimen* pQimen);
    };
}
#endif
