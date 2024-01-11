# strong_types.hpp

This single-header lib provides a simple to plug-in way to define common operators on custom types with _mostly_ zero-cost as you'd see in many other similar strong types implementations.

The intent is extreme simplicity; for a much more feature rich implementation, consider [rollbear's version](https://github.com/rollbear/strong_type)

## Usage

Add the folder to your project, include "strong_type/st.hpp"

Add your types and express your operators:

```cpp
#include "strong_type/st.hpp

using px_t = type<double, struct price_tag>;
using qty_t = type<double, struct quantity_tag>;

template<>
struct operators<px_t, px_t> {
  using add = px_t;
  using subtract = px_t;
};

template<>
struct operators<px_t, double> {
  using multiply = px_t;
};
```

See `st_test.cpp` for some examples.

The beauty is getting compilation errors when you try to do things you've not explicitly defined.

```cpp
px_t price{100.0};
qty_t qty{2,0};

(price + qty) // Error
(price - price) // Fine
(qty + qty) // Error -- not yet defined
(qty == price) // Error
(price * 5) // Fine
```

New operators can easily be added by adding them to `unary_operators.hpp` or `binary_operators.hpp`

## Performance

[TBD]

## Why macros?!!

There's a good argument to avoid such ugly preprocessor hacking -- it gets a bit redundant to define a greater set of operators however.
