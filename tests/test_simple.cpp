#include <doctest/doctest.h>
#include <sighan/sighan.hpp>

using doctest::test_suite;

TEST_CASE ("Simple" * test_suite{"simple"}) {
    sighan::SignalHandler sh{2};
}
