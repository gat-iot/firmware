# GAT562 T9 Editor Regression Tests

Build with a host C++17 compiler, from the repository root:

```sh
c++ -std=c++17 -DGAT562_T9_KEYBOARD=1 -Itest/host_gat562_ime/stubs test/host_gat562_ime/test.cpp -o test-cn
c++ -std=c++17 -DGAT562_T9_KEYBOARD=1 -DCJK_IME_ZHUYIN -DZHUYIN_SINGLE_CHAR -Itest/host_gat562_ime/stubs test/host_gat562_ime/test.cpp -o test-tw
c++ -std=c++17 -DGAT562_T9_KEYBOARD=1 -DCJK_IME_NONE -Itest/host_gat562_ime/stubs test/host_gat562_ime/test.cpp -o test-en
./test-cn
./test-tw
./test-en
python bin/generate-gat562-predictions.py --check
```

`zig c++` can replace `c++` on Windows. Keep assertions enabled.
The tests compile the actual VirtualKeyboard.cpp and real prediction dictionaries.
Hardware, drawing and the binary-only pinyin lookup library are stubbed.
Display widths approximate the device font; these tests do not replace device testing.

Coverage: English case cycling, all four candidate directions, underline position,
suffix-only prediction display, variable-length paging in both directions, prediction commit without duplicate
prefixes, continued typing immediately after selection, deletion and submission.

The generated CN prediction table uses existing dictionary weights, at most 16
words per leading character and a 40,000-byte word pool. Words unsupported by the
GAT562 simplified Chinese font are excluded. Run the generator without `--check`
to regenerate the table after updating its source dictionary or font.
