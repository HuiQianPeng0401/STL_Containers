#include "vector.h"
#include <vector>
#include "list.h"
#include <list>
#include "deque.h"
#include <deque>
#include "stack.h"
#include <stack>
#include "queue.h"
#include <queue>
#include "set.h"
#include <set>
#include "map.h"
#include <map>
#include "unordered_set.h"
#include <unordered_set>
#include "unordered_map.h"
#include <unordered_map>

#include "test/test.h"


namespace phq
{
namespace test
{
namespace vector_test
{

void vector_test()
{
  std::cout << "[===============================================================]\n";
  std::cout << "[----------------- Run container test : vector -----------------]\n";
  std::cout << "[-------------------------- API test ---------------------------]\n";
  int a[] = { 1,2,3,4,5 };
  phq::vector<int> v1;
  phq::vector<int> v2(10);
  phq::vector<int> v3(10, 1);
  phq::vector<int> v4(a, a + 5);
  phq::vector<int> v5(v2);
  phq::vector<int> v6(std::move(v2));
  phq::vector<int> v7{ 1,2,3,4,5,6,7,8,9 };
  phq::vector<int> v8, v9, v10;
  v8 = v3;
  v9 = std::move(v3);
  v10 = { 1,2,3,4,5,6,7,8,9 };

  FUN_AFTER(v1, v1.assign(8, 8));
  FUN_AFTER(v1, v1.assign(a, a + 5));
  FUN_AFTER(v1, v1.emplace(v1.begin(), 0));
  FUN_AFTER(v1, v1.emplace_back(6));
  FUN_AFTER(v1, v1.push_back(6));
  FUN_AFTER(v1, v1.insert(v1.end(), 7));
  FUN_AFTER(v1, v1.insert(v1.begin() + 3, 2, 3));
  FUN_AFTER(v1, v1.insert(v1.begin(), a, a + 5));
  FUN_AFTER(v1, v1.pop_back());
  FUN_AFTER(v1, v1.erase(v1.begin()));
  FUN_AFTER(v1, v1.erase(v1.begin(), v1.begin() + 2));
  FUN_AFTER(v1, v1.reverse());
  FUN_AFTER(v1, v1.swap(v4));
  FUN_VALUE(*v1.begin());
  FUN_VALUE(*(v1.end() - 1));
  FUN_VALUE(*v1.rbegin());
  FUN_VALUE(*(v1.rend() - 1));
  FUN_VALUE(v1.front());
  FUN_VALUE(v1.back());
  FUN_VALUE(v1[0]);
  FUN_VALUE(v1.at(1));
  int* p = v1.data();
  *p = 10;
  *++p = 20;
  p[1] = 30;
  std::cout << " After change v1.data() :" << "\n";
  COUT(v1);
  std::cout << std::boolalpha;
  FUN_VALUE(v1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(v1.size());
  FUN_VALUE(v1.max_size());
  FUN_VALUE(v1.capacity());
  FUN_AFTER(v1, v1.resize(10));
  FUN_VALUE(v1.size());
  FUN_VALUE(v1.capacity());
  FUN_AFTER(v1, v1.shrink_to_fit());
  FUN_VALUE(v1.size());
  FUN_VALUE(v1.capacity());
  FUN_AFTER(v1, v1.resize(6, 6));
  FUN_VALUE(v1.size());
  FUN_VALUE(v1.capacity());
  FUN_AFTER(v1, v1.shrink_to_fit());
  FUN_VALUE(v1.size());
  FUN_VALUE(v1.capacity());
  FUN_AFTER(v1, v1.clear());
  FUN_VALUE(v1.size());
  FUN_VALUE(v1.capacity());
  FUN_AFTER(v1, v1.reserve(5));
  FUN_VALUE(v1.size());
  FUN_VALUE(v1.capacity());
  FUN_AFTER(v1, v1.reserve(20));
  FUN_VALUE(v1.size());
  FUN_VALUE(v1.capacity());
  FUN_AFTER(v1, v1.shrink_to_fit());
  FUN_VALUE(v1.size());
  FUN_VALUE(v1.capacity());
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]\n";
  std::cout << "|---------------------|-------------|-------------|-------------|\n";
  std::cout << "|      push_back      |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P1(vector<int>, push_back, rand(), SCALE_LL(LEN1), SCALE_LL(LEN2), SCALE_LL(LEN3));
#else
  CON_TEST_P1(vector<int>, push_back, rand(), SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#endif
  std::cout << "\n";
  std::cout << "|---------------------|-------------|-------------|-------------|\n";
  PASSED;
#endif
  std::cout << "[----------------- End container test : vector -----------------]\n";
}

} // namespace vector_test
} // namespace test

namespace test
{
namespace list_test
{

// 一个辅助测试函数
bool is_odd(int x) { return x & 1; }

void list_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[------------------ Run container test : list ------------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  int a[] = { 1,2,3,4,5 };
  phq::list<int> l1;
  phq::list<int> l2(5);
  phq::list<int> l3(5, 1);
  phq::list<int> l4(a, a + 5);
  phq::list<int> l5(l2);
  phq::list<int> l6(std::move(l2));
  phq::list<int> l7{ 1,2,3,4,5,6,7,8,9 };
  phq::list<int> l8;
  l8 = l3;
  phq::list<int> l9;
  l9 = std::move(l3);
  phq::list<int> l10;
  l10 = { 1, 2, 2, 3, 5, 6, 7, 8, 9 };

  FUN_AFTER(l1, l1.assign(8, 8));
  FUN_AFTER(l1, l1.assign(a, a + 5));
  FUN_AFTER(l1, l1.assign({ 1,2,3,4,5,6 }));
  FUN_AFTER(l1, l1.insert(l1.end(), 6));
  FUN_AFTER(l1, l1.insert(l1.end(), 2, 7));
  FUN_AFTER(l1, l1.insert(l1.begin(), a, a + 5));
  FUN_AFTER(l1, l1.push_back(2));
  FUN_AFTER(l1, l1.push_front(1));
  FUN_AFTER(l1, l1.emplace(l1.begin(),1));
  FUN_AFTER(l1, l1.emplace_front(0));
  FUN_AFTER(l1, l1.emplace_back(10));
  FUN_VALUE(l1.size());
  FUN_AFTER(l1, l1.pop_front());
  FUN_AFTER(l1, l1.pop_back());
  FUN_AFTER(l1, l1.erase(l1.begin()));
  FUN_AFTER(l1, l1.erase(l1.begin(), l1.end()));
  FUN_VALUE(l1.size());
  FUN_AFTER(l1, l1.resize(10));
  FUN_AFTER(l1, l1.resize(5, 1));
  FUN_AFTER(l1, l1.resize(8, 2));
  FUN_VALUE(l1.size());
  FUN_AFTER(l1, l1.splice(l1.end(), l4));
  FUN_AFTER(l1, l1.splice(l1.begin(), l5, l5.begin()));
  FUN_AFTER(l1, l1.splice(l1.end(), l6, l6.begin(), ++l6.begin()));
  FUN_VALUE(l1.size());
  FUN_AFTER(l1, l1.remove(0));
  FUN_AFTER(l1, l1.remove_if(is_odd));
  FUN_VALUE(l1.size());
  FUN_AFTER(l1, l1.assign({ 9,5,3,3,7,1,3,2,2,0,10 }));
  FUN_VALUE(l1.size());
  FUN_AFTER(l1, l1.sort());
  FUN_AFTER(l1, l1.unique());
  FUN_AFTER(l1, l1.unique([&](int a, int b) {return b == a + 1; }));
  FUN_AFTER(l1, l1.merge(l7));
  FUN_AFTER(l1, l1.sort(std::greater<int>()));
  FUN_AFTER(l1, l1.merge(l8, std::greater<int>()));
  FUN_AFTER(l1, l1.reverse());
  FUN_AFTER(l1, l1.clear());
  FUN_AFTER(l1, l1.swap(l9));
  FUN_VALUE(*l1.begin());
  FUN_VALUE(*l1.rbegin());
  FUN_VALUE(l1.front());
  FUN_VALUE(l1.back());
  std::cout << std::boolalpha;
  FUN_VALUE(l1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(l1.size());
  FUN_VALUE(l1.max_size());
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|       insert        |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P2(list<int>, insert, end, rand(), SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#else
  CON_TEST_P2(list<int>, insert, end, rand(), SCALE_M(LEN1), SCALE_M(LEN2), SCALE_M(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|         sort        |";
#if LARGER_TEST_DATA_ON
  LIST_SORT_TEST(SCALE_M(LEN1), SCALE_M(LEN2), SCALE_M(LEN3));
#else
  LIST_SORT_TEST(SCALE_S(LEN1), SCALE_S(LEN2), SCALE_S(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[------------------ End container test : list ------------------]" << std::endl;
}

} // namespace list_test
} // namespace test


namespace test
{
namespace deque_test
{

void deque_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[----------------- Run container test : deque ------------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  int a[] = { 1,2,3,4,5 };
  phq::deque<int> d1;
  phq::deque<int> d2(5);
  phq::deque<int> d3(5, 1);
  phq::deque<int> d4(a, a + 5);
  phq::deque<int> d5(d2);
  phq::deque<int> d6(std::move(d2));
  phq::deque<int> d7;
  d7 = d3;
  phq::deque<int> d8;
  d8 = std::move(d3);
  phq::deque<int> d9{ 1,2,3,4,5,6,7,8,9 };
  phq::deque<int> d10;
  d10 = { 1,2,3,4,5,6,7,8,9 };

  FUN_AFTER(d1, d1.assign(5, 1));
  FUN_AFTER(d1, d1.assign(8, 8));
  FUN_AFTER(d1, d1.assign(a, a + 5));
  FUN_AFTER(d1, d1.assign({ 1,2,3,4,5 }));
  FUN_AFTER(d1, d1.insert(d1.end(), 6));
  FUN_AFTER(d1, d1.insert(d1.end() - 1, 2, 7));
  FUN_AFTER(d1, d1.insert(d1.begin(), a, a + 5));
  FUN_AFTER(d1, d1.erase(d1.begin()));
  FUN_AFTER(d1, d1.erase(d1.begin(), d1.begin() + 4));
  FUN_AFTER(d1, d1.emplace_back(8));
  FUN_AFTER(d1, d1.emplace_front(8));
  FUN_AFTER(d1, d1.emplace(d1.begin() + 1, 9));
  FUN_AFTER(d1, d1.push_front(1));
  FUN_AFTER(d1, d1.push_back(2));
  FUN_AFTER(d1, d1.pop_back());
  FUN_AFTER(d1, d1.pop_front());
  FUN_AFTER(d1, d1.shrink_to_fit());
  FUN_AFTER(d1, d1.resize(5));
  FUN_AFTER(d1, d1.resize(8, 8));
  FUN_AFTER(d1, d1.clear());
  FUN_AFTER(d1, d1.shrink_to_fit());
  FUN_AFTER(d1, d1.swap(d4));
  FUN_VALUE(*(d1.begin()));
  FUN_VALUE(*(d1.end() - 1));
  FUN_VALUE(*(d1.rbegin()));
  FUN_VALUE(*(d1.rend() - 1));
  FUN_VALUE(d1.front());
  FUN_VALUE(d1.back());
  FUN_VALUE(d1.at(1));
  FUN_VALUE(d1[2]);
  std::cout << std::boolalpha;
  FUN_VALUE(d1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(d1.size());
  FUN_VALUE(d1.max_size());
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|     push_front      |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P1(deque<int>, push_front, rand(), SCALE_LL(LEN1), SCALE_LL(LEN2), SCALE_LL(LEN3));
#else
  CON_TEST_P1(deque<int>, push_front, rand(), SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|     push_back       |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P1(deque<int>, push_back, rand(), SCALE_LL(LEN1), SCALE_LL(LEN2), SCALE_LL(LEN3));
#else
  CON_TEST_P1(deque<int>, push_back, rand(), SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[----------------- End container test : deque ------------------]" << std::endl;
}

} // namespace deque_test
} // namespace test


namespace test
{
namespace stack_test
{

void stack_print(phq::stack<int> s)
{
  while (!s.empty())
  {
    std::cout << " " << s.top();
    s.pop();
  }
  std::cout << std::endl;
}

// stack 的遍历输出
#define STACK_COUT(s) do {                       \
    std::string s_name = #s;                     \
    std::cout << " " << s_name << " :";          \
    stack_print(s);                              \
} while(0)

#define STACK_FUN_AFTER(con, fun) do {           \
  std::string fun_name = #fun;                   \
  std::cout << " After " << fun_name << " :\n";  \
  fun;                                           \
  STACK_COUT(con);                               \
} while(0)

void stack_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[----------------- Run container test : stack ------------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  int a[] = { 1,2,3,4,5 };
  phq::deque<int> d1(5);
  phq::stack<int> s1;
  phq::stack<int> s2(5);
  phq::stack<int> s3(5, 1);
  phq::stack<int> s4(a, a + 5);
  phq::stack<int> s5(d1);
  phq::stack<int> s6(std::move(d1));
  phq::stack<int> s7(s2);
  phq::stack<int> s8(std::move(s2));
  phq::stack<int> s9;
  s9 = s3;
  phq::stack<int> s10;
  s10 = std::move(s3);
  phq::stack<int> s11{ 1,2,3,4,5 };
  phq::stack<int> s12;
  s12 = { 1,2,3,4,5 };

  STACK_FUN_AFTER(s1, s1.push(1));
  STACK_FUN_AFTER(s1, s1.push(2));
  STACK_FUN_AFTER(s1, s1.push(3));
  STACK_FUN_AFTER(s1, s1.pop());
  STACK_FUN_AFTER(s1, s1.emplace(4));
  STACK_FUN_AFTER(s1, s1.emplace(5));
  std::cout << std::boolalpha;
  FUN_VALUE(s1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(s1.size());
  FUN_VALUE(s1.top());
  while (!s1.empty())
  {
    STACK_FUN_AFTER(s1, s1.pop());
  }
  STACK_FUN_AFTER(s1, s1.swap(s4));
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|         push        |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P1(stack<int>, push, rand(), SCALE_LL(LEN1), SCALE_LL(LEN2), SCALE_LL(LEN3));
#else
  CON_TEST_P1(stack<int>, push, rand(), SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[----------------- End container test : stack ------------------]" << std::endl;
}

} // namespace stack_test
} // namespace test


namespace test
{
namespace queue_test
{

void queue_print(phq::queue<int> q)
{
  while (!q.empty())
  {
    std::cout << " " << q.front();
    q.pop();
  }
  std::cout << std::endl;
}


//  queue 的遍历输出
#define QUEUE_COUT(q) do {                       \
    std::string q_name = #q;                     \
    std::cout << " " << q_name << " :";          \
    queue_print(q);                              \
} while(0)

// priority_queue 的遍历输出
#define P_QUEUE_COUT(p) do {                     \
    std::string p_name = #p;                     \
    std::cout << " " << p_name << " :";          \
    p_queue_print(p);                            \
} while(0)

#define QUEUE_FUN_AFTER(con, fun) do {           \
  std::string fun_name = #fun;                   \
  std::cout << " After " << fun_name << " :\n";  \
  fun;                                           \
  QUEUE_COUT(con);                               \
} while(0)

#define P_QUEUE_FUN_AFTER(con, fun) do {         \
  std::string fun_name = #fun;                   \
  std::cout << " After " << fun_name << " :\n";  \
  fun;                                           \
  P_QUEUE_COUT(con);                             \
} while(0)

void queue_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[----------------- Run container test : queue ------------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  int a[] = { 1,2,3,4,5 };
  phq::deque<int> d1(5);
  phq::queue<int> q1;
  phq::queue<int> q2(5);
  phq::queue<int> q3(5, 1);
  phq::queue<int> q4(a, a + 5);
  phq::queue<int> q5(d1);
  phq::queue<int> q6(std::move(d1));
  phq::queue<int> q7(q2);
  phq::queue<int> q8(std::move(q2));
  phq::queue<int> q9;
  q9 = q3;
  phq::queue<int> q10;
  q10 = std::move(q3);
  phq::queue<int> q11{ 1,2,3,4,5 };
  phq::queue<int> q12;
  q12 = { 1,2,3,4,5 };

  QUEUE_FUN_AFTER(q1, q1.push(1));
  QUEUE_FUN_AFTER(q1, q1.push(2));
  QUEUE_FUN_AFTER(q1, q1.push(3));
  QUEUE_FUN_AFTER(q1, q1.pop());
  QUEUE_FUN_AFTER(q1, q1.emplace(4));
  QUEUE_FUN_AFTER(q1, q1.emplace(5));
  std::cout << std::boolalpha;
  FUN_VALUE(q1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(q1.size());
  FUN_VALUE(q1.front());
  FUN_VALUE(q1.back());
  while (!q1.empty())
  {
    QUEUE_FUN_AFTER(q1, q1.pop());
  }
  QUEUE_FUN_AFTER(q1, q1.swap(q4));
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|         push        |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P1(queue<int>, push, rand(), SCALE_LL(LEN1), SCALE_LL(LEN2), SCALE_LL(LEN3));
#else
  CON_TEST_P1(queue<int>, push, rand(), SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[----------------- End container test : queue ------------------]" << std::endl;
}
} // namespace queue_test
} // namespace test


namespace test
{
namespace set_test
{

void set_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[------------------ Run container test : set -------------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  int a[] = { 5,4,3,2,1 };
  phq::set<int> s1;
  phq::set<int, std::greater<int>> s2;
  phq::set<int> s3(a, a + 5);
  phq::set<int> s4(a, a + 5);
  phq::set<int> s5(s3);
  phq::set<int> s6(std::move(s3));
  phq::set<int> s7;
  s7 = s4;
  phq::set<int> s8;
  s8 = std::move(s4);
  phq::set<int> s9{ 1,2,3,4,5 };
  phq::set<int> s10;
  s10 = { 1,2,3,4,5 };

  for (int i = 5; i > 0; --i)
  {
    FUN_AFTER(s1, s1.emplace(i));
  }
  FUN_AFTER(s1, s1.emplace_hint(s1.begin(), 0));
  FUN_AFTER(s1, s1.erase(s1.begin()));
  FUN_AFTER(s1, s1.erase(0));
  FUN_AFTER(s1, s1.erase(1));
  FUN_AFTER(s1, s1.erase(s1.begin(), s1.end()));
  for (int i = 0; i < 5; ++i)
  {
    FUN_AFTER(s1, s1.insert(i));
  }
  FUN_AFTER(s1, s1.insert(a, a + 5));
  FUN_AFTER(s1, s1.insert(5));
  FUN_AFTER(s1, s1.insert(s1.end(), 5));
  FUN_VALUE(s1.count(5));
  FUN_VALUE(*s1.find(3));
  FUN_VALUE(*s1.lower_bound(3));
  FUN_VALUE(*s1.upper_bound(3));
  auto first = *s1.equal_range(3).first;
  auto second = *s1.equal_range(3).second;
  std::cout << " s1.equal_range(3) : from " << first << " to " << second << std::endl;
  FUN_AFTER(s1, s1.erase(s1.begin()));
  FUN_AFTER(s1, s1.erase(1));
  FUN_AFTER(s1, s1.erase(s1.begin(), s1.find(3)));
  FUN_AFTER(s1, s1.clear());
  FUN_AFTER(s1, s1.swap(s5));
  FUN_VALUE(*s1.begin());
  FUN_VALUE(*s1.rbegin());
  std::cout << std::boolalpha;
  FUN_VALUE(s1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(s1.size());
  FUN_VALUE(s1.max_size());
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|       emplace       |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P1(set<int>, emplace, rand(), SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#else
  CON_TEST_P1(set<int>, emplace, rand(), SCALE_M(LEN1), SCALE_M(LEN2), SCALE_M(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[------------------ End container test : set -------------------]" << std::endl;
}

void multiset_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[---------------- Run container test : multiset ----------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  int a[] = { 5,4,3,2,1 };
  phq::multiset<int> s1;
  phq::multiset<int, std::greater<int>> s2;
  phq::multiset<int> s3(a, a + 5);
  phq::multiset<int> s4(a, a + 5);
  phq::multiset<int> s5(s3);
  phq::multiset<int> s6(std::move(s3));
  phq::multiset<int> s7;
  s7 = s4;
  phq::multiset<int> s8;
  s8 = std::move(s4);
  phq::multiset<int> s9{ 1,2,3,4,5 };
  phq::multiset<int> s10;
  s10 = { 1,2,3,4,5 };

  for (int i = 5; i > 0; --i)
  {
    FUN_AFTER(s1, s1.emplace(i));
  }
  FUN_AFTER(s1, s1.emplace_hint(s1.begin(), 0));
  FUN_AFTER(s1, s1.erase(s1.begin()));
  FUN_AFTER(s1, s1.erase(0));
  FUN_AFTER(s1, s1.erase(1));
  FUN_AFTER(s1, s1.erase(s1.begin(), s1.end()));
  for (int i = 0; i < 5; ++i)
  {
    FUN_AFTER(s1, s1.insert(i));
  }
  FUN_AFTER(s1, s1.insert(a, a + 5));
  FUN_AFTER(s1, s1.insert(5));
  FUN_AFTER(s1, s1.insert(s1.end(), 5));
  FUN_VALUE(s1.count(5));
  FUN_VALUE(*s1.find(3));
  FUN_VALUE(*s1.lower_bound(3));
  FUN_VALUE(*s1.upper_bound(3));
  auto first = *s1.equal_range(3).first;
  auto second = *s1.equal_range(3).second;
  std::cout << " s1.equal_range(3) : from " << first << " to " << second << std::endl;
  FUN_AFTER(s1, s1.erase(s1.begin()));
  FUN_AFTER(s1, s1.erase(1));
  FUN_AFTER(s1, s1.erase(s1.begin(), s1.find(3)));
  FUN_AFTER(s1, s1.clear());
  FUN_AFTER(s1, s1.swap(s5));
  FUN_VALUE(*s1.begin());
  FUN_VALUE(*s1.rbegin());
  std::cout << std::boolalpha;
  FUN_VALUE(s1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(s1.size());
  FUN_VALUE(s1.max_size());
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|       emplace       |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P1(multiset<int>, emplace, rand(), SCALE_M(LEN1), SCALE_M(LEN2), SCALE_M(LEN3));
#else
  CON_TEST_P1(multiset<int>, emplace, rand(), SCALE_S(LEN1), SCALE_S(LEN2), SCALE_S(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[---------------- End container test : multiset ----------------]" << std::endl;
}

} // namespace set_test
} // namespace test


namespace test
{
namespace map_test
{

// pair 的宏定义
#define PAIR    phq::pair<int, int>

// map 的遍历输出
#define MAP_COUT(m) do { \
    std::string m_name = #m; \
    std::cout << " " << m_name << " :"; \
    for (auto it : m)    std::cout << " <" << it.first << "," << it.second << ">"; \
    std::cout << std::endl; \
} while(0)

// map 的函数操作
#define MAP_FUN_AFTER(con, fun) do { \
    std::string str = #fun; \
    std::cout << " After " << str << " :" << std::endl; \
    fun; \
    MAP_COUT(con); \
} while(0)

// map 的函数值
#define MAP_VALUE(fun) do { \
    std::string str = #fun; \
    auto it = fun; \
    std::cout << " " << str << " : <" << it.first << "," << it.second << ">\n"; \
} while(0)

void map_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[------------------ Run container test : map -------------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  phq::vector<PAIR> v;
  for (int i = 0; i < 5; ++i)
    v.push_back(PAIR(i, i));
  phq::map<int, int> m1;
  phq::map<int, int, std::greater<int>> m2;
  phq::map<int, int> m3(v.begin(), v.end());
  phq::map<int, int> m4(v.begin(), v.end());
  phq::map<int, int> m5(m3);
  phq::map<int, int> m6(std::move(m3));
  phq::map<int, int> m7;
  m7 = m4;
  phq::map<int, int> m8;
  m8 = std::move(m4);
  phq::map<int, int> m9{ PAIR(1,1),PAIR(3,2),PAIR(2,3) };
  phq::map<int, int> m10;
  m10 = { PAIR(1,1),PAIR(3,2),PAIR(2,3) };

  for (int i = 5; i > 0; --i)
  {
    MAP_FUN_AFTER(m1, m1.emplace(i, i));
  }
  MAP_FUN_AFTER(m1, m1.emplace_hint(m1.begin(), 0, 0));
  MAP_FUN_AFTER(m1, m1.erase(m1.begin()));
  MAP_FUN_AFTER(m1, m1.erase(0));
  MAP_FUN_AFTER(m1, m1.erase(1));
  MAP_FUN_AFTER(m1, m1.erase(m1.begin(), m1.end()));
  for (int i = 0; i < 5; ++i)
  {
    MAP_FUN_AFTER(m1, m1.insert(PAIR(i, i)));
  }
  MAP_FUN_AFTER(m1, m1.insert(v.begin(), v.end()));
  MAP_FUN_AFTER(m1, m1.insert(m1.end(), PAIR(5, 5)));
  FUN_VALUE(m1.count(1));
  MAP_VALUE(*m1.find(3));
  MAP_VALUE(*m1.lower_bound(3));
  MAP_VALUE(*m1.upper_bound(2));
  auto first = *m1.equal_range(2).first;
  auto second = *m1.equal_range(2).second;
  std::cout << " m1.equal_range(2) : from <" << first.first << ", " << first.second
    << "> to <" << second.first << ", " << second.second << ">" << std::endl;
  MAP_FUN_AFTER(m1, m1.erase(m1.begin()));
  MAP_FUN_AFTER(m1, m1.erase(1));
  MAP_FUN_AFTER(m1, m1.erase(m1.begin(), m1.find(3)));
  MAP_FUN_AFTER(m1, m1.clear());
  MAP_FUN_AFTER(m1, m1.swap(m9));
  MAP_VALUE(*m1.begin());
  MAP_VALUE(*m1.rbegin());
  FUN_VALUE(m1[1]);
  MAP_FUN_AFTER(m1, m1[1] = 3);
  FUN_VALUE(m1.at(1));
  std::cout << std::boolalpha;
  FUN_VALUE(m1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(m1.size());
  FUN_VALUE(m1.max_size());
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|       emplace       |";
#if LARGER_TEST_DATA_ON
  MAP_EMPLACE_TEST(map, SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#else
  MAP_EMPLACE_TEST(map, SCALE_M(LEN1), SCALE_M(LEN2), SCALE_M(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[------------------ End container test : map -------------------]" << std::endl;
}

void multimap_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[---------------- Run container test : multimap ----------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  phq::vector<PAIR> v;
  for (int i = 0; i < 5; ++i)
    v.push_back(PAIR(i, i));
  phq::multimap<int, int> m1;
  phq::multimap<int, int, std::greater<int>> m2;
  phq::multimap<int, int> m3(v.begin(), v.end());
  phq::multimap<int, int> m4(v.begin(), v.end());
  phq::multimap<int, int> m5(m3);
  phq::multimap<int, int> m6(std::move(m3));
  phq::multimap<int, int> m7;
  m7 = m4;
  phq::multimap<int, int> m8;
  m8 = std::move(m4);
  phq::multimap<int, int> m9{ PAIR(1,1),PAIR(3,2),PAIR(2,3) };
  phq::multimap<int, int> m10;
  m10 = { PAIR(1,1),PAIR(3,2),PAIR(2,3) };

  for (int i = 5; i > 0; --i)
  {
    MAP_FUN_AFTER(m1, m1.emplace(i, i));
  }
  MAP_FUN_AFTER(m1, m1.emplace_hint(m1.begin(), 0, 0));
  MAP_FUN_AFTER(m1, m1.erase(m1.begin()));
  MAP_FUN_AFTER(m1, m1.erase(0));
  MAP_FUN_AFTER(m1, m1.erase(1));
  MAP_FUN_AFTER(m1, m1.erase(m1.begin(), m1.end()));
  for (int i = 0; i < 5; ++i)
  {
    MAP_FUN_AFTER(m1, m1.insert(phq::make_pair(i, i)));
  }
  MAP_FUN_AFTER(m1, m1.insert(v.begin(), v.end()));
  MAP_FUN_AFTER(m1, m1.insert(PAIR(5, 5)));
  MAP_FUN_AFTER(m1, m1.insert(m1.end(), PAIR(5, 5)));
  FUN_VALUE(m1.count(3));
  MAP_VALUE(*m1.find(3));
  MAP_VALUE(*m1.lower_bound(3));
  MAP_VALUE(*m1.upper_bound(2));
  auto first = *m1.equal_range(2).first;
  auto second = *m1.equal_range(2).second;
  std::cout << " m1.equal_range(2) : from <" << first.first << ", " << first.second
    << "> to <" << second.first << ", " << second.second << ">" << std::endl;
  MAP_FUN_AFTER(m1, m1.erase(m1.begin()));
  MAP_FUN_AFTER(m1, m1.erase(1));
  MAP_FUN_AFTER(m1, m1.erase(m1.begin(), m1.find(3)));
  MAP_FUN_AFTER(m1, m1.clear());
  MAP_FUN_AFTER(m1, m1.swap(m9));
  MAP_FUN_AFTER(m1, m1.insert(PAIR(3, 3)));
  MAP_VALUE(*m1.begin());
  MAP_VALUE(*m1.rbegin());
  std::cout << std::boolalpha;
  FUN_VALUE(m1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(m1.size());
  FUN_VALUE(m1.max_size());
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|       emplace       |";
#if LARGER_TEST_DATA_ON
  MAP_EMPLACE_TEST(multimap, SCALE_M(LEN1), SCALE_M(LEN2), SCALE_M(LEN3));
#else
  MAP_EMPLACE_TEST(multimap, SCALE_S(LEN1), SCALE_S(LEN2), SCALE_S(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[---------------- End container test : multimap ----------------]" << std::endl;
}

} // namespace map_test
} // namespace test


namespace test
{
namespace unordered_set_test
{

void unordered_set_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[-------------- Run container test : unordered_set -------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  int a[] = { 5,4,3,2,1 };
  phq::unordered_set<int> us1;
  phq::unordered_set<int> us2(520);
  phq::unordered_set<int> us3(520, std::hash<int>());
  phq::unordered_set<int> us4(520, std::hash<int>(), std::equal_to<int>());
  phq::unordered_set<int> us5(a, a + 5);
  phq::unordered_set<int> us6(a, a + 5, 100);
  phq::unordered_set<int> us7(a, a + 5, 100, std::hash<int>());
  phq::unordered_set<int> us8(a, a + 5, 100, std::hash<int>(), std::equal_to<int>());
  phq::unordered_set<int> us9(us5);
  phq::unordered_set<int> us10(std::move(us5));
  phq::unordered_set<int> us11;
  us11 = us6;
  phq::unordered_set<int> us12;
  us12 = std::move(us6);
  phq::unordered_set<int> us13{ 1,2,3,4,5 };
  phq::unordered_set<int> us14;
  us13 = { 1,2,3,4,5 };

  FUN_AFTER(us1, us1.emplace(1));
  FUN_AFTER(us1, us1.emplace_hint(us1.end(), 2));
  FUN_AFTER(us1, us1.insert(5));
  FUN_AFTER(us1, us1.insert(us1.begin(), 5));
  FUN_AFTER(us1, us1.insert(a, a + 5));
  FUN_AFTER(us1, us1.erase(us1.begin()));
  FUN_AFTER(us1, us1.erase(us1.begin(), us1.find(3)));
  FUN_AFTER(us1, us1.erase(1));
  std::cout << std::boolalpha;
  FUN_VALUE(us1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(us1.size());
  FUN_VALUE(us1.bucket_count());
  FUN_VALUE(us1.max_bucket_count());
  FUN_VALUE(us1.bucket(1));
  FUN_VALUE(us1.bucket_size(us1.bucket(5)));
  FUN_AFTER(us1, us1.clear());
  FUN_AFTER(us1, us1.swap(us7));
  FUN_VALUE(*us1.begin());
  std::cout << std::boolalpha;
  FUN_VALUE(us1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(us1.size());
  FUN_VALUE(us1.max_size());
  FUN_VALUE(us1.bucket_count());
  FUN_AFTER(us1, us1.reserve(1000));
  FUN_VALUE(*us1.begin(us1.bucket(1)));
  FUN_VALUE(us1.size());
  FUN_VALUE(us1.bucket_count());
  FUN_VALUE(us1.bucket_size(1));
  FUN_VALUE(us1.bucket_size(2));
  FUN_VALUE(us1.bucket_size(3));
  FUN_AFTER(us1, us1.rehash(150));
  FUN_VALUE(us1.bucket_count());
  FUN_VALUE(us1.count(1));
  FUN_VALUE(*us1.find(3));
  auto first = *us1.equal_range(3).first;
  auto second = *us1.equal_range(3).second;
  std::cout << " us1.equal_range(3) : from " << first << " to " << second << std::endl;
  FUN_VALUE(us1.load_factor());
  FUN_VALUE(us1.max_load_factor());
  FUN_AFTER(us1, us1.max_load_factor(1.5f));
  FUN_VALUE(us1.max_load_factor());
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|       emplace       |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P1(unordered_set<int>, emplace, rand(), SCALE_L(LEN1), SCALE_L(LEN2), SCALE_L(LEN3));
#else
  CON_TEST_P1(unordered_set<int>, emplace, rand(), SCALE_M(LEN1), SCALE_M(LEN2), SCALE_M(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[-------------- End container test : unordered_set -------------]" << std::endl;
}

void unordered_multiset_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[------------ Run container test : unordered_multiset ----------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  int a[] = { 5,4,3,2,1 };
  phq::unordered_multiset<int> us1;
  phq::unordered_multiset<int> us2(520);
  phq::unordered_multiset<int> us3(520, std::hash<int>());
  phq::unordered_multiset<int> us4(520, std::hash<int>(), std::equal_to<int>());
  phq::unordered_multiset<int> us5(a, a + 5);
  phq::unordered_multiset<int> us6(a, a + 5, 100);
  phq::unordered_multiset<int> us7(a, a + 5, 100, std::hash<int>());
  phq::unordered_multiset<int> us8(a, a + 5, 100, std::hash<int>(), std::equal_to<int>());
  phq::unordered_multiset<int> us9(us5);
  phq::unordered_multiset<int> us10(std::move(us5));
  phq::unordered_multiset<int> us11;
  us11 = us6;
  phq::unordered_multiset<int> us12;
  us12 = std::move(us6);
  phq::unordered_multiset<int> us13{ 1,2,3,4,5 };
  phq::unordered_multiset<int> us14;
  us14 = { 1,2,3,4,5 };

  FUN_AFTER(us1, us1.emplace(1));
  FUN_AFTER(us1, us1.emplace_hint(us1.end(), 2));
  FUN_AFTER(us1, us1.insert(5));
  FUN_AFTER(us1, us1.insert(us1.begin(), 5));
  FUN_AFTER(us1, us1.insert(a, a + 5));
  FUN_AFTER(us1, us1.erase(us1.begin()));
  FUN_AFTER(us1, us1.erase(us1.begin(), us1.find(3)));
  FUN_AFTER(us1, us1.erase(1));
  std::cout << std::boolalpha;
  FUN_VALUE(us1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(us1.size());
  FUN_VALUE(us1.bucket_count());
  FUN_VALUE(us1.max_bucket_count());
  FUN_VALUE(us1.bucket(1));
  FUN_VALUE(us1.bucket_size(us1.bucket(5)));
  FUN_AFTER(us1, us1.clear());
  FUN_AFTER(us1, us1.swap(us7));
  FUN_VALUE(*us1.begin());
  std::cout << std::boolalpha;
  FUN_VALUE(us1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(us1.size());
  FUN_VALUE(us1.max_size());
  FUN_VALUE(us1.bucket_count());
  FUN_AFTER(us1, us1.reserve(1000));
  FUN_VALUE(*us1.begin(us1.bucket(1)));
  FUN_VALUE(us1.size());
  FUN_VALUE(us1.bucket_count());
  FUN_VALUE(us1.bucket_size(1));
  FUN_VALUE(us1.bucket_size(2));
  FUN_VALUE(us1.bucket_size(3));
  FUN_AFTER(us1, us1.rehash(150));
  FUN_VALUE(us1.bucket_count());
  FUN_VALUE(us1.count(1));
  FUN_VALUE(*us1.find(3));
  auto first = *us1.equal_range(3).first;
  auto second = *us1.equal_range(3).second;
  std::cout << " us1.equal_range(3) : from " << first << " to " << second << std::endl;
  FUN_VALUE(us1.load_factor());
  FUN_VALUE(us1.max_load_factor());
  FUN_AFTER(us1, us1.max_load_factor(1.5f));
  FUN_VALUE(us1.max_load_factor());
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|       emplace       |";
#if LARGER_TEST_DATA_ON
  CON_TEST_P1(unordered_multiset<int>, emplace, rand(), SCALE_M(LEN1), SCALE_M(LEN2), SCALE_M(LEN3));
#else
  CON_TEST_P1(unordered_multiset<int>, emplace, rand(), SCALE_S(LEN1), SCALE_S(LEN2), SCALE_S(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[------------ End container test : unordered_multiset ----------]" << std::endl;
}

} // namespace unordered_set_test
} // namespace test

namespace test
{
namespace unordered_map_test
{

void unordered_map_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[-------------- Run container test : unordered_map -------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  phq::vector<PAIR> v;
  for (int i = 0; i < 5; ++i)
    v.push_back(PAIR(5 - i, 5 - i));
  phq::unordered_map<int, int> um1;
  phq::unordered_map<int, int> um2(520);
  phq::unordered_map<int, int> um3(520, std::hash<int>());
  phq::unordered_map<int, int> um4(520, std::hash<int>(), std::equal_to<int>());
  phq::unordered_map<int, int> um5(v.begin(), v.end());
  phq::unordered_map<int, int> um6(v.begin(), v.end(), 100);
  phq::unordered_map<int, int> um7(v.begin(), v.end(), 100, std::hash<int>());
  phq::unordered_map<int, int> um8(v.begin(), v.end(), 100, std::hash<int>(), std::equal_to<int>());
  phq::unordered_map<int, int> um9(um5);
  phq::unordered_map<int, int> um10(std::move(um5));
  phq::unordered_map<int, int> um11;
  um11 = um6;
  phq::unordered_map<int, int> um12;
  um12 = std::move(um6);
  phq::unordered_map<int, int> um13{ PAIR(1,1),PAIR(2,3),PAIR(3,3) };
  phq::unordered_map<int, int> um14;
  um14 = { PAIR(1,1),PAIR(2,3),PAIR(3,3) };

  MAP_FUN_AFTER(um1, um1.emplace(1, 1));
  MAP_FUN_AFTER(um1, um1.emplace_hint(um1.begin(), 1, 2));
  MAP_FUN_AFTER(um1, um1.insert(PAIR(2, 2)));
  MAP_FUN_AFTER(um1, um1.insert(um1.end(), PAIR(3, 3)));
  MAP_FUN_AFTER(um1, um1.insert(v.begin(), v.end()));
  MAP_FUN_AFTER(um1, um1.erase(um1.begin()));
  MAP_FUN_AFTER(um1, um1.erase(um1.begin(), um1.find(3)));
  MAP_FUN_AFTER(um1, um1.erase(1));
  std::cout << std::boolalpha;
  FUN_VALUE(um1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(um1.size());
  FUN_VALUE(um1.bucket_count());
  FUN_VALUE(um1.max_bucket_count());
  FUN_VALUE(um1.bucket(1));
  FUN_VALUE(um1.bucket_size(um1.bucket(5)));
  MAP_FUN_AFTER(um1, um1.clear());
  MAP_FUN_AFTER(um1, um1.swap(um7));
  MAP_VALUE(*um1.begin());
  FUN_VALUE(um1.at(1));
  FUN_VALUE(um1[1]);
  std::cout << std::boolalpha;
  FUN_VALUE(um1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(um1.size());
  FUN_VALUE(um1.max_size());
  FUN_VALUE(um1.bucket_count());
  FUN_VALUE(um1.max_bucket_count());
  FUN_VALUE(um1.bucket(1));
  FUN_VALUE(um1.bucket_size(um1.bucket(1)));
  MAP_FUN_AFTER(um1, um1.reserve(1000));
  FUN_VALUE(um1.size());
  FUN_VALUE(um1.bucket_count());
  FUN_VALUE(um1.bucket_size(1));
  FUN_VALUE(um1.bucket_size(2));
  FUN_VALUE(um1.bucket_size(3));
  MAP_FUN_AFTER(um1, um1.rehash(150));
  FUN_VALUE(um1.bucket_count());
  FUN_VALUE(um1.count(1));
  MAP_VALUE(*um1.find(3));
  auto first = *um1.equal_range(3).first;
  auto second = *um1.equal_range(3).second;
  std::cout << " um1.equal_range(3) : from <" << first.first << ", " << first.second
    << "> to <" << second.first << ", " << second.second << ">" << std::endl;
  FUN_VALUE(um1.load_factor());
  FUN_VALUE(um1.max_load_factor());
  MAP_FUN_AFTER(um1, um1.max_load_factor(1.5f));
  FUN_VALUE(um1.max_load_factor());
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|       emplace       |";
#if LARGER_TEST_DATA_ON
  MAP_EMPLACE_TEST(unordered_map, SCALE_M(LEN1), SCALE_M(LEN2), SCALE_M(LEN3));
#else
  MAP_EMPLACE_TEST(unordered_map, SCALE_S(LEN1), SCALE_S(LEN2), SCALE_S(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[-------------- End container test : unordered_map -------------]" << std::endl;
}

void unordered_multimap_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[----------- Run container test : unordered_multimap -----------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  phq::vector<PAIR> v;
  for (int i = 0; i < 5; ++i)
    v.push_back(PAIR(5 - i, 5 - i));
  phq::unordered_multimap<int, int> um1;
  phq::unordered_multimap<int, int> um2(520);
  phq::unordered_multimap<int, int> um3(520, std::hash<int>());
  phq::unordered_multimap<int, int> um4(520, std::hash<int>(), std::equal_to<int>());
  phq::unordered_multimap<int, int> um5(v.begin(), v.end());
  phq::unordered_multimap<int, int> um6(v.begin(), v.end(), 100);
  phq::unordered_multimap<int, int> um7(v.begin(), v.end(), 100, std::hash<int>());
  phq::unordered_multimap<int, int> um8(v.begin(), v.end(), 100, std::hash<int>(), std::equal_to<int>());
  phq::unordered_multimap<int, int> um9(um5);
  phq::unordered_multimap<int, int> um10(std::move(um5));
  phq::unordered_multimap<int, int> um11;
  um11 = um6;
  phq::unordered_multimap<int, int> um12;
  um12 = std::move(um6);
  phq::unordered_multimap<int, int> um13{ PAIR(1,1),PAIR(2,3),PAIR(3,3) };
  phq::unordered_multimap<int, int> um14;
  um14 = { PAIR(1,1),PAIR(2,3),PAIR(3,3) };

  MAP_FUN_AFTER(um1, um1.emplace(1, 1));
  MAP_FUN_AFTER(um1, um1.emplace_hint(um1.begin(), 1, 2));
  MAP_FUN_AFTER(um1, um1.insert(PAIR(2, 2)));
  MAP_FUN_AFTER(um1, um1.insert(um1.end(), PAIR(3, 3)));
  MAP_FUN_AFTER(um1, um1.insert(v.begin(), v.end()));
  MAP_FUN_AFTER(um1, um1.erase(um1.begin()));
  MAP_FUN_AFTER(um1, um1.erase(um1.begin(), um1.find(3)));
  MAP_FUN_AFTER(um1, um1.erase(1));
  std::cout << std::boolalpha;
  FUN_VALUE(um1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(um1.size());
  FUN_VALUE(um1.bucket_count());
  FUN_VALUE(um1.max_bucket_count());
  FUN_VALUE(um1.bucket(1));
  FUN_VALUE(um1.bucket_size(um1.bucket(5)));
  MAP_FUN_AFTER(um1, um1.clear());
  MAP_FUN_AFTER(um1, um1.swap(um7));
  MAP_VALUE(*um1.begin());
  std::cout << std::boolalpha;
  FUN_VALUE(um1.empty());
  std::cout << std::noboolalpha;
  FUN_VALUE(um1.size());
  FUN_VALUE(um1.max_size());
  FUN_VALUE(um1.bucket_count());
  FUN_VALUE(um1.max_bucket_count());
  FUN_VALUE(um1.bucket(1));
  FUN_VALUE(um1.bucket_size(um1.bucket(1)));
  MAP_FUN_AFTER(um1, um1.reserve(1000));
  FUN_VALUE(um1.size());
  FUN_VALUE(um1.bucket_count());
  FUN_VALUE(um1.bucket_size(1));
  FUN_VALUE(um1.bucket_size(2));
  FUN_VALUE(um1.bucket_size(3));
  MAP_FUN_AFTER(um1, um1.rehash(150));
  FUN_VALUE(um1.bucket_count());
  FUN_VALUE(um1.count(1));
  MAP_VALUE(*um1.find(3));
  auto first = *um1.equal_range(3).first;
  auto second = *um1.equal_range(3).second;
  std::cout << " um1.equal_range(3) : from <" << first.first << ", " << first.second
    << "> to <" << second.first << ", " << second.second << ">" << std::endl;
  FUN_VALUE(um1.load_factor());
  FUN_VALUE(um1.max_load_factor());
  MAP_FUN_AFTER(um1, um1.max_load_factor(1.5f));
  FUN_VALUE(um1.max_load_factor());
  PASSED;
#if PERFORMANCE_TEST_ON
  std::cout << "[--------------------- Performance Testing ---------------------]" << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  std::cout << "|       emplace       |";
#if LARGER_TEST_DATA_ON
  MAP_EMPLACE_TEST(unordered_multimap, SCALE_M(LEN1), SCALE_M(LEN2), SCALE_M(LEN3));
#else
  MAP_EMPLACE_TEST(unordered_multimap, SCALE_S(LEN1), SCALE_S(LEN2), SCALE_S(LEN3));
#endif
  std::cout << std::endl;
  std::cout << "|---------------------|-------------|-------------|-------------|" << std::endl;
  PASSED;
#endif
  std::cout << "[----------- End container test : unordered_multimap -----------]" << std::endl;
}
} // namespace unordered_map_test
} // namespace test

} // namespace phq