#include <type_traits>

using namespace std;
int main() {
  static_assert(is_same_v<int, int>);
  static_assert(is_same_v<conditional_t<true, int, double>, int>);
  static_assert(is_same_v<remove_cvref_t<volatile int const&&>, int>);
}
