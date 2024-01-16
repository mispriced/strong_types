#include "strong_type/st.hpp"
#include <iostream>

namespace strong_type {
using px_t = type<double, struct price_tag>;
using qty_t = type<double, struct quantity_tag>;
using not_t = type<double, struct notional_tag>;

// --- px_t operators ---
template <> struct operators<px_t, px_t> {
  using add = px_t;
  using subtract = px_t;
};

template <> struct operators<px_t, double> { using multiply = px_t; };

template <> struct operators<px_t, qty_t> { using multiply = not_t; };

template <> struct operators<px_t> { using minus = px_t; };

// --- qty_t operators ---
template <> struct operators<qty_t, qty_t> {
  using add = qty_t;
  using subtract = qty_t;
};

template <> struct operators<qty_t> {
  using pre_dec = qty_t;
  using pre_inc = qty_t;
  // using post_inc = qty_t;
};

} // namespace strong_type

using notional = strong_type::not_t;
using price = strong_type::px_t;
using quantity = strong_type::qty_t;

notional f(price px, quantity qty) { return -px * 2.0 * ++qty; }
double g(double px, double qty) { return -px * 2.0 * ++qty; }

int main(int argc, char **argv) {
  price px{100.0};
  quantity qty{2.0};

  auto result = f(px, --qty);
  ++qty;
  auto result2 = g(px.get(), --qty.value);

  std::cout << result.get() << " vs " << result2 << std::endl;
}