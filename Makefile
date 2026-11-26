CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra

LAB_BINS = Lab_10_1_A/Lab_10_1_A Lab_10_1_B/Lab_10_1_B Lab_10_2_A/Lab_10_2_A Lab_10_2_B/Lab_10_2_B
TEST_BINS = tests/UnitTest_10_1_A tests/UnitTest_10_1_B tests/UnitTest_10_2_A tests/UnitTest_10_2_B

.PHONY: all labs tests test clean

all: labs tests

labs: $(LAB_BINS)

tests: $(TEST_BINS)

Lab_10_1_A/Lab_10_1_A: Lab_10_1_A/Lab_10_1_A.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

Lab_10_1_B/Lab_10_1_B: Lab_10_1_B/Lab_10_1_B.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

Lab_10_2_A/Lab_10_2_A: Lab_10_2_A/Lab_10_2_A.cpp Lab_10_2_A/student.cpp Lab_10_2_A/sort_search.cpp Lab_10_2_A/student.h Lab_10_2_A/sort_search.h
	$(CXX) $(CXXFLAGS) Lab_10_2_A/Lab_10_2_A.cpp Lab_10_2_A/student.cpp Lab_10_2_A/sort_search.cpp -o $@

Lab_10_2_B/Lab_10_2_B: Lab_10_2_B/Lab_10_2_B.cpp Lab_10_2_B/student.cpp Lab_10_2_B/sort_search.cpp Lab_10_2_B/student.h Lab_10_2_B/sort_search.h
	$(CXX) $(CXXFLAGS) Lab_10_2_B/Lab_10_2_B.cpp Lab_10_2_B/student.cpp Lab_10_2_B/sort_search.cpp -o $@

tests/UnitTest_10_1_A: tests/UnitTest_10_1_A.cpp Lab_10_1_A/Lab_10_1_A.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

tests/UnitTest_10_1_B: tests/UnitTest_10_1_B.cpp Lab_10_1_B/Lab_10_1_B.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

tests/UnitTest_10_2_A: tests/UnitTest_10_2_A.cpp Lab_10_2_A/student.cpp Lab_10_2_A/sort_search.cpp Lab_10_2_A/student.h Lab_10_2_A/sort_search.h
	$(CXX) $(CXXFLAGS) tests/UnitTest_10_2_A.cpp Lab_10_2_A/student.cpp Lab_10_2_A/sort_search.cpp -o $@

tests/UnitTest_10_2_B: tests/UnitTest_10_2_B.cpp Lab_10_2_B/student.cpp Lab_10_2_B/sort_search.cpp Lab_10_2_B/student.h Lab_10_2_B/sort_search.h
	$(CXX) $(CXXFLAGS) tests/UnitTest_10_2_B.cpp Lab_10_2_B/student.cpp Lab_10_2_B/sort_search.cpp -o $@

test: tests
	@for t in $(TEST_BINS); do \
		echo "===> Running $$t <==="; \
		$$t || exit 1; \
	done

clean:
	rm -f $(LAB_BINS) $(TEST_BINS) *.o */*.o *.dat *.bin
