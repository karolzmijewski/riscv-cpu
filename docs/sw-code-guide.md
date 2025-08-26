# Coding Guide

This document provides a collection of tips, best practices, rules, and limitations for programming **IP**
(Intellectual Property) cores using **HLS** (High-Level Synthesis).

Unlike traditional software development, HLS-based IP design requires thinking in terms of hardware
architecture rather than sequential instruction execution. Although HLS allows you to describe hardware
using high-level programming languages (such as C/C++ or SystemC), the resulting implementation is ultimately
a circuit, not a software program.

## Performance

Programming for an **IP (Intellectual Property) core** differs significantly from common software application
programming, particularly in terms of performance considerations. While software focuses on instructions
executed sequentially by a processor, hardware design emphasizes **parallelism**, **timing**, and **critical path**
(the longest chain of dependent operations that a signal must propagate through in one clock cycle) optimization.
Unlike in software, not all basic operations have the same execution time in hardware.

Example:
- A logical `AND` takes a single gate delay.
- An arithmetic `ADD` requires a full-adder chain - which takes longer.

If both are in the same cycle, the the slower `ADD` operation determines the cycle duration, which directly
affects the total time needed to process data


Another key difference from software is how loops are executed. In software, loop iterations are processed
sequentially. In hardware, however, loops can be unrolled by synthesis tools and executed in parallel,
if provided operations are independent.

Example:
- In a software program, two independent functions inside a loop are executed one after another.
- In hardware (IP), those same functions can be executed in parallel, as long as neither one depends on the
result or state of the other.

In traditional CPU performance modeling, the overall execution time is given by:

```
time(P, D, C) = nmi(P, D, C) * cpi(P, D, C) * c(C)
```

Where:
- `P`: Program
- `D`: Data
- `C`: CPU
- `nmi`: Number of Machine Instructions (depends on P, D and C)
- `cpi`: Cycles Per Instruction (depends on P, D, and C)
- `c`: Cycle duration in seconds (depends only on C)

For IP cores, the same principles apply, but the emphasis shifts toward minimizing c(C) (cycle duration) by shortening the critical path and maximizing parallel execution to reduce cpi.

## Improving IP Performance

To improve performance, the main objective is to reduce the execution cycle time. This is achieved by
minimizing the length of the **critical path** — the longest path a signal must travel through computational
gates. The shorter the critical path, the faster the design can operate.

Some strategies include:

- Eliminating unnecessary conditional logic. For instance, instead of:

  `if (condition) doSomething();`

  If `doSomething()` does not affect global state or the next iteration’s result, it may be beneficial to
  execute it unconditionally. This avoids adding conditional gates, which can lengthen the **critical path**.

- Splitting code into independent functions:
  Even if functions are written sequentially, synthesis tools can recognize patterns and schedule independent
  functions in parallel. This allows the hardware to exploit concurrency, improving overall performance.
