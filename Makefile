CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra

LAB_BINS = Lab_10_1_A/Lab_10_1_A Lab_10_1_B/Lab_10_1_B Lab_10_2_A/Lab_10_2_A Lab_10_2_B/Lab_10_2_B Lab_10_3/Lab_10_3
TEST_BINS = tests/UnitTest_10_1_A tests/UnitTest_10_1_B tests/UnitTest_10_2_A tests/UnitTest_10_2_B tests/UnitTest_10_3

.PHONY: all labs tests test clean

all: labs tests

labs: $(LAB_BINS)

tests: $(TEST_BINS)

Lab_10_1_A/Lab_10_1_A: Lab_10_1_A/Lab_10_1_A.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

Lab_10_1_B/Lab_10_1_B: Lab_10_1_B/Lab_10_1_B.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

Lab_10_2_A/Lab_10_2_A: Lab_10_2_A/Lab_10_2_A.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

Lab_10_2_B/Lab_10_2_B: Lab_10_2_B/Lab_10_2_B.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

Lab_10_3/Lab_10_3: Lab_10_3/Lab_10_3.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

tests/UnitTest_10_1_A: tests/UnitTest_10_1_A.cpp Lab_10_1_A/Lab_10_1_A.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

tests/UnitTest_10_1_B: tests/UnitTest_10_1_B.cpp Lab_10_1_B/Lab_10_1_B.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

tests/UnitTest_10_2_A: tests/UnitTest_10_2_A.cpp Lab_10_2_A/Lab_10_2_A.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

tests/UnitTest_10_2_B: tests/UnitTest_10_2_B.cpp Lab_10_2_B/Lab_10_2_B.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

tests/UnitTest_10_3: tests/UnitTest_10_3.cpp Lab_10_3/Lab_10_3.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

test: tests
	@for t in $(TEST_BINS); do \
		echo "===> Running $$t <==="; \
		$$t || exit 1; \
	done

clean:
	rm -f $(LAB_BINS) $(TEST_BINS) *.dat *.bin
