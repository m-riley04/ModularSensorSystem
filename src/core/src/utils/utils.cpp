#include "utils/utils.hpp"

ns generateTimestampNs()
{
	// Get the current time point from the high-resolution clock
	std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();

	// Get the duration since the clock's epoch
	auto duration_since_epoch = now.time_since_epoch();

	// Cast the duration to nanoseconds
	std::chrono::nanoseconds nanoseconds_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(duration_since_epoch);

	// Get the count of nanoseconds
	long long nanoseconds_count = nanoseconds_duration.count();

	return nanoseconds_count;
}