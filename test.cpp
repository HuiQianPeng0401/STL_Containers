#include <iostream>

#include "vector.h"
#include <random>
#include <ctime>
#include <cstdlib>
#include <list>
#include "deque.h"
#include <deque>
#include "list.h"
#include "test.h"
#include <stack>
#include "stack.h"
#include <queue>
#include "rb_tree.h"
#include <set>
#include "set.h"
#include "queue.h"

using std::cin;
using std::cout;
using std::endl;




namespace phq
{
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
} // namespace phq




int main() {
  phq::test::set_test::multiset_test();
}