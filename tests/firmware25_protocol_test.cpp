#include "SparkBase.hpp"

#include <array>
#include <cassert>
#include <cstdint>

namespace
{
constexpr bool StandardPidSlotsHaveEightParameterStride()
{
  constexpr std::array<uint32_t, 8> slotZero = {13, 14, 15, 16, 17, 18, 19, 20};
  constexpr std::array<std::array<uint32_t, 8>, 4> expected = {{
    {{13, 14, 15, 16, 17, 18, 19, 20}},
    {{21, 22, 23, 24, 25, 26, 27, 28}},
    {{29, 30, 31, 32, 33, 34, 35, 36}},
    {{37, 38, 39, 40, 41, 42, 43, 44}},
  }};

  for (uint8_t slot = 0; slot < expected.size(); ++slot) {
    for (size_t field = 0; field < slotZero.size(); ++field) {
      if (sparkcan::firmware25::SlotParameterId(slotZero[field], slot) !=
          expected[slot][field]) {
        return false;
      }
    }
  }
  return true;
}
} // namespace

static_assert(PARAM_TYPE_INT == 1);
static_assert(PARAM_TYPE_UINT == 2);
static_assert(PARAM_TYPE_FLOAT == 3);
static_assert(PARAM_TYPE_BOOL == 4);
static_assert(StandardPidSlotsHaveEightParameterStride());

int main()
{
  using sparkcan::firmware25::ParameterReadArbId;
  using sparkcan::firmware25::ParameterWriteArbId;
  using sparkcan::firmware25::ParameterWriteResponseArbId;

  // Frames observed and verified against a SPARK MAX on firmware 25.0.4.
  assert(ParameterReadArbId(1, 13) == 0x02053D81);
  assert(ParameterReadArbId(1, 14) == 0x02053DC1);
  assert(ParameterReadArbId(1, 37) == 0x02054081);
  assert(ParameterWriteArbId(1) == 0x02053801);
  assert(ParameterWriteResponseArbId(1) == 0x02053841);

  // Both parameters in a pair intentionally share one RTR arbitration ID.
  assert(ParameterReadArbId(7, 12) == ParameterReadArbId(7, 13));
  assert(ParameterReadArbId(7, 20) == ParameterReadArbId(7, 21));
  return 0;
}
