
#include "qm_v1.h"

namespace cppbox {

// V2 : 转盘 时家 阴盘
class CQimenV2 : public CQiMenV1
{
public:
    CQimenV2();
    ~CQimenV2() override;
public:
    // 传入日期数据
    bool Run(const QiInfomation& info, CalendarType type) override;

private:
    void getJushu(const CDateTime& datetime);
};

}