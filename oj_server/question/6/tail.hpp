#ifndef COMPILE
#include "header.hpp"
#endif

void Test1()
{
    vector<int> v{2,7,11,15};

    auto ret = Solution().twoSum(v);

    if(ret == 15)
    {
        cout << "通过测试用例一" <<endl;
    }
    else
    {
        cout << "未通过测试用例一" <<endl;
    }
}

void Test2()
{
    vector<int> v{3,2,4};

    auto ret = Solution().twoSum(v);

    if(ret == 4)
    {
        cout << "通过测试用例一" <<endl;
    }
    else
    {
        cout << "未通过测试用例一" <<endl;
    }
}

int main()
{
    Test1();
    Test2();
    cout << " AAAAAAAAAAAAAAA " << endl;
    return 0;
}